#ifndef CMD_FISH_H_
#define CMD_FISH_H_

#include <string.h>
#include "qs.h"
#include "GameConfig.h"

//--------------------------------------------ȫ������(3����Ϸ����ֵ)-------------------------------------------//
//����ģʽö��
#define DCM_Normal 0
#define DCM_PointResData 1

//ȫ�ֲ���ÿ��
#define GE_WheelChessNum 15
#define GE_MaxIDNum 20
#define GE_Line20Num  20  //20����
#define GE_Line15Num  15  //15��
#define GE_MaxFreeNum  20


//�����������
#define RT_Lose 0
#define RT_Win 1
#define RT_FreeWin 2
#define RT_BonusWin 3
#define RT_JpMaior  4

//������ʽ����
#define OT_Normal 0
#define OT_Give   1

//�ʽ�����
#define JT_None 0
#define JT_Major 1
#define JT_minor 2
#define JT_mini 3

//-------------------------------------------------------------------------------------

//���һ���ߵĽ��
typedef struct {
	uint8_t bIsEliminate;              //�Ƿ�������
	uint8_t bHasWild;                  //�Ƿ���WIld
	uint8_t chessTypeVec[5];         //�����ϵ�5��ͼ��
	uint8_t posVec[5];               //�����ϵ�5��λ��
	uint8_t  nWildNum;              //wild��Ŀ
	uint8_t  nAvailChessType;       //��Ч������Type
	uint8_t  nEliminateNum;         //������Ŀ
	uint8_t  nEliminateLineId;      //����������
} CheckOnLineResult_t;

//��ʼ��CheckOnLineResult_t
void CheckOnLineResult_Init(CheckOnLineResult_t* pResult);

//-------------------------------------------------------------------------------------
//����
typedef struct {
	uint8_t resultType;
	uint8_t idVecSize;
	uint8_t dataArray[GE_WheelChessNum];
} Matrix_u;

// Matrix_u ��غ�������
uint8_t Matrix_u_getIntData(Matrix_u* pMatrix, uint8_t pos);
void Matrix_u_setIntData(Matrix_u* pMatrix, uint8_t* pData);
void Matrix_u_print(Matrix_u* pMatrix);
uint8_t Matrix_u_getTypeNum(Matrix_u* pMatrix, uint8_t type);
void Matrix_u_reset(Matrix_u* pMatrix);
void Matrix_u_copy(Matrix_u* pDest, Matrix_u* pSrc);

void Matrix_u_insertScatter(Matrix_u* pMatrix, uint8_t num);
void Matrix_u_insertCol(Matrix_u* pMatrix, uint8_t type, uint8_t col);//����һ����ͬ����ͼ��
void Matrix_u_insertBonus(Matrix_u* pMatrix, uint8_t num);
void Matrix_u_GenerateAnyResult(Matrix_u* pMatrix, uint8_t hasWild, uint8_t hasScatter, uint8_t hasBonus);
void Matrix_u_GenerateLooseResult(Matrix_u* pMatrix, uint8_t hasWild, uint8_t hasScatter, uint8_t hasBonus, uint8_t hasBoost);

uint8_t Matrix_u_computerMatrix_243(Matrix_u* pMatri, int32_t* idVec); //243�߼���
int32_t Matrix_u_computerMatrixById(Matrix_u* pMatrix, int32_t* idVec,SlotGameConfig_t* gameConfig, uint32_t gameId);//���߼���
uint8_t Matrix_u_checkOnLine(Matrix_u* pMatrix, uint8_t lineIdx, CheckOnLineResult_t* pClr, SlotGameConfig_t* gameConfig);

//������
typedef struct {
	uint8_t  openType;                   //�������ͣ���ͨ-����
	uint8_t resType;                   //�������
	uint8_t JPType;
	int32_t IDVec[GE_MaxIDNum];             //������ID���ͣ����Ϊ20��
	int32_t nMatrixBet;                     //��������Ӧ������
	Matrix_u matrix;                        //����

	/*���������Ͳ�ΪRT_FreeWin��������ֵ��������*/
	int32_t nTotalFreeBet;                  //��ʾ�����Ϸ������
	uint8_t nTotalFreeTime;                 //������ʾ�����Ϸ����
	int32_t FreeBetArray[GE_MaxFreeNum];

	//uint8_t uint16_t
	/*���������Ͳ�ΪRT_BonusWin��������ֵ��������*/
	int32_t nBonusBet;
	uint8_t BlindSymbol; //����ͼ��
	int32_t BonusData[GE_WheelChessNum];                  //�󽱽��
	uint8_t nBonusType;
	
	int32_t nJPBet;
} OutResult_t;

// OutResult_t ��غ�������
void OutResult_Init(OutResult_t* pResult);
void OutResult_reset(OutResult_t* pResult);

//���Կ��ƽṹ
typedef struct {
	uint8_t mode;    //����ģʽ
	uint8_t resType;       //�������
	uint8_t bonusType;       //������
	uint8_t jpType;       //������
} DebugControlMode_t;

void DebugControlMode_Init(DebugControlMode_t* pMode);
void  DLL_SetControlDebugMode(DebugControlMode_t* pDebugMode);
extern DebugControlMode_t gDebugControlMode;


//������Ϣ
typedef struct {
	int64_t dwPlayScore;       //�㷨�����
	int64_t dwWinScore;        //�㷨��Ӯ��
	int64_t dwTotalPlayTime;      //�������
	int64_t dwNormalOpenTime;     //��ͨ��������
	int64_t dwGiveOpenTime;       //���Ϳ�������
	int64_t dwNormalWinTime;      //��ͨ������
	int64_t dwBonusTime;          //�ʽ����
	int64_t dwFreeGameTime;       //�����Ϸ��������
	int64_t dwLooseTime;          //���н�����
	int64_t dwNormalWinTotalBet;   //��ͨӮ�ͳ���������
	int64_t dwFreeGameTotalBet;   //�����Ϸ�ͳ���������
	int64_t dwBonusGameTotalBet;  //BonusGame�ͳ���������
	int64_t dwFreeGameBetError;   //�����Ϸ���µ����
} DebugInfo;

void DebugInfo_reset(DebugInfo* pInfo);
void DebugInfo_accum(DebugInfo* pDest, DebugInfo* pSrc);
int8_t* DebugInfo_toJason(DebugInfo* pInfo);
// DebugInfo_toJason 返回堆内存字符串，调用方必须 free

// ȫ�ֱ�������


#endif // CMD_FISH_H_
