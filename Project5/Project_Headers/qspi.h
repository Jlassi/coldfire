/*
 * qspi.h
 *
 * Project 5
 * Ramsey Kant (rkant@asu.edu), Michael Steptoe (msteptoe@asu.edu)
 * CSE325 Embedded Microprocessor Systems Spring 2013
 */

#ifndef QSPI_H_
#define QSPI_H_

#include <stdio.h>

#include "support_common.h"

extern uint32 __VECTOR_RAM[];

extern uint16_t g_cmd;

void qspi_init(int baud, int delay);
void qspi_send(uint8_t *data, unsigned short size);
//void qspi_recv(uint8_t *out_data, unsigned short size);

#endif /* QSPI_H_ */
