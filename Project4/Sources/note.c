#include "note.h"

void note_init() {
	// A
	N_PS[N_A] = 3;
	N_SC[N_A] = 92;
	N_PWP[N_A] = 247;
	
	// C
	N_PS[N_C] = 3;
	N_SC[N_C] = 152;
	N_PWP[N_C] = 253;
	
	// D
	N_PS[N_D] = 4;
	N_SC[N_D] = 80;
	N_PWP[N_D] = 214;
	
	// E
	N_PS[N_E] = 4;
	N_SC[N_E] = 74;
	N_PWP[N_E] = 206;
	
	// F
	N_PS[N_F] = 3;
	N_SC[N_F] = 114;
	N_PWP[N_F] = 252;
	
	// G
	N_PS[N_G] = 4;
	N_SC[N_G] = 57;
	N_PWP[N_G] = 226;
}

void set_note(int note) {
	MCF_PWM_PWMPRCLK = N_PS[note];
	MCF_PWM_PWMSCLB = N_SC[note];
	MCF_PWM_PWMPER7 = N_PWP[note];
}
