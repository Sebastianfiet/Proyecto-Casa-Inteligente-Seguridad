/*!
\file   keypad.h
\date   2023-09-22
\author Sebasti�n Belalc�zar Vivas <sebasbelalcazar@unicauca.edu.co>
\brief  Functions Keypad 4x4.

\par Copyright
Information contained herein is proprietary to and constitutes valuable
confidential trade secrets of unicauca, and 
is subject to restrictions on use and disclosure.

\par
Copyright (c) unicauca 2023. All rights reserved.

\par
The copyright notices above do not evidence any actual or intended publication of this material.
******************************************************************************
*/

#ifndef KEYPAD_H
#define	KEYPAD_H

#ifdef	__cplusplus
extern "C" {
#endif

/*Assigns a symbolic name for every bit of PORTD's pins */    
#define PB7     7 
#define PB6     6
#define PB5     5
#define PB4     4
#define PB3     3
#define PB2     2
#define PB1     1
#define PB0     0

/* FILAS-SALIDAS */
#define ROW1   PB0
#define ROW2   PB1
#define ROW3   PB2
#define ROW4   PB3
    
/* COLUMNAS-ENTRADAS */
#define COL1   PB4
#define COL2   PB5
#define COL3   PB6
#define COL4   PB7

/*define macros for PORTB and TRISB Registers*/
#define PTECLADO  TRISB
#define FILAS     PORTB
#define COLUMNAS  PORTB

/*declares functions for their use in the source file*/
void keypad_init (void);
char keypad_getkey(void);
    
#ifdef	__cplusplus
}
#endif

#endif	/* KEYPAD_H */