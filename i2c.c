#include "i2c.h"

void i2c_init(void) {
	I2C_Init_TypeDef I2C_Init_Struct;
	I2C_Init_Struct.clhr = _I2C_CTRL_CLHR_ASYMMETRIC;			// set clock duty cycle to 6:3 (low:high) ratio (33%) <- don't know for sure if this is right
	I2C_Init_Struct.enable = false;								// don't enable I2C after initialization
	I2C_Init_Struct.freq = I2C_FREQ_FAST_MAX;					// max SCL freq of Si7021 temp sensor is 400 kHz
	I2C_Init_Struct.master = true;								// set pearl gecko as master
	I2C_Init_Struct.refFreq = 0;								// select correct freq based on current processor freq (currently configured ref clock)
	I2C_Init(I2C0, &I2C_Init_Struct);

	//Sleep_Block_Mode(I2C_EM_BLOCK);							// lowest sleep mode setting for I2C

	I2C0->ROUTELOC0 = 0x00000F0F;								//route SDA and SCL lines from peripheral to external pins (PC11 for SCL (routing #15) and PC10 for SDA (routing #15))
	I2C0->ROUTEPEN = I2C_ROUTEPEN_SCLPEN | I2C_ROUTEPEN_SDAPEN; //enable SDA and SCL pins

	GPIO_PinModeSet(SCL_PORT, SCL_PIN, gpioModeWiredAnd, SCL_AND_SDA_DOUT); //set up GPIO pin PC11 (SCL)
	GPIO_PinModeSet(SDA_PORT, SDA_PIN, gpioModeWiredAnd, SCL_AND_SDA_DOUT); //set up GPIO pin PC10 (SDA)
	GPIO_PinModeSet(SENS_EN_PORT, SENS_EN_PIN, gpioModePushPull, ENABLE_SENSOR); //set up GPIO pin PB10 (temp sensor enable) and enable temp sensor by asserting DOUT
	//^not 100% sure this mode should be gpioModePushPull

	I2C_Enable(I2C0, true);										// enable I2C
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
