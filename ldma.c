#include "ldma.h"
#include "em_ldma.h"
#include "uart.h"

int16_t TxBuffer[TX_BUFFER_SIZE];

void LDMA_Setup(void) {
	LDMA_Init_t ldmaInit = LDMA_INIT_DEFAULT;
	LDMA_Init(&ldmaInit);
	// LDMA descriptor and config for transferring TxBuffer
	LDMA_Descriptor_t ldmaTXDescriptor = (LDMA_Descriptor_t)LDMA_DESCRIPTOR_SINGLE_M2P_BYTE(TxBuffer, &(LEUART0->TXDATA), TX_BUFFER_SIZE); // Source: TxBuffer, Destination: USART1->TXDATA, Bytes to send: 10
	LDMA_TransferCfg_t ldmaTXConfig = (LDMA_TransferCfg_t)LDMA_TRANSFER_CFG_MEMORY();

	// LDMA descriptor and config for receiving data from the slave
//	LDMA_Descriptor_t ldmaRXDescriptor = (LDMA_Descriptor_t)LDMA_DESCRIPTOR_SINGLE_P2M_BYTE(&(USART1->RXDATA), RxBuffer, RX_BUFFER_SIZE); // Source: USART1->RXDATA, Destination: RxBuffer, Bytes to receive: 10
//	LDMA_TransferCfg_t ldmaRXConfig = (LDMA_TransferCfg_t)LDMA_TRANSFER_CFG_PERIPHERAL(ldmaPeripheralSignal_USART1_RXDATAV);         // Setting the transfer to trigger whenever data comes into USART1_RXDATAV

	  // Starting both transfers
//	LDMA_StartTransfer(RX_DMA_CHANNEL, &ldmaRXConfig, &ldmaRXDescriptor);
	LDMA_StartTransfer(TX_DMA_CHANNEL, &ldmaTXConfig, &ldmaTXDescriptor);
}

void LDMA_ftoa_send(float number) {									// convert float to ascii value and send via UART
    int16_t integer = (int16_t)number;
    uint16_t decimal;

    if(integer < 0) {										// test if negative
        TxBuffer[0] = NEGATIVE_SIGN;						// send negative sign
        decimal = (((-1) * (number - integer)) * 10);		// find decimal value
        integer = -1 * integer;								// make value positive for all following operations

    }
    else {
        TxBuffer[0] = POSITIVE_SIGN;						// send positive sign
        decimal = ((number - integer) * 10);				// find decimal values
    }

    if(((integer % 1000) / 100) != 0) {						// hundreds place
    	TxBuffer[1] = ((integer / 100) + ASCII_OFFSET);

    }
    else {
    	TxBuffer[1] = 0x20;								    // if 0 value, send space instead
    }

    if((((integer % 100) / 10) != 0) || (((integer % 1000) / 100) != 0)) {	// tens place
    	TxBuffer[2] = (((integer % 100) / 10) + ASCII_OFFSET);
    }
    else {
    	TxBuffer[2] = 0x20;		    						// if 0 value, send space instead
    }

    if(((integer % 10) != 0) || (((integer % 1000) / 100) != 0) || (((integer % 100) / 10) != 0)) {		// ones place
    	TxBuffer[3] = ((integer % 10) + ASCII_OFFSET);
    }
    else {
    	TxBuffer[3] = SPACE;								// if 0 value, send space instead
    }

    TxBuffer[4] = DECIMAL_POINT;							// decimal point
    TxBuffer[5] = decimal + ASCII_OFFSET;					// tenths place
}
