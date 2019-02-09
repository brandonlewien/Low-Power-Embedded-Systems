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
  uint8_t data_before, data_after;
  uint8_t i;

  CHIP_Init(); 											  	// Chip errata

  EMU_DCDCInit(&dcdcInit);									// init DCDC regulator
  em23Init.vScaleEM23Voltage = emuVScaleEM23_LowPower;		// always start in low noise mode
  EMU_EM23Init(&em23Init);									// init DCDC
  CMU_HFXOInit(&hfxoInit);									// init HFXO with kit specific parameters

  //These 2 lines are now in cmu_init (I'm just keeping them commented here for now so that we can go back to this if needed)
  //CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);			// Switch HFCLK to HFXO
  //CMU_OscillatorEnable(cmuOsc_HFRCO, false, false);			// disable HFRCO

  cmu_init();
  gpio_init();												// sets up LED, I2C, and temp sensor enable pins
  letimer_init();
  I2C_Setup();
  I2C_Reset_Bus();

  data_before = 1;
  data_before = I2C_Read_from_Reg(I2C_SLAVE_ADDRESS, USER_REG_1_R);
  I2C_Write_to_Reg(I2C_SLAVE_ADDRESS, USER_REG_1_W, 0x3A);
  data_after = I2C_Read_from_Reg(I2C_SLAVE_ADDRESS, USER_REG_1_R);

  i++;



  while (1) {
	  //for(int i = 0; i < 100000; i++);
	  //I2C_Encode_Buffer();


	  //Enter_Sleep();
	  	//heart beat LED
		/*for (int i = 0; i < 1500000; i++);
		GPIO_PinOutClear(LED1_port, LED1_pin);
		for (int i = 0; i < 2500000; i++);
		GPIO_PinOutSet(LED1_port, LED1_pin);*/

  }
}
