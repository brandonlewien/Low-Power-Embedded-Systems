/*
 * uart.c
 *
 *  Created on: Feb 26, 2019
 *      Author: Brandon
 */
#include "em_leuart.h"
#include "uart.h"

extern uint8_t UART_data;

void UART_send_byte(uint8_t data){
	while(!(LEUART0->IF & LEUART_IF_TXBL));			// wait for space to be avaliable in the transmit buffer
	LEUART0->TXDATA = data;
}

void LEUART0_Interrupt_Enable(void){
	LEUART0->IEN = 0;
	LEUART0->IEN = LEUART_IEN_RXDATAV;
	NVIC_EnableIRQ(LEUART0_IRQn);
}

void LEUART0_IRQHandler(void) {
	uint32_t status;
	status = LEUART0->IF;

	if(status & LEUART_IF_RXDATAV){
		UART_data = LEUART0->RXDATA;
	}
 }
