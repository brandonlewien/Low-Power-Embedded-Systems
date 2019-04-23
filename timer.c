#include "timer.h"

//extern uint8_t read_reg_data;
volatile uint16_t temp_ms_read;
volatile uint16_t temp_ls_read;
extern float celsius;
extern volatile bool isCelsius;

extern uint8_t schedule_event;

void letimer_init(void) {
    uint32_t comp0;
    uint32_t comp1;
    uint8_t prescalar = 1;
    uint8_t presc_power = 0;

    do{
       comp0 = (TEMP_MEAS_PERIOD*LFXO_FREQ)/prescalar;
       if((comp0 > TIMER_MAX_COUNT) && (prescalar <= cmuClkDiv_16384)){      // if comp0 is too big and if hardware supports larger prescalar
         prescalar = prescalar << 1;                                         // prescalars are powers of 2
                                                                             // (shift instead of multiply to reduce clock cycles and energy <3)
         presc_power++;
       }
    }
    while (comp0 > TIMER_MAX_COUNT);

    comp1 = comp0 - (SENSOR_PWR_UP*LFXO_FREQ)/prescalar;


    while(LETIMER0->SYNCBUSY);                                               // wait for any previous writes to complete or be synchronized
    CMU->LFAPRESC0 = presc_power;                                            // set prescalar

    LETIMER_CompareSet(LETIMER0, 0, comp0);                                  // set COMP0 to be period of LED PWM
    LETIMER_CompareSet(LETIMER0, 1, comp1);                                  // set COMP1 to be the time the LED is on

    /*initialize timer: no top buff, top is COMP0 = period, stops for
    debug halt, DON'T start timer after init completes, free-running mode: */
    LETIMER_Init_TypeDef LETIMER_init_struct = LETIMER_INIT_DEFAULT;         // (set to default)
    LETIMER_init_struct.comp0Top = true;                                     // (modify from default)
    LETIMER_init_struct.topValue = comp0;                                    // (modify from default)
    LETIMER_init_struct.enable = false;                                      // (modify from default)

    LETIMER_Init(LETIMER0, &LETIMER_init_struct);

    //interrupt config
    LETIMER0->IFC = LETIMER_IFC_COMP0 |LETIMER_IFC_COMP1;                    // clear flags
    LETIMER0->IEN = LETIMER_IEN_COMP0 |LETIMER_IEN_COMP1;                    // enable interrupts
    NVIC_EnableIRQ(LETIMER0_IRQn);                                           // enable interrupts for TIMER0 into the CORTEX-M3/4 CPU core

    Sleep_Block_Mode(LETIMER_EM_BLOCK);                                      // lowest sleep mode setting for LETIMER

    LETIMER_Enable(LETIMER0, true);                                          // START TIMER
}



void LETIMER0_IRQHandler(void) { // COMP0 -> desired period for taking temp, COMP1 -> min time to power up Si7021
    uint32_t int_flags = LETIMER0->IF;

    if(int_flags & LETIMER_IFC_COMP0){                                           // if COMP0 flag is set,
       GPIO->P[SENS_EN_PORT].DOUT |= (1 << SENS_EN_PIN);                         // turn on temp sensor
       LETIMER0->IFC = LETIMER_IFC_COMP0;                                        // clear flag (by writing 1 to inter. clear reg)
    }
    if(int_flags & LETIMER_IFC_COMP1){                                           // if COMP1 flag is set,
       /* LPM Enable Routine */
       Sleep_Block_Mode(I2C_EM_BLOCK);                                           // set sleep mode block for master I2C operation
       GPIO_PinModeSet(SCL_PORT, SCL_PIN, gpioModeWiredAnd, SCL_AND_SDA_DOUT);   // set up GPIO pin PC11 (SCL)
       GPIO_PinModeSet(SDA_PORT, SDA_PIN, gpioModeWiredAnd, SCL_AND_SDA_DOUT);   // set up GPIO pin PC10 (SDA)
       for (int i = 0; i < 9; i++) {                                             // reset slave I2C device state machine
         GPIO_PinOutClear(SCL_PORT, SCL_PIN);
         GPIO_PinOutSet(SCL_PORT, SCL_PIN);
       }
       I2C0->CMD = I2C_CMD_ABORT;                                                // reset pearl gecko I2C state machine
       I2C0->IFC |= I2C_IFC_ACK;                                                 // clear ACK flag
#ifdef RW_FROM_REGISTER
       /* read/write routine */
       for(int i = 0; i < 100000; i++);
       I2C_Write_to_Reg_NoInterrupts(I2C_SLAVE_ADDRESS, USER_REG_1_W, USR_REG1_12BIT_RES);
       for(int i = 0; i < 100000; i++);
       I2C_Read_to_Reg_NoInterrupts(I2C_SLAVE_ADDRESS, USER_REG_1_R);            // read data from temp sensor
       for(int i = 0; i < 100000; i++);

#endif

#ifdef READ_TEMPERATURE


       I2C_Temperature_Read_NoInterrupts(I2C_SLAVE_ADDRESS, 0xE3);               // read data from temp sensor


       if (isCelsius) {
    	   Temp_Code_To_Celsius(temp_ms_read, temp_ls_read, &celsius);
       }
       else {
    	   Temp_Code_To_Celsius(temp_ms_read, temp_ls_read, &celsius);
    	   celsius = (celsius * 1.8) + 32;
       }

       schedule_event |= SEND_TEMP;										                      		 // set event flag to send temp to bluetooth module

//       if(TEMP_ALERT > celsius) {                                              // if data read is not the data that was written
//         GPIO->P[LED0_port].DOUT |= (1 << LED0_pin);                           // turn on LED0
//       }
//       else {
//         GPIO->P[LED0_port].DOUT &= ~(1 << LED0_pin);                          // turn off LED0
//       }
#endif

       /* LPM Disable Routine */
       GPIO_PinModeSet(SCL_PORT, SCL_PIN, gpioModeDisabled, SCL_AND_SDA_DOUT);   // disable GPIO pin PC11 (SCL)
       GPIO_PinModeSet(SDA_PORT, SDA_PIN, gpioModeDisabled, SCL_AND_SDA_DOUT);   // disable GPIO pin PC10 (SDA)
       GPIO->P[SENS_EN_PORT].DOUT &= ~(1 << SENS_EN_PIN);                        // turn off temp sensor
       Sleep_UnBlock_Mode(I2C_EM_BLOCK);                                         // unblock sleep mode setting for I2C

       LETIMER0->IFC = LETIMER_IFC_COMP1;                                        // clear flag

    }
}
