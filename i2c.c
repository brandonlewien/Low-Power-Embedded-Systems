#include "i2c.h"

void i2c_init(void) {
	I2C_Init_TypeDef I2C_Init_Struct;
	I2C_Init_Struct.clhr = _I2C_CTRL_CLHR_STANDARD;	//set clock duty cycle to 4:4 ratio (50%)
	I2C_Init_Struct.enable = false;					// don't enable I2C after initialization
	I2C_Init_Struct.freq = I2C_FREQ_FAST_MAX;		// max SCL freq of Si7021 temp sensor is 400 kHz
	I2C_Init_Struct.master = true;					// set pearl gecko as master
	I2C_Init_Struct.refFreq = 0;					// select correct freq based on current processor freq (currently configured ref clock)

	//I2C_Init();
}
