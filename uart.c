#include "uart.h"

//extern volatile char * receiving;

volatile uint16_t rincrement = 0;
volatile bool ready_to_TX;
extern char lookback_buffer[30];

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

    LEUART0->CTRL |= LEUART_CTRL_LOOPBK;
    GPIO_PinModeSet(TX_PORT, TX_PIN, gpioModePushPull, UART_ON);
    GPIO_PinModeSet(RX_PORT, RX_PIN, gpioModePushPull, UART_ON);

    Sleep_Block_Mode(LEUART_EM_BLOCK);                              // lowest sleep mode setting for LEUART
    ready_to_TX = 0;												// initialize transmit ready flag to 0

    LEUART_Enable(LEUART0, leuartEnable);
}

void UART_send_byte(uint8_t data) {
	LEUART0->IEN |= LEUART_IEN_TXBL;								// enable TXBL interrupt (only want this enabled when we want to transmit data)
	while(!ready_to_TX){
    	Enter_Sleep();                          					// sleep while waiting for space to be available in the transmit buffer
    }
    LEUART0->TXDATA = data;											// send data
    ready_to_TX = 0;												// reset flag to 0
}

void UART_send_n(char * data, uint32_t length) {
	//LEUART0->IEN &= ~LEUART_IEN_RXDATAV;							// disable RXDATAV interrupt
	for(int i = 0; i < length; i++) {
        UART_send_byte(data[i]);                                     // Loop through data and send
    }
	//LEUART0->IEN |= LEUART_IEN_RXDATAV;								// enable RXDATAV interrupt
}


void LEUART0_Interrupt_Enable(void) {
    LEUART0->IEN = 0;
    LEUART0->IEN = LEUART_IEN_RXDATAV;
    NVIC_EnableIRQ(LEUART0_IRQn);
}

void LEUART0_Interrupt_Disable(void) {
    LEUART0->IEN &= ~LEUART_IEN_RXDATAV;
    NVIC_DisableIRQ(LEUART0_IRQn);
}

void LEUART0_IRQHandler(void) {
    uint32_t status;
    status = LEUART0->IF;
	if(status & LEUART_IF_TXBL) {
		ready_to_TX = 1;
		LEUART0->IEN &= ~LEUART_IEN_TXBL;								// disable TXBL interrupt (only want this enabled when we want to transmit data)
	}
    if(status & LEUART_IF_RXDATAV) {
       lookback_buffer[rincrement++] = LEUART0->RXDATA;
       rincrement %= 30;
    }
}
