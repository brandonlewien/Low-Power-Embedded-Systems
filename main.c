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


int main(void)
{
  EMU_DCDCInit_TypeDef dcdcInit = EMU_DCDCINIT_DEFAULT;
  CMU_HFXOInit_TypeDef hfxoInit = CMU_HFXOINIT_DEFAULT;


  CHIP_Init(); 											  // Chip errata

                                                          // Init DCDC regulator and HFXO with kit specific parameters
  EMU_DCDCInit(&dcdcInit);
  CMU_HFXOInit(&hfxoInit);

                                                          // Switch HFCLK to HFXO and disable HFRCO
  CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
  CMU_OscillatorEnable(cmuOsc_HFRCO, false, false);

  cmu_letimer_init();
  gpio_init();
  letimer_init();

  I2C_ISR_Enable();

  while (1) {
	  	Enter_Sleep();
	  	//heart beat LED
		/*for (int i = 0; i < 1500000; i++);
		GPIO_PinOutClear(LED1_port, LED1_pin);
		for (int i = 0; i < 2500000; i++);
		GPIO_PinOutSet(LED1_port, LED1_pin);*/

  }
}
