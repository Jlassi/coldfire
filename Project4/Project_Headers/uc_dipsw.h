/*
 * uc_dipsw.h
 *
 * Lab 3
 * Ramsey Kant (rkant@asu.edu), Michael Steptoe (msteptoe@asu.edu)
 * CSE325 Embedded Microprocessor Systems Spring 2013
 */

#ifndef UC_DIPSW_H_
#define UC_DIPSW_H_

#include "gpio.h"

int uc_dipsw_get_state(int p_switch);
void uc_dipsw_init();

#endif
