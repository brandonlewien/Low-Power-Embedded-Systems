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
bool disable_letimer = false;
bool letimer_enabled = true;

/******************************************************************************
 * @brief main
 * @param none
 * @return status
 *****************************************************************************/
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
    //I2C_Interrupt_Enable();                                // enable I2C Interrupts, however we don't use I2C interrupts
    I2C_Reset_Bus();                                         // Reset I2C Bus
    CAPSENSE_Init();                                         // initialize capsense     
    LEUART0_Interrupt_Enable();                              // enable LEUART Interrupts
    CRYOTIMER_setup();                                       // initialize cryotimer
    CRYOTIMER_Interrupt_Enable();                            // enable cryotimer Interrupts

    schedule_event = DO_NOTHING;
    while (1) {
        (schedule_event == DO_NOTHING) Enter_Sleep();        // enter EM3
        if(schedule_event & SEND_TEMP){                      // send data to bluetooth
            LDMA_ftoa_send(celsius);
            if (isCelsius) {
                TxBuffer[TX_BUFFER_SIZE - 1] = 0x43;         // Send C
            }
            else {
                TxBuffer[TX_BUFFER_SIZE - 1] = 0x46;         // Send F
            }
            Sleep_Block_Mode(LEUART_EM_BLOCK);
            LEUART0->CTRL |= LEUART_CTRL_TXDMAWU;            // DMA Wakeup
            LDMA_StartTransfer(TX_DMA_CHANNEL, &ldmaTXConfig, &ldmaTXDescriptor);
            schedule_event &= ~SEND_TEMP;
        }
        if(schedule_event & READ_TOUCH){
            CAPSENSE_Sense();                                // read all capsense areas
            isPressed = CAPSENSE_getPressed(TOUCH_CHANNEL0); // retrieve if channel 0 is pressed

            static int state = 0;                            // retain state of button press
            if(isPressed && state == 0) {                    // if pressed and not pressed before
                disable_letimer ^= true;                     // toggle letimer disable
                state = 1;                                   // we now pressed before
            }
            else if (!isPressed && state == 1){              // if not pressed and pressed before
                state = 0;                                   // we have not pressed before
            }

            if(!disable_letimer && !letimer_enabled) {
                letimer_enabled = 1;                                        // don't do this if statement again until we disable letimer again
                LETIMER0->CNT = 0;                                          // reset letimer to count from 0
                LETIMER0->IFC = LETIMER_IEN_COMP0 | LETIMER_IEN_COMP1;      // clear comp1 and comp0 flags
                LETIMER0->IEN = LETIMER_IEN_COMP0 | LETIMER_IEN_COMP1;      // re-enable interrupts
                NVIC_EnableIRQ(LETIMER0_IRQn);                              // re-enable interrupts for LETIMER0 into the CORTEX-M3/4 CPU core
            }
            schedule_event &= ~READ_TOUCH;
        }
    }
}
