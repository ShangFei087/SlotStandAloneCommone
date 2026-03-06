#ifndef _Test_H_
#define _Test_H_


#define   _InternalVersion 1  // 内部版本号
#define   _Version "20251104.100" // 版本字符串
#define   LocalDebug   //本地跑
//#define   _Release    // 开启此宏定义即为发布版本，该宏定义只能出现在该文件，不可在任何地方引用
#define _LocalLottery

#ifdef _Release
#define _WritePlayerData			// 写算法Json到日志
#define _ShowConsole
#else
#define _ShowConsole
#define _WritePlayerData			// 写算法Json到日志
#define _WritePlayerLog				// 写中文日志

#define _DebugControlMode			// 开启调试模式
#define _AlwaysNewControlData		// 每次启动初始化控制数据

#define _ShowDebugInfo				// 显示跑算法调试信息
#define _DebugInfoInterval  100		// 每100局显示一次信息
//#define _IMHERE						// 算法完成后任何调试语句都必须在改宏定义之间写，保证发布时不会带有调试语句

#endif


//#define _HighScore
//#define _ShowFatalMessageBox  
//#define _ShowMatrix

#endif // __SF_Test_h__