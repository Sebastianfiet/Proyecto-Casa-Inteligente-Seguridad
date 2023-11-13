/*!
\file   keypad.c
\date   2023-09-22
\author Sebastián Belalcázar Vivas <sebasbelalcazar@unicauca.edu.co>
\brief  Digital sensors and blinking led.

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

/****************************************************************************
* NAME: monitoring_sensor
*----------------------------------------------------------------------------
* PARAMS:   none
* return:   Wheter the alarm has been activated or not.
*----------------------------------------------------------------------------
* PURPOSE:
* To check the state of each individual digital sensor, if any of them activates, it will trigger the alarm.
*----------------------------------------------------------------------------
* NOTE:
* Metal and Hall Effect sensors activate with a '1'.
* Same goes for the alarm, '0' means it's inactive, and '1' means it's activated.
* The infrared sensor (SENSOR_IR) however, activates with a '0'.
*****************************************************************************/
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


/****************************************************************************
* NAME: blink_led
*----------------------------------------------------------------------------
* PARAMS:   none
* return:   none
*----------------------------------------------------------------------------
* PURPOSE:
* To make the RGB Led blink every second
*----------------------------------------------------------------------------
* NOTE:
* It configures the "PORTE" register to get the LED to alternate between ON (Red color) and OFF to get the blinking effect.
*****************************************************************************/
void blink_led(void){
    while (1) {
       PORTE = 0x01;
        __delay_ms(1000);
        PORTE = 0x00;
        __delay_ms(1000);
    }
    
}