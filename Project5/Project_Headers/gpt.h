/*
 * gpt.h
 *
 * Project 5
 * Ramsey Kant (rkant@asu.edu), Michael Steptoe (msteptoe@asu.edu)
 * CSE325 Embedded Microprocessor Systems Spring 2013
 */

#ifndef GPT_H_
#define GPT_H_

#include <stdio.h>

#include "support_common.h"

extern uint32 __VECTOR_RAM[];
extern uint32_t program_mode;

void gpt_port_ta_init();
__declspec(interrupt) void gpt_isr();

#endif /* GPT_H_ */
