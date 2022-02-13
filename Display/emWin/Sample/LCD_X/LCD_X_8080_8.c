/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2019  SEGGER Microcontroller GmbH                *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V6.10 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software has been licensed to  ARM LIMITED whose registered office
is situated at  110 Fulbourn Road,  Cambridge CB1 9NJ,  England solely
for  the  purposes  of  creating  libraries  for  ARM7, ARM9, Cortex-M
series,  and   Cortex-R4   processor-based  devices,  sublicensed  and
distributed as part of the  MDK-ARM  Professional  under the terms and
conditions  of  the   End  User  License  supplied  with  the  MDK-ARM
Professional. 
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
Licensing information
Licensor:                 SEGGER Software GmbH
Licensed to:              ARM Ltd, 110 Fulbourn Road, CB1 9NJ Cambridge, UK
Licensed SEGGER software: emWin
License number:           GUI-00181
License model:            LES-SLA-20007, Agreement, effective since October 1st 2011 
Licensed product:         MDK-ARM Professional
Licensed platform:        ARM7/9, Cortex-M/R4
Licensed number of seats: -
----------------------------------------------------------------------
File        : LCD_X_8080_8.c
Purpose     : Port routines 8080 interface, 8 bit data bus
----------------------------------------------------------------------
*/

#include "GUI.h"

#include "LCD_X_8080_8.h"

/*********************************************************************
*
*       Defines: Configuration
*
**********************************************************************
  Needs to be adapted to custom hardware.
*/
#define Chip_30600
#include <IOM16C.H>

#define LCD_CLR_RESET()   P7 &= ~(1<<0)
#define LCD_SET_RESET()   P7 |=  (1<<0)
#define LCD_CLR_A0()      P8 &= ~(1<<0)
#define LCD_SET_A0()      P8 |=  (1<<0)
#define LCD_CLR_WR()      P8 &= ~(1<<1)
#define LCD_SET_WR()      P8 |=  (1<<1)
#define LCD_CLR_RD()      P8 &= ~(1<<2)
#define LCD_SET_RD()      P8 |=  (1<<2)
#define LCD_CLR_CS()      P8 &= ~(1<<4)
#define LCD_SET_CS()      P8 |=  (1<<4)
#define LCD_DATA_IN       P10
#define LCD_DATA_OUT      P10
#define LCD_SET_DIR_IN()  P10D = 0
#define LCD_SET_DIR_OUT() P10D = 0xff
#define LCD_DELAY(ms)     GUI_X_Delay(ms)

/*********************************************************************
*
*       Defines: Common
*
**********************************************************************
  Usually, there is no need to modify these macros.
  It should be sufficient ot modify the low-level macros
  above.
*/
#define LCD_X_READ(c)     \
  LCD_SET_DIR_IN();       \
  LCD_CLR_CS();           \
  LCD_CLR_RD();           \
  c = LCD_DATA_IN;        \
  LCD_SET_RD();           \
  LCD_SET_CS();           \
  LCD_SET_DIR_OUT()


#define LCD_X_WRITE(data) \
  LCD_DATA_OUT = data;    \
  LCD_CLR_CS();           \
  LCD_CLR_WR();           \
  LCD_SET_WR();           \
  LCD_SET_CS()

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       LCD_X_8080_8_Init
*
* Purpose:
*   This routine should be called from your application program
*   to set port pins to their initial values.
*   
*/
void LCD_X_8080_8_Init(void) {
  LCD_SET_CS();
  LCD_SET_RD();
  LCD_SET_WR();
  LCD_CLR_RESET();
  LCD_DELAY(20);
  LCD_SET_RESET();
  LCD_DELAY(20);
  LCD_SET_DIR_OUT();
}

/*********************************************************************
*
*       LCD_X_8080_8_Sync
*
* Purpose:
*   This routine can be used under certain conditions to synchronize
*   the communication with the display controller at startup.
*   
*/
void LCD_X_8080_8_Sync(void) {
  LCD_CLR_CS();
  LCD_SET_RD();
  LCD_CLR_A0();
  LCD_CLR_WR();
  LCD_DELAY(1);
  LCD_SET_WR();
  LCD_DELAY(1);
  LCD_CLR_WR();
  LCD_DELAY(1);
  LCD_SET_WR();
  LCD_DELAY(1);
  LCD_CLR_WR();
  LCD_DELAY(1);
  LCD_SET_WR();
  LCD_DELAY(1);
  LCD_CLR_WR();
  LCD_DELAY(1);
  LCD_SET_WR();
  LCD_DELAY(1);
  LCD_SET_A0();
  LCD_SET_CS();
  LCD_DELAY(50);
}

/*********************************************************************
*
*       LCD_X_8080_8_Write00
*
* Purpose:
*   Write to controller, with A0 = 0
*/
void LCD_X_8080_8_Write00(unsigned char c) {
  LCD_CLR_A0();
  LCD_X_WRITE(c);
}

/*********************************************************************
*
*       LCD_X_8080_8_Write01
*
* Purpose:
*   Write to controller, with A0 = 1
*/
void LCD_X_8080_8_Write01(unsigned char c) {
  LCD_SET_A0();
  LCD_X_WRITE(c);
}

/*********************************************************************
*
*       LCD_X_8080_8_WriteM00
*
* Purpose:
*   Write multiple bytes to controller, with A0 = 0
*/
void LCD_X_8080_8_WriteM00(unsigned char * pData, int NumBytes) {
  LCD_CLR_A0();
  for (; NumBytes; NumBytes--) {
    LCD_X_WRITE(*pData++);
  }
}

/*********************************************************************
*
*       LCD_X_8080_8_WriteM01
*
* Purpose:
*   Write multiple bytes to controller, with A0 = 1
*/
void LCD_X_8080_8_WriteM01(unsigned char * pData, int NumBytes) {
  LCD_SET_A0();
  for (; NumBytes; NumBytes--) {
    LCD_X_WRITE(*pData++);
  }
}

/*********************************************************************
*
*       LCD_X_8080_8_Read00
*
* Purpose:
*   Read from controller, with A0 = 0
*/
unsigned char LCD_X_8080_8_Read00(void) {
  unsigned char c;
  LCD_CLR_A0();
  LCD_X_READ(c);
  return c;
}

/*********************************************************************
*
*       LCD_X_8080_8_Read01
*
* Purpose:
*   Read from controller, with A0 = 1
*/
unsigned char LCD_X_8080_8_Read01(void) {
  unsigned char c;
  LCD_SET_A0();
  LCD_X_READ(c);
  return c;
}

/*********************************************************************
*
*       LCD_X_8080_8_ReadM00
*
* Purpose:
*   Read multiple bytes from controller, with A0 = 0
*/
void LCD_X_8080_8_ReadM00(unsigned char * pData, int NumBytes) {
  LCD_SET_A0();
  for (; NumBytes; NumBytes--) {
    LCD_X_READ(*pData);
    pData++;
  }
}

/*********************************************************************
*
*       LCD_X_8080_8_ReadM01
*
* Purpose:
*   Read multiple bytes from controller, with A0 = 1
*/
/* Read multiple bytes from controller, with A0 = 1 */
void LCD_X_8080_8_ReadM01(unsigned char * pData, int NumBytes) {
  LCD_SET_A0();
  for (; NumBytes; NumBytes--) {
    LCD_X_READ(*pData);
    pData++;
  }
}

/*************************** End of file ****************************/
