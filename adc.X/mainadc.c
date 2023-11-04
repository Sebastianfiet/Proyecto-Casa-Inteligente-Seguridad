#include    <xc.h>
#include    <pic16f887.h>
#include    <math.h>
#include    "config.h"
#include    <stdio.h>
#include    <string.h>
#include    "lcd.h"

int value_potenciometro;
int value_temperature;
int value_fotocelda;
char buffer [17];

#define beta 4090 //the beta of the thermistor
#define resistance 10 //the value of the pull-down resistor

void adc_begin(void);
int adc_conversion(int channel);
int convertir_temperature(int temperature);

void main(void) {
    //OSCCON = 0b01110001;
    OSCCON = 0x71;
    TRISD = 0x00; // salida puerto D
    TRISA = 0x23; // salida puerto D
    ANSEL = 0x13; //  Configura el Puerto como Entrada Analógica.
    
    adc_begin();
     LCD_Init(); //Inicializa el LCD
     LCD_String_xy(0,0,"cond ambientales");
     __delay_ms(2000);
   
    while (1) {
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
        __delay_ms(2000);
        LCD_Clear();
    }
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
     ADCON0bits.ADON = 0; //  Habilita el Módulo AD.
     return ((ADRESH << 8) | ADRESL);
}

int convertir_temperature(int temperature){
    
    long a =1023 - temperature;
    //the calculating formula of temperature
    float tempC = (float) (beta /(log((1025.0 * 10 / a - 10) / 10) + beta / 298.0) - 273.0);
    float tempF = (float) (tempC + 273.15);
    return (int)tempC;
}