#include "i2ctemp.h"
#include "i2c.h"

volatile bool ACK_done;
extern volatile uint16_t temp_ms_read;
extern volatile uint16_t temp_ls_read;
/******************************************************************************
 * @brief Read temperature from si7021 slave device without interrupts
 * @param slave_addr_rw = address of slave device, cmd = command to send to slave
 * @return temp_ms/ls_read returns most and least significant data chunks
 *****************************************************************************/
void I2C_Temperature_Read_NoInterrupts(uint8_t slave_addr_rw, uint8_t cmd) {
                                                            // Made for Hold Master Mode (0xE3)
    I2C0->CMD = I2C_CMD_START;
    I2C0->TXDATA = (slave_addr_rw << 1) | I2C_WRITE;        // send slave addr in upper 7 bits and WRITE bit in LSB to send command before reading
    while(!(I2C0->IF & I2C_IF_ACK));                        // wait for ACK from slave
    I2C0->IFC |= I2C_IFC_ACK;

    I2C0->TXDATA = cmd;                                     // send command to temp sensor
    while(!(I2C0->IF & I2C_IF_ACK));                        // wait for ACK from slave
    I2C0->IFC |= I2C_IFC_ACK;

    I2C0->CMD = I2C_CMD_START;                              // send REPEATED START to slave
    I2C0->TXDATA = (slave_addr_rw << 1) | I2C_READ;         // send slave addr and READ bit
    while(!(I2C0->IF & I2C_IF_ACK));                        // wait for ACK from slave
    I2C0->IFC |= I2C_IFC_ACK;

    while(!(I2C0->IF & I2C_IF_RXDATAV));                    // wait for entire byte to be transmitted and available in RX buffer
    temp_ms_read = I2C0->RXDATA;
    I2C0->CMD = I2C_CMD_ACK;
    while(!(I2C0->IF & I2C_IF_RXDATAV));                    // wait for entire byte to be transmitted and available in RX buffer
    temp_ls_read = I2C0->RXDATA;

    I2C0->CMD = I2C_CMD_NACK;                               // send NACK to slave
    I2C0->CMD = I2C_CMD_STOP;
}
/******************************************************************************
 * @brief Read temperature from si7021 slave device with interrupts
 * @param slave_addr_rw = address of slave device, cmd = command to send to slave
 * @return ACK_done = to clear global acknowledgements for I2C with interrupts
 *****************************************************************************/
void I2C_Temperature_Read_Interrupts(uint8_t slave_addr_rw, uint8_t cmd) {
    ACK_done     = 0;
    I2C0->CMD    = I2C_CMD_START;                           // send START condition to slave
    I2C0->TXDATA = (slave_addr_rw << 1) | I2C_WRITE;        // send slave addr in upper 7 bits
                                                            // WRITE bit in LSB to send command before reading
    while(!ACK_done);
    ACK_done     = 0;
    I2C0->TXDATA = cmd;                                     // send command to temp sensor
    while(!ACK_done);
    ACK_done     = 0;
    I2C0->CMD    = I2C_CMD_START;                           // send REPEATED START to slave
    I2C0->TXDATA = (slave_addr_rw << 1) | I2C_READ;         // send slave addr and READ bit
    while(!ACK_done);
    ACK_done = 0;
}
/******************************************************************************
 * @brief Convert temp code from si7021 temp sensor to celsius
 * @param MSData = most significant byte of data from temp sensor,
 *        LSData = least significant byte of data from temp sensor,
 *        DataRet = celsius temp
 * @return none
 *****************************************************************************/
void Temp_Code_To_Celsius(uint16_t MSData, uint16_t LSData, float * DataRet) {
    uint16_t Combined_Data1 = 0;
    Combined_Data1 = (MSData << 8) + LSData;
    *DataRet = ((175.72 * Combined_Data1) / 65536) - 46.85;
}
