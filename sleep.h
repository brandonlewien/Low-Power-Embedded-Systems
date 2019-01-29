//***********************************************************************************
// Include files
//***********************************************************************************
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "gpio.h"


//***********************************************************************************
// defined files
//***********************************************************************************

#define SLEEP_LOWEST_ENERGY_MODE_DEFAULT EnergyMode3

//***********************************************************************************
// global variables
//***********************************************************************************

typedef enum{
	EnergyMode0 = 0,
	EnergyMode1 = 1,
	EnergyMode2 = 2,
	EnergyMode3 = 3,
	EnergyMode4 = 4
} EM;

typedef struct {
	bool (*sleepCallback)(EM emode);
	void (*wakeupCallback)(EM emode);
	uint32_t (*restoreCallback)(EM emode);
} Sleep_Init;

//****************************
//*******************************************************
// function prototypes
//***********************************************************************************
void Sleep_Block_Mode(unsigned int EM);
void Sleep_UnBlock_Mode(unsigned int EM);
void Enter_Sleep(void);
static EM EnterEM(EM EnergyModeWanted);
