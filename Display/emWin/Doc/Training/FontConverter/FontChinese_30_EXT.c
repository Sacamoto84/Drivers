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
* Height:      30                                                    *
*                                                                    *
**********************************************************************
*                                                                    *
* Initial font height: 30                                            *
* Range disabled: 0000 - FFFF                                        *
* Read pattern file: C:\Work\emWin_Training\Samples\FontConverter\Chinese_Unicode.txt*
*                                                                    *
**********************************************************************
*/

#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

/* The following line needs to be included in any file selecting the
   font. A good place would be GUIConf.H
*/
extern GUI_CONST_STORAGE GUI_FONT GUI_FontChinese_30;

/* Start of unicode area <CJK Symbols and Punctuation> */
GUI_CONST_STORAGE unsigned char acGUI_FontChinese_30_3002[  7] = { /* code 3002, IDEOGRAPHIC FULL STOP */
  ________,
  _XXXXX__,
  XX___X__,
  X_____X_,
  X_____X_,
  _X___X__,
  __XXX___};

/* Start of unicode area <CJK Unified Ideographs> */
GUI_CONST_STORAGE unsigned char acGUI_FontChinese_30_4E00[ 12] = { /* code 4E00, <CJK Ideograph, First> */
  ________,________,_______X,X_______,
  ________,________,______XX,XX______,
  _XXXXXXX,XXXXXXXX,XXXXXX__,________};

GUI_CONST_STORAGE unsigned char acGUI_FontChinese_30_54C1[ 78] = { /* code 54C1 */
  ____X___,________,X_______,
  ____XXXX,XXXXXXXX,XX______,
  ____XX__,________,X_______,
  ____XX__,________,X_______,
  ____XX__,________,X_______,
  ____XX__,________,X_______,
  ____XX__,________,X_______,
  ____XX__,________,X_______,
  ____XX__,________,X_______,
  ____XXXX,XXXXXXXX,XX______,
  ____XX__,________,XX______,
  ____XX__,________,X_______,
  ________,________,________,
  ________,________,_____X__,
  XXXXXXXX,X____XXX,XXXXXXX_,
  X______X,X____X__,____XX__,
  X______X,X____X__,____XX__,
  X______X,X____X__,____XX__,
  X______X,X____X__,____XX__,
  X______X,X____X__,____XX__,
  X______X,X____X__,____XX__,
  X______X,X____X__,____XX__,
  XXXXXXXX,X____XXX,XXXXXX__,
  X______X,X____X__,____XX__,
  X______X,X____X__,____XX__,
  X_______,_____X__,____X___};

GUI_CONST_STORAGE unsigned char acGUI_FontChinese_30_5916[108] = { /* code 5916 */
  ______XX,________,_XX_____,________,
  ______XX,X_______,_XX_____,________,
  ______XX,________,_XX_____,________,
  ______XX,________,_XX_____,________,
  _____XX_,________,_XX_____,________,
  _____XX_,____X___,_XX_____,________,
  _____XX_,____XX__,_XX_____,________,
  ____XX_X,XXXXXX__,_XX_____,________,
  ____XX__,___XXX__,_XX_____,________,
  ____X___,___XX___,_XX_____,________,
  ___XX___,___XX___,_XXX____,________,
  ___XXX__,___X____,_XX_XX__,________,
  __XX_XXX,__XX____,_XX__XXX,________,
  __X___XX,__XX____,_XX___XX,X_______,
  _X_____X,XXX_____,_XX____X,XX______,
  X______X,_XX_____,_XX_____,XX______,
  ________,XX______,_XX_____,________,
  ________,XX______,_XX_____,________,
  _______X,X_______,_XX_____,________,
  _______X,________,_XX_____,________,
  ______XX,________,_XX_____,________,
  _____XX_,________,_XX_____,________,
  ____XX__,________,_XX_____,________,
  ___X____,________,_XX_____,________,
  __X_____,________,_XX_____,________,
  XX______,________,_XX_____,________,
  ________,________,_X______,________};

GUI_CONST_STORAGE unsigned char acGUI_FontChinese_30_662F[104] = { /* code 662F */
  ______X_,________,__XX____,________,
  ______XX,XXXXXXXX,XXXXX___,________,
  ______XX,________,__XX____,________,
  ______XX,________,__XX____,________,
  ______XX,________,__XX____,________,
  ______XX,XXXXXXXX,XXXX____,________,
  ______XX,________,__XX____,________,
  ______XX,________,__XX____,________,
  ______XX,________,__XX____,________,
  ______XX,XXXXXXXX,XXXX____,________,
  ______XX,________,__XX____,________,
  ______X_,________,_______X,________,
  ________,________,______XX,X_______,
  XXXXXXXX,XXXXXXXX,XXXXXXXX,XX______,
  _____X__,____XX__,________,________,
  ______XX,____XX__,________,________,
  ______XX,____XX__,_____X__,________,
  _____XX_,____XX__,____XXX_,________,
  _____XX_,____XXXX,XXXX____,________,
  _____XX_,____XX__,________,________,
  ____XX_X,____XX__,________,________,
  ____X___,XX__XX__,________,________,
  ___XX___,_XXXXX__,________,________,
  __XX____,___XXXX_,________,________,
  _XX_____,_____XXX,XXXXXXXX,XX______,
  XX______,________,_XXXXXXX,________};

GUI_CONST_STORAGE unsigned char acGUI_FontChinese_30_6837[112] = { /* code 6837 */
  ________,________,________,________,
  _____XX_,____X___,______XX,________,
  _____XX_,_____XX_,_____XXX,________,
  _____XX_,______XX,_____XX_,________,
  _____XX_,______XX,X___XX__,________,
  _____XX_,_______X,X___X___,________,
  _____XX_,_X_____X,___X____,X_______,
  XXXXXXXX,XXXXXXXX,XXXXXXXX,XX______,
  _____XX_,________,_XX_____,________,
  _____XX_,________,_XX_____,________,
  _____XX_,________,_XX_____,________,
  ____XXX_,________,_XX____X,________,
  ____XXXX,X___XXXX,XXXXXXXX,X_______,
  ____XXX_,XX______,_XX_____,________,
  ___XXXX_,_XX_____,_XX_____,________,
  ___X_XX_,_X______,_XX_____,________,
  ___X_XX_,________,_XX_____,________,
  __X__XX_,________,_XX_____,XX______,
  __X__XX_,__XXXXXX,XXXXXXXX,XXX_____,
  _X___XX_,________,_XX_____,________,
  X____XX_,________,_XX_____,________,
  _____XX_,________,_XX_____,________,
  _____XX_,________,_XX_____,________,
  _____XX_,________,_XX_____,________,
  _____XX_,________,_XX_____,________,
  _____XX_,________,_XX_____,________,
  _____XX_,________,_XX_____,________,
  _____X__,________,_X______,________};

GUI_CONST_STORAGE unsigned char acGUI_FontChinese_30_7684[ 84] = { /* code 7684 */
  ________,________,________,
  ____XX__,______XX,________,
  ____XX__,______XX,X_______,
  ___XX___,______XX,________,
  ___X____,______XX,________,
  ___X____,_____XX_,________,
  X_X____X,X____X__,_____XX_,
  XX_XXXXX,XX__XXXX,XXXXXXXX,
  X______X,X___XX__,_____XX_,
  X______X,X__XX___,_____XX_,
  X______X,X__X____,_____XX_,
  X______X,X_X_____,_____XX_,
  X______X,XX__X___,_____XX_,
  X______X,X____X__,_____XX_,
  X______X,X_____X_,_____X__,
  XXXXXXXX,X_____XX,_____X__,
  X______X,X_____XX,_____X__,
  X______X,X_____XX,_____X__,
  X______X,X_______,_____X__,
  X______X,X_______,_____X__,
  X______X,X_______,____XX__,
  X______X,X_______,____XX__,
  X______X,X_______,____XX__,
  X______X,X_______,____XX__,
  XXXXXXXX,X______X,____XX__,
  X______X,X_______,XXXXX___,
  X______X,________,__XXX___,
  ________,________,__X_____};

GUI_CONST_STORAGE unsigned char acGUI_FontChinese_30_8BED[104] = { /* code 8BED */
  ___X____,________,_______X,X_______,
  ____XX__,__XXXXXX,XXXXXXXX,XX______,
  ____XX__,_______X,________,________,
  _____X__,_______X,________,________,
  _____X__,______XX,________,________,
  ________,______XX,_____XX_,________,
  ________,___XXXXX,XXXXXXX_,________,
  ________,______XX,_____XX_,________,
  _____X__,______XX,_____XX_,________,
  _XXXXXX_,______X_,_____XX_,________,
  _____X__,______X_,_____XX_,_X______,
  _____X__,XXXXXXXX,XXXXXXXX,XXX_____,
  _____X__,________,________,________,
  _____X__,________,________,________,
  _____X__,________,________,________,
  _____X__,____XXXX,XXXXXXXX,X_______,
  _____X__,____X___,______XX,________,
  _____X__,____X___,______XX,________,
  _____X__,XX__X___,______XX,________,
  _____X_X,X___X___,______XX,________,
  ____XXXX,____X___,______XX,________,
  ____XXX_,____X___,______XX,________,
  _____X__,____X___,______XX,________,
  ________,____XXXX,XXXXXXXX,________,
  ________,____X___,______XX,________,
  ________,________,________,________};

GUI_CONST_STORAGE unsigned char acGUI_FontChinese_30_8FD9[104] = { /* code 8FD9 */
  ___X____,_______X,________,________,
  ____XX__,________,XX______,________,
  ____XXX_,________,XX______,________,
  _____XX_,________,_X______,________,
  _____X__,________,_X______,X_______,
  ________,________,_______X,XX______,
  ________,__XXXXXX,XXXXXXXX,XXX_____,
  ________,________,____XX__,________,
  ________,________,____X___,________,
  _____XX_,___X____,___XX___,________,
  _XXXXXX_,____X___,___XX___,________,
  _____XX_,_____XX_,___X____,________,
  _____XX_,_______X,X_XX____,________,
  _____XX_,________,XXX_____,________,
  _____XX_,________,_XX_____,________,
  _____XX_,________,XXXX____,________,
  _____XX_,_______X,X__XXX__,________,
  _____XX_,______XX,____XXX_,________,
  _____XX_,_____XX_,_____XXX,________,
  _____XX_,____X___,______XX,________,
  ____XXX_,__XX____,______XX,________,
  __XXX__X,_X______,_______X,________,
  XXX_____,XX______,________,________,
  _XX_____,_XXXXX__,________,_XXX____,
  ________,___XXXXX,XXXXXXXX,XX______,
  ________,________,_XXXXXXX,X_______};

GUI_CONST_STORAGE unsigned char acGUI_FontChinese_30_95E8[ 78] = { /* code 95E8 */
  __X_____,________,________,
  ___XX___,________,________,
  ___XXX__,________,___XX___,
  ____XX__,_XXXXXXX,XXXXX___,
  ____XX__,________,___XX___,
  X_______,________,___XX___,
  XX______,________,___XX___,
  X_______,________,___XX___,
  X_______,________,___XX___,
  X_______,________,___XX___,
  X_______,________,___XX___,
  X_______,________,___XX___,
  X_______,________,___XX___,
  X_______,________,___XX___,
  X_______,________,___XX___,
  X_______,________,___XX___,
  X_______,________,___XX___,
  X_______,________,___XX___,
  X_______,________,___XX___,
  X_______,________,___XX___,
  X_______,________,___XX___,
  X_______,________,___XX___,
  X_______,________,___XX___,
  X_______,______XX,X_XXX___,
  X_______,________,XXXX____,
  XX______,________,__XX____};

GUI_CONST_STORAGE GUI_CHARINFO_EXT GUI_FontChinese_30_CharInfo[10] = {
   {   7,   7,   4,  19,  30, acGUI_FontChinese_30_3002 } /* code 3002, IDEOGRAPHIC FULL STOP */
  ,{  26,   3,   2,  12,  30, acGUI_FontChinese_30_4E00 } /* code 4E00, <CJK Ideograph, First> */
  ,{  23,  26,   4,   2,  30, acGUI_FontChinese_30_54C1 } /* code 54C1 */
  ,{  26,  27,   2,   2,  30, acGUI_FontChinese_30_5916 } /* code 5916 */
  ,{  26,  26,   2,   2,  30, acGUI_FontChinese_30_662F } /* code 662F */
  ,{  27,  28,   1,   1,  30, acGUI_FontChinese_30_6837 } /* code 6837 */
  ,{  24,  28,   4,   1,  30, acGUI_FontChinese_30_7684 } /* code 7684 */
  ,{  28,  26,   1,   3,  30, acGUI_FontChinese_30_8BED } /* code 8BED */
  ,{  28,  26,   1,   2,  30, acGUI_FontChinese_30_8FD9 } /* code 8FD9 */
  ,{  22,  26,   5,   2,  30, acGUI_FontChinese_30_95E8 } /* code 95E8 */
};

GUI_CONST_STORAGE GUI_FONT_PROP_EXT GUI_FontChinese_30_Prop10 = {
   0x95E8 /* first character */
  ,0x95E8 /* last character  */
  ,&GUI_FontChinese_30_CharInfo[  9] /* address of first character */
  ,(GUI_CONST_STORAGE GUI_FONT_PROP_EXT *)0 /* pointer to next GUI_FONT_PROP_EXT */
};

GUI_CONST_STORAGE GUI_FONT_PROP_EXT GUI_FontChinese_30_Prop9 = {
   0x8FD9 /* first character */
  ,0x8FD9 /* last character  */
  ,&GUI_FontChinese_30_CharInfo[  8] /* address of first character */
  ,&GUI_FontChinese_30_Prop10 /* pointer to next GUI_FONT_PROP_EXT */
};

GUI_CONST_STORAGE GUI_FONT_PROP_EXT GUI_FontChinese_30_Prop8 = {
   0x8BED /* first character */
  ,0x8BED /* last character  */
  ,&GUI_FontChinese_30_CharInfo[  7] /* address of first character */
  ,&GUI_FontChinese_30_Prop9 /* pointer to next GUI_FONT_PROP_EXT */
};

GUI_CONST_STORAGE GUI_FONT_PROP_EXT GUI_FontChinese_30_Prop7 = {
   0x7684 /* first character */
  ,0x7684 /* last character  */
  ,&GUI_FontChinese_30_CharInfo[  6] /* address of first character */
  ,&GUI_FontChinese_30_Prop8 /* pointer to next GUI_FONT_PROP_EXT */
};

GUI_CONST_STORAGE GUI_FONT_PROP_EXT GUI_FontChinese_30_Prop6 = {
   0x6837 /* first character */
  ,0x6837 /* last character  */
  ,&GUI_FontChinese_30_CharInfo[  5] /* address of first character */
  ,&GUI_FontChinese_30_Prop7 /* pointer to next GUI_FONT_PROP_EXT */
};

GUI_CONST_STORAGE GUI_FONT_PROP_EXT GUI_FontChinese_30_Prop5 = {
   0x662F /* first character */
  ,0x662F /* last character  */
  ,&GUI_FontChinese_30_CharInfo[  4] /* address of first character */
  ,&GUI_FontChinese_30_Prop6 /* pointer to next GUI_FONT_PROP_EXT */
};

GUI_CONST_STORAGE GUI_FONT_PROP_EXT GUI_FontChinese_30_Prop4 = {
   0x5916 /* first character */
  ,0x5916 /* last character  */
  ,&GUI_FontChinese_30_CharInfo[  3] /* address of first character */
  ,&GUI_FontChinese_30_Prop5 /* pointer to next GUI_FONT_PROP_EXT */
};

GUI_CONST_STORAGE GUI_FONT_PROP_EXT GUI_FontChinese_30_Prop3 = {
   0x54C1 /* first character */
  ,0x54C1 /* last character  */
  ,&GUI_FontChinese_30_CharInfo[  2] /* address of first character */
  ,&GUI_FontChinese_30_Prop4 /* pointer to next GUI_FONT_PROP_EXT */
};

GUI_CONST_STORAGE GUI_FONT_PROP_EXT GUI_FontChinese_30_Prop2 = {
   0x4E00 /* first character */
  ,0x4E00 /* last character  */
  ,&GUI_FontChinese_30_CharInfo[  1] /* address of first character */
  ,&GUI_FontChinese_30_Prop3 /* pointer to next GUI_FONT_PROP_EXT */
};

GUI_CONST_STORAGE GUI_FONT_PROP_EXT GUI_FontChinese_30_Prop1 = {
   0x3002 /* first character */
  ,0x3002 /* last character  */
  ,&GUI_FontChinese_30_CharInfo[  0] /* address of first character */
  ,&GUI_FontChinese_30_Prop2 /* pointer to next GUI_FONT_PROP_EXT */
};

GUI_CONST_STORAGE GUI_FONT GUI_FontChinese_30 = {
   GUI_FONTTYPE_PROP_EXT /* type of font    */
  ,30 /* height of font  */
  ,30 /* space of font y */
  ,1 /* magnification x */
  ,1 /* magnification y */
  ,{&GUI_FontChinese_30_Prop1}
  ,26 /* Baseline */
  ,13 /* Height of lowercase characters */
  ,21 /* Height of capital characters */
};

