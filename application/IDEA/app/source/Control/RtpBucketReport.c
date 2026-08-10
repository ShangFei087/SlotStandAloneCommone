#include "RtpBucketReport.h"

#include <stdio.h>
#include <string.h>
#include "ComputerData.h"
#include "idea_qs.h"

typedef struct {
	uint64_t count;
	uint64_t betSum;
	uint64_t winSum;
} BucketStat;

enum {
	NORMAL_BUCKET_COUNT = 6,
	BONUS_BUCKET_COUNT = 6,
	FREE_BUCKET_COUNT = 6,
	JP_BUCKET_COUNT = 3 /* Major / Minor / Mini */
};

static BucketStat gLose;
static BucketStat gNormal[NORMAL_BUCKET_COUNT];
static BucketStat gBonus[BONUS_BUCKET_COUNT];
static BucketStat gFree[FREE_BUCKET_COUNT];
static BucketStat gJackpot[JP_BUCKET_COUNT];

static const char* kNormalNames[NORMAL_BUCKET_COUNT] = {
	"<1x", "1-2x", "2-5x", "5-10x", "10-20x", ">20x"
};
static const char* kBonusNames[BONUS_BUCKET_COUNT] = {
	"<200x", "200-300x", "300-400x", "400-500x", "500-600x", ">600x"
};
static const char* kFreeNames[FREE_BUCKET_COUNT] = {
	"<50x", "50-80x", "80-100x", "100-120x", "120-150x", ">150x"
};
static const char* kJpNames[JP_BUCKET_COUNT] = {
	"Major", "Minor", "Mini"
};

void RtpBucketReport_Reset(void)
{
	memset(&gLose, 0, sizeof(gLose));
	memset(gNormal, 0, sizeof(gNormal));
	memset(gBonus, 0, sizeof(gBonus));
	memset(gFree, 0, sizeof(gFree));
	memset(gJackpot, 0, sizeof(gJackpot));
}

static void Bucket_Add(BucketStat* b, uint64_t bet, uint64_t win)
{
	if (b == NULL) return;
	b->count += 1;
	b->betSum += bet;
	b->winSum += win;
}

/* Attribute win only (no round count / bet). Used for trigger-spin line wins. */
static void Bucket_AddWinOnly(BucketStat* b, uint64_t win)
{
	if (b == NULL || win == 0) return;
	b->winSum += win;
}

static void Bucket_Accum(BucketStat* dst, const BucketStat* src)
{
	if (dst == NULL || src == NULL) return;
	dst->count += src->count;
	dst->betSum += src->betSum;
	dst->winSum += src->winSum;
}

/* Multiplier is relative to total bet: featureMul / lineCount. */
static int32_t PickNormalBucket(double mul)
{
	if (mul < 1.0) return 0;
	if (mul < 2.0) return 1;
	if (mul < 5.0) return 2;
	if (mul < 10.0) return 3;
	if (mul < 20.0) return 4;
	return 5;
}

static int32_t PickBonusBucket(double mul)
{
	if (mul < 200.0) return 0;
	if (mul < 300.0) return 1;
	if (mul < 400.0) return 2;
	if (mul < 500.0) return 3;
	if (mul < 600.0) return 4;
	return 5;
}

static int32_t PickFreeBucket(double mul)
{
	if (mul < 50.0) return 0;
	if (mul < 80.0) return 1;
	if (mul < 100.0) return 2;
	if (mul < 120.0) return 3;
	if (mul < 150.0) return 4;
	return 5;
}

static int32_t PickPrimaryJpType(const OutResult_t* outRes)
{
	int32_t bestType = JT_Mini;
	int32_t bestBet = -1;
	int32_t i = 0;

	if (outRes == NULL) return JT_Mini;
	for (i = 0; i < outRes->nJPCount && i < GAME_Local_JP_MAX; ++i)
	{
		int32_t t = (int32_t)outRes->JPTypeArray[i];
		int32_t w = outRes->JPBetArray[i];
		if (t < JT_Major || t > JT_Mini) continue;
		if (w > bestBet)
		{
			bestBet = w;
			bestType = t;
		}
	}
	return bestType;
}

/* Line win on Free/Bonus/Jackpot trigger spins goes into Normal win (not round count). */
static void AttributeTriggerMatrixToNormal(int32_t betValue, double invLine, const OutResult_t* outRes)
{
	double mul;
	uint64_t win;
	int32_t idx;

	if (outRes == NULL || betValue <= 0 || outRes->nMatrixBet <= 0) return;

	mul = (double)outRes->nMatrixBet * invLine;
	win = (uint64_t)betValue * (uint64_t)outRes->nMatrixBet;
	idx = PickNormalBucket(mul);
	Bucket_AddWinOnly(&gNormal[idx], win);
}

void RtpBucketReport_OnRound(int32_t betValue, int32_t lineCount, const OutResult_t* outRes)
{
	uint64_t totalBet = 0;
	double invLine = 0.0;

	if (outRes == NULL || betValue <= 0 || lineCount <= 0) return;

	totalBet = (uint64_t)betValue * (uint64_t)lineCount;
	invLine = 1.0 / (double)lineCount;

	switch (outRes->resType)
	{
	case RT_Lose:
		Bucket_Add(&gLose, totalBet, 0);
		break;
	case RT_Win:
	{
		double mul = (double)outRes->nMatrixBet * invLine;
		uint64_t win = (uint64_t)betValue * (uint64_t)outRes->nMatrixBet;
		int32_t idx = PickNormalBucket(mul);
		Bucket_Add(&gNormal[idx], totalBet, win);
		break;
	}
	case RT_BonusWin:
	{
		double mul = (double)outRes->nBonusBet * invLine;
		uint64_t win = (uint64_t)betValue * (uint64_t)outRes->nBonusBet;
		int32_t idx = PickBonusBucket(mul);
		Bucket_Add(&gBonus[idx], totalBet, win);
		AttributeTriggerMatrixToNormal(betValue, invLine, outRes);
		break;
	}
	case RT_FreeWin:
	{
		double mul = (double)outRes->nTotalFreeBet * invLine;
		uint64_t win = (uint64_t)betValue * (uint64_t)outRes->nTotalFreeBet;
		int32_t idx = PickFreeBucket(mul);
		Bucket_Add(&gFree[idx], totalBet, win);
		AttributeTriggerMatrixToNormal(betValue, invLine, outRes);
		break;
	}
	case RT_Jackpot:
	{
		int32_t jpType = PickPrimaryJpType(outRes);
		uint64_t win = (uint64_t)outRes->nTotalJackpotBet;
		if (jpType < JT_Major || jpType > JT_Mini) jpType = JT_Mini;
		Bucket_Add(&gJackpot[jpType], totalBet, win);
		AttributeTriggerMatrixToNormal(betValue, invLine, outRes);
		break;
	}
	default:
		break;
	}
}

static void PrintBucketRow(const char* name, const BucketStat* b, uint64_t totalRounds, uint64_t totalBetAll)
{
	double avgMul = 0.0;
	double prob = 0.0;
	double rtp = 0.0;

	if (b == NULL || name == NULL) return;
	if (b->betSum > 0) avgMul = (double)b->winSum / (double)b->betSum;
	if (totalRounds > 0) prob = (double)b->count * 100.0 / (double)totalRounds;
	if (totalBetAll > 0) rtp = (double)b->winSum * 100.0 / (double)totalBetAll;

	QS_LOG("%-14s %10lld %14lld %14lld %10.2f %10.2f%% %10.2f%%\n",
		name,
		(long long)b->count,
		(long long)b->betSum,
		(long long)b->winSum,
		avgMul,
		prob,
		rtp);
}

static void PrintGroup(const char* title, BucketStat* arr, int32_t n, const char** names,
	uint64_t totalRounds, uint64_t totalBetAll, BucketStat* outSub)
{
	int32_t i = 0;
	BucketStat sub;
	memset(&sub, 0, sizeof(sub));

	QS_LOG("--- %s ---\n", title);
	for (i = 0; i < n; ++i)
	{
		PrintBucketRow(names[i], &arr[i], totalRounds, totalBetAll);
		Bucket_Accum(&sub, &arr[i]);
	}
	PrintBucketRow("Subtotal", &sub, totalRounds, totalBetAll);
	if (outSub != NULL) *outSub = sub;
}

void RtpBucketReport_Print(void)
{
	BucketStat normalSub, bonusSub, freeSub, jpSub, grand;
	uint64_t totalRounds = 0;
	uint64_t totalBetAll = 0;
	uint64_t totalWinAll = 0;
	double totalRtp = 0.0;
	double normalRtp = 0.0;
	double bonusRtp = 0.0;
	double freeRtp = 0.0;
	double jpRtp = 0.0;
	int32_t i = 0;

	memset(&normalSub, 0, sizeof(normalSub));
	memset(&bonusSub, 0, sizeof(bonusSub));
	memset(&freeSub, 0, sizeof(freeSub));
	memset(&jpSub, 0, sizeof(jpSub));
	memset(&grand, 0, sizeof(grand));

	Bucket_Accum(&grand, &gLose);
	for (i = 0; i < NORMAL_BUCKET_COUNT; ++i) Bucket_Accum(&grand, &gNormal[i]);
	for (i = 0; i < BONUS_BUCKET_COUNT; ++i) Bucket_Accum(&grand, &gBonus[i]);
	for (i = 0; i < FREE_BUCKET_COUNT; ++i) Bucket_Accum(&grand, &gFree[i]);
	for (i = 0; i < JP_BUCKET_COUNT; ++i) Bucket_Accum(&grand, &gJackpot[i]);

	totalRounds = grand.count;
	totalBetAll = grand.betSum;
	totalWinAll = grand.winSum;
	if (totalBetAll > 0)
	{
		totalRtp = (double)totalWinAll * 100.0 / (double)totalBetAll;
	}

	for (i = 0; i < NORMAL_BUCKET_COUNT; ++i) Bucket_Accum(&normalSub, &gNormal[i]);
	for (i = 0; i < BONUS_BUCKET_COUNT; ++i) Bucket_Accum(&bonusSub, &gBonus[i]);
	for (i = 0; i < FREE_BUCKET_COUNT; ++i) Bucket_Accum(&freeSub, &gFree[i]);
	for (i = 0; i < JP_BUCKET_COUNT; ++i) Bucket_Accum(&jpSub, &gJackpot[i]);

	if (totalBetAll > 0)
	{
		normalRtp = (double)normalSub.winSum * 100.0 / (double)totalBetAll;
		bonusRtp = (double)bonusSub.winSum * 100.0 / (double)totalBetAll;
		freeRtp = (double)freeSub.winSum * 100.0 / (double)totalBetAll;
		jpRtp = (double)jpSub.winSum * 100.0 / (double)totalBetAll;
	}

	QS_LOG("\n========== RTP Bucket Report ==========\n");
	QS_LOG("Rounds:%lld  TotalBet:%lld  TotalWin:%lld  TotalRTP:%.2f%%\n",
		(long long)totalRounds,
		(long long)totalBetAll,
		(long long)totalWinAll,
		totalRtp);
	QS_LOG("BaseRTP:%.2f%%  BonusRTP:%.2f%%  FreeRTP:%.2f%%  JackpotRTP:%.2f%%\n",
		normalRtp, bonusRtp, freeRtp, jpRtp);
	QS_LOG("%-14s %10s %14s %14s %10s %10s %10s\n",
		"Type", "Cnt", "Bet", "Win", "AvgMul", "Prob%", "RTP%");

	QS_LOG("--- Lose ---\n");
	PrintBucketRow("Subtotal", &gLose, totalRounds, totalBetAll);

	PrintGroup("Normal", gNormal, NORMAL_BUCKET_COUNT, kNormalNames, totalRounds, totalBetAll, &normalSub);
	PrintGroup("Bonus", gBonus, BONUS_BUCKET_COUNT, kBonusNames, totalRounds, totalBetAll, &bonusSub);
	PrintGroup("Jackpot", gJackpot, JP_BUCKET_COUNT, kJpNames, totalRounds, totalBetAll, &jpSub);
	PrintGroup("Free", gFree, FREE_BUCKET_COUNT, kFreeNames, totalRounds, totalBetAll, &freeSub);

	QS_LOG("--- Total ---\n");
	PrintBucketRow("Total", &grand, totalRounds, totalBetAll);
	QS_LOG("=======================================\n\n");
}
