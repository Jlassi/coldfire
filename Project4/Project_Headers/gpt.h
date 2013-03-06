/*
 * gpt.h
 *
 *  Created on: Feb 25, 2013
 *      Author: michaeltsteptoe
 */

#ifndef GPT_H_
#define GPT_H_

#include "support_common.h"

extern uint32 __VECTOR_RAM[];

void gpt_port_ta_init();
__declspec(interrupt) void change_tempo();

#endif /* GPT_H_ */
