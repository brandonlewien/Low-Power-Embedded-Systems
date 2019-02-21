/**************************************************************************//**
 * @file gpio.h
 * @brief General purpose input output header
 * @author Silicon Labs
 * @version 1.00
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/

#ifndef GPIO_H_
#define GPIO_H_

#include "main.h"
#include "em_gpio.h"
#include "all.h"

#define	LED0_port	gpioPortF
#define LED0_pin		    4
#define LED0_default	false 		// off

#define LED1_port	gpioPortF
#define LED1_pin		    5
#define LED1_default    false		// off

#define SCL_PIN      11
#define SDA_PIN      10
#define SENS_EN_PIN  10

#define SCL_PORT		gpioPortC
#define SDA_PORT		gpioPortC
#define SENS_EN_PORT	gpioPortB

#define ENABLE_SENSOR	 1
#define DISABLE_SENSOR 	 0
#define SCL_AND_SDA_DOUT 1
#define OFF              0

void gpio_init(void);

#endif /* GPIO_H_ */

