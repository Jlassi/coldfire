#include <stdio.h>

typedef int CF_LONGWord; 
typedef unsigned char CF_Byte;
typedef short CF_Word; 

void printBits(CF_LONGWord *number);
/*void setBit(CF_LONGWord &number, int pos); // Sets the bit at position “pos” in CF_LONGWord “number”.
void clearBit(CF_LONGWord &number, int pos); // Clears the bit at position “pos” in CF_LONGWord “number”. 
void setBit(CF_LONGWord &number, int pos, bool value); // Sets the bit at position “pos” in CF_LONGWord “number” to Boolean value “value”. 
bool getBit(CF_LONGWord number, int pos); // Returns the value of the bit at position “pos” in CF_LONGWord “number”. 
CF_LONGWord getBits(CF_LONGWord number, int hpos, int lpos); // Returns the value of bits from “hpos” to “lpos” in CF_LONGWord “number”. 
void setBits(CF_LONGWord &number, int hpos, int lpos, CF_LONGWord value); // Sets the bits from “hpos” to “lpos” in integer “number” to value “value”. 
CF_Byte getByte(CF_LONGWord number, int pos); // Returns the byte at position “pos”. 
CF_Word getWord(CF_LONGWord number, int pos); // Returns the word at position “pos”. 
CF_Byte setByte(CF_LONGWord number, int pos, CF_Byte value); // Returns the byte at position “pos” to value “value”. 
CF_Word setWord(CF_LONGWord number, int pos, CF_Word value); //Returns the word at position “pos” to value “value”.*/

void printBits(CF_LONGWord *number) {
	int i = 0;
	
	for (i = 128; i > 0; i >>= 1) {
        printf("%i", (((*number) & i) == i) ? 1 : 0);
    }
	
	printf("\n");
}


int main(void) {
	int x = 5;

	printBits(&x);
	return 0;
}
