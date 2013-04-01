/*
 * pit.h
 * 
 * Project 5
 * Ramsey Kant (rkant@asu.edu), Michael Steptoe (msteptoe@asu.edu)
 * CSE325 Embedded Microprocessor Systems Spring 2013
 */

#ifndef PIT_H_
#define PIT_H_

#include "exceptions.h"
#include "support_common.h"

extern uint32 pit_counter;
extern uint32 pit_next_play_count;

extern uint32 __VECTOR_RAM[];

void pit_init();
__declspec(interrupt) void pit_isr();

#endif /* PIT_H_ */
