/**************************************************************************//**
 * @file i2c.h
 * @brief General i2c header
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

#ifndef I2C_H_
#define I2C_H_

#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "em_i2c.h"
#include "bsp.h"
#include "all.h"

#define I2C_EM_BLOCK 1				// lowest energy mode is 2, so block 3

#define I2C_WRITE 0
#define I2C_READ 1

/* temp sensor commands */
#define MEAS_REL_HUM_HOLD 			0xE5
#define MEAS_REL_HUM_NO_HOLD		0xF5
#define	MEAS_TEMP_HOLD				0xE3
#define	MEAS_TEMP_NO_HOLD			0xF3
#define	READ_PREV_TEMP				0xE0
#define RESET						0xFE
#define USER_REG_1_W				0xE6
#define USER_REG_1_R				0xE7
#define	HEAT_CTL_REG_W				0x51
#define HEAT_CTL_REG_R				0x11
//#define READ_ID_B1
//#define READ_ID_B1
//#define READ_FIRM_REV

#define USR_REG1_RESET				0x3A
#define USR_REG1_12BIT_RES			0x3B


#define CORE_FREQUENCY              14000000
#define I2C_SLAVE_ADDRESS           0x40
#define I2C_RXBUFFER_SIZE           20


void I2C_Setup(void);
void I2C_Reset_Bus(void);

void I2C_Write_to_Reg_NoInterrupts(uint8_t slave_addr_rw, uint8_t cmd, uint8_t data);
uint8_t I2C_Read_from_Reg_NoInterrupts(uint8_t slave_addr_rw, uint8_t cmd);
//uint16_t I2C_Read_Measurement(uint8_t slave_addr_rw, uint8_t cmd);

void I2C_Write_Interrupts(uint8_t slave_addr, uint8_t cmd, uint8_t data);
void I2C_Read_Interrupts(uint8_t slave_addr, uint8_t cmd);

void I2C_Interrupt_Enable(void);
void I2C_Interrupt_Disable(void);

#endif /* I2C_H_ */
