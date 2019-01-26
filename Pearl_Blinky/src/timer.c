//***********************************************************************************
// defines
//***********************************************************************************
#include "timer.h"
//***********************************************************************************
// functions
//***********************************************************************************
void letimer_init(void){
	//set and initialize compare values:
	uint32_t period = 2; 					//CHANGE VALS				//calculate based on source clock freq, prescaler, and desired time
	uint32_t on_time = 1;
	LETIMER_CompareSet(LETIMER0, 0, period); 							//set COMP0 to be period of LED PWM
	LETIMER_CompareSet(LETIMER0, 1, on_time); 							// set COMP1 to be the time the LED is on
//test
//test21


	//initialize timer: no top buff, top is COMP0 = period, stops for
	//debug halt, DON'T start timer after init completes, free-running mode:
	LETIMER_Init_TypeDef LETIMER_init_struct = LETIMER_INIT_DEFAULT; 	//(set to default)
	LETIMER_init_struct.comp0Top = true; 								//(modify from default)
	LETIMER_init_struct.topValue = period; 								//(modify from default)
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
		GPIO_PinOutClear(LED0_port, LED0_pin);							//turn LED off
		LETIMER0->IFC = LETIMER_IFC_COMP0; 								//clear flag (by writing 1 to inter. clear reg)
	}
	else if(int_flags & LETIMER_IFC_COMP1){ 							//if COMP1 flag is set,
		GPIO_PinOutSet(LED0_port, LED0_pin);							//turn LED on
		LETIMER0->IFC = LETIMER_IFC_COMP1; 								//clear flag
	}
}
