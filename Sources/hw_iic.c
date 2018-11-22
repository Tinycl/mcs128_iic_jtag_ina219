/******************************************************************************
* 		                                         COPYRIGHT (c) MOTOROLA 2002   
* FILE NAME: i2c.c (Mot basic.c)                                                      
*                                                                           
* PURPOSE: I2C demo - master software      											 
*                                                                           
*******************************************************************************
*******************************************************************************  
**  THIS CODE IS ONLY INTENDED AS AN EXAMPLE FOR THE METROWERKS COMPILER AND **  
**      THE STAR12 EVB AND HAS ONLY BEEN GIVEN A MIMIMUM LEVEL OF TEST.      **  
**  IT IS PROVIDED 'AS SEEN' WITH NO GUARANTEES AND NO PROMISE OF SUPPORT.   **  
*******************************************************************************  
*******************************************************************************
*                                                                           
* DOCUMENTATION SOURCE: EKB Apps                                          
*                                                                           
* TARGET DEVICE: 68HC912DP256                                               
*                                                                                                                                                                                                                                                            
* COMPILER: Metrowerks                     VERSION: MOT v1.2                     
*                                                                           
* DESCRIPTION:                                                                                                                                
*                                                                         
* AUTHOR: Grant More       LOCATION: EKB      LAST EDIT DATE: 20/05/02
*                                                                         
* UPDATE HISTORY                                                         
* REV      AUTHOR       DATE       DESCRIPTION OF CHANGE                  
* ---      ------      ---------    ---------------------                 
* 1.0	   G.More	   20/05/02		Initial Coding                     
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

/* include files */
#include <mc9s12dg128.h>
#include "hw_iic.h"
#include "tiny_rti.h"
#include "tiny_global_var.h"
#define MAX_IIC_WAIT_TIME 2   /* wait 2 ms before deciding bus hung */

//#pragma DATA_SEG S12_IIC   	
//tIIC 		Iic;
#pragma DATA_SEG DEFAULT

/* additional #defines */
#define SLAVE 0xF0
#define PACKETSIZE 9

/* Global variables */
tU08 TxPacket[PACKETSIZE];			/* global transmit packet */
tU08 *TxPacketpositionptr;			/* pointer to current location in transmit packet */ 
tU08 *TxPacketendptr;				/* (fixed) pointer to end of transmit packet */
tU08 TxBufferemptyflag;				/* global flag for transmit functionality */
tU08 RxPacket[PACKETSIZE];			/* global receive packet */
tU08 *RxPacketpositionptr;			/* pointer to current location in receive packet */ 
tU08 *RxPacketendptr;				/* (fixed) pointer to end of receive packet */
tU08 RxBufferfullflag = 0, MasterRxFlag = 0;	/* global flags for receive functionality */
tU08 TxCompleteflag = 0, RxCompleteflag = 0;	/* global flags for ISR functionality */
static tU08 iicErrors;

/*****************************************************************************/
/* Start of interrupt service routine  */
/* placed into non-banked memory segment as vectors are only 2 bytes */
/******************************************************************************
Function Name	:	I2C_ISR
Engineer		:	G.More	
Date			:	31/05/02
Parameters		:	NONE
Returns			:	NONE
Notes			:	Full-function interrupt service routine for I2C module.
******************************************************************************/
#pragma CODE_SEG __NEAR_SEG NON_BANKED
#pragma MESSAGE DISABLE C12056 /* SP debug info */
interrupt 31 void I2C_ISR( void )
{
  volatile tU08 dummyibdrRead;				/* variable to allow dummy reads of IBDR */

	if(IBSR & IBAL) {
	  IBSR |= IBAL;          /* Set the arbitration lost bit to reset hardware  */
	  TxPacketpositionptr = &TxPacket[0];	/* point to first TxPacket element */
    TxPacketendptr = &TxPacket[(PACKETSIZE-1)]; /* point to last TxPacket element */
    TxBufferemptyflag = CLEAR;
    RxPacketpositionptr = &RxPacket[0];	/* point to first TxPacket element */
  	RxPacketendptr = &RxPacket[(PACKETSIZE-1)]; /* point to last TxPacket element */
	  RxBufferfullflag = CLEAR;
    MasterRxFlag = 0;
    TxCompleteflag = 0; 
    RxCompleteflag = 0;
    IBSR |= IBIF;						/* reset the interrupt */
    IBCR = (IBEN|TXRX);	/* send stop signal (clear MSSL) */
	  return;
	}
  IBSR |= IBIF;						/* reset the interrupt */
  if(IBCR & MSSL)				/* if module is in master mode */
  {
    if(IBCR & TXRX)			/* if module is to transmit */
    {
      if(TxCompleteflag == SET) 			/* if the last byte has been transmitted */		
      {
		    TxCompleteflag = CLEAR;			/* clear the ISR transmit complete flag */
			  TxBufferemptyflag = SET;		/* set the non-ISR transmit buffer empty flag */
        IBCR = (IBEN|TXRX);	/* send stop signal (clear MSSL) */
			}	
      else								/* last byte has not been transmitted */                      
      {	
        if((IBSR & RXAK) == CLEAR)	/* if the slave acknowledged last transfer */
        {
          if(MasterRxFlag == SET)		/* if the last transfer was an address and master is now to Rx */
          {					
            IBCR &= ~TXRX; 		/* switch to Rx mode */                 
            dummyibdrRead = IBDR;	/* dummy read of IBDR register */
          }
          else						/* last transfer was to instruct other slave to receive */
          {
            IBDR = *TxPacketpositionptr;		/* write next byte to IBDR */
            if (TxPacketpositionptr == TxPacketendptr)	/* if the last data has been Tx'd */
              TxCompleteflag = SET;	/* set the ISR transmit complete flag */
            else /* there is still data to be transmitted */
              TxPacketpositionptr++;	/* move to next byte to Tx */
          }
        }
        else							/* slave did not acknowledge last transfer */
          IBCR = (IBEN|TXRX);	/* send stop signal (clear MSSL) */
      }
    }
    else									/* module is to receive data */
    {			
      if(RxPacketpositionptr == RxPacketendptr)/* if the last byte has been received */
      {
        IBCR = (IBEN);			/* send stop signal (clear MSSL) */
        RxCompleteflag = SET;			/* set the ISR receive complete flag */
      }
      else /* last byte has not been received yet */
      {
        if(RxPacketpositionptr == (RxPacketendptr - 1))	/* if the second last byte has been received */
          IBCR |= TXAK;	/* disable active low acknowledge bit (signal to slave to stop Tx) */
        else /* second last byte is still to be received */
        { /* do nothing */
        }
      }
			
      *RxPacketpositionptr = IBDR;	/* read the data from the slave */

      if (RxCompleteflag == SET) /* if the last byte of data has been received */
      {
        RxBufferfullflag = SET;  /* set the receive buffer full flag */
        RxCompleteflag = CLEAR;  /* clear the ISR receive complete flag */
      }
      else /* there is still data to receive */	
        RxPacketpositionptr++;		/* increment the Rx packet position pointer */
    }
  }	
}
					
#pragma CODE_SEG DEFAULT
/*******************************************************************************
Function Name	:	main
Engineer		:	G.More
Date			:	23/05/02
Parameters		:	NONE
Returns			:	NONE
Notes			:	Main program functionality
*******************************************************************************/
void HwIIC_Init(void) 
{									
  TxPacketpositionptr = &TxPacket[0];	/* point to first TxPacket element */
  TxPacketendptr = &TxPacket[(PACKETSIZE-1)]; /* point to last TxPacket element */
  TxBufferemptyflag = CLEAR;
  RxPacketpositionptr = &RxPacket[0];	/* point to first TxPacket element */
	RxPacketendptr = &RxPacket[(PACKETSIZE-1)]; /* point to last TxPacket element */
	RxBufferfullflag = CLEAR;
  MasterRxFlag = 0;
  TxCompleteflag = 0; 
  RxCompleteflag = 0;	
	
	ConfigureI2CModule();				/* initialize function */

#if 0 /* test */
 {	
	TxPacket[0] = 0x01;
	TxPacket[1] = 0x02;
	TxPacket[2] = 0x03;
	TxPacket[3] = 0x04;
	TxPacket[4] = 0x05;
	TxPacket[5] = 0x06;
	TxPacket[6] = 0x07;
	TxPacket[7] = 0x08;	
	
	MasterTransmitPacket2Slave(SLAVE,TxPacket,8);/* xmt initial packet to slave */
    	
	while(FOREVER)							/* forever loop */
	{
		MasterReceivePacketFromSlave(SLAVE,RxPacket,8);	/* receive data from slave */
		TxPacket[0] = RxPacket[0] + 1;
		TxPacket[1] = RxPacket[1] + 1;
		TxPacket[2] = RxPacket[2] + 1;
		TxPacket[3] = RxPacket[3] + 1;
		TxPacket[4] = RxPacket[4] + 1;
		TxPacket[5] = RxPacket[5] + 1;	
		TxPacket[6] = RxPacket[6] + 1;
		TxPacket[7] = RxPacket[7] + 1;		
		MasterTransmitPacket2Slave(SLAVE,RxPacket,8);	/* xmt new packet to slave */		
	}
 }
#endif
}


/*****************************************************************************
DESCRIPTION: Configure and set up I2C bus
Engineer		:	G.More
Date			:	23/05/02
Parameters		:	NONE
Returns			:	NONE
*******************************************************************************/
void ConfigureI2CModule(void)
{		
	IBFD = 0x85; 				/* frequency divider register */
	IBAD = 0xA0;			/* slave address of this module */
	IBCR = (IBEN|IBIE); 	/* enable I2C module and interrupt */
}

/******************************************************************************
  DESCRIPTION: Wait until I2C bus free
	RETURNS: TRUE if bus now free, FALSE if timed out
	NOTES:  
	ORIGINATED: 4/2/07, TaB
*******************************************************************************/
tBool WaitUntilIicBusFree( void )
{
	tU32 timeout;
  rti_iic_time = 0;
	timeout = rti_iic_time;
	while(IBSR & IBB)	/* while bus is busy */
	{
	  if((rti_iic_time - timeout) >= MAX_IIC_WAIT_TIME )
	    return FALSE;
	}
	return TRUE;
}

/******************************************************************************
  DESCRIPTION: Wait until I2C transmit buffer empty
	RETURNS: TRUE if transmit buffer now empty, FALSE if timed out
	NOTES:  
	ORIGINATED: 4/2/07, TaB
*******************************************************************************/
tBool WaitUntilIicXmtBufEmpty( void )
{
	tU32 timeout;
	rti_iic_time = 0;
	timeout = rti_iic_time;
	while(TxBufferemptyflag == CLEAR)	/* wait while message is transmitted */
	{
	  if((rti_iic_time - timeout) >= MAX_IIC_WAIT_TIME )
	    return FALSE;
	}	return TRUE;
}

/******************************************************************************
  DESCRIPTION: Wait until complete I2C message has been received
	RETURNS: TRUE if transmit buffer now empty, FALSE if timed out
	NOTES:  
	ORIGINATED: 4/2/07, TaB
*******************************************************************************/
tBool WaitUntilIicCmpltMsgRcvd( void )
{
	tU32 timeout;
  rti_iic_time = 0;
	timeout = rti_iic_time;
	while(RxBufferfullflag == CLEAR)	/* wait while message is being received */
	{
	  if((rti_iic_time - timeout) >= MAX_IIC_WAIT_TIME )
	    return FALSE;
	}
	return TRUE;
}

/******************************************************************************
  DESCRIPTION:  Turn off IIC module, and manually force 9 clocks and a stop
    condition to forcibly reset the IIC bus
  RETURNS: nothing
  ORIGINATED: 8/11/08, MER
******************************************************************************/
void ResetIicBus(void) {
  tU08 i;
  
  iicErrors++;
  IBCR = 0; 	/* disable I2C module and interrupt */
  DDRJ &= ~0xC0; /* ensure SCL and SDA lines inputs */
  PTJ &= ~0xC0; /* make output of SCL and SDA driven low */
 
  for(i = 0; i < 9; i++) { /* Send 9 clock cycles */
    DDRJ |= 0x80; /* make SCL line a driven output (pull it low) */
    delay_mcu_clocks(160);
    DDRJ &= ~0x80; /* make SCL line an input (pull up resistor will drive it high)*/
    delay_mcu_clocks(160);
  }
  DDRJ |= 0x80; /* make SCL line a driven output (pull it low) */
  delay_mcu_clocks(160); 
  DDRJ |= 0xC0; /* drive SCL and SDA low */
  delay_mcu_clocks(160);
  DDRJ &= ~0x80; /* SCL gets pulled high */
  delay_mcu_clocks(160);
  DDRJ &= ~0xC0; /* SDA transitions high while SCL is high (STOP signal) */
  HwIIC_Init(); /* restart the i2c bus */
}

tU08 GetIicErrors(void) {
  return iicErrors;
}

void ClearIicErrors(void) {
  iicErrors = 0;
}

/******************************************************************************
Function Name: MasterTransmitPacket2Slave
Engineer  : G.More
Date      :	23/05/02
Parameters: tU08 slaveAddress
Returns   : TRUE is successfully transmitted
Notes     : Procedure which sends data on the I2C bus
******************************************************************************/
tBool MasterTransmitPacket2Slave (tU08 slaveAddress, tU08 *pBuf, tU08 len)
{
  tU16 i;
  for (i=0;i<len;i++)
  {
    TxPacket[i]= pBuf[i];
  }
  TxPacketendptr = &TxPacket[(len-1)];
  
	if( !WaitUntilIicBusFree()) /* Wait until IIC bus free */
	  return FALSE; /* Bus timeout occurred */

	IBCR = (IBEN|IBIE|MSSL|TXRX);	/* grab bus - start condition initiated */
	
	IBDR = slaveAddress;		/* address the slave */				
	
	if( !WaitUntilIicXmtBufEmpty()) /* Wait until all bytes transmitted on IIC */
	  return FALSE; /* Bus timeout occurred */
	TxPacketpositionptr = &TxPacket[0];	/* point to first TxPacket element */
	TxBufferemptyflag = CLEAR;
	return TRUE;
}
	
/******************************************************************************
Function Name: MasterReceivePacketFromSlave
Engineer  : G.More
Date      : 23/05/02
Parameters: tU08 slaveAddress
Returns   : TRUE is successfully received
Notes     : Procedure which sends data on the I2C bus
******************************************************************************/
tBool MasterReceivePacketFromSlave (tU08 slaveAddress, tU08 *pBuf, tU08 len)
{
  tU16 i;
  tBool OK;
  
	MasterRxFlag = SET;
  RxPacketendptr = &RxPacket[(len-1)];
  
	if( !WaitUntilIicBusFree())  /* Wait until IIC bus free */
     return FALSE;   /* Bus timeout occurred */
     
	IBCR = (IBEN|IBIE|MSSL|TXRX);	/* grab bus - start condition initiated */

  if ( len == 1 )
  {
	  IBCR |= TXAK;	/* disable active low acknowledge bit (signal to slave to stop Tx) 
	                            This has to be here when we only want to read one byte.  */
  }
	
	IBDR = (slaveAddress | 0x01);	/* address the slave, insruct slave to transmit */				
	
	OK = WaitUntilIicCmpltMsgRcvd(); /* Wait until complete IIC message received */
	/* = FALSE if bus timeout occurred */
	for (i=0;i<len;i++)
	{
	  pBuf[i]=RxPacket[i];
  }
	RxPacketpositionptr = &RxPacket[0];	/* point to first RxPacket element */
	RxBufferfullflag = CLEAR;
	MasterRxFlag = CLEAR;
	return OK;
}	



  /*
  ina219_rx[0] = 0x00;
  if(MasterTransmitPacket2Slave(INA219_IIC_7BIT_ADDRESS_VCORE<<1,ina219_rx,1)) 
  {
      if(MasterReceivePacketFromSlave(INA219_IIC_7BIT_ADDRESS_VCORE<<1,ina219_rx,2)) 
      {
      
      }
  } 
  */