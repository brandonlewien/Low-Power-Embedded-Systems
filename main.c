/***************************************************************************//**
 * @main.c
 *******************************************************************************
 * # License
 * Copyright 2018 Silicon Laboratories Inc. www.silabs.com
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
#include <string.h>
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
#include "uart.h"
#include "i2c.h"
#include "ldma.h"
#include "i2ctemp.h"
#include "touch.h"
#include "capsense.h"
#include "cryotimer.h"

volatile uint16_t increment;
volatile char * receiving;
char receive_buffer[RECEIVE_BUFFER_SIZE];
uint8_t schedule_event;
float celsius;
extern int8_t TxBuffer[TX_BUFFER_SIZE];
extern volatile bool isCelsius;
extern LDMA_Descriptor_t ldmaTXDescriptor;
extern LDMA_TransferCfg_t ldmaTXConfig;
bool isPressed;

int main(void){
    EMU_DCDCInit_TypeDef dcdcInit = EMU_DCDCINIT_DEFAULT;
    CMU_HFXOInit_TypeDef hfxoInit = CMU_HFXOINIT_DEFAULT;
    EMU_EM23Init_TypeDef em23Init = EMU_EM23INIT_DEFAULT;

    CHIP_Init();                                             // Chip errata

    EMU_DCDCInit(&dcdcInit);                                 // init DCDC regulator
    em23Init.vScaleEM23Voltage = emuVScaleEM23_LowPower;     // always start in low noise mode
    EMU_EM23Init(&em23Init);                                 // init DCDC
    CMU_HFXOInit(&hfxoInit);                                 // init HFXO with kit specific parameters

    cmu_init();                                              // initialize clock trees
    uart_init();
    gpio_init();                                             // sets up LED, I2C, and temp sensor enable pins
    LDMA_Setup();                                            // initialize DMA
    letimer_init();                                          // initialize letimer for LED and I2C operation
    I2C_Setup();                                             // initialize I2C
    //I2C_Interrupt_Enable();                                // Enable Interrupts
    I2C_Reset_Bus();                                         // Reset I2C Bus
    CAPSENSE_Init();
    LEUART0_Interrupt_Enable();
    CRYOTIMER_setup();
    CRYOTIMER_Interrupt_Enable();

    schedule_event = DO_NOTHING;
    while (1) {
    	if(schedule_event == DO_NOTHING) Enter_Sleep();		// enter EM3
    	if(schedule_event & SEND_TEMP){						// send data to bluetooth
    		LDMA_ftoa_send(celsius);
    	    if (isCelsius) {
    	    	TxBuffer[TX_BUFFER_SIZE - 1] = 0x43;
    	    }
    	    else {
    	    	TxBuffer[TX_BUFFER_SIZE - 1] = 0x46;
    	    }

    	    Sleep_Block_Mode(LEUART_EM_BLOCK);
    	    LEUART0->CTRL |= LEUART_CTRL_TXDMAWU;           // DMA Wakeup
    	    LDMA_StartTransfer(TX_DMA_CHANNEL, &ldmaTXConfig, &ldmaTXDescriptor);

    	    schedule_event &= ~SEND_TEMP;
    	}
    	if(schedule_event & READ_TOUCH){
    		CAPSENSE_Sense();
    		isPressed = CAPSENSE_getPressed(TOUCH_CHANNEL0);
    		if(isPressed) {
    			isPressed = false;
    		}
    		else {

    		}
    		 schedule_event &= ~READ_TOUCH;
    	}
    }
}
