#include "sleep.h"
#include <em_core.h>
#include "em_emu.h"


void Sleep_Block_Mode(unsigned int EM) {
	CORE_DECLARE_IRQ_STATE;
	//EM IfAllowed;
	//EM CurrentMode = EnergyMode0;
	CORE_ENTER_CRITICAL();


}
void Sleep_UnBlock_Mode(unsigned int EM) {

}
void Enter_Sleep(void) {
	EM tmpLowestEM = EnergyMode0;
	if (SLEEP_LOWEST_ENERGY_MODE_DEFAULT < tmpLowestEM) {
		tmpLowestEM = SLEEP_LOWEST_ENERGY_MODE_DEFAULT;
	}

}

static EM EnterEM(EM EnergyModeWanted) {
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
	return EnergyModeWanted;
}


