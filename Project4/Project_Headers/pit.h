/*
 * pit.h
 *
 * Lab 3
 * Ramsey Kant (rkant@asu.edu), Michael Steptoe (msteptoe@asu.edu)
 * CSE325 Embedded Microprocessor Systems Spring 2013
 */

#ifndef PIT_H_
#define PIT_H_

#include "exceptions.h"

void pit_init();
vectorTableEntryType pit_isr();

#endif /* PIT_H_ */
