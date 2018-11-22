/************************hard ware iic with ack*******************************/
/****************************************************************************
*                                             COPYRIGHT (c) MOTOROLA 2000   
* FILE NAME: i2c.h (Mot:basic.h)                                                        
*                                                                           
* PURPOSE: header file for i2c.c (Mot basic.c)      		 							 
*                                                                           
*  ***********************************************************************  
*  *  THIS CODE IS ONLY INTENDED AS AN EXAMPLE OF CODE FOR THE           *  
*  *  METROWERKS COMPILER AND THE STAR12 EVB AND HAS ONLY BEEN GIVEN A   *  
*  *  MIMIMUM LEVEL OF TEST. IT IS PROVIDED 'AS SEEN' WITH NO GUARANTEES *  
*  *  AND NO PROMISE OF SUPPORT.                                         *  
*  ***********************************************************************  
*                                                                           
*                                                                           
* DESCRIPTION:  definitions for 'basic' application parameters 			 
*                                                                           
*                                                                           
*                                                                           
* AUTHOR: Martyn Gallop    LOCATION: EKB Apps     LAST EDIT DATE: 24.07.01  
*                                                                           
* UPDATE HISTORY                                                            
* REV      AUTHOR       DATE       DESCRIPTION OF CHANGE                    
* ---      ------      ---------   ---------------------                    
* 1.0      M.Gallop    15/07/01	   Original coding                                      
*
*                                                                           
******************************************************************************/                                                                        


/*===========================================================================*/
/* Motorola reserves the right to make changes without further notice to any */
/* product herein to improve reliability, function, or design. Motorola does */
/* not assume any  liability arising  out  of the  application or use of any */
/* product,  circuit, or software described herein;  neither  does it convey */
/* any license under its patent rights  nor the  rights of others.  Motorola */
/* products are not designed, intended,  or authorized for use as components */
/* in  systems  intended  for  surgical  implant  into  the  body, or  other */
/* applications intended to support life, or  for any  other application  in */
/* which the failure of the Motorola product  could create a situation where */
/* personal injury or death may occur. Should Buyer purchase or use Motorola */
/* products for any such intended  or unauthorized  application, Buyer shall */
/* indemnify and  hold  Motorola  and its officers, employees, subsidiaries, */
/* affiliates,  and distributors harmless against all claims costs, damages, */
/* and expenses, and reasonable  attorney  fees arising  out of, directly or */
/* indirectly,  any claim of personal injury  or death  associated with such */
/* unintended or unauthorized use, even if such claim alleges that  Motorola */
/* was negligent regarding the  design  or manufacture of the part. Motorola */
/* and the Motorola logo* are registered trademarks of Motorola Ltd.         */
/*****************************************************************************/
#ifndef __HW_IIC_H__
#define __HW_IIC_H__

/*Include files */

#include <hidef.h>
#include "s12_iic.h"

/* additional common definitions  - others can be found in HIDEF.H*/

#define OFF     0
#define ON      1
#define CLEAR 	0
#define SET     1

#define FOREVER 1

#define FAIL			0
#define PASS    		1

/* Function Prototypes */
#pragma CODE_SEG __NEAR_SEG NON_BANKED
extern interrupt 31 void I2C_ISR( void );

#pragma CODE_SEG DEFAULT

void HwIIC_Init(void);
void ConfigureI2CModule(void);
tBool MasterTransmitPacket2Slave (tU08 addr, tU08 *pBuf, tU08 len);
tBool MasterReceivePacketFromSlave(tU08 addr, tU08 *pBuf, tU08 len);
void ResetIicBus(void);
tU08 GetIicErrors(void);
void ClearIicErrors(void);

#endif

