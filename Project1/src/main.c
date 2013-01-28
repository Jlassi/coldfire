/****************************************************************
*   Team Member 1: Ramsey Kant
*   Team Member 2: Michael Steptoe
*   CSE325 Embedded Microprocessor Systems
*   Spring 2013
*   Lab Project 1
*   Effort Division- Team Member 1: 50%, Team Member 2: 50%
*****************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

typedef int CF_LONGWord; 
typedef unsigned char CF_Byte;
typedef short CF_Word;
#define LONG_WORD_BITS 8*sizeof(CF_LONGWord)
#define WORD_BITS 8*sizeof(CF_Word)

void printBits(void *number, int bit_len);
void setBit(CF_LONGWord *number, int pos);
void clearBit(CF_LONGWord *number, int pos);
void setBitVal(CF_LONGWord *number, int pos, bool value);
bool getBit(CF_LONGWord number, int pos);
CF_LONGWord getBits(CF_LONGWord number, int hpos, int lpos);
void setBits(CF_LONGWord *number, int hpos, int lpos, bool value);
CF_Byte getByte(CF_LONGWord number, int pos); // Returns the byte at position pos. 
CF_Word getWord(CF_LONGWord number, int pos); // Returns the word at position pos. 
void setByte(CF_LONGWord *number, int pos, CF_Byte value); // Returns the byte at position pos to value value.
void setWord(CF_LONGWord *number, int pos, CF_Word value); //Returns the word at position pos to value value.

// Prints a binary representation of a CF_LONGWord
void printBits(void *number, int bit_len) {
	int i = 0, m = 0, bc = 0;
	int *num = number;
	
	for (i = bit_len-1; i >= 0; i--) {
		m = (*num) >> i;
 
		if (m & 1)
			printf("1");
		else
			printf("0");
			
		bc++;
	}
	
	printf("\n");
	printf("Bit count: %i\n", bc);
}

// Sets the bit at position pos in CF_LONGWord number
void setBit(CF_LONGWord *number, int pos) {
	*number |= 1 << pos;
}

// Clears the bit at position pos in CF_LONGWord number. 
void clearBit(CF_LONGWord *number, int pos) {
	*number &= ~(1 << pos);
}

// Sets the bit at position pos in CF_LONGWord number to Boolean value. 
void setBitVal(CF_LONGWord *number, int pos, bool value) {
	if(value)
		setBit(number, pos);
	else
		clearBit(number, pos);
}

// Returns the value of the bit at position pos in CF_LONGWord number. 
bool getBit(CF_LONGWord number, int pos) {
	return number & (1 << pos);
}

// Returns the value of bits from hpos to lpos in CF_LONGWord number.
CF_LONGWord getBits(CF_LONGWord number, int hpos, int lpos) {
	CF_LONGWord ret = number;
	
	// Clear any bits not within the hpos and lpos range
	for(int i = LONG_WORD_BITS-1; i >= 0; i--) {
		if(i > hpos || i < lpos) {
			clearBit(&ret, i);
		}
	}
	
	return ret;
}

// Sets the bits from [lpos, hpos) in integer number to value.
void setBits(CF_LONGWord *number, int hpos, int lpos, bool value) {
	// Set all bits within the hpos and lpos range
	for(int i = LONG_WORD_BITS-1; i >= 0; i--) {
		if((i < hpos) && (i >= lpos)) {
			setBitVal(number, i, value);
		}
	}
}

// Returns the byte at position pos.
CF_Byte getByte(CF_LONGWord number, int pos){
    CF_Byte result =0;
    return result |= number >> pos*8;
}

// Returns the word at position pos. 
CF_Word getWord(CF_LONGWord number, int pos){
    CF_Word result = 0;
    return result |= number >> pos*16;
}

// Sets the byte at position pos to value.
void setByte(CF_LONGWord *number, int pos, CF_Byte value) {
	setBits(number, (pos+1)*8, (pos)*8, value);
}

// Sets the word at position pos to value.
void setWord(CF_LONGWord *number, int pos, CF_Word value){
	setBits(number, (pos*8)+WORD_BITS, pos*8, value);
}

int main(void) {
	printf("CF_LONGWord size: %i\n", sizeof(CF_LONGWord));
	printf("CF_Word size: %i\n", sizeof(CF_Word));
	printf("CF_Byte size: %i\n", sizeof(CF_Byte));
	printf("LONG_WORD_BITS: %i\n", LONG_WORD_BITS);

	CF_LONGWord x = 5;
	CF_LONGWord w = 0;
	printBits(&x, LONG_WORD_BITS);
	
	printf("Bits between 3 (high) and 1 (low): ");
	w = getBits(x, 3, 1);
	printBits(&w, LONG_WORD_BITS);
	
	printf("Set bits between 10 (high) and 7 (low) to 1: ");
	setBits(&x, 10, 7, true);
	printBits(&x, LONG_WORD_BITS);
	
	printf("Setting bit at position 4: ");
	setBitVal(&x, 4, true);
	printBits(&x, LONG_WORD_BITS);
	
	printf("Clearing bit at position 4: ");
	clearBit(&x, 4);
	printBits(&x, LONG_WORD_BITS);
	
	printf("Bit at position 0: %i\n", (int)getBit(x, 0));
	printf("Bit at position 1: %i\n", (int)getBit(x, 1));
    
    x = 4294967295;
	printf("Number (x) set to: ");
	printBits(&x, LONG_WORD_BITS);
    
    setByte(&x,0,0);
    printf("Setting byte at position 0 to 0: ");
    printBits(&x, LONG_WORD_BITS);
	
    CF_Byte y = getByte(x,0);
    printf("Getting byte at position 0: ");
    printBits(&y, 8);
    
    CF_Word z = getWord(x,0);
    printf("Getting Word at position 0: ");
    printBits(&z, WORD_BITS);
    
    setWord(&x,1,0);
    printf("Setting Word at position 1 to 0: ");
    printBits(&x, LONG_WORD_BITS);
    
	return 0;
}
