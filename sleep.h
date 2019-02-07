/**************************************************************************//**
 * @file sleep.h
 * @brief Sleep header
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
#ifndef SLEEP_H_
#define SLEEP_H_

//***********************************************************************************
// Include files
//***********************************************************************************
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "gpio.h"


//***********************************************************************************
// defined files
//***********************************************************************************

#define SLEEP_LOWEST_ENERGY_MODE_DEFAULT EnergyMode3
#define SLEEP_FLAG_NO_CLOCK_RESTORE      0x1u
#define MAX_NUM_SLEEP_MODES 5

//***********************************************************************************
// global variables
//***********************************************************************************


typedef enum{
	EnergyMode0 = 0,
	EnergyMode1 = 1,
	EnergyMode2 = 2,
	EnergyMode3 = 3,
	EnergyMode4 = 4
} EM;

typedef struct {
	bool (*sleepCallback)(EM emode);
	void (*wakeupCallback)(EM emode);
	uint32_t (*restoreCallback)(EM emode);
} Sleeper;

//****************************
//*******************************************************
// function prototypes
//***********************************************************************************

void Sleep_Block_Mode(unsigned int EM);
void Sleep_UnBlock_Mode(unsigned int EM);
void Sleep_Init();
void Enter_Sleep(void);
EM Enter_Lowest_EM_Mode(void);



#endif /* SLEEP_H_ */
