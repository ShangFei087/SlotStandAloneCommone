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
#ifndef _QS_BASE64_H_
#define _QS_BASE64_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */


/**************************************************************************
 *                   G E N E R A L    C O N S T A N T S                   *
 **************************************************************************/


/**************************************************************************
 *                             M A C R O S                                *
 **************************************************************************/


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


/**************************************************************************
 *                                                                       
 *  Function Name:                                      
 *                                                                        
 *  Descriptions:                                    
 *                                                                        
 *  Arguments:
 *                                                                        
 *  Returns:                                                    
 *                                                                        
 *  See also:                                                             
 *                                                                        
 **************************************************************************/
size_t qs_base64_encode(uint8_t *pData, size_t DataSize, uint8_t *pResult);

/**************************************************************************
 *                                                                       
 *  Function Name:                                      
 *                                                                        
 *  Descriptions:                                    
 *                                                                        
 *  Arguments:
 *                                                                        
 *  Returns:                                                    
 *                                                                        
 *  See also:                                                             
 *                                                                        
 **************************************************************************/
size_t qs_base64_decode(uint8_t *pData, size_t DataSize, uint8_t *pResult);

/**************************************************************************
*
*  Function Name:
*
*  Descriptions:
*
*  Arguments:
*
*  Returns:
*
*  See also:
*
**************************************************************************/
BOOL qs_base64_check(char c);

#ifdef __cplusplus
}
#endif /* __cplusplus  */


#endif /* _QS_BASE64_H_ */
/* END OF FILE */
