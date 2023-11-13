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

/*This section defines the states for the state machine.*/
enum state
{
  seguridad,
  monitoreo_digital,
  monitoreo_ambiental,
  bloqueado,
  alarma_seguridad,
  alarma_ambiental
};

/*All the states and it's changes will be stored in "estado".*/
enum state estado;

/*Password for the security state.*/
const char password[5] ={'2','7','0','9',0};
char pass_user[5];
unsigned char idx = 0;


/*Variables to store the values returned by the analog and digital sensors.*/
unsigned char var_sensor_ir = 0;
unsigned char var_sensor_hall = 0;
unsigned char var_sensor_metal = 0;
int value_potenciometro;
int value_temperature;
int value_fotocelda;
unsigned char intentos=0;

char buffer [17];

/*Declare the functions needed for the file.*/
void function_seguridad(void);
void function_monitoreo_digital(void);
void function_monitoreo_ambiental(void);
void function_bloqueado(void);
void function_alarma_seguridad(void);
void function_alarma_ambiental(void);

void adc_begin(void);
int adc_conversion(int channel);
int convertir_temperature(int temperature);


/****************************************************************************
* NAME: main
*----------------------------------------------------------------------------
* PARAMS:   none
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Make the necessary register configurations for the analog and digital sensors, RGB Led, the LCD and the Keypad to work properly.
* Initialize the LCD, Keypad and the ADC.
* Store the values of the sensors in the variables that were declared earlier.
* Define a while cycle for the code to know which function call depending of the state it's currently in.
*----------------------------------------------------------------------------
* NOTE:
* the state by default the system will initialize in is "seguridad".
* The flame sensor will be known as "potenciometro" in the code.
*****************************************************************************/
void main() {

    TRISD = 0x00; /* Configures all of the PORTD pins to be outputs.*/
    TRISA = 0x23; /* Configures the corresponding PORTA pins to be outputs.*/
    ANSEL = 0x13; /* Configures the input model to allow analog functions on the pin to operate correctly.*/
    
    TRISE = 0; /* Configures all of the PORTE pins to be outputs.*/
    PORTE = 0x00;
    OSCCON = 0x71; /* Configures Internal Oscillator (FOSC = 8Mhz).*/
     
    LCD_Init(); /* Initializes the LCD.*/

    keypad_init(); /* Initializes the keypad.*/
    TRISC = 0x07;
    
    adc_begin(); /* Initializes the ADC (Analog to digital converter).*/
    
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


/****************************************************************************
* NAME: function_monitoreo_digital
*----------------------------------------------------------------------------
* PARAMS:   none
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* It shows the information given by the digital sensors, and if any of them activate, the system will go to the "alarma_seguridad" state.
* If none of them activate after 2 seconds, it will go the "monitoreo_ambiental" state.
*----------------------------------------------------------------------------
* NOTE:
*
*****************************************************************************/
void function_monitoreo_digital (void) {
    if(monitoring_sensor() == 1){
        LCD_Clear();
        estado = alarma_seguridad;
          int n=sprintf (buffer, "ir=%d,hl=%d,mt=%d", var_sensor_ir, var_sensor_hall, var_sensor_metal); /*Display digital sensors' values on LCD 16x2.*/
           LCD_String_xy(0,0,buffer);
    }      
    else{ 
            LCD_Clear();
            estado = monitoreo_ambiental;
    }
     __delay_ms(2000);
}


/****************************************************************************
* NAME: function_monitoreo_ambiental
*----------------------------------------------------------------------------
* PARAMS:   none
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* It shows the information given by the analog sensors. 
* If the temperature sensor detects a temperature value higher than 30°C, the system will go to the "alarma_ambiental" state.
* If the condition above doesn't happen after 3 seconds, it will go back to the  "monitoreo_digital" state.
*----------------------------------------------------------------------------
* NOTE:
*
*****************************************************************************/
void function_monitoreo_ambiental (void) {
    int value_potenciometro= adc_conversion(0);
        __delay_ms(10);
        int value_temperature= adc_conversion(1);
        int tempcelsius = convertir_temperature(value_temperature);
        __delay_ms(10);
        value_fotocelda= adc_conversion(4);
        int n=sprintf (buffer, "P=%d,T=%d",  value_potenciometro, tempcelsius); /*Display flame and temperature sensors' values on LCD 16x2.*/
           LCD_String_xy(1,0,buffer);
           int e=sprintf (buffer, "L=%d",  value_fotocelda); /*Display photoresistor sensor values on LCD 16x2.*/
           LCD_String_xy(2,0,buffer);
        //__delay_ms(2000);
        
    if(value_temperature > 30 ){
        estado = alarma_ambiental;
    }
    else{ 
           estado = monitoreo_digital;
    }
    __delay_ms(3000);
        
}


/****************************************************************************
* NAME: function_seguridad
*----------------------------------------------------------------------------
* PARAMS:   none
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Allow the user to type a password which will be shown on the LCD Screen. 
* If the password is correct, the system will go to the "monitoreo_digital" state.
* If the password is incorrect, the user will have to type the password again.
* If the user has failed to type the password correctly 3 times, the system will go to the "bloqueado" state.
*----------------------------------------------------------------------------
* NOTE:
* The LCD 16x2 display the characters pressed by the user in an encrypted way, that is displaying "*" instead of the pressed key.
* While not required, the RGB LED will show a yellow color if the password is incorrect, and a red color if the system is blocked.
* This change was made to show a bigger contrast between getting the password wrong and actually blocking the system.
*****************************************************************************/
void function_seguridad(void){
    char key = '0';
    LCD_Clear();
    PORTE = 0x06;
    /*Asking the user to type the password. The RGB Led will show a blue color while waiting.*/
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
        
        /*The password is correct, the RGB Led will show a green color and the system will go to the "monitoreo_digital" state.*/
        if(strncmp(pass_user,password,4)==0){
            LCD_Clear();
            LCD_String_xy(0,0,"Clave Correcta");
            estado = monitoreo_digital;
            PORTE = 0x02;
        }
        /*The password is incorrect, the RGB Led will show a yellow color and the system will ask the user to try again.*/
        else{
            LCD_Clear();
            LCD_String_xy(0,0,"Clave Incorrecta"); 
            idx = 0;
            intentos++;
            PORTE = 0x03;
            __delay_ms(1000);
        }
        
        /*The user got the password wrong 3 times, the system will go to the "bloqueado" state.*/
        if (intentos>2){
            idx = 0;
            estado=bloqueado;
        }
      
}


/****************************************************************************
* NAME: function_alarma_seguridad
*----------------------------------------------------------------------------
* PARAMS:   none
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* This is the state where the system goes if any of the digital sensors activate while in "monitoreo_digital" state.
* It prints a message on the LCD 16x2 screen and blinks the RGB Led (Red color) while in here.
*----------------------------------------------------------------------------
* NOTE:
* After 5 seconds, it will go back to the "monitoreo_digital" state.
*****************************************************************************/
void function_alarma_seguridad(void){
    LCD_Clear();
    LCD_String_xy(0,0,"Alarma");
    LCD_String_xy(2,0,"Seguridad");
    blink_led();
    __delay_ms(5000);
    estado = monitoreo_digital;
}


/****************************************************************************
* NAME: function_alarma_ambiental
*----------------------------------------------------------------------------
* PARAMS:   none
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* This is the state where the system goes if the temperature is higher than 30°C while in "monitoreo_ambiental" state.
* It prints a message on the LCD 16x2 screen and blinks the RGB Led (Red color) while in here.
*----------------------------------------------------------------------------
* NOTE:
* After 5 seconds, it will go back to the "monitoreo_ambiental" state.
*****************************************************************************/
void function_alarma_ambiental(void){
    LCD_Clear();
    LCD_String_xy(0,0,"Alarma");
    LCD_String_xy(2,0,"Ambiental");
    blink_led();
    __delay_ms(5000);
    estado = monitoreo_ambiental;
}


/****************************************************************************
* NAME: function_bloqueado
*----------------------------------------------------------------------------
* PARAMS:   none
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* This is the state where the system goes if the user gets the password wrong 3 times in a row while in "seguridad" state.
* It prints a message on the LCD 16x2 screen and puts the RGB Led on Red color.
*----------------------------------------------------------------------------
* NOTE:
* After 10 seconds, it will go back to the "seguridad" state.
* It resets the number of tries the user has to type the password correctly.
*****************************************************************************/
void function_bloqueado(void){
    LCD_Clear();
    LCD_String_xy(0,0,"sistema");
    LCD_String_xy(2,0,"bloqueado");
     PORTE = 0x01;
     idx = 0;
     intentos=0;
    __delay_ms(10000);
    estado = seguridad;
    
}


/****************************************************************************
* NAME: adc_begin
*----------------------------------------------------------------------------
* PARAMS:   none
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* Initialize the analog to digital converter. With it's the necessary settings.
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
void adc_begin(void){
    ADCON1bits.ADFM = 1; /* A/D Conversion Result Format Select bit ('1'= Right justified). */
    ADCON1bits.VCFG0 = 0; /*  Voltage Reference bit (5v-0v). */
    ADCON1bits.VCFG1 = 0; /*  Voltage Reference bit (5v-0v). */
    ADCON0bits.ADCS = 0b01; /* A/D Conversion Clock Select bits (Fosc/8). */
    
}

/****************************************************************************
* NAME: adc_conversion
*----------------------------------------------------------------------------
* PARAMS:   channel: Analog channel where the conversion will be made.
* return:   Result of the conversion.
*----------------------------------------------------------------------------
* PURPOSE:
* Execute the analog to digital converter in the specified channel.
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
int adc_conversion(int channel){
    
     ADCON0bits.CHS = (0x0f & channel); /* Selects the desired analog channel. */
     ADCON0bits.ADON = 1; /* ADC Enable bit ('1'= ADC is enabled). */
     __delay_us(30);
     ADCON0bits.GO = 1; /* A/D Conversion Status bit ('1'= A/D conversion cycle in progress). */
     while (ADCON0bits.GO); /* Waiting for the ADC to complete. */
     ADCON0bits.ADON = 0; /* ADC Enable bit ('0'= ADC is disabled). */
     return ((ADRESH << 8) | ADRESL);
}


/****************************************************************************
* NAME: convertir_temperature
*----------------------------------------------------------------------------
* PARAMS:   temperature: Raw value returned by the temperature analog sensor.
* return:   tempC: proper temperature value in Celsius degrees.
*----------------------------------------------------------------------------
* PURPOSE:
* Turn the temperature given by the sensor into Celsius degrees.
*----------------------------------------------------------------------------
* NOTE:
* 
*****************************************************************************/
int convertir_temperature(int temperature){
    
    long a =1023 - temperature;
    /*the calculating formula of temperature*/
    float tempC = (float) (beta /(log((1025.0 * 10 / a - 10) / 10) + beta / 298.0) - 273.0);
    float tempF = (float) (tempC + 273.15); /*Unused Fahrenheit converter.*/
    return (int)tempC;
}