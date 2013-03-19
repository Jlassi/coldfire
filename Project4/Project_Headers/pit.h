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
#include "support_common.h"

#include "song.h"
#include "uc_led.h"

static uint32 pit_counter;
static uint32 pit_next_play_count;
static uint32 pit_tempo_bpm;
static uint32 pit_tempo_dir;
static uint32 tog;

extern uint32 __VECTOR_RAM[];

void pit_setup_vars();
void pit_init();
__declspec(interrupt) void pit_isr();

#endif /* PIT_H_ */
