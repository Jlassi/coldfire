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
void setBits(CF_LONGWord *number, int hpos, int lpos, CF_LONGWord value);
CF_Byte getByte_BE(CF_LONGWord number, int pos); // Returns the byte at position pos. 
CF_Word getWord_BE(CF_LONGWord number, int pos); // Returns the word at position pos. 
void setByte_BE(CF_LONGWord *number, int pos, CF_Byte value); // Returns the byte at position pos to value value.
void setWord_BE(CF_LONGWord *number, int pos, CF_Word value); //Returns the word at position pos to value value.

//Little-Endian
CF_Byte getByte_LE(CF_LONGWord number, int pos); // Returns the byte at position pos.
CF_Word getWord_LE(CF_LONGWord number, int pos); // Returns the word at position pos.
void setByte_LE(CF_LONGWord *number, int pos, CF_Byte value); // Returns the byte at position pos to value value.
void setWord_LE(CF_LONGWord *number, int pos, CF_Word value); //Returns the word at position pos to value value.


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
	//printf("Bit count: %i\n", bc);
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
void setBits(CF_LONGWord *number, int hpos, int lpos, CF_LONGWord value) {
	// Set all bits within the hpos and lpos range
	
    for(int i=lpos; i<hpos && i<32; i++){
        clearBit(number,i);
    }
    *number |= value << lpos;

    /*
    for(int i = LONG_WORD_BITS-1; i >= 0; i--) {
		if((i < hpos) && (i >= lpos)) {
			setBitVal(number, i, value);
		}
	}
     */
}

// Returns the byte at position pos.
CF_Byte getByte_BE(CF_LONGWord number, int pos){
    CF_Byte result = 0;
    return result |= number >> pos*8;
}

// Returns the word at position pos. 
CF_Word getWord_BE(CF_LONGWord number, int pos){
    CF_Word result = 0;
    return result |= number >> pos*16;
}

// Sets the byte at position pos to value.
void setByte_BE(CF_LONGWord *number, int pos, CF_Byte value) {
	/*for(int i=pos*8; i<(pos+1)*8 && i<32; i++){
        clearBit(number,i);
    }
    *number |= value << pos*8;*/
    setBits(number, (pos+1)*8, (pos)*8, value);
}

// Sets the word at position pos to value.
void setWord_BE(CF_LONGWord *number, int pos, CF_Word value){
/*	setByte(number,pos,value);
    setByte(number,(pos+1),(value >> 8));
*/    setBits(number, (pos+1)*16, pos*16, value);
}


//Little-Endian Functions
// Returns the byte at position pos.
CF_Byte getByte_LE(CF_LONGWord number, int pos){
    CF_Byte result =0;
    return result |= number >> (24-pos*8);
}

// Returns the word at position pos.
CF_Word getWord_LE(CF_LONGWord number, int pos){
    CF_Word result = 0;
    return result |= number >> (16-pos*16);
}


// Sets the byte at position pos to value.
void setByte_LE(CF_LONGWord *number, int pos, CF_Byte value){
/*    for(int i=(31-pos*8); i>(31-(pos+1)*8) && i>-1; i--){
        clearBit(number,i);
    }
    *number |= value << (24-pos*8);
*/    setBits(number, (32-pos*8), (32-(pos+1)*8), value);
}

// Sets the word at position pos to value.
void setWord_LE(CF_LONGWord *number, int pos, CF_Word value){
    setByte_LE(number, (pos*2)+1, (value >> 8));
    setByte_LE(number, pos*2, value);
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
	
	printf("Set bits from 10 (high) to 7 (low) to the value 1: ");
	setBits(&x, 10, 7, 1);
	printBits(&x, LONG_WORD_BITS);
	
	printf("Setting bit at position 4: ");
	setBitVal(&x, 4, true);
	printBits(&x, LONG_WORD_BITS);
	
	printf("Clearing bit at position 4: ");
	clearBit(&x, 4);
	printBits(&x, LONG_WORD_BITS);
	
	printf("Bit at position 0: %i\n", (int)getBit(x, 0));
	printf("Bit at position 1: %i\n", (int)getBit(x, 1));
    
    x = 302503000;
	printf("Number (x) set to: ");
	printBits(&x, LONG_WORD_BITS);
    
    setByte_BE(&x,0,0);
    printf("Setting byte at position 0 to 0: ");
    printBits(&x, LONG_WORD_BITS);
	
    CF_Byte y = getByte_BE(x,0);
    printf("Getting byte at position 0: ");
    printBits(&y, 8);
    
	y = getByte_BE(x,2);
    printf("Getting byte at position 2: ");
    printBits(&y, 8);

    CF_Word z = getWord_BE(x,0);
    printf("Getting Word at position 0: ");
    printBits(&z, WORD_BITS);
    
    z = getWord_BE(x,1);
    printf("Getting Word at position 1: ");
    printBits(&z, WORD_BITS);
    
    setWord_BE(&x,1,0);
    printf("Setting Word at position 1 to 0: ");
    printBits(&x, LONG_WORD_BITS);

    setWord_BE(&x,1,6767);
    printf("Little-Endian_ Setting Word at position 1 to 6767: ");
    printBits(&x, LONG_WORD_BITS);
    
    x = 302503000;
	printf("Number (x) set to: ");
	printBits(&x, LONG_WORD_BITS);
    
    setByte_LE(&x,0,0);
    printf("Little-Endian_ Setting byte at position 0 to 0: ");
    printBits(&x, LONG_WORD_BITS);
	
    y = getByte_LE(x,0);
    printf("Little-Endian_ Getting byte at position 0: ");
    printBits(&y, 8);
    
    y = getByte_LE(x,2);
    printf("Little-Endian_ Getting byte at position 2: ");
    printBits(&y, 8);

    z = getWord_LE(x,0);
    printf("Little-Endian_ Getting Word at position 0: ");
    printBits(&z, WORD_BITS);
    
    z = getWord_LE(x,1);
    printf("Little-Endian_ Getting Word at position 1: ");
    printBits(&z, WORD_BITS);
    
    setWord_LE(&x,1,0);
    printf("Little-Endian_ Setting Word at position 1 to 0: ");
    printBits(&x, LONG_WORD_BITS);
    
    setWord_LE(&x,1,6767);
    printf("Little-Endian_ Setting Word at position 1 to 6767: ");
    printBits(&x, LONG_WORD_BITS);
    
	return 0;
}
