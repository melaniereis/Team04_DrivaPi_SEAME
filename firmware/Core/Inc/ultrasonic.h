/*
 * ultrasonic.h
 *
 *  Created on: Feb 13, 2026
 *      Author: hugofslopes
 */

#ifndef INC_ULTRASONIC_H_
#define INC_ULTRASONIC_H_

#include "app_threadx.h"

// SRF08 Commands
#define SRF08_ADDR          0xE0
#define CMD_REG             0x00
#define RANGE_REG           0x02
#define CMD_CENTIMETERS     0x51

// Safety Thresholds
#define BRAKE_THRESHOLD_CM  17   // Hard Stop
#define TTC_THRESHOLD_MS    450 // Predictive Stop
#define DT_SECONDS          0.057f


#endif /* INC_ULTRASONIC_H_ */
