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
#ifndef _QS_LIST_H_
#define _QS_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */


/**************************************************************************
 *                   G E N E R A L    C O N S T A N T S                   *
 **************************************************************************/


/**************************************************************************
 *                             M A C R O S                                *
 **************************************************************************/
/// node 管理结构体
typedef struct _qs_list_node {
	void *pObject;                 ///< node 对象指针
	struct _qs_list_node *next;   ///< 下一个对象的指针
} qs_list_node;

/// list 管理结构体
typedef struct _qs_list {
	size_t size;                   ///< node 对象数量
	qs_list_node *pFront;         ///< 第一个node指针
	qs_list_node *pRear;          ///< 最后一个node指针
} qs_list;

/**************************************************************************
 *                         D A T A   T Y P E S                            *
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
 *   \brief    申请list管理对象句柄
 *   \returns  #qs_list 管理对象的指针, #NULL 申请失败
 */
qs_list *qs_list_alloc(void);

/**
 *   \brief    释放list管理对象句柄
 *   \param    pList     list管理对象句柄
 *   \param    f         #TRUE 释放对象, #FALSE 不释放对象
 *   \returns  #NULL
 */
qs_list *qs_list_free(qs_list *pList, BOOL f);

/**
 *   \brief    得到list中对象列表中对象的数量
 *   \param    pList     list管理对象句柄
 *   \returns  对象列表中对象的数量
 */
size_t qs_list_size(qs_list *pList);

/**
 *   \brief    向list对象列表前端中追加一个对象
 *   \param    pList     list管理对象句柄
 *   \param    pObject   对象指针
 *   \returns  #TRUE 成功, #FALSE 失败
 */
BOOL qs_list_append_to_front(qs_list *pList, void *pObject);

/**
 *   \brief    向list对象列表末端中追加一个对象
 *   \param    pList     list管理对象句柄
 *   \param    pObject   对象指针
 *   \returns  #TRUE 成功, #FALSE 失败
 */
BOOL qs_list_append_to_rear(qs_list *pList, void *pObject);

/**
 *   \brief    从list对象列表前端中移除一个对象
 *   \param    pList     list管理对象句柄
 *   \returns  对象指针或NULL
 */
void *qs_list_remove_from_front(qs_list *pList);

/**
 *   \brief    从list对象列表末端中移除一个对象
 *   \param    pList     list管理对象句柄
 *   \returns  对象指针或NULL
 */
void *qs_list_remove_from_rear(qs_list *pList);

/**
 *   \brief    从list对象列表移除指定的对象
 *   \param    pList     list管理对象句柄
 *   \param    pObject   对象指针
 *   \returns  #TRUE 成功, #FALSE 失败
 */
BOOL qs_list_remove_by_object(qs_list *pList, void *pObject);

/**
 *   \brief    从list对象列表移除指定的对象
 *   \param    pList     list管理对象句柄
 *   \param    index     索引号
 *   \returns  对象指针或NULL
 */
void *qs_list_remove_by_index(qs_list *pList, uint32_t index);

/**
 *   \brief    从list对象列表中得到指定对象的指针
 *   \param    pList     list管理对象句柄
 *   \param    index     索引号
 *   \returns  对象指针或NULL
 */
void *qs_list_get(qs_list *pList, uint32_t index);


#ifdef __cplusplus
}
#endif /* __cplusplus  */


#endif /* _QS_LIST_H_ */
/* END OF FILE */
