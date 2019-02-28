#include "cmu.h"

void cmu_init(void){
															// High freq clock tree:
	CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);		// route HFXO to HFCLK branch					(I2C clock tree 1)
	CMU_OscillatorEnable(cmuOsc_HFRCO, false, false);		// disable HFRCO
	CMU_OscillatorEnable(cmuOsc_HFXO, true, true);			// enable HFXO 									(I2C clock tree 2)
	CMU_ClockSelectSet(cmuClock_HFPER, cmuSelect_HFCLK);	// route HFCLK to HFPERCLK(high freq periph clk)(I2C clock tree 4)
	CMU_ClockEnable(cmuClock_HF, true);						// enable HFCLK									(I2C clock tree 3)
	CMU_ClockEnable(cmuClock_HFPER, true);					// enable HFPERCLK								(I2C clock tree 5)
	CMU_HFXOAutostartEnable(true,true,true);				// auto start HFXO after wake up from sleep

															// By default, LFRCO is enabled:
	CMU_OscillatorEnable(cmuOsc_LFRCO, false, false);		// DISABLE LFRCO
	CMU_OscillatorEnable(cmuOsc_ULFRCO, false, false);		// DISABLE ULFRCO

															// Low freq clock tree:
	CMU_OscillatorEnable(cmuOsc_LFXO, true, true);			// enable LFXO									(LETIMER and LEUART clock tree 1)
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);		// route LFXO to LFACLK							(LETIMER clock tree 2)
	CMU_ClockEnable(cmuClock_LFA, true);					// enable LFACLK								(LETIMER clock tree 3)
	CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO);		// route LFXO to LFBCLK							(LEUART clock tree 2)
	CMU_ClockEnable(cmuClock_LFB, true);					// enable LFBCLK								(LEUART clock tree 3)
	CMU_ClockEnable(cmuClock_CORELE, true);

															// Enable peripheral clocks:
	CMU_ClockEnable(cmuClock_GPIO, true);					// connect clock source to GPIO clock
	CMU_ClockEnable(cmuClock_LETIMER0, true); 				// connect clock source(LFA) to LETIMER clock tree	(LETIMER clock tree 4)
	CMU_ClockEnable(cmuClock_LEUART0, true); 				// connect clock source(LFB) to LEUART clock tree	(LEUART clock tree 4)
	CMU_ClockEnable(cmuClock_I2C0, true); 					// connect clock source (HFPER) to I2C clock tree	(I2C clock tree 6)
}

