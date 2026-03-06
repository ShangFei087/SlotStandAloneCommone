#ifndef __IDEA_QS_H__
#define __IDEA_QS_H__

#include "qs.h"

/**
 *  @brief
 *  @param
 *  @return
 *  @details
 */
void idea_database_exec(uint32_t millisecond);

/**
 *  @brief
 *  @param
 *  @return
 *  @details
 */
void idea_database_init(void);

/**
 *  @brief
 *  @param
 *  @return
 *  @details
 */
BOOL idea_database_connect(void* pData, size_t DataSize);


/**
 *  @brief
 *  @param
 *  @return
 *  @details
 */
void idea_task_connect(void (*pfnTask)(uint32_t millisecond));

/**
 *  @brief
 *  @param
 *  @return
 *  @details
 */
void idea_task_disconnect(void (*pfnTask)(uint32_t millisecond));

/**
 *  @brief
 *  @param
 *  @return
 *  @details
 */
void idea_task_exec(uint32_t millisecond);

/**
 *  @brief
 *  @param
 *  @return
 *  @details
 */
void idea_task_init(void);


#endif // !__IDEA_QS_H__
