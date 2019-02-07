#include "i2c.h"

void i2c_init(void) {
	I2C_Init_TypeDef I2C_Init_Struct;
	I2C_Init_Struct.clhr =
	I2C_Init_Struct.enable = false;				//don't enable I2C after initialization
	I2C_Init_Struct.freq =
	I2C_Init_Struct.master =
	I2C_Init_Struct.refFreq = 0;				//select correct freq based on current processor freq (currently configured ref clock)

	//I2C_Init();
}

void I2C_ISR_Enable(void) {
	I2C0->IEN &= ~I2C_IEN_ADDR    &
			     ~I2C_IEN_RXDATAV &
				 ~I2C_IEN_SSTOP;
	I2C0->IEN |=  I2C_IEN_ADDR    |
			      I2C_IEN_RXDATAV |
				  I2C_IEN_SSTOP;
	NVIC_EnableIRQ(I2C0_IRQn);
}

void I2C0_IRQHandler(void) {
	int status;
	status = I2C0->IF;
}
