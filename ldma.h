

#ifndef SRC_LDMA_H_
#define SRC_LDMA_H_

#include "em_ldma.h"

#define TX_DMA_CHANNEL  1
#define LDMA_IEN_DONE_CH1 2
#define LDMA_IF_DONE_CH1 2
#define LDMA_IFC_DONE_CH1 2

void LDMA_Setup(void);
void LDMA_Interrupt_Enable(void);
void LDMA_ftoa_send(float number);

enum TXBufferIdx{
	Tx0,
	Tx1,
	Tx2,
	Tx3,
	Tx4,
	Tx5,
	Tx6,
};

#endif /* SRC_LDMA_H_ */
