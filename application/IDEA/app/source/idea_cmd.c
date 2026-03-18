/**
 * @file    
 * @author  Huang Wen <Email:ww1383@163.com, QQ:214890094, WeChat:w18926268887>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * @section DESCRIPTION
 *
 * This file is ...
 */
#include "app.h"
#include "Control/CMD_Fish.h"
#include "Control/NatureAlg.h"
#include "Control/ComputerData.h"
#include "Control/DllInterface.h"
#include "Control/GameConfig.h"
#include "Control/LotteryManager.h"
#include "Control/GameRegistry.h"
/**************************************************************************
 *                   G E N E R A L    C O N S T A N T S                   *
 **************************************************************************/
#define JSON_DATA_LEN                 56

/**************************************************************************
 *                         D A T A   T Y P E S                            *
 **************************************************************************/


/**************************************************************************
 *                             M A C R O S                                *
 **************************************************************************/
typedef struct _context {
	qs_senv* pSenv;
	qs_json* pJson;
	uint32_t SummaryIndex;
	BOOL update;
} context;

/**************************************************************************
 *                 E X T E R N A L   R E F E R E N C E S                  *
 **************************************************************************/


/**************************************************************************
 *                        G L O B A L   D A T A                           *
 **************************************************************************/
static context gCtx;
static context* gpCtx;

/**************************************************************************
 *               F U N C T I O N   D E C L A R A T I O N S                *
 **************************************************************************/

static void idea_decode_proc(idea_coder_decode_data* pData)
{
	if (comm_data_set_coder(pData)) {
		player_reset();
		ga_reset(TRUE);
	}
}

/**
*  @brief          接收到json数据对象
*  @param[in]      pSenv 安全信封通道句柄
*  @param[out]     json json对象
*  @param[out]     source json数据对象来源地址#qs_senv_address
*  @return         成功：TRUE，失败：FALSE
*  @details
*/


int32_t giveTime = 0;//免费次数
static void SenvReadCallback(qs_senv *pSenv, qs_json *json)
{
	uint32_t cmd = qs_json_GetObjectItem(json, "cmd")->valueint;
	QS_LOG("\r\n Cmd:%d", cmd);
	switch (cmd) 
	{
	//游戏初始化, 用于IDEA程序做一些必要的初始化操作
	case 20000:
	{
		int32_t res[6] = { 0 };
		uint8_t pos = 0;
		int32_t ret = 0;
		GameId_t gameId = GAME_ID_INVALID;
		if (GameRegistry_InitAndRegisterDefaults(&gameId))
		{
			ret = gameId;
		}
		else
		{
			ret = 0;
		}
		res[0] = ret;
		//TODO
		qs_json_DeleteItemFromObject(json, "data");
		qs_json_AddItemToObject(json, "data", qs_json_CreateIntArray(res, sizeof(res) / sizeof(int32_t)));
		qs_json_SetNumberValue(qs_json_GetObjectItem(json, "target"), qs_json_GetObjectItem(json, "source")->valueint);
		qs_senv_manager_write(gpCtx->pSenv, json);
	}
	break;
	//读取游戏配置数据
	case 20002:
	{
		qs_json* pJsonArray = qs_json_GetObjectItem(json, "data");
		size_t JsonArraySize = qs_json_GetArraySize(pJsonArray);
		int32_t res[67] = { 0 };

		if (JsonArraySize == 2) {
			comm_data CommData;
			int32_t pos = 0;
			if (comm_data_get(&CommData) == TRUE) {
				res[0] = 0;

				res[2] = CommData.PlaceType;                         // 场地类型，0：普通，1：技巧，2：专家
				res[3] = CommData.difficulty;                        // 难度，0~7
				res[4] = CommData.odds;                              // 倍率，0：低倍率，1：高倍率，2：随机

				res[5] = CommData.WinLock;                           // 盈利宕机
				res[6] = CommData.MachineId;                         // 机台编号，8位有效十进制数
				res[7] = CommData.LineId;                            // 线号，4位有效十进制数

				res[8] = CommData.TicketMode;                        // 退票模式，0：即中即退，1：退票
				res[9] = CommData.TicketValue;                       // 1票对应几分（彩票比例）
				res[10] = CommData.scoreTicket;                      // 1分对应几票
				res[11] = CommData.CoinValue;						 // 投币比例 (投币:1币多少分？)
				//暂时强行改成1000; 
				//res[11] = 1000;
				res[12] = CommData.MaxBet;                           // 最大押注
				res[13] = CommData.MinBet;                           // 最小押注
				res[14] = CommData.CountDown;                        // 例计时
				res[15] = CommData.MachineIdLock;                    // 1：机台号已锁定，除超级管理员外，无法更改，0：机台号未锁定
				res[16] = CommData.BetsMinOfJackpot;                 // 中彩金最小押分值
				res[17] = CommData.JackpotStartValue;                // 彩金初始值
				res[18] = CommData.LimitBetsWins;                    // 限红值

				res[19] = CommData.ReturnScore;
				res[20] = CommData.SwitchBetsUnitMin;
				res[21] = CommData.SwitchBetsUnitMid;
				res[22] = CommData.SwitchBetsUnitMax;
				res[23] = CommData.ScoreUpUnit;
				res[24] = CommData.PrintMode;
				res[25] = CommData.ShowMode;
				res[26] = CommData.CheckTime;
				res[27] = CommData.OpenBoxTime;
				res[28] = CommData.PrintLevel;
				res[29] = CommData.PlayerWinLock;
				res[30] = CommData.LostLock;
				res[31] = CommData.PulseValue;
				res[32] = CommData.NewGameMode;
				res[33] = CommData.NetJackpot;                // 是否采用联网彩金

			}
			else {
				res[0] = 1;
			}
			res[1] = pwd_type();
		}
		else {
			res[0] = -1;
			res[1] = 0;
		}

		qs_json_DeleteItemFromObject(json, "data");
		qs_json_AddItemToObject(json, "data", qs_json_CreateIntArray(res, sizeof(res) / sizeof(int32_t)));
		qs_json_SetNumberValue(qs_json_GetObjectItem(json, "target"), qs_json_GetObjectItem(json, "source")->valueint);
		qs_senv_manager_write(gpCtx->pSenv, json);
	}
	break;
	// 设定游戏配置数据
	case 20003:
	{
		qs_json* pJsonArray = qs_json_GetObjectItem(json, "data");
		size_t JsonArraySize = qs_json_GetArraySize(pJsonArray);
		int32_t res[2] = { 0 };
		uint32_t result = 0;

		if (JsonArraySize > 14) {
			comm_data CommData;
			CommData.PlaceType = (uint32_t)qs_json_GetArrayItem(pJsonArray, 0)->valueint;     // 场地类型，0：普通，1：技巧，2：专家
			CommData.difficulty = (uint32_t)qs_json_GetArrayItem(pJsonArray, 1)->valueint;    // 难度，0~7
			CommData.odds = (uint32_t)qs_json_GetArrayItem(pJsonArray, 2)->valueint;          // 倍率，0：低倍率，1：高倍率，2：随机

			CommData.WinLock = (uint32_t)qs_json_GetArrayItem(pJsonArray, 3)->valueint;       // 盈利宕机
			CommData.MachineId = (uint32_t)qs_json_GetArrayItem(pJsonArray, 4)->valueint;     // 机台编号，8位有效十进制数
			CommData.LineId = (uint32_t)qs_json_GetArrayItem(pJsonArray, 5)->valueint;        // 线号，4位有效十进制数

			CommData.TicketMode = (uint32_t)qs_json_GetArrayItem(pJsonArray, 6)->valueint;    // 退票模式，0：即中即退，1：退票
			CommData.TicketValue = (uint32_t)qs_json_GetArrayItem(pJsonArray, 7)->valueint;   // 1票对应几分（彩票比例）
			CommData.scoreTicket = (uint32_t)qs_json_GetArrayItem(pJsonArray, 8)->valueint;   // 1分对应几票
			CommData.CoinValue = (uint32_t)qs_json_GetArrayItem(pJsonArray, 9)->valueint;     // 投币比例
			CommData.MaxBet = (uint32_t)qs_json_GetArrayItem(pJsonArray, 10)->valueint;       // 最大押注
			CommData.MinBet = (uint32_t)qs_json_GetArrayItem(pJsonArray, 11)->valueint;       // 最小押注
			CommData.CountDown = (uint32_t)qs_json_GetArrayItem(pJsonArray, 12)->valueint;    // 例计时
			CommData.BetsMinOfJackpot = (uint32_t)qs_json_GetArrayItem(pJsonArray, 13)->valueint;    // 中彩金最小押分值
			CommData.JackpotStartValue = (uint32_t)qs_json_GetArrayItem(pJsonArray, 14)->valueint;   // 彩金初始值
			CommData.LimitBetsWins = (uint32_t)qs_json_GetArrayItem(pJsonArray, 15)->valueint;       // 限红值

			CommData.ReturnScore = (uint32_t)qs_json_GetArrayItem(pJsonArray, 16)->valueint;
			CommData.SwitchBetsUnitMin = (uint32_t)qs_json_GetArrayItem(pJsonArray, 17)->valueint;
			CommData.SwitchBetsUnitMid = (uint32_t)qs_json_GetArrayItem(pJsonArray, 18)->valueint;
			CommData.SwitchBetsUnitMax = (uint32_t)qs_json_GetArrayItem(pJsonArray, 19)->valueint;

			CommData.ScoreUpUnit = (uint32_t)qs_json_GetArrayItem(pJsonArray, 20)->valueint;
			CommData.PrintMode = (uint32_t)qs_json_GetArrayItem(pJsonArray, 21)->valueint;
			CommData.ShowMode = (uint32_t)qs_json_GetArrayItem(pJsonArray, 22)->valueint;
			CommData.CheckTime = (uint32_t)qs_json_GetArrayItem(pJsonArray, 23)->valueint;
			CommData.OpenBoxTime = (uint32_t)qs_json_GetArrayItem(pJsonArray, 24)->valueint;
			CommData.PrintLevel = (uint32_t)qs_json_GetArrayItem(pJsonArray, 25)->valueint;
			CommData.PlayerWinLock = (uint32_t)qs_json_GetArrayItem(pJsonArray, 26)->valueint;
			CommData.LostLock = (uint32_t)qs_json_GetArrayItem(pJsonArray, 27)->valueint;
			CommData.PulseValue = (uint32_t)qs_json_GetArrayItem(pJsonArray, 28)->valueint;
			CommData.NewGameMode = (uint32_t)qs_json_GetArrayItem(pJsonArray, 29)->valueint;

			CommData.NetJackpot = (uint32_t)qs_json_GetArrayItem(pJsonArray, 30)->valueint;   // 是否接入连线彩金
			if (pwd_type() != PWD_TYPE_NULL) {
				result = comm_data_set(&CommData);
				if (result) {
					if (result == 1) {
						res[0] = 0;

						//TODO 调用算法的conf
					}
					else {
						res[0] = result;
					}
				}
				else {
					res[0] = -1;
				}
			}
			else {
				res[0] = 1;
			}
		}
		else {
			res[0] = -1;
		}
		res[1] = pwd_type();

		qs_json_DeleteItemFromObject(json, "data");
		qs_json_AddItemToObject(json, "data", qs_json_CreateIntArray(res, sizeof(res) / sizeof(int32_t)));
		qs_json_SetNumberValue(qs_json_GetObjectItem(json, "target"), qs_json_GetObjectItem(json, "source")->valueint);
		qs_senv_manager_write(gpCtx->pSenv, json);
	}
	break;
	//密码验证
	case 20004:
	{
		qs_json* pJsonArray = qs_json_GetObjectItem(json, "data");
		size_t JsonArraySize = qs_json_GetArraySize(pJsonArray);
		int32_t res[2] = { 0 };

		if (JsonArraySize == 2) {
			uint32_t value = (uint32_t)qs_json_GetArrayItem(pJsonArray, 0)->valueint;
			if (pwd_check(value)) {
				res[0] = 0;
			}
			else {
				res[0] = 1;
			}
			res[1] = res[0] == 0 ? pwd_type() : PWD_TYPE_NULL;
		}
		else {
			res[0] = -1;
			res[1] = 0;
		}
		////test
		//res[0] = 0;
		//res[1] = 1;
		qs_json_DeleteItemFromObject(json, "data");
		qs_json_AddItemToObject(json, "data", qs_json_CreateIntArray(res, sizeof(res) / sizeof(int32_t)));
		qs_json_SetNumberValue(qs_json_GetObjectItem(json, "target"), qs_json_GetObjectItem(json, "source")->valueint);
		qs_senv_manager_write(gpCtx->pSenv, json);
	}
	break;
	//  密码修改
	case 20005:
	{
		qs_json* pJsonArray = qs_json_GetObjectItem(json, "data");
		size_t JsonArraySize = qs_json_GetArraySize(pJsonArray);
		int32_t res[2] = { 0 };

		if (JsonArraySize == 2) {
			uint32_t value = (uint32_t)qs_json_GetArrayItem(pJsonArray, 0)->valueint;
			if (pwd_set(value)) {
				res[0] = 0;
			}
			else {
				res[0] = 1;
			}
			res[1] = res[0] == 0 ? pwd_type() : PWD_TYPE_NULL;
		}
		else {
			res[0] = -1;
			res[1] = 0;
		}

		qs_json_DeleteItemFromObject(json, "data");
		qs_json_AddItemToObject(json, "data", qs_json_CreateIntArray(res, sizeof(res) / sizeof(int32_t)));
		qs_json_SetNumberValue(qs_json_GetObjectItem(json, "target"), qs_json_GetObjectItem(json, "source")->valueint);
		qs_senv_manager_write(gpCtx->pSenv, json);
	}
	break;
	// 请求打码数据
	case 20006:
	{
		qs_json* pJsonArray = qs_json_GetObjectItem(json, "data");
		size_t JsonArraySize = qs_json_GetArraySize(pJsonArray);
		int32_t res[15] = { 0 };

		if (JsonArraySize == 2) {
			idea_coder_code_context ctx;
			comm_data CommData;
			uint64_t bets = 0;
			int64_t wins = 0;

			comm_data_get(&CommData);
			player_total_bets_wins(&bets, &wins);

			ctx.uid = CommData.MachineId;
			ctx.bets = bets & 0xffffffff;
			ctx.wins = wins & 0xffffffff;
			idea_coder_request(&ctx);

			res[0] = 0;
			res[1] = (bets >> 32) & 0xffffffff;
			res[2] = (bets >> 0) & 0xffffffff;
			res[3] = (wins >> 32) & 0xffffffff;
			res[4] = (wins >> 0) & 0xffffffff;

			res[5] = CommData.MachineId;
			res[6] = ctx.times;
			res[7] = ctx.token;
			res[8] = idea_coder_remain_minute();
		}
		else {
			res[0] = -1;
			res[1] = 0;
		}

		qs_json_DeleteItemFromObject(json, "data");
		qs_json_AddItemToObject(json, "data", qs_json_CreateIntArray(res, sizeof(res) / sizeof(int32_t)));
		qs_json_SetNumberValue(qs_json_GetObjectItem(json, "target"), qs_json_GetObjectItem(json, "source")->valueint);
		qs_senv_manager_write(gpCtx->pSenv, json);
	}
	break;
	//打码
	case 20007:
	{
		qs_json* pJsonArray = qs_json_GetObjectItem(json, "data");
		size_t JsonArraySize = qs_json_GetArraySize(pJsonArray);
		int32_t res[2] = { 0 };

		if (JsonArraySize == 4) {
			idea_coder_code_context ctx;
			idea_coder_decode_data dat;
			comm_data CommData;
			uint64_t Code = 0;
			uint32_t flag = (uint32_t)qs_json_GetArrayItem(pJsonArray, 0)->valueint;
			BOOL result;
			uint64_t bets = 0;
			uint64_t wins = 0;

			Code = (uint32_t)qs_json_GetArrayItem(pJsonArray, 2)->valueint;
			Code <<= 32;
			Code |= (uint32_t)qs_json_GetArrayItem(pJsonArray, 1)->valueint;

			comm_data_get(&CommData);
			player_total_bets_wins(&bets, &wins);

			ctx.uid = CommData.MachineId;
			ctx.bets = bets & 0xffffffff;
			ctx.wins = wins & 0xffffffff;
			result = idea_coder_decode(&ctx, &dat, Code);
			if (result != TRUE) {
				res[0] = -1;
				res[1] = 0;
			}
			else {
				idea_decode_proc(&dat);

				res[1] = pwd_type() * 1000 + dat.clean;
			}
		}
		else {
			res[0] = -1;
			res[1] = 0;
		}

		qs_json_DeleteItemFromObject(json, "data");
		qs_json_AddItemToObject(json, "data", qs_json_CreateIntArray(res, sizeof(res) / sizeof(int32_t)));
		qs_json_SetNumberValue(qs_json_GetObjectItem(json, "target"), qs_json_GetObjectItem(json, "source")->valueint);
		qs_senv_manager_write(gpCtx->pSenv, json);
	}
	break;
	// 玩家押分数据
	case 20012:
	{
		qs_json* pJsonArray = qs_json_GetObjectItem(json, "data");
		size_t JsonArraySize = qs_json_GetArraySize(pJsonArray);
		uint32_t PlayerId;
		uint32_t Balance;
		uint32_t Rfu;
		//uint32_t Bet;
		uint32_t i;
		uint32_t pos = 0;
		uint32_t index = 0;
		uint32_t total = 0;
		uint32_t BetsTable[15];
		BOOL result = FALSE;
		int32_t res[19] = { 0 };
		index++;
		if (JsonArraySize && (JsonArraySize % 18) == 0) 
		{
			for (pos = 0; pos < JsonArraySize; ) {
				//uint32_t Bet;
				PlayerId = qs_json_GetArrayItem(pJsonArray, pos++)->valueint;
				res[index++] = PlayerId;
			/*	if (PlayerId) {
					PlayerId--;
				}*/

				if (PlayerId >= PLAYER_NUM_MAX) {
					result = FALSE;
					break;
				}
				Balance = qs_json_GetArrayItem(pJsonArray, pos++)->valueint;
				res[index++] = Balance;
				Rfu = qs_json_GetArrayItem(pJsonArray, pos++)->valueint;
				res[index++] = Rfu;
				for (i = 0; i < 15; i++) {
					BetsTable[i] = (uint32_t)qs_json_GetArrayItem(pJsonArray, pos++)->valueint;
					total += BetsTable[i];
					res[index++] = BetsTable[i];
				}
				player_data_item* pItem = player_statistics(PlayerId);
				if (pItem != NULL) {
					pItem->Bet = total;
					result = TRUE;
				}
				else {
					result = FALSE;
				}

				if (result != TRUE) {
					break;
				}
			}
			
		}

		if (result == FALSE)
		{
			res[0] = 1;
		}
		else 
		{
			res[0] = 0;
		}

		qs_json_DeleteItemFromObject(json, "data");
		qs_json_AddItemToObject(json, "data", qs_json_CreateIntArray(res, sizeof(res) / sizeof(int32_t)));
		qs_json_SetNumberValue(qs_json_GetObjectItem(json, "target"), qs_json_GetObjectItem(json, "source")->valueint);
		qs_senv_manager_write(gpCtx->pSenv, json);
	}
	break;
	// 投退币，上下分刷新
	case 20015:
	{
		qs_json* pJsonArray = qs_json_GetObjectItem(json, "data");
		size_t JsonArraySize = qs_json_GetArraySize(pJsonArray);
		uint32_t pos = 0;
		uint32_t PlayerId;
		uint32_t CoinIn;
		uint32_t CoinOut;
		uint32_t CoinOutWin;
		uint32_t ScoreUp;
		uint32_t ScoreDown;

		uint8_t idx = 0;
		int32_t res[65] = { 0 };
		if (JsonArraySize && (!(JsonArraySize % 6)))
		{
			while (JsonArraySize)
			{
				PlayerId = qs_json_GetArrayItem(pJsonArray, pos++)->valueint;
				if (PlayerId) 
				{
					PlayerId--;
				}
				CoinIn = qs_json_GetArrayItem(pJsonArray, pos++)->valueint;
				CoinOut = qs_json_GetArrayItem(pJsonArray, pos++)->valueint;
				CoinOutWin = qs_json_GetArrayItem(pJsonArray, pos++)->valueint;
				ScoreUp = qs_json_GetArrayItem(pJsonArray, pos++)->valueint;
				ScoreDown = qs_json_GetArrayItem(pJsonArray, pos++)->valueint;
				JsonArraySize -= 6;

				player_coin_info_update(PlayerId, CoinIn, CoinOut, CoinOutWin, ScoreUp, ScoreDown);
			}
			//ga_update_player_credit(PlayerId);
			gpCtx->update = TRUE;

			pos = 0;
			res[pos++] = 0;
			for (idx = 0; idx < PLAYER_NUM_MAX; idx++) 
			{
				player_data_item* pItem = player_statistics(idx);
				if (pItem != NULL)
				{
					res[pos++] = idx;
					res[pos++] = pItem->CoinIn;
					res[pos++] = pItem->CoinOut;
					res[pos++] = pItem->ScoreUp;
					res[pos++] = pItem->ScoreDown;
					res[pos++] = pItem->Credit;
					res[pos++] = pItem->Bets;
					res[pos++] = pItem->Wins;
				}
			}
		}
		else
		{
			res[0] = -1;
		}

		// 删除旧的data字段
		qs_json_DeleteItemFromObject(json, "data");
		//添加新的data字段（包含处理结果）
		qs_json_AddItemToObject(json, "data", qs_json_CreateIntArray(res, sizeof(res) / sizeof(int32_t)));
		// 设置目标地址（将源地址设为目标，实现回传）
		qs_json_SetNumberValue(qs_json_GetObjectItem(json, "target"), qs_json_GetObjectItem(json, "source")->valueint);
		//发送响应
		qs_senv_manager_write(gpCtx->pSenv, json);
	}
	break;
	//获取玩家账目
	case 20016: 
	{		
		qs_json* pJsonArray = qs_json_GetObjectItem(json, "data");
		size_t JsonArraySize = qs_json_GetArraySize(pJsonArray);
		uint8_t idx = 0;
		uint8_t pos = 0;
		int32_t res[65] = { 0 };
		if (JsonArraySize > 0)
		{
			res[pos++] = 0;
			for (idx = 0; idx < PLAYER_NUM_MAX; idx++) {
				player_data_item* pItem = player_statistics(idx);
				if (pItem != NULL) 
				{
					res[pos++] = idx;
					res[pos++] = pItem->CoinIn;
					res[pos++] = pItem->CoinOut;
					res[pos++] = pItem->ScoreUp;
					res[pos++] = pItem->ScoreDown;
					res[pos++] = pItem->Credit;
					res[pos++] = pItem->Bets;
					res[pos++] = pItem->Wins;
				}
			}	
		}
		else 
		{
			res[0] = -1;
		}
		// 删除旧的data字段
		qs_json_DeleteItemFromObject(json, "data");
		//添加新的data字段（包含处理结果）
		qs_json_AddItemToObject(json, "data", qs_json_CreateIntArray(res, sizeof(res) / sizeof(int32_t)));
		// 设置目标地址（将源地址设为目标，实现回传）
		qs_json_SetNumberValue(qs_json_GetObjectItem(json, "target"), qs_json_GetObjectItem(json, "source")->valueint);
		//发送响应
		qs_senv_manager_write(gpCtx->pSenv, json);
	}
	break;
	// 请求是否可以开始新的一局
	case 20018:
	{
		qs_json* pJsonArray = qs_json_GetObjectItem(json, "data");
		size_t JsonArraySize = qs_json_GetArraySize(pJsonArray);
		int32_t res[2] = { 0 };

		if (JsonArraySize == 2) {
			res[0] = idea_coder_state() & 0x0f;
			res[1] = 0;
		}
		else {
			res[0] = -1;
			res[1] = 0;
		}

		qs_json_DeleteItemFromObject(json, "data");
		qs_json_AddItemToObject(json, "data", qs_json_CreateIntArray(res, sizeof(res) / sizeof(int32_t)));
		qs_json_SetNumberValue(qs_json_GetObjectItem(json, "target"), qs_json_GetObjectItem(json, "source")->valueint);
		qs_senv_manager_write(gpCtx->pSenv, json);
	}
	break;
	//app spin时调用
	case 20100:
	{
		qs_json* pJsonArray = qs_json_GetObjectItem(json, "data");
		size_t JsonArraySize = qs_json_GetArraySize(pJsonArray);
		int32_t res[150] = { 0 };
		//TODO
		qs_json_DeleteItemFromObject(json, "data");
		qs_json_AddItemToObject(json, "data", qs_json_CreateIntArray(res, sizeof(res) / sizeof(int32_t)));
		qs_json_SetNumberValue(qs_json_GetObjectItem(json, "target"), qs_json_GetObjectItem(json, "source")->valueint);
		qs_senv_manager_write(gpCtx->pSenv, json);
	}
	break;
	//获取滚轮调用
	case 20101:			
	{
		qs_json* pJsonArray = qs_json_GetObjectItem(json, "data");
		size_t JsonArraySize = qs_json_GetArraySize(pJsonArray);
		int32_t res[150] = { 0 }; //250个以下的int32_t
		uint32_t pos = 0;
		uint32_t ret =0;
		uint32_t jpType = 0;
		uint32_t jpvaule = 0;
		GameId_t gameId = g_CurrentGameInstance->id;
		OutResult_t outres;
		uint32_t TotalWin = 0;
		OutResult_Init(&outres);
		
		uint32_t player_id = qs_json_GetArrayItem(pJsonArray, 0)->valueint;
		player_data_item* pItem = player_statistics(player_id);
		
		uint32_t totalbet = pItem->Bet;//总押注
		uint32_t betmultiple = pItem->Bet / 50;//下注倍数
		QS_LOG("\r\n Bet:%d", totalbet);
		outres.openType = OT_Normal;
		if (giveTime > 0)
		{
			giveTime--;
			outres.openType = OT_Give;
		}

		if (outres.openType == OT_Normal)
		{
			BOOL xret = player_bets_info_update(player_id, totalbet, 0);
			if (xret) {
				QS_LOG("\r\n 1111");
			}
			LotteryManager_OnPlay(&gLotteryManager, totalbet);
			//检测是否可以获得彩金
			LotteryManager_TryGetLottery(&gLotteryManager, totalbet, &jpType, &jpvaule);
			if (jpvaule > 0)
			{
				outres.JPType = jpType;
				outres.nJPBet = jpvaule;
			}
		}

		//调用游戏算法生成结果
		DLL_GetGameResultById(pItem, betmultiple, &outres, &ret, gameId);
		if (outres.resType == RT_FreeWin)
		{
			giveTime = outres.nTotalFreeTime;

		}
		
		//先把所有的分数加完
		if (outres.openType == OT_Normal)
		{
			TotalWin = betmultiple *(outres.nMatrixBet + outres.nTotalFreeBet + outres.nBonusBet);

			QS_LOG("\r\n Win:%d", TotalWin);
			player_bets_info_update(player_id, 0, TotalWin);
		}
	
		ret = gameId;
		/*
			* 1:OpenType:0:普通开奖 1:赠送
			* 2.ResultType:0:输 1:盈 2:免费游戏 3:彩金游戏
			* 3:lineNum:中几条线
			* 4:TotalBet
			* 5:IDVec万千位标识线， 百位标识消除多少个， 十个位标识ID
			* 6:Matrix:
		*/
		res[pos++] = (int32_t)ret;							// ret
		res[pos++] = (int32_t)outres.openType;				// OpenType
		res[pos++] = (int32_t)outres.resType;				// ResultType
		res[pos++] = outres.matrix.idVecSize;				// lineNum
		res[pos++] = outres.nMatrixBet;						// TotalBet
		res[pos++] = GE_WheelChessNum;						// MatrixLength
		res[pos++] = outres.nTotalFreeTime;					// nTotalFreeTime
		res[pos++] = outres.nTotalFreeBet;					// nTotalFreeBet
		res[pos++] = outres.nBonusBet;						// nTotalFreeBet
		res[pos++] = outres.nBonusType;						// nTotalFreeBet
		

		// IDVec
		for (int32_t i = 0; i < outres.matrix.idVecSize; i++)
		{
			res[pos++] = outres.IDVec[i];
		}
		//Matrix
		for (int32_t i = 0; i < GE_WheelChessNum; i++)
		{
			res[pos++] = outres.matrix.dataArray[i];
		}

		//FreeBetArray
		for (int32_t i = 0; i < outres.nTotalFreeTime; i++)
		{
			res[pos++] = outres.FreeBetArray[i];
		}

		//BonusData
		for (int32_t i = 0; i < GE_WheelChessNum; i++)
		{
			res[pos++] = outres.BonusData[i];
		}

		//BonusMultiply或者BlindSymbol
		res[pos++] = outres.BlindSymbol;			//当大奖为神秘游戏就是BlindSymbol ,乘数游戏或者彩金游戏时就是BonusMultiply
	
	
		qs_json_DeleteItemFromObject(json, "data");
		qs_json_AddItemToObject(json, "data", qs_json_CreateIntArray(res, sizeof(res) / sizeof(int32_t)));
		qs_json_SetNumberValue(qs_json_GetObjectItem(json, "target"), qs_json_GetObjectItem(json, "source")->valueint);
		qs_senv_manager_write(gpCtx->pSenv, json);
		
	}
	break;
	//滚轮停止后发送
	case 20102:
	{
		qs_json* pJsonArray = qs_json_GetObjectItem(json, "data");
		size_t JsonArraySize = qs_json_GetArraySize(pJsonArray);
		int32_t res[2] = { 0 };
		uint8_t idx = 0;
		uint8_t pos = 0;
		if (JsonArraySize == 1) {
			player_data_item* pItem = player_statistics(idx);
			res[0] = 0;
			res[1] = pItem->Credit;
		}
		else {
			res[0] = -1;
			res[1] = 0;
		}

		// 删除旧的data字段
		qs_json_DeleteItemFromObject(json, "data");
		//添加新的data字段（包含处理结果）
		qs_json_AddItemToObject(json, "data", qs_json_CreateIntArray(res, sizeof(res) / sizeof(int32_t)));
		// 设置目标地址（将源地址设为目标，实现回传）
		qs_json_SetNumberValue(qs_json_GetObjectItem(json, "target"), qs_json_GetObjectItem(json, "source")->valueint);
		//发送响应
		qs_senv_manager_write(gpCtx->pSenv, json);
	}
	break;
	//切换游戏
	case 20200:
	{
		qs_json* pJsonArray = qs_json_GetObjectItem(json, "data");
		size_t JsonArraySize = qs_json_GetArraySize(pJsonArray);
		int32_t res[2] = { 0 };

		if (JsonArraySize == 2) {
			uint32_t gameid = (uint32_t)qs_json_GetArrayItem(pJsonArray, 0)->valueint;
			//初始化调试模式
			DebugControlMode_t dcm;
			DebugControlMode_Init(&dcm);
			DLL_SetControlDebugMode(&dcm);
			//切换游戏
			if (DLL_GameSwitch(gameid))
			{
				res[0] = gameid;
				res[1] = 0;
			}
		}
		else {
			res[0] = -1;
			res[1] = 0;
		}
		//TODO
		qs_json_DeleteItemFromObject(json, "data");
		qs_json_AddItemToObject(json, "data", qs_json_CreateIntArray(res, sizeof(res) / sizeof(int32_t)));
		qs_json_SetNumberValue(qs_json_GetObjectItem(json, "target"), qs_json_GetObjectItem(json, "source")->valueint);
		qs_senv_manager_write(gpCtx->pSenv, json);
	}
	break;
	//获取显示值彩金
	case 20201:
	{
		qs_json* pJsonArray = qs_json_GetObjectItem(json, "data");
		size_t JsonArraySize = qs_json_GetArraySize(pJsonArray);
		int32_t res[4] = { 0 }; 
		int32_t pos = 0;
		res[pos++] = 0;
		for (uint8_t i = 0; i < GAME_Local_JP_MAX; ++i)
		{
			res[pos++] = Lottery_GetShowValue(&gLotteryManager.mLotterys[i]);
		}
		//TODO
		qs_json_DeleteItemFromObject(json, "data");
		qs_json_AddItemToObject(json, "data", qs_json_CreateIntArray(res, sizeof(res) / sizeof(int32_t)));
		qs_json_SetNumberValue(qs_json_GetObjectItem(json, "target"), qs_json_GetObjectItem(json, "source")->valueint);
		qs_senv_manager_write(gpCtx->pSenv, json);
	}
	break;
	//调试模式
	case 20202:
	{
		qs_json* pJsonArray = qs_json_GetObjectItem(json, "data");
		size_t JsonArraySize = qs_json_GetArraySize(pJsonArray);
		int32_t res[1] = { 0 };
		int32_t pos = 0;
		DebugControlMode_t dcm;
		DebugControlMode_Init(&dcm);
		if (JsonArraySize == 4)
		{
			dcm.mode = (uint8_t)qs_json_GetArrayItem(pJsonArray, pos++)->valueint;
			dcm.resType = (uint8_t)qs_json_GetArrayItem(pJsonArray, pos++)->valueint;
			dcm.bonusType = (uint8_t)qs_json_GetArrayItem(pJsonArray, pos++)->valueint;
			dcm.jpType = (uint8_t)qs_json_GetArrayItem(pJsonArray, pos++)->valueint;

			DLL_SetControlDebugMode(&dcm);

			res[0] = 0;
		}
		else
		{
			res[0] = -1;
		}
		
		//TODO
		qs_json_DeleteItemFromObject(json, "data");
		qs_json_AddItemToObject(json, "data", qs_json_CreateIntArray(res, sizeof(res) / sizeof(int32_t)));
		qs_json_SetNumberValue(qs_json_GetObjectItem(json, "target"), qs_json_GetObjectItem(json, "source")->valueint);
		qs_senv_manager_write(gpCtx->pSenv, json);
	}
	break;
	// 玩家赢得联网彩金的彩金信息
	case 20030:
	{
		qs_json* pJsonArray = qs_json_GetObjectItem(json, "data");
		size_t JsonArraySize = qs_json_GetArraySize(pJsonArray);
		uint32_t PlayerId = 0;;
		uint32_t MachineId = 0;
		uint32_t JackpotType = 0;
		uint32_t JackpotWins = 0;
		uint32_t pos = 0;
		int32_t res[4] = { 0 };

		uint32_t beforeCredit = 0;
		uint32_t afterCredit = 0;
		if (JsonArraySize && JsonArraySize ==4)
		{
			MachineId= qs_json_GetArrayItem(pJsonArray, 0)->valueint;
			PlayerId = qs_json_GetArrayItem(pJsonArray, 1)->valueint;
			JackpotType = qs_json_GetArrayItem(pJsonArray, 2)->valueint;
			JackpotWins = qs_json_GetArrayItem(pJsonArray, 3)->valueint;

			player_data_item* pItem = player_statistics(PlayerId);
			beforeCredit = pItem->Credit;
			player_bets_info_update(PlayerId, 0, JackpotWins);
			afterCredit = pItem->Credit ;
			res[0] = 0;
			res[1] = JackpotWins;
			res[2] = beforeCredit;
			res[3] = afterCredit;
			/*QS_LOG("\r\n PlayerId:%d", PlayerId);
			QS_LOG("\r\n beforeCredit:%d", beforeCredit);
			QS_LOG("\r\n afterCredit:%d", afterCredit);*/
		}
		else
		{
			res[0] =-1;
			res[1] = 0;
		}


		//TODO
		qs_json_DeleteItemFromObject(json, "data");
		qs_json_AddItemToObject(json, "data", qs_json_CreateIntArray(res, sizeof(res) / sizeof(int32_t)));
		qs_json_SetNumberValue(qs_json_GetObjectItem(json, "target"), qs_json_GetObjectItem(json, "source")->valueint);
		qs_senv_manager_write(gpCtx->pSenv, json);
	}
	break;
	}
}
static void cmd_exec_0(uint32_t millisecond)
{
	static uint32_t mdt = 0;

	mdt += millisecond;
	if ((mdt >= 300) || (gpCtx->update == TRUE)) {
		//player_data_item* pPlayer;
		//ga_player_reward PlayerReward;
		uint32_t i;
		int32_t data[JSON_DATA_LEN];
		uint32_t pos = 0;
		uint32_t temp = 0;
		//uint32_t lottery = 0;
		qs_json* pJsonArray = qs_json_GetObjectItem(gpCtx->pJson, "data");

		gpCtx->update = FALSE;

		// 激活状态
		temp = idea_coder_state() & 0x0f;

		if (comm_machine_id_lock()) {
			temp |= (1 << 4);
		}

		data[pos++] = temp;//0
		//data[pos++] = 0;//idea_printer_state() & 0x0ff;
		/*int32_t nJp[3] = { 0,0,0 };
		ga_jackpot(nJp);
		for (int32_t i = 0; i < 3; i++) {
			data[pos++] = nJp[i];
		}*/
		data[pos++] = idea_coder_remain_minute();//1
		data[pos++] = comm_win_lock_Balance();//2

		//test
		data[2] = 0;// ga_jackpot();
		data[3] = idea_coder_remain_minute();
		data[4] = comm_win_lock_Balance();
		data[5] = 1;
		data[6] = 0;
		data[7] = 0;
		pos = 8;
		
		for (i = 0; i <= 8; i++)
		{
			player_data_item* pItem = player_statistics(i);
			data[pos++] = i;//PlayerId
			data[pos++] = pItem->Credit;//Score 
			data[pos++] = pItem->CreditWin;//Wins
			data[pos++] = 0;//PointsMax
			data[pos++] = 0; //BoxPoints
			data[pos++] = 0; //BoxOpenFlags
		}

		for (i = 0; i < sizeof(data) / sizeof(int32_t); i++) {
			qs_json_SetNumberValue(qs_json_GetArrayItem(pJsonArray, i), data[i]);
		}
		qs_senv_manager_write(gpCtx->pSenv, gpCtx->pJson);
		mdt = 0;
	}
}

/**
 *  @brief      循环调用函数，本函数会被系统循环，禁止在函数里使用死锁语句
 *  @param[in]  millisecond 每次被调用的时间差，毫秒为单位
 *  @return
 *  @details
 */
void idea_cmd_exec(uint32_t millisecond)
{
	static BOOL connected = TRUE;

	if (connected != qs_senv_connected(gpCtx->pSenv))
	{
		connected = qs_senv_connected(gpCtx->pSenv);
		if (connected == TRUE)
		{
			qs_led_green_blink(60, 1940, QS_MAX);
		}
		else 
		{
			qs_led_green_blink(60, 440, QS_MAX);
		}
	}

	if (connected == TRUE) 
	{
		cmd_exec_0(millisecond);
	}

}

/**
 *  @brief      在这里初始化你的应用
 *  @param
 *  @return
 *  @details
 */
void idea_cmd_init(void)
{
	int32_t data[JSON_DATA_LEN] = { 0 };

	gpCtx = &gCtx;
	memset(gpCtx, 0, sizeof(gCtx));

	gpCtx->pJson = qs_json_CreateObject();
	qs_json_AddNumberToObject(gpCtx->pJson, "cmd", 20001);
	qs_json_AddNumberToObject(gpCtx->pJson, "source", QS_SENV_IO_ADDRESS);
	qs_json_AddNumberToObject(gpCtx->pJson, "target", 1);
	qs_json_AddItemToObject(gpCtx->pJson, "data", qs_json_CreateIntArray(data, sizeof(data) / sizeof(int32_t)));

	gpCtx->pSenv = qs_system_senv_setup(SenvReadCallback);
}

void idea_cmd_close(void)
{
	DLL_GameClose(g_CurrentGameInstance->id);
}


/* END OF FILE */
