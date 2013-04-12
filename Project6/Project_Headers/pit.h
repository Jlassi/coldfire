/*
 * pit.h
 *
 * Project 5
 * Ramsey Kant (rkant@asu.edu), Michael Steptoe (msteptoe@asu.edu)
 * CSE325 Embedded Microprocessor Systems Spring 2013
 */

#ifndef PIT_H_
#define PIT_H_

#include <stdio.h>

#include "exceptions.h"
#include "support_common.h"

#include "led.h"
#include "game.h"

extern uint32 __VECTOR_RAM[];

extern uint32_t g_pit0_counter;

void pit0_init();
void pit0_stop();
__declspec(interrupt) void pit0_isr();

void pit1_init();
void pit1_stop();
__declspec(interrupt) void pit1_isr();

#endif /* PIT_H_ */
