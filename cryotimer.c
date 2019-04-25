#include "cryotimer.h"
#include "main.h"

extern uint8_t schedule_event;

void CRYOTIMER_setup(void){

	CRYOTIMER_Init_TypeDef CRYO_Init_Struct;

	CRYO_Init_Struct.debugRun  = CRYO_DEBUG_DISABLE;
	CRYO_Init_Struct.em4Wakeup = CRYO_EM4_WAKEUP;
	CRYO_Init_Struct.enable    = CRYO_DISABLE;
	CRYO_Init_Struct.osc       = cryotimerOscULFRCO;
	CRYO_Init_Struct.period    = cryotimerPeriod_4;
	CRYO_Init_Struct.presc     = cryotimerPresc_128;
	// ^Period of cryotimer wakeup events = ((2^presc)*(2^period))/cryo_freq = ((128)*(256))/32768 = 1sec :)

	CRYOTIMER_Init(&CRYO_Init_Struct);
	CRYOTIMER_Enable(CRYO_ENABLE);
}

void CRYOTIMER_Interrupt_Enable(void){
	CRYOTIMER->IEN = 0;
	CRYOTIMER->IEN = CRYOTIMER_IEN_PERIOD;
	NVIC_EnableIRQ(CRYOTIMER_IRQn);
}


void CRYOTIMER_IRQHandler(void) {
	uint32_t status;
	status = CRYOTIMER->IF & CRYOTIMER->IEN;
	if(status & CRYOTIMER_IF_PERIOD) {
		schedule_event |= READ_TOUCH;
		CRYOTIMER->IFC = CRYOTIMER_IFC_PERIOD;
	}

}
