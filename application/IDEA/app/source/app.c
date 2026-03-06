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
#include "idea_app.h"

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

//static void fram_test(void)
//{
//	uint32_t i;
//	size_t size = 0;
//	uint32_t pos = 0;
//	uint8_t buf[64];
//	
//	qs_driver *pDriver = qs_driver_open("fram");
//	
//	qs_driver_ioctl(pDriver, QS_DRIVER_IOCTL_SET_POS, &pos);
//	size = qs_driver_read(pDriver, buf, sizeof(buf));
//	qs_log_buf("---read0---", buf, size);
//	
//	for(i = 0;i < sizeof(buf);i++) {
//		buf[i] = i;
//	}
//	qs_driver_ioctl(pDriver, QS_DRIVER_IOCTL_SET_POS, &pos);
//	size = qs_driver_write(pDriver, buf, sizeof(buf));
//	
//	qs_driver_ioctl(pDriver, QS_DRIVER_IOCTL_SET_POS, &pos);
//	size = qs_driver_read(pDriver, buf, sizeof(buf));
//	qs_log_buf("---read1---", buf, size);	
//}

static void task_0(uint32_t millisecond)
{
	idea_app_exec(millisecond);
}

/**
 *  @brief          
 *  @param          
 *  @return         
 *  @details        
 */
void app_init(void)
{
	idea_task_connect(task_0);

	idea_app_init();
	//QS_LOG("\r\n call app_init=======>");
}

/* END OF FILE */
