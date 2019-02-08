/**************************************************************************//**
 * @file timer.h
 * @brief Timer header
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
#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>
#include <stdbool.h>
#include "em_letimer.h"
#include "gpio.h"
#include "cmu.h"
#include "sleep.h"

#define TIMER_MAX_COUNT 65535 		//(2^16)-1
#define LFXO_FREQ 32768 			//(2^15)

#define LED_ON_TIME .4  //(in seconds)
#define LED_PERIOD .5 //(in seconds) -> set to .5 for lab 3

#define LETIMER_EM_BLOCK 1 //set lowest EM to 1 for Lab 3


void letimer_init(void);

#endif /* TIMER_H_ */
