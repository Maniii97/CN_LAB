/**
Write a C program to extract each byte from a given number and store them in
separate character variables and print the content of those variables.
 */
#include <stdio.h>

int main() {
	int number;
	char byte1, byte2, byte3, byte4;

	printf("Enter a number: ");
	scanf("%u", &number);

    char *ptr = (char *)&number;
    for (int i = 0; i < 4; i++) {
        printf("Byte %d: %d\n", i + 1, *(ptr + i));
    }
    // second logic 
    
    int set = 0xff; // ye 1 byte of set bits store krta hai
	byte1 = (number & set); 
	byte2 = (number >> 8) & set; 
	byte3 = (number >> 16) & set; 
	byte4 = (number >> 24) & set; 

	printf("Byte 1: %d\n", byte1);
	printf("Byte 2: %d\n", byte2);
	printf("Byte 3: %d\n", byte3);
	printf("Byte 4: %d\n", byte4);

	return 0;
}