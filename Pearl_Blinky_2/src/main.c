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
#include "em_device.h"
#include <em_timer.h>
#include <em_letimer.h>
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "bsp.h"
#include "main.h"
#include "gpio.h"
#include "cmu.h"

volatile uint32_t flag;

void LETIMER0_IRQHandler(void){
	if((flag & LETIMER_IFC_UF) != false) GPIO_PinOutClear(LED0_port, LED0_pin);
	else if((flag & LETIMER_IFC_COMP1) != false) GPIO_PinOutSet(LED0_port, LED0_pin);
	else GPIO_PinOutSet(LED1_port, LED1_pin);
	flag = false;
}
//edit
static void LETIMER0_enable(){
	//const int seconds_ticks = 40000;
	//const int on_ticks = 12500;
	LETIMER_Init_TypeDef InitBlink = LETIMER_INIT_DEFAULT;

	//LETIMER_CompareSet(InitBlink, 0, seconds_ticks);
    //LETIMER_CompareSet(InitBlink, 1, on_ticks);

    LETIMER_Init(LETIMER0, &InitBlink);

    LETIMER0->IFC = LETIMER_IFC_UF | LETIMER_IFC_COMP1;
    LETIMER0->IEN = LETIMER_IEN_UF | LETIMER_IEN_COMP1;
    //Sleep_Block_Mode(Letimer0_EM);
    NVIC_EnableIRQ(LETIMER0_IRQn);
}

int main(void){
	EMU_DCDCInit_TypeDef dcdcInit = EMU_DCDCINIT_DEFAULT;
	CMU_HFXOInit_TypeDef hfxoInit = CMU_HFXOINIT_DEFAULT;

	/* Chip errata */
	CHIP_Init();

	/* Init DCDC regulator and HFXO with kit specific parameters */
	EMU_DCDCInit(&dcdcInit);
	CMU_HFXOInit(&hfxoInit);

    /* Switch HFCLK to HFXO and disable HFRCO */
	CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
    CMU_OscillatorEnable(cmuOsc_HFRCO, false, false);
    //Testing Github add
    /* Initialize clocks */
    cmu_init();

    /* Initialize GPIO */
    gpio_init();

    /* Initialize LETimer */
    LETIMER0_enable();
    /* Infinite blink loop */
    while (1) {
	  	//Enter_Sleep();
  }
}
