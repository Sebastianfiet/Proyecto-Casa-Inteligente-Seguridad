/*!
\file   keypad.c
\date   2023-09-22
\author Sebastián Belalcázar Vivas <sebasbelalcazar@unicauca.edu.co>
\brief  Keypad sensor IR, HALL and Metal.

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
#include "sensor.h"
#include "fuses.h"

unsigned char monitoring_sensor(void){
        unsigned char alarm_activ=0;
    if(SENSOR_IR == 0){
        alarm_activ=1;
    }
    if(SENSOR_HALL == 1){
        alarm_activ=1;
    }
    if(SENSOR_METAL == 1){
        alarm_activ=1;
    }
    return alarm_activ;
 }

void blink_led(void){
    while (1) {
       PORTE = 0x01;
        __delay_ms(500);
        PORTE = 0x00;
        __delay_ms(500);
    }
    
}