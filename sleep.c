/**************************************************************************//**
 * @file sleep.c
 * @brief Sleep class
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

#include "sleep.h"
#include <em_core.h>
#include "em_emu.h"


#define MAX_EM_Element 5

static uint8_t sleepBlockEnable[MAX_EM_Element];     // max number of nested blocks is (2^8)-1 = 255


/******************************************************************************
 * @brief Block energy mode on mode assigned by input EM
 * @param EM: energy mode to block on
 * @return sleepBlockEnable: global variable modified with new sleep mode block
 *****************************************************************************/
void Sleep_Block_Mode(unsigned int EM) {
    CORE_ATOMIC_IRQ_DISABLE();
    if(sleepBlockEnable[EM] < 255){
       sleepBlockEnable[EM]++;                       // add block nesting to energy mode EM
    }
    CORE_ATOMIC_IRQ_ENABLE();
}

/******************************************************************************
 * @brief Unlock energy mode on mode assigned by input EM
 * @param EM: energy mode to unblock on
 * @return sleepBlockEnable: global variable modified with new sleep mode unblock
 *****************************************************************************/
void Sleep_UnBlock_Mode(unsigned int EM) {
    CORE_ATOMIC_IRQ_DISABLE();
    if(sleepBlockEnable[EM] > 0){                    // check that energy mode is blocked
       sleepBlockEnable[EM]--;                       // subtract block nesting to energy mode EM
    }
    CORE_ATOMIC_IRQ_ENABLE();
}

/******************************************************************************
 * @brief Initialize energy modes to have no blocks
 * @param none
 * @return sleepBlockEnable: set all modes to have no blocks
 *****************************************************************************/
void Sleep_Init(void) {
    sleepBlockEnable[EnergyMode0] = 0;
    sleepBlockEnable[EnergyMode1] = 0;
    sleepBlockEnable[EnergyMode2] = 0;
    sleepBlockEnable[EnergyMode3] = 0;
    sleepBlockEnable[EnergyMode4] = 0;
}

/******************************************************************************
 * @brief Enter lowest unblocked sleep mode
 * @param sleepBlockEnable: used to determine which energy mode to enter based
 *        on which modes are currently blocked
 * @return none
 *****************************************************************************/
void Enter_Sleep(void) {
    if (sleepBlockEnable[0] > 0) {
       return;
    }
    else if (sleepBlockEnable[1] > 0) {
       return;
    }
    else if (sleepBlockEnable[2] > 0) {
       EMU_EnterEM1();
    }
    else if (sleepBlockEnable[3] > 0) {
       EMU_EnterEM2(true);
    }
    else {
       EMU_EnterEM3(true);
    }
    return;
}
