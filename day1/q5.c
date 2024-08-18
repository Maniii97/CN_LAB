/**
Write a C program to check whether the Host machine is in Little Endian or
Big Endian. Enter a number, print the content of each byte location and
Convert the Endianness of the same i.e. Little to Big Endian and vice-versa.
*/
#include <stdio.h>
void swap(char* a, char* b){
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}
int main(){
    int n = 1;
    char byte = n & 0xff;
    if(byte == 1) printf("System is little endian \n");
    else printf("big endian ");
    // using pointer
    char *bytee = (char*)&n;
    if(*bytee == 1) printf("System is little endian");
    else printf("big endian");

    // converting endianness
    int number;
    printf("Enter a number: ");
    scanf("%u", &number);
    char *ptr = (char *)&number;

    printf("Before converting endianness: %x\n", number);
    for (int i = 0; i < 4; i++) {
        printf("%d ",*(ptr + i));
    }
    printf("\nAfter converting endianness:\n");
    for (int i = 3; i >= 2; i--) {
        swap((ptr + i), (ptr + 3 - i));
    }
    for (int i = 0; i < 4; i++) {
        printf("%d ",*(ptr + i) );
    }

}