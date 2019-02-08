#include "cmu.h"

void cmu_init(void){
															// High freq clock tree:
	CMU_OscillatorEnable(cmuOsc_HFXO, true, true);			// enables HFXO 									(I2C clock tree 1)
	CMU_ClockSelectSet(cmuClock_HFPER, cmuSelect_HFXO);		// route HFXO to HFPERCLK(high freq peripheral clk) (I2C clock tree 2)
	CMU_ClockEnable(cmuClock_HFPER, true);					// enable HFPERCLK									(I2C clock tree 3)

															// By default, LFRCO is enabled:
	CMU_OscillatorEnable(cmuOsc_LFRCO, false, false);		// DISABLE LFRCO
	CMU_OscillatorEnable(cmuOsc_ULFRCO, false, false);		// DISABLE ULFRCO

															// Low freq clock tree:
	CMU_OscillatorEnable(cmuOsc_LFXO, true, true);			// enable LFXO										(LETIMER clock tree 1)
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);		// route LFXO to LFA								(LETIMER clock tree 2)
	CMU_ClockEnable(cmuClock_LFA, true);					// enable LFA										(LETIMER clock tree 3)
	CMU_ClockEnable(cmuClock_CORELE, true);

															// Enable peripheral clocks:
	CMU_ClockEnable(cmuClock_GPIO, true);					// connect clock source to GPIO clock
	CMU_ClockEnable(cmuClock_LETIMER0, true); 				// connect clock source(LFA) to LETIMER clock tree	(LETIMER clock tree 4)
	CMU_ClockEnable(cmuClock_I2C0, true); 					// connect clock source (HFPER) to I2C clock tree	(I2C clock tree 4)
}
