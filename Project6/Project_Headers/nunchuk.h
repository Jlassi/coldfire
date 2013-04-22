/*
 * nunchuk.h
 *
 *  Created on: Apr 21, 2013
 *      Author: Ramsey
 */

#ifndef NUNCHUK_H_
#define NUNCHUK_H_

#include "support_common.h"

#include "pit.h"
#include "i2c.h"

// I2C constants
#define NUNCHUK_I2C_ADDR 0x52 // Nunchuk address
#define NUNCHUK_I2C_DELAY_US 150 // Time between I2C reads/writes

// Input constants
#define NUNCHUK_INPUT_Z 0x00
#define NUNCHUK_INPUT_C 0x01
#define NUNCHUK_INPUT_UP 0x02
#define NUNCHUK_INPUT_DOWN 0x03
#define NUNCHUK_INPUT_LEFT 0x04
#define NUNCHUK_INPUT_RIGHT 0x05

// Controller input callback that accepts a byte as the input command
extern void (*g_callback_input)(uint8_t);

void nunchuk_init();
void nunchuk_default_callback(uint8_t input_cmd);
void nunchuk_set_input_callback(void (*cback)(uint8_t));
void nunchuk_read();
void nunchuk_xmit_cmd(uint8_t reg, uint8_t cmd);


#endif /* NUNCHUK_H_ */
