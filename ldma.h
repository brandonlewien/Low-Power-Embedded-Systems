

#ifndef SRC_LDMA_H_
#define SRC_LDMA_H_

#include "em_ldma.h"

#define TX_DMA_CHANNEL  1
#define LDMA_IEN_DONE_CH1 2
#define LDMA_IF_DONE_CH1 2

void LDMA_Setup(void);
void LDMA_Interrupt_Enable(void);


#endif /* SRC_LDMA_H_ */
