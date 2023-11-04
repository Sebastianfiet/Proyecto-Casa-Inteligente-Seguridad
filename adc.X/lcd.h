/*!
\file lcd.h
\date   2023-10-23
\author Sebastián Belalcázar Vivas <sebasbelalcazar@unicauca.edu.co>
\brief Example test MCU PIC16F887.
\par Copyright
Information contained herein is proprietary to and constitutes valuable confidential trade secrets of unicauca, and
is subject to restrictions on use and disclosure.
\par
Copyright (c) unicauca 2023. All rights reserved.
\par
The copyright notices above do not evidence any actual or intended publication of this material.
*/

#ifndef LCD_H
#define	LCD_H

#ifdef	__cplusplus
extern "C" {
#endif
   
#define RS RD0  /*PIN 0 of PORTD is assigned for register select Pin of LCD */
#define EN RD1  /*PIN 1 of PORTD is assigned for enable Pin of LCD */
#define ldata PORTD  /*PORTD(PD4-PD7) is assigned for LCD Data Output*/ 
#define LCD_Port TRISD  /*define macros for PORTD Direction Register*/
    
void LCD_Init(void);                   /*Initialize LCD*/
void LCD_Command(unsigned char );  /*Send command to LCD*/
void LCD_Char(unsigned char x);    /*Send data to LCD*/
void LCD_String(const char *);     /*Display data string on LCD*/
void LCD_String_xy(char, char , const char *);
void LCD_Clear(void);                  /*Clear LCD Screen*/


#ifdef	__cplusplus
}
#endif
#endif
