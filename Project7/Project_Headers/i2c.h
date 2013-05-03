/*
 * i2c.h
 *
 * Project 7
 * Ramsey Kant (rkant@asu.edu), Michael Steptoe (msteptoe@asu.edu)
 * CSE325 Embedded Microprocessor Systems Spring 2013
 */

#ifndef I2C_H_
#define I2C_H_

#include "support_common.h"

#include "dtim.h"

#define I2C_ADDR 0xBE // Unique I2C address identifying the board
#define I2C_READ 0x01
#define I2C_WRITE 0x00

void i2c_init();
void i2c_acquire_bus();
void i2c_reset();
void i2c_rx(uint8_t addr, int size, uint8_t *data, int delay_us);
uint8_t i2c_rx_byte(int delay_us);
void i2c_rxtx_end();
void i2c_tx(uint8_t addr, int size, uint8_t *data, int delay_us);
void i2c_tx_addr(uint8_t addr, uint8_t rw, int delay_us);
void i2c_tx_byte(uint8_t data, int delay_us);
int i2c_tx_complete();

#endif /* I2C_H_ */
