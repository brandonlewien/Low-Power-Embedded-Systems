/**************************************************************************//**
 * @file i2ctemp.h
 * @brief General I2C read functions for Si7021-A20 temp sensor module header
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

#ifndef SRC_I2CTEMP_H_
#define SRC_I2CTEMP_H_

#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "em_i2c.h"
#include "bsp.h"
#include "all.h"


void I2C_Temperature_Read_NoInterrupts(uint8_t slave_addr_rw, uint8_t cmd);
void I2C_Temperature_Read_Interrupts(uint8_t slave_addr_rw, uint8_t cmd);
void Temp_Code_To_Celsius(uint8_t MSData, uint8_t LSData, uint16_t * DataRet);

#endif /* SRC_I2CTEMP_H_ */
