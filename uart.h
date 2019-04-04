/**************************************************************************//**
 * @file cmu.h
 * @brief Clock management unit header
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

#ifndef SRC_UART_H_
#define SRC_UART_H_

#include <em_leuart.h>
#include <em_gpio.h>
#include "sleep.h"

#define LEUART_EM_BLOCK 3   // block EM3 to work down to EM2

#define UART_BAUD_RATE  9600
#define UART_DATA_BITS  leuartDatabits8
#define UART_ENABLE     leuartEnable
#define UART_DISABLE    leuartDisable
#define UART_PARITY     leuartNoParity
#define UART_REF_FREQ   0
#define UART_STOP_BITS  leuartStopbits1
#define UART_OFF        0
#define UART_ON         1

#define TX_PORT         gpioPortD
#define TX_PIN          10
#define RX_PORT         gpioPortD
#define RX_PIN          11

#define TX_BUFFER_SIZE       7
#define RECEIVE_BUFFER_SIZE 10

// ASCII defines
#define DECIMAL_POINT    0x2E
#define SPACE		     0x20
#define ASCII_OFFSET	 48
#define NEGATIVE_SIGN	 0x2D
#define POSITIVE_SIGN	 0x2B
#define QUESTION_MARK	 0x3F
#define HASHTAG			 0x23
#define LOWER_C			 0x63
#define UPPER_C			 0x43
#define LOWER_D			 0x64
#define UPPER_D		     0x44
#define LOWER_F 		 0x66
#define UPPER_F	    	 0x46

#define CF_CMD_IDX 2
#define D_CMD_IDX 1

void uart_init(void);

void UART_send_byte(uint8_t data);
void UART_send_n(char * data, uint32_t length);
void LEUART0_Interrupt_Enable(void);
void LEUART0_Interrupt_Disable(void);
void UART_ftoa_send(float number);

#endif /* SRC_UART_H_ */
