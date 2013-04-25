/*
 * gpt.h
 *
 * Project 6
 * Ramsey Kant (rkant@asu.edu), Michael Steptoe (msteptoe@asu.edu)
 * CSE325 Embedded Microprocessor Systems Spring 2013
 */

#ifndef GPT_H_
#define GPT_H_

#include <stdio.h>

#include "support_common.h"

#define MODE_PAUSE 0
#define MODE_PLAY 1

// Imported globals
extern uint32 __VECTOR_RAM[];

// Globals
extern uint32_t g_program_mode;

void gpt_port_ta_init();
__declspec(interrupt) void gpt_isr();

#endif /* GPT_H_ */
