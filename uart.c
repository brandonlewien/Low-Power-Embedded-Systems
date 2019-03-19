#include "uart.h"

volatile uint16_t rincrement = 0;
volatile bool ready_to_TX;
extern char receive_buffer[LPBK_BUFFER_SIZE];
volatile bool isCelsius = true;

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

    LEUART0->ROUTELOC0 = LEUART_ROUTELOC0_RXLOC_LOC18				// Route UART pins
                       | LEUART_ROUTELOC0_TXLOC_LOC18;
    LEUART0->ROUTEPEN  = LEUART_ROUTEPEN_RXPEN
                       | LEUART_ROUTEPEN_TXPEN;

    //LEUART0->CMD = LEUART_CMD_RXBLOCKEN;							// set RX buffer to discard incoming frames (clear it only after '?' has been RXed)
//    LEUART0->CTRL |= LEUART_CTRL_SFUBRX;	//<- don't set this because we will manually unblock the RX buffer in STARTF interrupt so that we dont read the '?'! woohoo // set LEUART0 to unblock RX buffer if start frame is received
//    LEUART0->STARTFRAME = QUESTION_MARK;							// set start frame to ascii question mark
//    LEUART0->SIGFRAME = HASHTAG;									// set signal frame to ascii hashtag

    LEUART0->CTRL &= ~LEUART_CTRL_LOOPBK;							// disable loopback
    GPIO_PinModeSet(TX_PORT, TX_PIN, gpioModePushPull, UART_ON);	// enable UART pins
    GPIO_PinModeSet(RX_PORT, RX_PIN, gpioModePushPull, UART_ON);

    Sleep_Block_Mode(LEUART_EM_BLOCK);                              // lowest sleep mode setting for LEUART
    ready_to_TX = 0;                                                // initialize transmit ready flag to 0

    LEUART_Enable(LEUART0, leuartEnable);
}

void UART_send_byte(uint8_t data) {
    LEUART0->IEN |= LEUART_IEN_TXBL;                                // enable TXBL interrupt (only want this enabled when we want to transmit data)
    while(!ready_to_TX){
        Enter_Sleep();                                              // sleep while waiting for space to be available in the transmit buffer
    }
    LEUART0->TXDATA = data;                                         // send data
    ready_to_TX = 0;                                                // reset flag to 0
}

void UART_send_n(char * data, uint32_t length) {
    //LEUART0->IEN &= ~LEUART_IEN_RXDATAV;                          // disable RXDATAV interrupt
    for(int i = 0; i < length; i++) {
        UART_send_byte(data[i]);                                    // Loop through data and send
    }
    //LEUART0->IEN |= LEUART_IEN_RXDATAV;                           // enable RXDATAV interrupt
}

void UART_ftoa_send(float number) {									// convert float to ascii value and send via UART
    int16_t integer = (int16_t)number;
    uint16_t decimal;

    if(integer < 0) {										// test if negative
        UART_send_byte(NEGATIVE_SIGN);						// send negative sign
        decimal = (((-1) * (number - integer)) * 10);		// find decimal value
        integer = -1 * integer;								// make value positive for all following operations

    }
    else {
        UART_send_byte(POSITIVE_SIGN);						// send positive sign
        decimal = ((number - integer) * 10);				// find decimal values
    }

    if(((integer % 1000) / 100) != 0) {						// hundreds place
        UART_send_byte((integer / 100) + ASCII_OFFSET);

    }
    else {
        UART_send_byte(0x20);								// if 0 value, send space instead
    }
    if(((integer % 100) / 10) != 0) {						// tens place
        UART_send_byte(((integer % 100) / 10) + ASCII_OFFSET);
    }
    else {
        UART_send_byte(0x20);								// if 0 value, send space instead
    }
    if((integer % 10) != 0) {								// ones place
        UART_send_byte((integer % 10) + ASCII_OFFSET);
    }
    else {
        UART_send_byte(SPACE);								// if 0 value, send space instead
    }
    UART_send_byte(DECIMAL_POINT);							// decimal point
    UART_send_byte(decimal + ASCII_OFFSET);					// tenths place
}

void LEUART0_Interrupt_Enable(void) {
    LEUART0->IEN = 0;
    LEUART0->IEN = LEUART_IEN_RXDATAV;// |
    			  // LEUART_IEN_SIGF    |
				  // LEUART_IEN_STARTF;
    NVIC_EnableIRQ(LEUART0_IRQn);
}

void LEUART0_Interrupt_Disable(void) {
    LEUART0->IEN &= ~(LEUART_IEN_RXDATAV |
    				  LEUART_IEN_SIGF    |
			          LEUART_IEN_STARTF);
    NVIC_DisableIRQ(LEUART0_IRQn);
}

static void LEUART0_Receiver_Decoder(char * buffer) {
	if ((buffer[1] == LOWER_D) || (buffer[1] == UPPER_D)) {
		if ((buffer[2] == LOWER_C) || (buffer[2] == UPPER_C)) {
			rincrement = 0;
			isCelsius = true;
			buffer = "";
		}
		else if ((buffer[2] == LOWER_F) || (buffer[2] == UPPER_F)) {
			rincrement = 0;
			isCelsius = false;
			buffer = "";
		}
	}
}

void LEUART0_IRQHandler(void) {
    uint32_t status;
    status = LEUART0->IF & LEUART0->IEN;
    if(status & LEUART_IF_TXBL) {
        ready_to_TX = 1;									// set ready to TX flag
        LEUART0->IEN &= ~LEUART_IEN_TXBL;                    // disable TXBL interrupt (only want this enabled when we want to transmit data)
    }
    if(status & LEUART_IF_STARTF) {
    	LEUART0->CMD = LEUART_CMD_RXBLOCKDIS;
    	LEUART0->IFC = LEUART_IFC_STARTF;
    }
    if (status & LEUART_IF_RXDATAV) {
        receive_buffer[rincrement] = LEUART0->RXDATA;
        rincrement++;
    }
    if (status & LEUART_IF_SIGF) {
    	LEUART0->CMD = LEUART_CMD_RXBLOCKEN;
    	LEUART0_Receiver_Decoder(receive_buffer);
    	LEUART0->IFC = LEUART_IFC_SIGF;
    }
}
