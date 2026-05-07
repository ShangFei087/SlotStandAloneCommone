#ifndef _Test_H_
#define _Test_H_


#define   _InternalVersion 1  // 内部版本号
#define   _Version "2026324.100" // 版本字符串
//#define   _Release    // 定义后为发布版本；该宏仅允许在本文件中配置

#ifdef _Release

#else
#define _DebugControlMode				// 启用调试控制模式
#define _ShowDebugInfo					// 显示算法调试信息
#define _DebugInfoInterval  10000		// 每 10000 局显示RTP
//#define _IMHERE							// 临时调试标记位（调试完成后请移除）
//#define _LocalDebug						// 本地调试
//#define _WritePlayerData				// 写算法 Json 日志
//#define _WritePlayerLog					// 写玩家日志
#define _ExhibitionMode				// 展会模式：启用前端传入矩阵进行计算
#endif

#endif // __SF_Test_h__