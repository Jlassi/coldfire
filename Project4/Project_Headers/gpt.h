/*
 * gpt.h
 *
 *  Created on: Feb 25, 2013
 *      Author: michaeltsteptoe
 */

#ifndef GPT_H_
#define GPT_H_

#include "support_common.h"

#include "pit.h"

extern uint32 __VECTOR_RAM[];
static uint32 btn_press_count;

asm __declspec(register_abi) void asm_set_ipl(int);
void gpt_port_ta_init();
__declspec(interrupt) void change_tempo();

#endif /* GPT_H_ */
