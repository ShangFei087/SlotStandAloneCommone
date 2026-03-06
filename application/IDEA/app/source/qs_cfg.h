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
#ifndef _QS_CFG_H_
#define _QS_CFG_H_


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */


/*--------------------------------------------------------------------
 *
 * “¿¿µ:
 *--------------------------------------------------------------------*/
 #if(USE_LIB == 0)
#define QS_NAME                                           "idea"
#define QS_SOFTWARE_VERSION                               "1.0.1"
#define QS_HARDWARE_VERSION                               "1.0.1"
#define QS_VENDOR                                         "KAIXINMAN"
#define QS_SUPPLIER                                       ""
#endif//NO_LIB

/*--------------------------------------------------------------------
 *
 * “¿¿µ:
 *--------------------------------------------------------------------*/
#define QS_PLATFORM_COMMON                                0
#define QS_PLATFORM_WIN32                                 1
#define QS_PLATFORM_LINUX                                 2

#ifdef _WIN32
#define QS_PLATFORM                                       QS_PLATFORM_WIN32
#else 
#define QS_PLATFORM                                       QS_PLATFORM_COMMON
#endif

/*--------------------------------------------------------------------
 *
 * “¿¿µ:
 *--------------------------------------------------------------------*/
#if (QS_PLATFORM == QS_PLATFORM_WIN32)
#define QS_VIRTUAL                                        0
#endif

/*--------------------------------------------------------------------
 *
 * “¿¿µ:
 *--------------------------------------------------------------------*/
#define QS_MEM_TRACK                                      0

/*--------------------------------------------------------------------
 *
 * “¿¿µ:
 *--------------------------------------------------------------------*/
#define QS_CONSOLE                                        1
#if(QS_CONSOLE)
#define QS_CONSOLE_IOSTREAM_NAME                          "serial0"
#endif

/*--------------------------------------------------------------------
 *
 * “¿¿µ:
 *--------------------------------------------------------------------*/
#define QS_DRIVER_SUPPORT_TIMER_0                         1
#define QS_DRIVER_SUPPORT_TIMER_1                         0	
#define QS_DRIVER_SUPPORT_TIMER_2                         0
#define QS_DRIVER_SUPPORT_TIMER_3                         0
#define QS_DRIVER_SUPPORT_TIMER                           \
(QS_DRIVER_SUPPORT_TIMER_0 | QS_DRIVER_SUPPORT_TIMER_1 | QS_DRIVER_SUPPORT_TIMER_2 | QS_DRIVER_SUPPORT_TIMER_3)

/*--------------------------------------------------------------------
 *
 * “¿¿µ:
 *--------------------------------------------------------------------*/
#define QS_DRIVER_SUPPORT_SERIAL_0                        1
#define QS_DRIVER_SUPPORT_SERIAL_1                        1	
#define QS_DRIVER_SUPPORT_SERIAL_2                        1
#define QS_DRIVER_SUPPORT_SERIAL_3                        0
#define QS_DRIVER_SUPPORT_SERIAL                          \
(QS_DRIVER_SUPPORT_SERIAL_0 | QS_DRIVER_SUPPORT_SERIAL_1 | QS_DRIVER_SUPPORT_SERIAL_2 | QS_DRIVER_SUPPORT_SERIAL_3)

/*--------------------------------------------------------------------
 *
 * “¿¿µ:
 *--------------------------------------------------------------------*/
#define QS_DRIVER_SUPPORT_SPI_0                           0
#define QS_DRIVER_SUPPORT_SPI_1                           0
#define QS_DRIVER_SUPPORT_SPI_2                           0
#define QS_DRIVER_SUPPORT_SPI                             \
(QS_DRIVER_SUPPORT_SPI_0 | QS_DRIVER_SUPPORT_SPI_1 | QS_DRIVER_SUPPORT_SPI_2)

/*--------------------------------------------------------------------
 *
 * “¿¿µ:
 *--------------------------------------------------------------------*/
#ifdef _WIN32
	#define QS_DRIVER_SUPPORT_USB                             0
#else
	#define QS_DRIVER_SUPPORT_USB                             1
#endif // WIN32



/*--------------------------------------------------------------------
 *
 * “¿¿µ:
 *--------------------------------------------------------------------*/
#define QS_DRIVER_SUPPORT_IFLASH                          1

/*--------------------------------------------------------------------
 *
 * “¿¿µ:
 *--------------------------------------------------------------------*/
#define QS_DRIVER_SUPPORT_NVRAM                           0

/*--------------------------------------------------------------------
 *
 * “¿¿µ:
 *--------------------------------------------------------------------*/
#define QS_DRIVER_SUPPORT_FLASH                           0

 /*--------------------------------------------------------------------
 *
 * “¿¿µ:
 *--------------------------------------------------------------------*/
#define QS_DRIVER_SUPPORT_FRAM                            1

/*--------------------------------------------------------------------
 *
 * “¿¿µ:
 *--------------------------------------------------------------------*/
#define QS_DRIVER_SUPPORT_KEYBOARD                        0
#define QS_KEYBOARD_FULL                                  0

/*--------------------------------------------------------------------
 *
 * “¿¿µ:
 *--------------------------------------------------------------------*/
#define QS_DRIVER_SUPPORT_LED                             1
#define QS_LED_RED                                        1
#define QS_LED_GREEN                                      1
#define QS_LED_BLUE                                       1

/*--------------------------------------------------------------------
 *
 * “¿¿µ:
 *--------------------------------------------------------------------*/
#define QS_DRIVER_SUPPORT_BUZZER                          0

/*--------------------------------------------------------------------
 *
 * “¿¿µ:
 *--------------------------------------------------------------------*/
#define QS_DRIVER_SUPPORT_POWER                           0


 // --------------------------------------------------------
 /* Define for algo support */
#if ((QS_PLATFORM == QS_PLATFORM_WIN32) || (QS_PLATFORM == QS_PLATFORM_LINUX))
#define QS_SUPPORT_ALG                                    1
#else
#define QS_SUPPORT_ALG                                    1
#endif
#if (QS_SUPPORT_ALG)

 // RAND Support
#if ((QS_PLATFORM == QS_PLATFORM_WIN32) || (QS_PLATFORM == QS_PLATFORM_LINUX))
#define QS_SUPPORT_RAND                                   1
#else
#define QS_SUPPORT_RAND                                   2
#endif

 // SHA Support
#define QS_SUPPORT_SHA                                    1
#if (QS_SUPPORT_SHA)
#define QS_SUPPORT_SHA1                                   1
#define QS_SUPPORT_SHA224                                 0
#define QS_SUPPORT_SHA256                                 0
#define QS_SUPPORT_SHA384                                 0
#define QS_SUPPORT_SHA512                                 0
#endif

 // AES Support
#define QS_SUPPORT_AES                                    0
#if (QS_SUPPORT_AES)
#define QS_SUPPORT_AES_ECB                                2
#define QS_SUPPORT_AES_CBC                                2
#endif

 // DES Support
#if ((QS_PLATFORM == QS_PLATFORM_WIN32) || (QS_PLATFORM == QS_PLATFORM_LINUX))
#define QS_SUPPORT_DES                                    1
#else
#define QS_SUPPORT_DES                                    1
#endif

#if (QS_SUPPORT_DES)
#if ((QS_PLATFORM == QS_PLATFORM_WIN32) || (QS_PLATFORM == QS_PLATFORM_LINUX))
#define QS_SUPPORT_DES_ECB                                1
#else
#define QS_SUPPORT_DES_ECB                                1
#endif
#if ((QS_PLATFORM == QS_PLATFORM_WIN32) || (QS_PLATFORM == QS_PLATFORM_LINUX))
#define QS_SUPPORT_DES_CBC                                1
#else
#define QS_SUPPORT_DES_CBC                                1
#endif
#endif

 // CRC Support
#define QS_SUPPORT_CRC                                    1
#if (QS_SUPPORT_CRC)
#define QS_SUPPORT_CRC16                                  1
#endif

 // RSA Support
#define QS_SUPPORT_RSA                                    1

#endif

/*--------------------------------------------------------------------
 * Must >= 1
 * “¿¿µ:
 *--------------------------------------------------------------------*/
#define QS_TASK_MAX                                       5

 /*--------------------------------------------------------------------
 * 
 * “¿¿µ:
 *--------------------------------------------------------------------*/
#define QS_SUPPORT_SENV                                   1
#if(QS_SUPPORT_SENV)
#define QS_SENV_IO_NAME                                   "usb"
#define QS_SENV_IO_ADDRESS                                2
#define QS_SENV_SMALL                                     0
#endif

 /*--------------------------------------------------------------------
 *
 * “¿¿µ:
 *--------------------------------------------------------------------*/
#define QS_SUPPORT_MM                                     1


 /*--------------------------------------------------------------------
 *
 * “¿¿µ:
 *--------------------------------------------------------------------*/
#define QS_SUPPORT_DATABASE 1
#define QS_DATABASE_DEVICE_NAME "fram"
#define QS_DATABASE_START_ADDRESS (128)
#if (QS_SUPPORT_MM)
#define QS_SUPPORT_DATABASE_CIPHERTEXT 1
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _QS_CFG_H_ */
/* END OF FILE */
