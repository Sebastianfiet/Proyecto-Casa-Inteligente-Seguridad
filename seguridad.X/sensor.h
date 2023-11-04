/* 
 * File:   sensor.h
 * Author: Sebastian
 *
 * Created on 4 de octubre de 2023, 10:29 AM
 */

#ifndef SENSOR_H
#define	SENSOR_H

#ifdef	__cplusplus
extern "C" {
#endif

#define SENSOR_METAL RC2
#define SENSOR_IR RC1
#define SENSOR_HALL RC0
    
unsigned char monitoring_sensor(void);
void blink_led(void);


#ifdef	__cplusplus
}
#endif

#endif	/* SENSOR_H */

