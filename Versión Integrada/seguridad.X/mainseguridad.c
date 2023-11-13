/*!
\file   mainseguridad.c
\date   2023-09-22
\author Sebastián Belalcázar Vivas <sebasbelalcazar@unicauca.edu.co>
\brief  Project Casa Inteligente.

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

#include <xc.h>
#include <pic16f887.h>  /*Header file PIC16f887 definitions*/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "lcd.h"
#include "keypad.h"
#include "fuses.h"
#include "sensor.h"

#define beta 4090 //the beta of the thermistor
#define resistance 10 //the value of the pull-down resistor

enum state
{
  seguridad,
  monitoreo_digital,
  monitoreo_ambiental,
  bloqueado,
  alarma_seguridad,
  alarma_ambiental
};

enum state estado;

const char password[5] ={'2','7','0','9',0};
char pass_user[5];
unsigned char idx = 0;



unsigned char var_sensor_ir = 0;
unsigned char var_sensor_hall = 0;
unsigned char var_sensor_metal = 0;
int value_potenciometro;
int value_temperature;
int value_fotocelda;
unsigned char intentos=0;

char buffer [17];

void function_seguridad(void);
void function_monitoreo_digital(void);
void function_monitoreo_ambiental(void);
void function_bloqueado(void);
void function_alarma_seguridad(void);
void function_alarma_ambiental(void);

void adc_begin(void);
int adc_conversion(int channel);
int convertir_temperature(int temperature);

void main() {

    TRISD = 0x00; // salida puerto D
    TRISA = 0x23; // salida puerto A
    ANSEL = 0x13; //  Configura el Puerto como Entrada Analógica.
    
    TRISE = 0;
    PORTE = 0x00;
    //Configura Fosc = 8Mhz interno, Fuente de Fosc del sistema = interno
    OSCCON = 0x71; //Configura oscilador interno (FOSC = 8Mhz)
     
    LCD_Init(); //Inicializa el LCD

    keypad_init(); //Inicializa el keypad
    TRISC = 0x07;
    
    adc_begin();
    
    var_sensor_ir = SENSOR_IR;
    var_sensor_hall = SENSOR_HALL;
    var_sensor_metal = SENSOR_METAL;
    
    value_potenciometro = SENSOR_FLAME;
    value_temperature = SENSOR_TEMPERATURE;
    value_fotocelda = SENSOR_LIGHT;
    
    LCD_String_xy(0,0,"Casa");
    LCD_String_xy(2,0,"Inteligente");
    __delay_ms(2000);
    estado = seguridad;
    
    while (1) {
        if (estado == seguridad){
            function_seguridad();
        }
        else if (estado == monitoreo_digital){
            function_monitoreo_digital();
        }
        else if (estado == bloqueado){
            function_bloqueado();
        }
        else if (estado == alarma_seguridad){
            function_alarma_seguridad();
        }
        else if (estado == monitoreo_ambiental){
            function_monitoreo_ambiental();
        }
        else if (estado == alarma_ambiental){
            function_alarma_ambiental();
        }

             
  }
   
}
void function_monitoreo_digital (void) {
    if(monitoring_sensor() == 1){
        LCD_Clear();
        estado = alarma_seguridad;
          int n=sprintf (buffer, "ir=%d,hl=%d,mt=%d", var_sensor_ir, var_sensor_hall, var_sensor_metal);
           LCD_String_xy(0,0,buffer);
    }      
    else{ 
            LCD_Clear();
            estado = monitoreo_ambiental;
    }
     __delay_ms(2000);
}


void function_monitoreo_ambiental (void) {
    int value_potenciometro= adc_conversion(0);
        __delay_ms(10);
        int value_temperature= adc_conversion(1);
        int tempcelsius = convertir_temperature(value_temperature);
        __delay_ms(10);
        value_fotocelda= adc_conversion(4);
        int n=sprintf (buffer, "P=%d,T=%d",  value_potenciometro, tempcelsius);
           LCD_String_xy(1,0,buffer);
           int e=sprintf (buffer, "L=%d",  value_fotocelda);
           LCD_String_xy(2,0,buffer);
        //__delay_ms(2000);
        
    if(value_temperature > 30 ){     //|| value_potenciometro < 400
        estado = alarma_ambiental;
    }
    else{ 
           estado = monitoreo_digital;
    }
    __delay_ms(3000);
        
}


void function_seguridad(void){
    char key = '0';
    LCD_Clear();
    PORTE = 0x06;
        LCD_String_xy(0,0,"Ingrese clave");
        LCD_Command(0xC0); 
        do{
            key = keypad_getkey();
            if(key != 0){
                LCD_Char('*');/* display pressed key on LCD16x2 */
                //LCD_Char(key);/* display pressed key on LCD16x2 */
                pass_user[idx++] = key;
            }
            __delay_ms(100);
        }while(idx < 5);
        
        if(strncmp(pass_user,password,4)==0){
            LCD_Clear();
            LCD_String_xy(0,0,"Clave Correcta");
            estado = monitoreo_digital;
            PORTE = 0x02;
        }
        else{
            LCD_Clear();
            LCD_String_xy(0,0,"Clave Incorrecta"); 
            idx = 0;
            intentos++;
            PORTE = 0x03;
            __delay_ms(1000);
        }  
        if (intentos>2){
            idx = 0;
            estado=bloqueado;
        }
      
}

void function_alarma_seguridad(void){
    LCD_Clear();
    LCD_String_xy(0,0,"Alarma");
    LCD_String_xy(2,0,"Seguridad");
    blink_led();
    __delay_ms(5000);
    estado = monitoreo_digital;
}


void function_alarma_ambiental(void){
    LCD_Clear();
    LCD_String_xy(0,0,"Alarma");
    LCD_String_xy(2,0,"Ambiental");
    blink_led();
    __delay_ms(5000);
    estado = monitoreo_ambiental;
}


void function_bloqueado(void){
    LCD_Clear();
    LCD_String_xy(0,0,"sistema");
    LCD_String_xy(2,0,"bloqueado");
     PORTE = 0x01;
     idx = 0;
     intentos=0;
    __delay_ms(5000);
    estado = seguridad;
    
}


void adc_begin(void){
    ADCON1bits.ADFM = 1; //  Justificación derecha (modo-8bits).
    ADCON1bits.VCFG0 = 0; //  Selecciona Voltajes de Referencia (5v-0v).
    ADCON1bits.VCFG1 = 0; //  Selecciona Voltajes de Referencia (5v-0v). 
    ADCON0bits.ADCS = 0b01; //  Tiempo de Conversión Fosc/8.
    
}


int adc_conversion(int channel){
    
     ADCON0bits.CHS = (0x0f & channel); //  Selecciona el Canal Analógico AN0.
     ADCON0bits.ADON = 1; //  Habilita el Módulo AD.
     __delay_us(30);
     ADCON0bits.GO = 1; //  Inicia la COnversión AD.
     while (ADCON0bits.GO); //  Espera a que termine la conversión AD.
     ADCON0bits.ADON = 0; //  Deshabilita el Módulo AD.
     return ((ADRESH << 8) | ADRESL);
}


int convertir_temperature(int temperature){
    
    long a =1023 - temperature;
    //the calculating formula of temperature
    float tempC = (float) (beta /(log((1025.0 * 10 / a - 10) / 10) + beta / 298.0) - 273.0);
    float tempF = (float) (tempC + 273.15);
    return (int)tempC;
}