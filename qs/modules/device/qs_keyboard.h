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
#ifndef _QS_KEYBOARD_H_
#define _QS_KEYBOARD_H_

#ifdef __cplusplus
extern "C" {
#endif

#if(QS_DRIVER_SUPPORT_KEYBOARD)
/**************************************************************************
 *                   G E N E R A L    C O N S T A N T S                   *
 **************************************************************************/
#define QS_KEYVALUE_NULL              0

#define QS_KEYVALUE_ENTER             13
#define QS_KEYVALUE_ESCAPE            27
#define QS_KEYVALUE_UP                38
#define QS_KEYVALUE_DOWN              40
#define QS_KEYVALUE_LEFT              37
#define QS_KEYVALUE_RIGHT             39
#define QS_KEYVALUE_POWER             5
#define QS_KEYVALUE_MENU              18
#if(QS_KEYBOARD_FULL)
#define QS_KEYVALUE_0                 '0'
#define QS_KEYVALUE_1                 '1'
#define QS_KEYVALUE_2                 '2'
#define QS_KEYVALUE_3                 '3'
#define QS_KEYVALUE_4                 '4'
#define QS_KEYVALUE_5                 '5'
#define QS_KEYVALUE_6                 '6'
#define QS_KEYVALUE_7                 '7'
#define QS_KEYVALUE_8                 '8'
#define QS_KEYVALUE_9                 '9'
#define QS_KEYVALUE_BACKSPACE         8
#define QS_KEYVALUE_F1                112
#define QS_KEYVALUE_HELP              '?'
#define QS_KEYVALUE_SPACE             ' '
#define QS_KEYVALUE_POUND             '#'
#define QS_KEYVALUE_ASTERISK          '*'
#define QS_KEYVALUE_DOT               '.'
#define QS_KEYVALUE_RFU0              '?'
#define QS_KEYVALUE_RFU1              '?'
#define QS_KEYVALUE_RFU2              '?'
#define QS_KEYVALUE_RFU3              '?'
#define QS_KEYVALUE_RFU4              '?'
#define QS_KEYVALUE_RFU5              '?'
#define QS_KEYVALUE_RFU6              '?'
#endif

#define QS_KEYVALUE_MASK_ENTER        (1 << 0)
#define QS_KEYVALUE_MASK_ESCAPE       (1 << 1)
#define QS_KEYVALUE_MASK_UP           (1 << 2)
#define QS_KEYVALUE_MASK_DOWN         (1 << 3)
#define QS_KEYVALUE_MASK_LEFT         (1 << 4)
#define QS_KEYVALUE_MASK_RIGHT        (1 << 5)
#define QS_KEYVALUE_MASK_POWER        (1 << 6)
#define QS_KEYVALUE_MASK_MENU         (1 << 7)
#if(QS_KEYBOARD_FULL)
#define QS_KEYVALUE_MASK_0            (1 << 8)
#define QS_KEYVALUE_MASK_1            (1 << 9)
#define QS_KEYVALUE_MASK_2            (1 << 10)
#define QS_KEYVALUE_MASK_3            (1 << 11)
#define QS_KEYVALUE_MASK_4            (1 << 12)
#define QS_KEYVALUE_MASK_5            (1 << 13)
#define QS_KEYVALUE_MASK_6            (1 << 14)
#define QS_KEYVALUE_MASK_7            (1 << 15)
#define QS_KEYVALUE_MASK_8            (1 << 16)
#define QS_KEYVALUE_MASK_9            (1 << 17)

#define QS_KEYVALUE_MASK_BACKSPACE    (1 << 18)
#define QS_KEYVALUE_MASK_F1           (1 << 19)
#define QS_KEYVALUE_MASK_HELP         (1 << 20)
#define QS_KEYVALUE_MASK_SPACE        (1 << 21)
#define QS_KEYVALUE_MASK_POUND        (1 << 22)
#define QS_KEYVALUE_MASK_ASTERISK     (1 << 23)
#define QS_KEYVALUE_MASK_DOT          (1 << 24)
#define QS_KEYVALUE_MASK_RFU0         (1 << 25)
#define QS_KEYVALUE_MASK_RFU1         (1 << 26)
#define QS_KEYVALUE_MASK_RFU2         (1 << 27)
#define QS_KEYVALUE_MASK_RFU3         (1 << 28)
#define QS_KEYVALUE_MASK_RFU4         (1 << 29)
#define QS_KEYVALUE_MASK_RFU5         (1 << 30)
#define QS_KEYVALUE_MASK_RFU6         (1 << 31)
#endif


/**************************************************************************
 *                         D A T A   T Y P E S                            *
 **************************************************************************/


/**************************************************************************
 *                             M A C R O S                                *
 **************************************************************************/


/**************************************************************************
 *                 E X T E R N A L   R E F E R E N C E S                  *
 **************************************************************************/


/**************************************************************************
 *                        G L O B A L   D A T A                           *
 **************************************************************************/


/**************************************************************************
 *               F U N C T I O N   D E C L A R A T I O N S                *
 **************************************************************************/


/**
 *  @brief          
 *  @param          
 *  @return         
 *  @details        
 */
BOOL qs_keyboard_value_inquiry(uint32_t* pValue, uint32_t* pPressCounter);

/**
 *  @brief          
 *  @param          
 *  @return         
 *  @details        
 */
BOOL qs_keyboard_value(uint32_t *pValue, uint32_t *pPressCounter);

/**
 *  @brief          
 *  @param          
 *  @return         
 *  @details        
 */
void qs_keyboard_exec(uint32_t millisecond);

/**
 *  @brief          
 *  @param          
 *  @return         
 *  @details        
 */
void qs_keyboard_init(void);

#endif

#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

#endif //#ifndef _QS_KEYBOARD_H_
/* END OF FILE */
