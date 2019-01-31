

#include "sleep.h"
#include <em_core.h>
#include "em_emu.h"


//sleepBlockEnable[0] is for EM1, sleepBlockEnable[1] is for EM2, sleepBlockEnable[2] is for EM3
//max number of nested blocks is (2^8)-1 = 255
#define MAX_EM_Element 3
static uint8_t sleepBlockEnable[MAX_EM_Element];

void Sleep_Block_Mode(unsigned int EM) {
	CORE_ATOMIC_IRQ_DISABLE();
	if(sleepBlockEnable[EM] < 255){
		sleepBlockEnable[EM]++; //add block nesting to energy mode EM
	}
	CORE_ATOMIC_IRQ_ENABLE();
}
void Sleep_UnBlock_Mode(unsigned int EM) {
	CORE_ATOMIC_IRQ_DISABLE();
	if(sleepBlockEnable[EM] > 0){ //check that energy mode is blocked
		sleepBlockEnable[EM]--; //subtract block nesting to energy mode EM
	}
	CORE_ATOMIC_IRQ_ENABLE();
}

void Sleep_Init(void) {
	sleepBlockEnable[EnergyMode0] = 0;
	sleepBlockEnable[EnergyMode1] = 0;
	sleepBlockEnable[EnergyMode2] = 0;
	sleepBlockEnable[EnergyMode3] = 0;
	sleepBlockEnable[EnergyMode4] = 0;

}

void Enter_Sleep(void) {
	if (sleepBlockEnable[0] > 0) {
		return;
	}
	else if (sleepBlockEnable[1] > 0) {
		return;
	}
	else if (sleepBlockEnable[2] > 0) {
		EMU_EnterEM1();
	}
	else if (sleepBlockEnable[3] > 0) {
		EMU_EnterEM2(true);
	}
	else {
		EMU_EnterEM3(true);
	}
	return;
}

/*EM Enter_Lowest_EM_Mode(void) {

	EM EMTemp = EnergyMode1;
	if (sleepBlockEnable[EnergyMode2 - 2] == 0) {
		EMTemp = EnergyMode2;
		if (sleepBlockEnable[EnergyMode3 - 2] == 0) {
			EMTemp = EnergyMode3;
		}
	}
	if (SLEEP_LOWEST_ENERGY_MODE_DEFAULT < EMTemp) {
		EMTemp = SLEEP_LOWEST_ENERGY_MODE_DEFAULT;
	}

	return EMTemp;
}

void EnterEM(EM EnergyModeWanted) {
	switch(EnergyModeWanted) {
		case(EnergyMode1):
			EMU_EnterEM1();
			break;
		case(EnergyMode2):
			EMU_EnterEM2(false);
			break;
		case(EnergyMode3):
			EMU_EnterEM3(false);
			break;
		case(EnergyMode4):
			EMU_EnterEM4();
			break;
		default:
			break;
	}
}

*/
