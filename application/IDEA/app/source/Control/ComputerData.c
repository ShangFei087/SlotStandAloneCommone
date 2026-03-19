#include "ComputerData.h"

int32_t JPWeight[4] = { 100, 50, 30, 20 };
//比如玩家游戏得分越高 / 投注越大，系统会选择更大的 JPDrawRateUnit 数值，让中奖概率按倍数提升（比如选 4 倍，则 “一份” 奖的概率从 100 / 万变成 400 / 万），是 “高投入高回报” 的核心逻辑。
int32_t JPDrawRateUnit[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
//中心奖池的抽取率配置，和 JPDrawRateUnit 一一对应，是最终中奖概率的 “绝对数值”（而非倍数）。
int32_t CenterJPDrawRate[8] = { 25, 50, 75, 100, 125, 150, 175, 200 };