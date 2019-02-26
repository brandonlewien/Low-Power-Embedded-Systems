#include "uart.h"

void uart_init(void) {
	LEUART_Init_TypeDef UART_Init_Struct;
	UART_Init_Struct.enable   = UART_DISABLE;
	UART_Init_Struct.refFreq  = UART_REF_FREQ;
	UART_Init_Struct.baudrate = UART_BAUD_RATE;
	UART_Init_Struct.databits = UART_DATA_BITS;
	UART_Init_Struct.parity   = UART_PARITY;
	UART_Init_Struct.stopbits = UART_STOP_BITS;
	LEUART_Init(LEUART0, &UART_Init_Struct);

	LEUART0->ROUTELOC0 = LEUART_ROUTELOC0_RXLOC_LOC8
			           | LEUART_ROUTELOC0_TXLOC_LOC8;
	LEUART0->ROUTEPEN  = LEUART_ROUTEPEN_RXPEN
					   | LEUART_ROUTEPEN_TXPEN;

	GPIO_PinModeSet(TX_PORT, TX_PIN, gpioModeDisabled, UART_OFF);
	GPIO_PinModeSet(RX_PORT, RX_PIN, gpioModeDisabled, UART_OFF);

	LEUART_Enable(LEUART0, true);
}
