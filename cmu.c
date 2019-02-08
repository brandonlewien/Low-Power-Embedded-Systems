#include "cmu.h"

void cmu_letimer_init(void){

	CMU_ClockSelectSet(cmuClock_HFPER, cmuSelect_HFXO);		//route HFXO to HFPERCLK(high freq peripheral clk) 	(I2C clock tree 1)
	CMU_ClockEnable(cmuClock_HFPER, true);					//enable HFPERCLK									(I2C clock tree 2)

															// By default, LFRCO is enabled
	CMU_OscillatorEnable(cmuOsc_LFRCO, false, false);		// using LFXO or ULFRCO
															// Route LF clock to the LF clock tree
	CMU_OscillatorEnable(cmuOsc_ULFRCO, false, false);
	CMU_OscillatorEnable(cmuOsc_LFXO, true, true);
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);		// routing LFXO to LFA								(LETIMER clock tree 1)

	CMU_ClockEnable(cmuClock_LFA, true);
	CMU_ClockEnable(cmuClock_CORELE, true);
															// Peripheral clocks enabled
	CMU_ClockEnable(cmuClock_GPIO, true);
	CMU_ClockEnable(cmuClock_LETIMER0, true); 				// connect clock source to LETIMER clock tree
}


void cmu_i2c_init(void){
////

	CMU_OscillatorEnable(cmuOsc_HFXO, true, true);				// enables HFXO clock
	CMU_ClockSelectSet(cmuClock_I2C0, cmuSelect_HFXO);      // routes I2C clock with HFXO
	CMU_ClockEnable(cmuClock_I2C0, true); 					// enable I2C clock
}
