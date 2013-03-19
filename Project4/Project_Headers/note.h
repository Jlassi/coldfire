/*
 * note.h
 *
 *  Created on: Mar 18, 2013
 *      Author: Ramsey
 */

#ifndef NOTE_H_
#define NOTE_H_

#include "pwm.h"
#include "support_common.h"

#define N_A 0
#define N_B 1
#define N_C 2
#define N_D 3
#define N_E 4
#define N_F 5
#define N_G 6

// MIDI frequencies in Hertz
#define MIDI_C4 65
#define MIDI_D4 73
#define MIDI_E4 82
#define MIDI_F4 87
#define MIDI_G4 97
#define MIDI_A4 110

static int N_PS[7];
static int N_SC[7];
static int N_PWP[7];

void note_init();
void set_note(int note);


#endif /* NOTE_H_ */
