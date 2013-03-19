#include "song.h"
#include "note.h"

void song_init() {
	song_cur_note = 0;
	
	// Initialize twinkle twinkle song
	song_note[0] = MIDI_C4; // C4
	song_delay[0] = DELAY_QUARTER;
	song_note[1] = MIDI_C4; // C4
	song_delay[1] = DELAY_QUARTER;
	song_note[2] = MIDI_G4; // G4
	song_delay[2] = DELAY_QUARTER;
	song_note[3] = MIDI_G4; // G4
	song_delay[3] = DELAY_QUARTER;
	song_note[4] = MIDI_A4; // A4
	song_delay[4] = DELAY_QUARTER;
	song_note[5] = MIDI_A4; // A4
	song_delay[5] = DELAY_QUARTER;
	song_note[6] = MIDI_G4; // G4
	song_delay[6] = DELAY_HALF;
	song_note[7] = MIDI_F4; // F4
	song_delay[7] = DELAY_QUARTER;
	song_note[8] = MIDI_F4; // F4
	song_delay[8] = DELAY_QUARTER;
	song_note[9] = MIDI_E4; // E4
	song_delay[9] = DELAY_QUARTER;
	song_note[10] = MIDI_E4; // E4
	song_delay[10] = DELAY_QUARTER;
	song_note[11] = MIDI_D4; // D4
	song_delay[11] = DELAY_QUARTER;
	song_note[12] = MIDI_D4; // D4
	song_delay[12] = DELAY_QUARTER;
	song_note[13] = MIDI_C4; // C4
	song_delay[13] = DELAY_HALF;
	song_note[14] = MIDI_G4; // G4
	song_delay[14] = DELAY_QUARTER;
	song_note[15] = MIDI_G4; // G4
	song_delay[15] = DELAY_QUARTER;
	song_note[16] = MIDI_F4; // F4
	song_delay[16] = DELAY_QUARTER;
	song_note[17] = MIDI_F4; // F4
	song_delay[17] = DELAY_QUARTER;
	song_note[18] = MIDI_E4; // E4
	song_delay[18] = DELAY_QUARTER;
	song_note[19] = MIDI_E4; // E4
	song_delay[19] = DELAY_QUARTER;
	song_note[20] = MIDI_D4; // D4
	song_delay[20] = DELAY_HALF;
	song_note[21] = MIDI_G4; // G4
	song_delay[21] = DELAY_QUARTER;
	song_note[22] = MIDI_G4; // G4
	song_delay[22] = DELAY_QUARTER;
	song_note[23] = MIDI_F4; // F4
	song_delay[23] = DELAY_QUARTER;
	song_note[24] = MIDI_F4; // F4
	song_delay[24] = DELAY_QUARTER;
	song_note[25] = MIDI_E4; // E4
	song_delay[25] = DELAY_QUARTER;
	song_note[26] = MIDI_E4; // E4
	song_delay[26] = DELAY_QUARTER;
	song_note[27] = MIDI_D4; // D4
	song_delay[27] = DELAY_HALF;
	song_note[28] = MIDI_C4; // C4
	song_delay[28] = DELAY_QUARTER;
	song_note[29] = MIDI_C4; // C4
	song_delay[29] = DELAY_QUARTER;
	song_note[30] = MIDI_G4; // G4
	song_delay[30] = DELAY_QUARTER;
	song_note[31] = MIDI_G4; // G4
	song_delay[31] = DELAY_QUARTER;
	song_note[32] = MIDI_A4; // A4
	song_delay[32] = DELAY_QUARTER;
	song_note[33] = MIDI_A4; // A4
	song_delay[33] = DELAY_QUARTER;
	song_note[34] = MIDI_G4; // G4
	song_delay[34] = DELAY_HALF;
	song_note[35] = MIDI_F4; // F4
	song_delay[35] = DELAY_QUARTER;
	song_note[36] = MIDI_F4; // F4
	song_delay[36] = DELAY_QUARTER;
	song_note[37] = MIDI_E4; // E4
	song_delay[37] = DELAY_QUARTER;
	song_note[38] = MIDI_E4; // E4
	song_delay[38] = DELAY_QUARTER;
	song_note[39] = MIDI_D4; // D4
	song_delay[39] = DELAY_QUARTER;
	song_note[40] = MIDI_D4; // D4
	song_delay[40] = DELAY_QUARTER;
	song_note[41] = MIDI_C4; // C4
	song_delay[41] = DELAY_HALF;
}

void song_progress() {
	if(song_cur_note >= SONG_LEN) {
		song_cur_note = 0;
		pit_counter = 0;
	}
	
	pwm_init();
	set_note(song_note[song_cur_note]);
	
	printf("song cur note: %i, pit_tempo_bpm: %i\n", song_cur_note, pit_tempo_bpm);
	
	int count_inc = (int)((song_delay[song_cur_note] / pit_tempo_bpm) / 0.05);
	
	pit_next_play_count = pit_counter + count_inc;
	
	song_cur_note++;
	
	MCF_PWM_PWMCNT7 = 0;
}
