#include "sleep.h"
#include <em_core.h>

typedef enum{
	EnergyMode0 = 0,
	EnergyMode1 = 1,
	EnergyMode2 = 2,
	EnergyMode3 = 3,
	EnergyMode4 = 4
} EM;

void Sleep_Block_Mode(unsigned int EM){
	//CORE_ENTER_CRITICAL();

}
void Sleep_UnBlock_Mode(unsigned int EM){

}
void Enter_Sleep(void){
	CORE_DECLARE_IRQ_STATE;
	//SLEEP_EnergyMode_t allowedEM;
	CORE_ENTER_CRITICAL();
	//Need to check if we can get to the energy mode state

}
