#include "i2c.h"

void i2c_init(void) {
	I2C_Init_TypeDef I2C_Init_Struct;
	I2C_Init_Struct.clhr = _I2C_CTRL_CLHR_STANDARD;		//set clock duty cycle to 4:4 ratio (50%)
	I2C_Init_Struct.enable = false;						// don't enable I2C after initialization
	I2C_Init_Struct.freq = I2C_FREQ_FAST_MAX;			// max SCL freq of Si7021 temp sensor is 400 kHz
	I2C_Init_Struct.master = true;						// set pearl gecko as master
	I2C_Init_Struct.refFreq = 0;						// select correct freq based on current processor freq (currently configured ref clock)

	//I2C_Init();
	I2C_Enable(I2C0, true);								// Enable I2C
}

void I2C_Encode_Buffer(void) {
	I2C_Interrupt_Disable();
	I2C_TransferSeq_TypeDef Buffer;
	GPIO_PinOutSet(gpioPortC, 0);

    Buffer.addr          = I2C_ADDRESS;
	Buffer.flags         = I2C_FLAG_WRITE;
	Buffer.buf[0].data   = i2c_txBuffer;
	Buffer.buf[0].len    = i2c_txBufferSize;
	Buffer.buf[1].data   = i2c_rxBuffer;
	Buffer.buf[1].len    = I2C_RXBUFFER_SIZE;
	I2C_TransferInit(I2C0, &Buffer);

	 while(I2C_Transfer(I2C0));

	 GPIO_PinOutClear(gpioPortC, 0);
	 I2C_Interrupt_Enable();
}

void I2C_Interrupt_Enable(void) {
	I2C0->IEN = 0;                   // Clear IEN
	I2C0->IEN |= I2C_IEN_ADDR    |
			     I2C_IEN_RXDATAV |
				 I2C_IEN_SSTOP;
	NVIC_EnableIRQ(I2C0_IRQn);
}

void I2C_Interrupt_Disable(void) {
	I2C0->IEN &= ~I2C_IEN_ADDR    |
			     ~I2C_IEN_RXDATAV |
				 ~I2C_IEN_SSTOP;
	NVIC_DisableIRQ(I2C0_IRQn);
}

void I2C0_IRQHandler(void) {
	int status;
	status = I2C0->IF;
}
