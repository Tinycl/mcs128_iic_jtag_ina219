/*****************************************************************************/
/*                                             COPYRIGHT (c) MOTOROLA 2002   */
/*                                                                           */
/* File Name          :    $RCSfile: s12_common.h,v $                        */
/*                                                                           */
/* Current Revision :      $Revision: 1.2 $                                  */
/*                                                                           */
/* PURPOSE: header file for HCS12 common definitions                         */
/*                                                                           */
/*  ***********************************************************************  */
/*  *  THIS CODE IS ONLY INTENDED AS AN EXAMPLE OF CODE FOR THE           *  */
/*  *  METROWERKS COMPILER AND THE STAR12 EVB AND HAS ONLY BEEN GIVEN A   *  */
/*  *  MIMIMUM LEVEL OF TEST. IT IS PROVIDED 'AS SEEN' WITH NO GUARANTEES *  */
/*  *  AND NO PROMISE OF SUPPORT.                                         *  */
/*  ***********************************************************************  */
/*                                                                           */
/*                                                                           */
/* DESCRIPTION:  common definitions for HCS12 core registers block.          */
/* The standard definitions used to describe register datastructures.        */
/* The following conventions are used -                                      */           
/*                                                                           */
/* names starting with 's' followed by a capital letter denote structs       */
/* names starting with 'u' followed by a capital letter denote unions        */
/* names starting with 't' followed by a capital letter denote typedefs      */
/*                                                                           */
/* MCU register definition is done in separate files, describing each        */
/* peripheral register block as a datastructure. Register naming follows as  */ 
/* close as possible the names used in the device specification.             */
/* Registers containing individual flags are defined as a union of the byte  */ 
/* and as a bitfield structure.                                              */
/*                                                                           */
/* Compiler issues (portability) -                                           */
/* ANSI C defines bitfields as 'int', obviously a problem when defining 8 bit*/
/* registers, most compilers have options to modify this.                    */
/* Cosmic compiler doesn't have a switch, but allows bitfields to be defined */ 
/* as chars.                                                                 */
/* ANSI C does not define bitfield ordering (LSB first in used), other       */
/* compilers may require modification to these files or use of a compiler    */
/* switch.                                                                   */
/*                                                                           */
/*                                                                           */
/* AUTHOR: r32151        LOCATION: SPSEKB          LAST EDIT DATE: 11/03/02  */
/*                                                                           */
/* UPDATE HISTORY                                                            */
/* REV  AUTHOR       DATE       DESCRIPTION OF CHANGE                        */
/* ---  ------      --------    ---------------------                        */
/* 1.00	r32151		28/04/00    - 				                             */
/* 1.01 r32151		10/05/00 	- fixed embedded comment on line 39          */
/*     							- added closing comment        		         */
/* 1.00 r32151    	26/07/01	- changed CPU id MCSDP256 to S12DP256        */
/*                              - Added disclaimer and reformated file info. */
/* 1.1  r32151    	11/03/02	- Added definitions for TRUE & FALSE         */
/*                              - cleaned up file info                       */
/*                              - Modified revision numbering to match RCS   */
/*                                                                           */
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

#ifndef S12_COMMON_H        /*prevent duplicated includes*/
#define S12_COMMON_H


/* Device selection - currently only the MC9S12DP256 is supported so this*/
/*  switch is not implemeted in current versions of s12 header files. However*/
/*  including it in an application build will help ensure correct operation*/
/*  if future devices are released with different module memory maps.*/

/* #define S12DP256      specify MC9S12DP256 as target device*/
/* #define S12xxx        specify MC9S12xxx as target device*/

/****

Above definitions have been moved to the compiler command line with -DS12DP256.

****/

typedef unsigned char   tBool;  /*boolean definition */
typedef unsigned char   tU08;   /*unsigned 8 bit definition */
typedef unsigned int    tU16;   /*unsigned 16 bit definition*/
typedef unsigned long   tU32;   /*unsigned 32 bit definition*/

typedef signed char     tS08;   /*signed 8 bit definition */
typedef signed int      tS16;   /*signed 16 bit definition*/
typedef signed long     tS32;   /*signed 32 bit definition*/
#pragma MESSAGE DISABLE C1106 /* Non-Standard bitfield type */
typedef union uREG08    /*8 bit register with byte and bit access*/
  {
  tU08  byte;           /*access whole register e.g. var.byte   = 0xFF;*/
  struct                /*access bit at a time  e.g. var.bit._7 = 1;   */
    {
    tU08 _0 :1;
    tU08 _1 :1;
    tU08 _2 :1;
    tU08 _3 :1;
    tU08 _4 :1;
    tU08 _5 :1;
    tU08 _6 :1;
    tU08 _7 :1;  
    }bit;
  }tREG08;

typedef union uREG16    /*16 bit register with word and byte access*/
  {
  tU16  word;           /*access whole word    */
  struct                /*access byte at a time*/
    {
    tREG08  msb;
    tREG08  lsb;
    }byte;
  }tREG16;

/******************************************************************************
Common Bit masks
******************************************************************************/

#if 0
#define BIT0    0x01    
#define BIT1    0x02
#define BIT2    0x04
#define BIT3    0x08
#define BIT4    0x10
#define BIT5    0x20
#define BIT6    0x40
#define BIT7    0x80
#endif

/******************************************************************************
Standard Definitions
******************************************************************************/

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/******************************************************************************
Standard Enumerations
******************************************************************************/


#endif /*S12_COMMON_H */
