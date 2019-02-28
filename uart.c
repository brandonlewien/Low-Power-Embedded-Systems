#include "uart.h"

extern uint8_t UART_data;

void uart_init(void) {
	LEUART_Init_TypeDef UART_Init_Struct;
	UART_Init_Struct.enable   = UART_DISABLE;
	UART_Init_Struct.refFreq  = UART_REF_FREQ;
	UART_Init_Struct.baudrate = UART_BAUD_RATE;
	UART_Init_Struct.databits = UART_DATA_BITS;
	UART_Init_Struct.parity   = UART_PARITY;
	UART_Init_Struct.stopbits = UART_STOP_BITS;
	LEUART0_Interrupt_Disable();
	LEUART_Reset(LEUART0);
	LEUART_Init(LEUART0, &UART_Init_Struct);

	LEUART0->ROUTELOC0 = LEUART_ROUTELOC0_RXLOC_LOC18
			           | LEUART_ROUTELOC0_TXLOC_LOC18;
	LEUART0->ROUTEPEN  = LEUART_ROUTEPEN_RXPEN
					   | LEUART_ROUTEPEN_TXPEN;

	GPIO_PinModeSet(TX_PORT, TX_PIN, gpioModePushPull, 1);
	GPIO_PinModeSet(RX_PORT, RX_PIN, gpioModePushPull, 1);

	LEUART_Enable(LEUART0, leuartEnable);
}

void UART_send_byte(uint8_t data){
	while(!(LEUART0->IF & LEUART_IF_TXBL));			// wait for space to be avaliable in the transmit buffer
	LEUART0->TXDATA = data;
	LEUART0_Interrupt_Disable();
}

void UART_send_n(char * data, uint32_t length){
    for(int i = 0; i < length; i++){
        UART_send_byte(data[i]);           // Loop through data and send
    }
}

void LEUART0_Interrupt_Enable(void){
	LEUART0->IEN = 0;
	LEUART0->IEN = LEUART_IEN_RXDATAV;
	NVIC_EnableIRQ(LEUART0_IRQn);
}
void LEUART0_Interrupt_Disable(void){
	LEUART0->IEN &= ~LEUART_IEN_RXDATAV;
	NVIC_DisableIRQ(LEUART0_IRQn);
}

void LEUART0_IRQHandler(void) {
	uint32_t status;
	status = LEUART0->IF;

	if(status & LEUART_IF_RXDATAV){
		UART_data = LEUART0->RXDATA;
	}
 }
