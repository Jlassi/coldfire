/*
 * qspi.h
 *
 * Project 6
 * Ramsey Kant (rkant@asu.edu), Michael Steptoe (msteptoe@asu.edu)
 * CSE325 Embedded Microprocessor Systems Spring 2013
 */

#ifndef QSPI_H_
#define QSPI_H_

#include <stdio.h>

#include "support_common.h"

// Imported globals
extern uint32 __VECTOR_RAM[];

// Globals
extern uint16_t g_cmd;

void qspi_init(int baud, int delay);
void qspi_send(uint8_t *data, unsigned short size);

#endif /* QSPI_H_ */
