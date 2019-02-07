#include "cmu.h"

void cmu_letimer_init(void){

	CMU_ClockSelectSet(cmuClock_HFPER, cmuSelect_HFXO);
	CMU_ClockEnable(cmuClock_HFPER, true);
															// By default, LFRCO is enabled
	CMU_OscillatorEnable(cmuOsc_LFRCO, false, false);		// using LFXO or ULFRCO
															// Route LF clock to the LF clock tree
	CMU_OscillatorEnable(cmuOsc_ULFRCO, false, false);
	CMU_OscillatorEnable(cmuOsc_LFXO, true, true);
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);		// routing clock to LFA

	CMU_ClockEnable(cmuClock_LFA, true);
	CMU_ClockEnable(cmuClock_CORELE, true);
															// Peripheral clocks enabled
	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockSelectSet(cmuClock_I2C0, cmuSelect_HFXO);      //links HFXO clock to I2C master clock (select reference clock for clock branch)		// connect clock source to LETIMER clock tree
}


void cmu_i2c_init(void){
	CMU_OscillatorEnable(cmuOsc_HFXO, true);				//enables HFXO clock
	CMU_ClockSelectSet(cmuClock_I2C0, cmuSelect_HFXO);      //links HFXO clock to I2C master clock (select reference clock for clock branch)
	CMU_ClockSelectSet(cmuClock_I2C0, cmuSelect_HFXO);      //links HFXO clock to I2C master clock (select reference clock for clock branch)
}
