#include "sleep.h"
#include <em_core.h>
#include "em_emu.h"


//sleepBlockEnable[0] is for EM1, sleepBlockEnable[1] is for EM2, sleepBlockEnable[2] is for EM3
//max number of nested blocks is (2^8)-1 = 255
static uint8_t sleepBlockEnable[3];

void Sleep_Block_Mode(unsigned int EM) {
	if((EM == EnergyMode1) || (EM == EnergyMode2) || (EM = EnergyMode3)){
		sleepBlockEnable[EM - 1]++; //add block nesting to energy mode EM
	}
	//why EM2block function????

}
void Sleep_UnBlock_Mode(unsigned int EM) {
	if((EM == EnergyMode1) || (EM == EnergyMode2) || (EM = EnergyMode3)){
		if(sleepBlockEnable[EM - 1] > 0){ //check that energy mode is blocked
			sleepBlockEnable[EM - 1]--; //subtract block nesting to energy mode EM
		}
	}
	//why EM2unblock function????
}

void Sleep_Init() {

}
void Enter_Sleep(void) {
	EM AllowedEM;
	EM CurrentMode = EnergyMode0;
	uint32_t flags = 0;
	CORE_DECLARE_IRQ_STATE;

	CORE_ENTER_CRITICAL();
	AllowedEM = Enter_Lowest_EM_Mode();
	if (AllowedEM == EnergyMode2 || AllowedEM == EnergyMode3) {
		EMU_Save();
	}
	do {
		AllowedEM = Enter_Lowest_EM_Mode();
		if ((AllowedEM >= EnergyMode1) && (AllowedEM <= EnergyMode3)) {
			Enter_Lowest_EM_Mode();
		}
	} while ((flags & SLEEP_FLAG-NO_CLOCK_RESTORE) > 0u);
}

EM Enter_Lowest_EM_Mode(void) {

	EM EMTemp = EnergyMode1;
	//if (True == )

	EM tmpLowestEM = EnergyMode0;
	if (SLEEP_LOWEST_ENERGY_MODE_DEFAULT < tmpLowestEM) {
		tmpLowestEM = SLEEP_LOWEST_ENERGY_MODE_DEFAULT;
	}

}

static void EnterEM(EM EnergyModeWanted) {
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


