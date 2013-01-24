#include <stdio.h>
#include <stdbool.h>
#include <math.h>

typedef int CF_LONGWord; 
typedef unsigned char CF_Byte;
typedef short CF_Word; 

void printBits(CF_LONGWord *number);
void setBit(CF_LONGWord *number, int pos);
void clearBit(CF_LONGWord *number, int pos);
void setBitVal(CF_LONGWord *number, int pos, bool value);
bool getBit(CF_LONGWord number, int pos);
CF_LONGWord getBits(CF_LONGWord number, int hpos, int lpos);
void setBits(CF_LONGWord *number, int hpos, int lpos, bool value);
CF_Byte getByte(CF_LONGWord number, int pos); // Returns the byte at position pos. 
CF_Word getWord(CF_LONGWord number, int pos); // Returns the word at position pos. 
CF_Byte setByte(CF_LONGWord number, int pos, CF_Byte value); // Returns the byte at position pos to value value. 
CF_Word setWord(CF_LONGWord number, int pos, CF_Word value); //Returns the word at position pos to value value.

// Prints a binary representation of a CF_LONGWord
void printBits(CF_LONGWord *number) {
	int i = 0;
	int k = 0;
	
	for (i = pow(2, (8*sizeof(CF_LONGWord))-2); i > 0; i >>= 1) {
        printf("%i", (((*number) & i) == i) ? 1 : 0);
        k++;
    }
	
	printf("\n");
	//printf("Bits: %i\n", k);
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
	for(int i = sizeof(CF_LONGWord)-1; i >= 0; i--) {
		if(i > hpos || i < lpos)
			clearBit(&ret, i);
	}
	
	return ret;
}

// Sets the bits from hpos to lpos in integer number to value.
void setBits(CF_LONGWord *number, int hpos, int lpos, bool value) {
	// Set all bits within the hpos and lpos range
	for(int i = sizeof(CF_LONGWord)-1; i >= 0; i--) {
		if((i <= hpos) && (i >= lpos)) {
			setBitVal(number, i, value);
		}
	}
}

int main(void) {
	/*printf("CF_LONGWord size: %i\n", sizeof(CF_LONGWord));
	printf("CF_Word size: %i\n", sizeof(CF_Word));
	printf("CF_Byte size: %i\n", sizeof(CF_Byte));*/
	
	CF_LONGWord x = 5;
	CF_LONGWord y = 0;
	printBits(&x);
	
	printf("Bits between 3 (high) and 1 (low): ");
	y = getBits(x, 3, 1);
	printBits(&y);
	
	printf("Set bits between 10 (high) and 7 (low) to 1: ");
	setBits(&x, 10, 7, true);
	printBits(&x);
	
	printf("Setting bit at position 4: ");
	setBitVal(&x, 4, true);
	printBits(&x);
	
	printf("Clearing bit at position 4: ");
	clearBit(&x, 4);
	printBits(&x);
	
	printf("Bit at position 0: %i\n", (int)getBit(x, 0));
	printf("Bit at position 1: %i\n", (int)getBit(x, 1));
	return 0;
}
