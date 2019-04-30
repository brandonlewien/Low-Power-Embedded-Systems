#include "ldma.h"
#include "em_ldma.h"
#include "uart.h"

int8_t TxBuffer[TX_BUFFER_SIZE];
LDMA_Descriptor_t  ldmaTXDescriptor;
LDMA_TransferCfg_t ldmaTXConfig;
LDMA_Descriptor_t  ldmaRXDescriptor;
LDMA_TransferCfg_t ldmaRXConfig;
extern char receive_buffer[RECEIVE_BUFFER_SIZE];

/******************************************************************************
 * @brief Initialize LDMA peripheral
 * @param none
 * @return global tx and rx descriptor and config for starting transfer
 *****************************************************************************/
void LDMA_Setup(void) {
    LDMA_Init_t ldmaInit = LDMA_INIT_DEFAULT;           // Using Init Default values
    LDMA_Init(&ldmaInit);                               // Passing above into predefined function
    Sleep_Block_Mode(LEUART_EM_BLOCK);

    // LDMA descriptor and config for transferring TxBuffer
    ldmaTXDescriptor = (LDMA_Descriptor_t)LDMA_DESCRIPTOR_SINGLE_M2P_BYTE(TxBuffer, &(LEUART0->TXDATA), TX_BUFFER_SIZE); // Source: TxBuffer, Destination: USART1->TXDATA, Bytes to send: 10
    ldmaTXConfig = (LDMA_TransferCfg_t)LDMA_TRANSFER_CFG_PERIPHERAL(ldmaPeripheralSignal_LEUART0_TXBL);                  // or ldmaPeripheralSignal_LEUART0_TXEMPTY?

    ldmaRXDescriptor = (LDMA_Descriptor_t)LDMA_DESCRIPTOR_SINGLE_P2M_BYTE(&(LEUART0->RXDATA), receive_buffer, RECEIVE_BUFFER_SIZE);
    ldmaRXConfig = (LDMA_TransferCfg_t)LDMA_TRANSFER_CFG_PERIPHERAL(ldmaPeripheralSignal_LEUART0_RXDATAV);

    LEUART0->CTRL |= LEUART_CTRL_RXDMAWU;               // DMA Wakeup

    LDMA_Interrupt_Enable();                            // Enable Interrupts for LDMA
    LDMA_StartTransfer(RX_DMA_CHANNEL, &ldmaRXConfig, &ldmaRXDescriptor);
}
/******************************************************************************
 * @brief Convert temperature from a float to an ASCII value and put into buffer 
          to prepare for DMA transfer
 * @param number = number to convert to ASCII
 * @return TxBuffer = global array that holds decoded converted value
 *****************************************************************************/ 
void LDMA_ftoa_send(float number) {                     // convert float to ascii value and send via UART
    int16_t integer = (int16_t)number;
    uint16_t decimal;

    if(integer < 0) {                                   // test if negative
        TxBuffer[Tx0] = NEGATIVE_SIGN;                  // send negative sign
        decimal = (((-1) * (number - integer)) * 10);   // find decimal value
        integer = -1 * integer;                         // make value positive for all following operations
    }
    else {
        TxBuffer[Tx0] = POSITIVE_SIGN;                  // send positive sign
        decimal = ((number - integer) * 10);            // find decimal values
    }
    if(((integer % 1000) / 100) != 0) {                 // hundreds place
        TxBuffer[Tx1] = ((integer / 100) + ASCII_OFFSET);
    }
    else {
        TxBuffer[Tx1] = 0x20;                           // if 0 value, send space instead
    }
    if((((integer % 100) / 10) != 0) || (((integer % 1000) / 100) != 0)) {  // tens place
        TxBuffer[Tx2] = (((integer % 100) / 10) + ASCII_OFFSET);
    }
    else {
        TxBuffer[Tx2] = 0x20;                           // if 0 value, send space instead
    }
    if(((integer % 10) != 0) || (((integer % 1000) / 100) != 0) || (((integer % 100) / 10) != 0)) {  // ones place
        TxBuffer[Tx3] = ((integer % 10) + ASCII_OFFSET);
    }
    else {
        TxBuffer[Tx3] = SPACE;                          // if 0 value, send space instead
    }

    TxBuffer[Tx4] = DECIMAL_POINT;                      // decimal point
    TxBuffer[Tx5] = decimal + ASCII_OFFSET;             // tenths place
}
/******************************************************************************
 * @brief enable LDMA interrupts
 * @param none
 * @return none
 *****************************************************************************/
void LDMA_Interrupt_Enable(void) {
    LDMA->IEN = 0;                                      // Clear LDMA Interrupt Enable
    LDMA->IEN |= LDMA_IEN_DONE_CH0 |
                 LDMA_IEN_DONE_CH1;                     // Set Ch1 LDMA Interrupt Enable
    NVIC_EnableIRQ(LDMA_IRQn);                          // Enable Interrupts
}
/******************************************************************************
 * @brief LDMA IRQ Handler
 * @param none
 * @return none
 *****************************************************************************/
void LDMA_IRQHandler(void){
    uint32_t status;
    status = LDMA->IF & LDMA->IEN;
    if(status & LDMA_IF_DONE_CH0) {                     // when DMA transfer for channel 0 is done
        LDMA->IFC |= LDMA_IFC_DONE_CH0;                 // Clear Channel 0 IF
        LEUART0->CTRL &= ~LEUART_CTRL_RXDMAWU;          // DMA Nighty night
    }
    if(status & LDMA_IF_DONE_CH1) {                     // when DMA transfer for channel 1 is done
        LDMA->IFC |= LDMA_IFC_DONE_CH1;                 // Clear Channel 1 IF
        LEUART0->CTRL &= ~LEUART_CTRL_TXDMAWU;          // DMA Nighty night
        LEUART0->IEN |= LEUART_IEN_TXC;                 // enable TXC interrupt to signify when last byte tx is complete
    }
}
