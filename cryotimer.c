#include "cryotimer.h"
#include "main.h"

extern uint8_t schedule_event;



/******************************************************************************
 * @brief Configure cryotimer to use ULFRCO with a 1 second wakeup event period
 * @param none
 * @return none
 *****************************************************************************/
void CRYOTIMER_setup(void){

	CRYOTIMER_Init_TypeDef CRYO_Init_Struct;

	CRYO_Init_Struct.debugRun  = CRYO_DEBUG_DISABLE;      // don't run timer during debug halt
	CRYO_Init_Struct.em4Wakeup = CRYO_EM4_WAKEUP;         // enable wakeup from EM4
	CRYO_Init_Struct.enable    = CRYO_DISABLE;            // don't enable timer yet
	CRYO_Init_Struct.osc       = cryotimerOscULFRCO;      // set oscillator to ULFRCO
	CRYO_Init_Struct.period    = cryotimerPeriod_8;       // for 1 sec period (see calculation below)
	CRYO_Init_Struct.presc     = cryotimerPresc_128;      // for 1 sec period (see calculation below)
	// ^Period of cryotimer wakeup events = ((2^presc)*(2^period))/cryo_freq = ((128)*(256))/32768 = 1sec :)

	CRYOTIMER_Init(&CRYO_Init_Struct);                    // initialize cryotimer
	CRYOTIMER_Enable(CRYO_ENABLE);                        // enable cryotimer
}


/******************************************************************************
 * @brief NVIC and register enable for the period interrupt for the cryotimer
 * @param none
 * @return none
 *****************************************************************************/
void CRYOTIMER_Interrupt_Enable(void){
	CRYOTIMER->IEN = 0;
	CRYOTIMER->IEN = CRYOTIMER_IEN_PERIOD;                // enable cryotimer period interrupt
	NVIC_EnableIRQ(CRYOTIMER_IRQn);
}


/******************************************************************************
 * @brief Set event to read value of touch sensor on every cryotimer period interrupt
 * @param schedule_event: bitmap of all events
 * @return schedule_event: on period interrupt, READ_TOUCH bit of schedule_event will be set
 *****************************************************************************/
void CRYOTIMER_IRQHandler(void) {
	uint32_t status;
	status = CRYOTIMER->IF & CRYOTIMER->IEN;               // set status to all enabled interrupts
	if(status & CRYOTIMER_IF_PERIOD) {                     // for every PERIOD interrupt:
		schedule_event |= READ_TOUCH;                      // set the schedule event to read the value of the cap touch sensor
		CRYOTIMER->IFC = CRYOTIMER_IFC_PERIOD;             // clear flag
	}

}
