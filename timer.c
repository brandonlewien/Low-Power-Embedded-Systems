#include "timer.h"
#include "i2c.h"

void letimer_init(void) {
	uint32_t comp0;
	uint32_t comp1;
	uint8_t prescalar = 1;
	uint8_t presc_power = 0;

	do{
		comp0 = (LED_PERIOD*LFXO_FREQ)/prescalar;
		if((comp0 > TIMER_MAX_COUNT) && (prescalar <= cmuClkDiv_16384)){ 		// if comp0 is too big and if hardware supports larger prescalar
			prescalar = prescalar << 1;             					 		// prescalars are powers of 2
																		 	 	 // (shift instead of multiply to reduce clock cycles and energy <3)
			presc_power++;
		}
	}
	while (comp0 > TIMER_MAX_COUNT);

	comp1 = (LED_ON_TIME*LFXO_FREQ)/prescalar;

	//CMU_ClockPrescSet(cmuClock_LETIMER0, 1); 						    		// this function is evil and has a bug in it for prescalar = 1 (the presc = CMU_PrescToLog2(presc); line in this function should set presc equal to 0 when a 1 is input, but it keeps it equal to a 1 instead)
	while(LETIMER0->SYNCBUSY);													// wait for any previous writes to complete or be synchronized
	CMU->LFAPRESC0 = presc_power;

	LETIMER_CompareSet(LETIMER0, 0, comp0); 									// set COMP0 to be period of LED PWM
	LETIMER_CompareSet(LETIMER0, 1, comp1); 									// set COMP1 to be the time the LED is on

	/*initialize timer: no top buff, top is COMP0 = period, stops for
	debug halt, DON'T start timer after init completes, free-running mode: */
	LETIMER_Init_TypeDef LETIMER_init_struct = LETIMER_INIT_DEFAULT; 			// (set to default)
	LETIMER_init_struct.comp0Top = true; 										// (modify from default)
	LETIMER_init_struct.topValue = comp0; 										// (modify from default)
	LETIMER_init_struct.enable = false;	 										// (modify from default)

	LETIMER_Init(LETIMER0, &LETIMER_init_struct);

	//interrupt config
	LETIMER0->IFC = LETIMER_IFC_COMP0 |LETIMER_IFC_COMP1; 						// clear flags
	LETIMER0->IEN = LETIMER_IEN_COMP0 |LETIMER_IEN_COMP1;						// enable interrupts
	NVIC_EnableIRQ(LETIMER0_IRQn);												// enable interrupts for TIMER0 into the CORTEX-M3/4 CPU core

	Sleep_Block_Mode(LETIMER_EM_BLOCK);								 			// lowest sleep mode setting for LETIMER

	LETIMER_Enable(LETIMER0, true);												// START TIMER
}



void LETIMER0_IRQHandler(void){
	uint32_t int_flags = LETIMER0->IF;
	uint8_t data;

	if(int_flags & LETIMER_IFC_COMP0){ 											// if COMP0 flag is set,
		GPIO->P[LED0_port].DOUT &= ~(1 << LED0_pin);							// turn LED off ->use LED0 as heart beat for Lab 3
		LETIMER0->IFC = LETIMER_IFC_COMP0; 										// clear flag (by writing 1 to inter. clear reg)
	}
	if(int_flags & LETIMER_IFC_COMP1){ 											// if COMP1 flag is set,
		 GPIO->P[LED0_port].DOUT |= (1 << LED0_pin);							// turn LED on ->use LED0 as heart beat for Lab 3

		 I2C_Write_to_Reg_NoInterrupts(I2C_SLAVE_ADDRESS, USER_REG_1_W, USR_REG1_12BIT_RES);	// write to temp sensor User Register 1
		 for(int i = 0; i < 500; i++);											// wait for temp register to change

		 data = I2C_Read_from_Reg_NoInterrupts(I2C_SLAVE_ADDRESS, USER_REG_1_R);				// read data from register
		 if(data != USR_REG1_12BIT_RES){										// if data read is not the data that was written
			 GPIO->P[LED1_port].DOUT |= (1 << LED1_pin);						// turn on LED1
			 while(1);															// enter inf loop to indicate error
		 }
		LETIMER0->IFC = LETIMER_IFC_COMP1; 										// clear flag
	}
}
