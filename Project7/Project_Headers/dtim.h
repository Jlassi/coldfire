/*
 * dtim.h
 *
 * Project 7
 * Ramsey Kant (rkant@asu.edu), Michael Steptoe (msteptoe@asu.edu)
 * CSE325 Embedded Microprocessor Systems Spring 2013
 */

#ifndef DTIM_H_
#define DTIM_H_

#include "support_common.h"

void dtim0_delay_us(int p_usecs);
void dtim0_init();
void dtim3_delay_us(int p_usecs);
void dtim3_init();

#endif /* DTIM_H_ */
