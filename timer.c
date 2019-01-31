//***********************************************************************************
// defines
//***********************************************************************************
#include "timer.h"
//***********************************************************************************
// functions
//***********************************************************************************
void letimer_init(void) {
	uint32_t comp0;
	uint32_t comp1;
	uint8_t prescalar = 1;
	uint8_t presc_power = 0;

	do{
		comp0 = (LED_PERIOD*LFXO_FREQ)/prescalar;
		if((comp0 > TIMER_MAX_COUNT) && (prescalar <= cmuClkDiv_16384)){ //if comp0 is too big and if hardware supports larger prescalar
			prescalar *= 2; //prescalars are powers of 2
			presc_power++;
		}
	}
	while (comp0 > TIMER_MAX_COUNT);

	comp1 = (LED_ON_TIME*LFXO_FREQ)/prescalar;

	//CMU_ClockPrescSet(cmuClock_LETIMER0, 8); //<- this function is evil and has a bug in it for prescalar = 1
	//while(CMU_SYNCBUSY_LFAPRESC0);
	CMU->LFAPRESC0 = presc_power;

	LETIMER_CompareSet(LETIMER0, 0, comp0); 							// set COMP0 to be period of LED PWM
	LETIMER_CompareSet(LETIMER0, 1, comp1); 							// set COMP1 to be the time the LED is on

	/*initialize timer: no top buff, top is COMP0 = period, stops for
	debug halt, DON'T start timer after init completes, free-running mode: */
	LETIMER_Init_TypeDef LETIMER_init_struct = LETIMER_INIT_DEFAULT; 	//(set to default)
	LETIMER_init_struct.comp0Top = true; 								//(modify from default)
	LETIMER_init_struct.topValue = comp0; 								//(modify from default)
	LETIMER_init_struct.enable = false;	 								//(modify from default)

	LETIMER_Init(LETIMER0, &LETIMER_init_struct);


	//interrupt config
	LETIMER0->IFC = LETIMER_IFC_COMP0 |LETIMER_IFC_COMP1; 				//clear flags
	LETIMER0->IEN = LETIMER_IEN_COMP0 |LETIMER_IEN_COMP1;				//enable interrupts
	NVIC_EnableIRQ(LETIMER0_IRQn);										//enable interrupts for TIMER0 into the CORTEX-M3/4 CPU core

	//Sleep_Block_Mode(Letimer0_EM);		//FIGURE THIS OUT			//lowest sleep mode setting for LETIMER


	LETIMER_Enable(LETIMER0, true);										//START TIMER
}



void LETIMER0_IRQHandler(void){
	uint32_t int_flags = LETIMER0->IF;
	if(int_flags & LETIMER_IFC_COMP0){ 									//if COMP0 flag is set,
		//GPIO_PinOutClear(LED0_port, LED0_pin);							//turn LED off
		GPIO->P[LED0_port].DOUT &= ~(1 << LED0_pin);
		LETIMER0->IFC = LETIMER_IFC_COMP0; 								//clear flag (by writing 1 to inter. clear reg)
	}
	else if(int_flags & LETIMER_IFC_COMP1){ 							//if COMP1 flag is set,
		//GPIO_PinOutSet(LED0_port, LED0_pin);							//turn LED on
		GPIO->P[LED0_port].DOUT |= (1 << LED0_pin);
		LETIMER0->IFC = LETIMER_IFC_COMP1; 								//clear flag
	}
}
