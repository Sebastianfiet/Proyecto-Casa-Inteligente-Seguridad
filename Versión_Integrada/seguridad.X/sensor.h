/*!
\file   sensor.h
\date   2023-08-04
\author Sebastián Belalcázar Vivas <sebasbelalcazar@unicauca.edu.co>
\brief  Sensors' pin names and functions.

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

#ifndef SENSOR_H
#define	SENSOR_H

#ifdef	__cplusplus
extern "C" {
#endif

/* this section defines "custom names" for the pins that are going to be used later on. To make the code easier to read.*/
#define SENSOR_METAL RC2
#define SENSOR_IR RC1
#define SENSOR_HALL RC0
    
#define SENSOR_FLAME RA0
#define SENSOR_TEMPERATURE RA1
#define SENSOR_LIGHT RA5

/* This section declares functions for later use.*/
unsigned char monitoring_sensor(void);
void blink_led(void);


#ifdef	__cplusplus
}
#endif

#endif	/* SENSOR_H */