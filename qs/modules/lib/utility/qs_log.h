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
#ifndef _QS_LOG_H_
#define _QS_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

#if (QS_PLATFORM == QS_PLATFORM_LINUX)
#define LOG_TAG "QSoft"
#include <utils/Log.h>
#endif

/**************************************************************************
 *                   G E N E R A L    C O N S T A N T S                   *
 **************************************************************************/


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
 *   \brief    ��ʽ��������ݵ�ָ����������
 *   \param    dest      ���������
 *   \param    format    ��ʽ����ʽ�ַ���
 *   \param    ...       �����б�
 */
void qs_vprintf(char *dest, const char *format, ...);

/**
 *   \brief    ��ʽ��������ݵ���ʾ�豸��
 *   \param    format    ��ʽ����ʽ�ַ���
 *   \param    ...       �����б�
 */
void qs_log(const char *format, ...);

/**
 *   \brief    16���Ƹ�ʽ������ݵ���ʾ�豸��
 *   \param    desc      ������ݵ�����
 *   \param    pData     ��Ҫ��������ݵ�ַ
 *   \param    DataSize  ���ݵ�����
 */
void qs_log_buf(const char *desc, uint8_t *pData, size_t DataSize);

/**
 *   \brief    ���ַ�����ʾ���豸��
 *   \param    desc      �ַ���������
 *   \param    pStr      ��Ҫ��������ַ�����ַ
 */
void qs_log_str(const char *desc, const char *pStr);

/**
 *   \brief    �ַ�����ʽ������ݵ���ʾ�豸��
 *   \param    desc      ������ݵ�����
 *   \param    pData     ��Ҫ��������ݵ�ַ
 *   \param    DataSize  ���ݵ�����
 */
void qs_log_str_by_size(const char *desc, uint8_t *pData, size_t DataSize);

/**
 *   \brief    ��ʾ�豸�ĳ�ʼ��
 */
void qs_log_init(void);

/// \sa printf
#if (QS_PLATFORM == QS_PLATFORM_WIN32)
#define qs_log                       printf
#elif (QS_PLATFORM == QS_PLATFORM_LINUX)
#define qs_log                       ALOGW
#endif

/// \sa qs_log
#define QS_LOG                       qs_log

/// \sa qs_log_buf
#define QS_LOG_BUF                   qs_log_buf

/// \sa qs_log_str
#define QS_LOG_STR                   qs_log_str

/// \sa qs_log_str_by_size
#define QS_LOG_STR_BY_SIZE           qs_log_str_by_size


#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

#endif //#ifndef _QS_LOG_H_
/* END OF FILE */
