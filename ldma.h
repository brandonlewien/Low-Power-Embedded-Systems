/**************************************************************************//**
 * @file ldma.h
 * @brief General ldma header
 * @author Silicon Labs
 * @version 1.00
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/

#ifndef SRC_LDMA_H_
#define SRC_LDMA_H_

#include "em_ldma.h"

#define RX_DMA_CHANNEL    0
#define TX_DMA_CHANNEL    1
#define LDMA_IF_DONE_CH0  1
#define LDMA_IEN_DONE_CH0 1
#define LDMA_IFC_DONE_CH0 1
#define LDMA_IEN_DONE_CH1 2
#define LDMA_IF_DONE_CH1  2
#define LDMA_IFC_DONE_CH1 2

enum TXBufferIdx{
	Tx0,
	Tx1,
	Tx2,
	Tx3,
	Tx4,
	Tx5,
	Tx6,
};

/* Function: initialize LDMA peripheral
 * Inputs: None
 * Outputs: None
 */
void LDMA_Setup(void);

/* Function: enable LDMA interrupts
 * Inputs: None
 * Outputs: None
 */
void LDMA_Interrupt_Enable(void);

/* Function: Convert temperature from a float to an ASCII value and put into buffer to prepare for DMA transfer
 * Inputs: number = number to convert to ASCII
 * Outputs: None
 */
void LDMA_ftoa_send(float number);

#endif /* SRC_LDMA_H_ */
