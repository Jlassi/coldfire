/*
 * song.h
 *
 *  Created on: Mar 18, 2013
 *      Author: Ramsey
 */

#ifndef SONG_H_
#define SONG_H_

#include "support_common.h"
#include "pit.h"

#define DELAY_HALF 120
#define DELAY_QUARTER 60

#define SONG_LEN 42

static int song_note[SONG_LEN];
static int song_delay[SONG_LEN];
static int song_cur_note;

void song_init();
void song_progress();

#endif /* SONG_H_ */
