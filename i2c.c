#include "i2c.h"
#include "gpio.h"

volatile bool ACK_done;
extern uint16_t read_data;



void I2C_Setup(void) {
	I2C_Init_TypeDef I2C_Init_Struct;
	I2C_Init_Struct.clhr    = _I2C_CTRL_CLHR_ASYMMETRIC;					// set clock duty cycle to 6:3 (low:high) ratio (33%)
	I2C_Init_Struct.enable  = false;										// don't enable I2C after I2C_Init()
	I2C_Init_Struct.freq    = I2C_FREQ_FAST_MAX;							// max SCL freq of Si7021 temp sensor is 400 kHz
	I2C_Init_Struct.master  = true;											// set pearl gecko as master
	I2C_Init_Struct.refFreq = 0;											// select correct freq based on current processor freq
	I2C_Init(I2C0, &I2C_Init_Struct);                                       // (currently configured ref clock)

	I2C0->SADDR     = I2C_SLAVE_ADDRESS;									// specifies address of temp sensor (with last bit already as the R/W bit)
	I2C0->ROUTELOC0 = I2C_ROUTELOC0_SDALOC_LOC15 							// route SDA and SCL lines from peripheral to external pins
			        | I2C_ROUTELOC0_SCLLOC_LOC15;	 						// (PC11 for SCL (routing #15) and PC10 for SDA (routing #15))
	I2C0->ROUTEPEN  = I2C_ROUTEPEN_SCLPEN
			        | I2C_ROUTEPEN_SDAPEN; 									// enable SDA and SCL pins

	//Sleep_Block_Mode(I2C_EM_BLOCK);										// lowest sleep mode setting for I2C
	GPIO_PinModeSet(gpioPortD, 15, gpioModeWiredAnd, 1);
    GPIO_PinModeSet(gpioPortD, 14, gpioModeWiredAnd, 1);
	for (int i = 0; i < 9; i++) {											// reset slave I2C device state machine
		GPIO_PinOutClear(SCL_PORT, SCL_PIN);
		GPIO_PinOutSet  (SCL_PORT, SCL_PIN);
	}
	I2C0->CMD = I2C_CMD_ABORT;												// reset pearl gecko I2C state machine
	I2C_Enable(I2C0, true);													// enable I2C
}


uint8_t I2C_Read_from_Reg_NoInterrupts(uint8_t slave_addr_rw, uint8_t cmd){
	uint8_t data;
	//uint8_t slave_addr_r = slave_addr_rw | I2C_READ;

	I2C0->CMD = I2C_CMD_START;												// send START condition to slave
	I2C0->TXDATA = (slave_addr_rw << 1) | I2C_WRITE;						// send slave addr in upper 7 bits and WRITE bit in LSB to send command before reading
	while(!(I2C0->IF & I2C_IF_ACK));										// wait for ACK from slave
	I2C0->IFC |= I2C_IFC_ACK;												// clear ACK flag

	I2C0->TXDATA = cmd;														// send command to temp sensor
	while(!(I2C0->IF & I2C_IF_ACK));										// wait for ACK from slave
	I2C0->IFC |= I2C_IFC_ACK;												// clear ACK flag

	I2C0->CMD = I2C_CMD_START;												// send REPEATED START to slave
	I2C0->TXDATA = (slave_addr_rw << 1) | I2C_READ;							// send slave addr and READ bit
	while(!(I2C0->IF & I2C_IF_ACK));										// wait for ACK from slave
	I2C0->IFC |= I2C_IFC_ACK;												// clear ACK flag

	while(!(I2C0->IF & I2C_IF_RXDATAV));									// wait for entire byte to be transmitted and available in RX buffer
	data = I2C0->RXDATA; 													// read data from RX buffer (automatically clears RXDATAV flag)

	I2C0->CMD = I2C_CMD_NACK;												// send NACK to slave
	I2C0->CMD = I2C_CMD_STOP;												// send STOP to slave

	return data;
}

void I2C_Write_to_Reg_NoInterrupts(uint8_t slave_addr_rw, uint8_t cmd, uint8_t data){
	I2C0->CMD = I2C_CMD_START;												// send START condition to slave
	I2C0->TXDATA = (slave_addr_rw << 1) | I2C_WRITE;						// send slave addr in upper 7 bits and WRITE bit in LSB to send command before reading
	while(!(I2C0->IF & I2C_IF_ACK));										// wait for ACK from slave
	I2C0->IFC |= I2C_IFC_ACK;												// clear ACK flag

	I2C0->TXDATA = cmd;														// send command to temp sensor
	while(!(I2C0->IF & I2C_IF_ACK));										// wait for ACK from slave
	I2C0->IFC |= I2C_IFC_ACK;												// clear ACK flag

	I2C0->TXDATA = data;													// send data to temp sensor

	I2C0->CMD = I2C_CMD_ACK;												// send ACK to slave
	I2C0->CMD = I2C_CMD_STOP;												// send STOP to slave
}

void I2C_Write_Interrupts(uint8_t slave_addr, uint8_t cmd, uint8_t data){
	ACK_done     = 0;
	I2C0->CMD    = I2C_CMD_START;							    			// send START condition to slave
	I2C0->TXDATA = (slave_addr << 1) | I2C_WRITE;							// send slave addr in upper 7 bits
	                                                                        // WRITE bit in LSB to send command before reading
	while(!ACK_done);
	ACK_done     = 0;
	I2C0->TXDATA = cmd;														// send command to temp sensor
	while(!ACK_done);
	ACK_done     = 0;
	I2C0->TXDATA = data;													// send data to temp sensor
	I2C0->CMD    = I2C_CMD_ACK;												// send ACK to slave
	I2C0->CMD    = I2C_CMD_STOP;											// send STOP to slave
}

void I2C_Read_Interrupts(uint8_t slave_addr, uint8_t cmd){
	ACK_done     = 0;
	I2C0->CMD    = I2C_CMD_START;											// send START condition to slave
	I2C0->TXDATA = (slave_addr << 1) | I2C_WRITE;							// send slave addr in upper 7 bits
	                                                                        // WRITE bit in LSB to send command before reading
	while(!ACK_done);
	ACK_done     = 0;
	I2C0->TXDATA = cmd;														// send command to temp sensor
	while(!ACK_done);
	ACK_done     = 0;
	I2C0->CMD    = I2C_CMD_START;											// send REPEATED START to slave
	I2C0->TXDATA = (slave_addr << 1) | I2C_READ;							// send slave addr and READ bit
	while(!ACK_done);
	ACK_done = 0;
}

/*
uint16_t I2C_Read_Measurement(uint8_t slave_addr_rw, uint8_t cmd){ //use RXDOUBLE, returns 2 bytes

}*/

void I2C_Reset_Bus(void) {
	if(I2C0->STATE & I2C_STATE_BUSY) {
		I2C0->CMD = I2C_CMD_ABORT;
	}
}

void I2C_Interrupt_Enable(void) {
	I2C0->IEN = 0;                   // Clear IEN
	I2C0->IEN |= I2C_IEN_RXDATAV |
				 I2C_IEN_ACK;
	NVIC_EnableIRQ(I2C0_IRQn);
}

void I2C_Interrupt_Disable(void) {
	I2C0->IEN &= ~(I2C_IEN_RXDATAV |
				   I2C_IEN_ACK);
	NVIC_DisableIRQ(I2C0_IRQn);
}

void I2C0_IRQHandler(void)
 {
	int status;
	status = I2C0->IF;

	if (status & I2C_IF_ACK) {
		I2C0->IFC |= I2C_IFC_ACK;					    // clear ACK flag
		ACK_done = 1;
	}
	if (status & I2C_IF_RXDATAV){
		read_data = I2C0->RXDOUBLE; 					// read data from RX buffer (automatically clears RXDATAV flag)
		I2C0->CMD = I2C_CMD_NACK;						// send NACK to slave
		I2C0->CMD = I2C_CMD_STOP;						// send STOP to slave
	}
}
