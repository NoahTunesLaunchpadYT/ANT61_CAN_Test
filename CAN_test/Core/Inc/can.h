/*
 * can.h
 *
 *  Created on: Jul 4, 2024
 *      Author: noahs
 */

#ifndef INC_CAN_H_
#define INC_CAN_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32l5xx_hal.h"
#include "stm32l5xx_ll_ucpd.h"
#include "stm32l5xx_ll_bus.h"
#include "stm32l5xx_ll_cortex.h"
#include "stm32l5xx_ll_rcc.h"
#include "stm32l5xx_ll_system.h"
#include "stm32l5xx_ll_utils.h"
#include "stm32l5xx_ll_pwr.h"
#include "stm32l5xx_ll_gpio.h"
#include "stm32l5xx_ll_dma.h"

#include "stm32l5xx_ll_exti.h"


/*---------------------------Public functions---------------------------------*/

/*	function: 		initialise_CAN
 *	parameters:		None
 *	returns:		void
 *	description:	Starts CAN, enables interrupts, and builds Tx Header
 */
void initialise_CAN(FDCAN_HandleTypeDef *hfdcan1);


/*	function: 		send_angles
 *	parameters: 	hfdcan1, the can port
 *					shoulder, the angle of the shoulder joint with a vertical reference angle
 *					elbow, the angle of the elbow joint with a vertical reference angle
 *					elbow, the angle of the elbow joint with a vertical reference angle
 *					wrist, the angle of the wrist joint with a vertical reference angle
 *	returns: 		void
 *	description: 	Sends a CAN message with the four angles with a default header and waits for the CAN FIFO stack to empty.
 */
void send_angles(FDCAN_HandleTypeDef hfdcan1, uint16_t shoulder, uint16_t elbow1, uint16_t elbow2, uint16_t wrist);

/*
 * functions: 		get_x
 * parameters: 		None
 * returns: 		uint16_t
 * description:		Gets a number from 0 to 65536 giving the angle of the joints relative
 * 					to the previous joint. 65536 is one full revolution.
 */
uint16_t get_shoulder();
uint16_t get_elbow1();
uint16_t get_elbow2();
uint16_t get_wrist();

/*
 * Used to convert from steps to degrees
 */
float decode(uint16_t steps);

/*
 * Used to convert from steps to degrees
 */
uint16_t encode(float degrees);

#endif /* INC_CAN_H_ */
