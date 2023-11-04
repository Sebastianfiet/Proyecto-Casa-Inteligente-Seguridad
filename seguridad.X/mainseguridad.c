/*!
\file   mainseguridad.c
\date   2023-09-22
\author Sebastián Belalcázar Vivas <sebasbelalcazar@unicauca.edu.co>
\brief  Project Casa Inteligente.
\par Copyright
Information contained herein is proprietary to and constitutes valuable
confidential trade secrets of unicauca, and is subject to restrictions on use and disclosure.
\par
Copyright (c) unicauca 2023. All rights reserved.
\par
The copyright notices above do not evidence any actual or intended publication of this material.
******************************************************************************
*/
#include <xc.h>
#include <pic16f887.h>  /*Header file PIC16f887 definitions*/
#include <stdio.h>
#include <string.h>
#include "lcd.h"
#include "keypad.h"
#include "fuses.h"
#include "sensor.h"

enum state
{
  seguridad,
  monitoreo,
  bloqueado,
  alarma 
};

enum state estado; //Enum Para corregir error

const char password[5] ={'2','7','0','9',0};
char pass_user[5];
unsigned char idx = 0;



unsigned char var_sensor_ir = 0;
unsigned char var_sensor_hall = 0;
unsigned char var_sensor_metal = 0;
unsigned char intentos=0;

char buffer [17];

void function_seguridad(void);
void function_monitoreo(void);
void function_bloqueado(void);
void function_alarma(void);

void main() {

    TRISE = 0;
    PORTE = 0x00;
    //Configura Fosc = 8Mhz interno, Fuente de Fosc del sistema = interno
    OSCCON = 0x71; //Configura oscilador interno (FOSC = 8Mhz)
     
    LCD_Init(); //Inicializa el LCD

    keypad_init(); //Inicializa el keypad
    TRISC = 0x07;
    
    
    var_sensor_ir = SENSOR_IR;
    var_sensor_hall = SENSOR_HALL;
    var_sensor_metal = SENSOR_METAL;
    
    estado = seguridad;
    
    while (1) {
        if (estado == seguridad){
            function_seguridad();
        }
        else if (estado == monitoreo){
            function_monitoreo();
        }
        else if (estado == bloqueado){
            function_bloqueado();
        }
        else if (estado == alarma){
            function_alarma();
        }

        
        
        
  }
   
}
void function_monitoreo (void) {
    if(monitoring_sensor() == 1){
        estado = alarma;
          int n=sprintf (buffer, "ir=%d,hl=%d,mt=%d", var_sensor_ir, var_sensor_hall, var_sensor_metal);
           LCD_String_xy(0,0,buffer);
            __delay_ms(500);
    }
}

void function_seguridad(void){
    char key = '0';
    LCD_Clear();
        LCD_String_xy(0,0,"Press a Key");
        LCD_Command(0xC0); 
        do{
            key = keypad_getkey();
            if(key != 0){
                //LCD_Char('*');/* display pressed key on LCD16x2 */
                LCD_Char(key);/* display pressed key on LCD16x2 */
                pass_user[idx++] = key;
            }
            __delay_ms(100);
        }while(idx < 5);
        
        if(strncmp(pass_user,password,4)==0){
            LCD_Clear();
            LCD_String_xy(0,0,"Clave Correcta");
            estado = monitoreo;
            idx = 0;
            PORTE = 0x02;
        }
        else{
            LCD_Clear();
            LCD_String_xy(0,0,"Clave Incorrecta"); 
            intentos++;
            PORTE = 0x01;
        }  
        if (intentos>3){
            idx = 0;
            estado=bloqueado;
        }
      
}

void function_alarma(void){
    LCD_Clear();
    LCD_String_xy(0,0,"Alarma ?");
     PORTE = 0x04;
    __delay_ms(500);
    estado = monitoreo;
}


void function_bloqueado(void){
    LCD_Clear();
    LCD_String_xy(0,0,"sistema bloqueado");
     PORTE = 0x04;
    __delay_ms(500);
    estado = seguridad;
    
}