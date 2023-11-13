/*!
\file   sensor.h
\date   2023-09-22
\author Sebastián Belalcázar Vivas <sebasbelalcazar@unicauca.edu.co>
\brief  Functions Sensors.

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

#define SENSOR_METAL RC2
#define SENSOR_IR RC1
#define SENSOR_HALL RC0
    
#define SENSOR_FLAME RA0
#define SENSOR_TEMPERATURE RA1
#define SENSOR_LIGHT RA5
    
unsigned char monitoring_sensor(void);
void blink_led(void);


#ifdef	__cplusplus
}
#endif

#endif	/* SENSOR_H */