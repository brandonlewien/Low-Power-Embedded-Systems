//***********************************************************************************
// Include files
//***********************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include "em_letimer.h"
#include "gpio.h"
#include "cmu.h"

//***********************************************************************************
// defines
//***********************************************************************************
#define TIMER_MAX_COUNT 65535 //(2^16)-1
#define LFXO_FREQ 32768 //(2^15)

#define LED_ON_TIME .4 //(in seconds)  .4 <-ok
#define LED_PERIOD 1.75 //(in seconds)  1.99 and anything below incorrect when using CMU_ClockPrescSet

//***********************************************************************************
// defined files
//***********************************************************************************



//***********************************************************************************
// global variables
//***********************************************************************************


//****************************
//*******************************************************
// function prototypes
//***********************************************************************************
void letimer_init(void);
