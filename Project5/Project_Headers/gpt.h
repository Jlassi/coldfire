/*
 * gpt.h
 *
 *  Created on: Feb 25, 2013
 *      Author: michaeltsteptoe
 */

#ifndef GPT_H_
#define GPT_H_

#include <stdio.h>

#include "support_common.h"
#include "pit.h"

extern uint32 __VECTOR_RAM[];
extern uint32 program_mode;
extern uint32 btn_press_count;

void gpt_port_ta_init();
__declspec(interrupt) void gpt_isr();

#endif /* GPT_H_ */
