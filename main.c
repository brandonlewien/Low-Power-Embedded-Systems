/***************************************************************************//**
 * @file
 * @brief Simple LED Blink Demo for SLSTK3402A
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "em_letimer.h"
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "bsp.h"

#include "sleep.h"
#include "main.h"
#include "gpio.h"
#include "timer.h"
#include "cmu.h"
#include "i2c.h"


int main(void){
  EMU_DCDCInit_TypeDef dcdcInit = EMU_DCDCINIT_DEFAULT;
  CMU_HFXOInit_TypeDef hfxoInit = CMU_HFXOINIT_DEFAULT;
  EMU_EM23Init_TypeDef em23Init = EMU_EM23INIT_DEFAULT;

  CHIP_Init(); 											  	// Chip errata

  EMU_DCDCInit(&dcdcInit);									// init DCDC regulator
  em23Init.vScaleEM23Voltage = emuVScaleEM23_LowPower;		// always start in low noise mode
  EMU_EM23Init(&em23Init);									// init DCDC
  CMU_HFXOInit(&hfxoInit);									// init HFXO with kit specific parameters

  cmu_init();												// initialize clock trees
  gpio_init();												// sets up LED, I2C, and temp sensor enable pins
  letimer_init();											// initialize letimer for LED and I2C operation
  I2C_Setup();												// initialize I2C

  while (1) {
	  Enter_Sleep();
  }
}
