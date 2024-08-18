/**
Write a C Program to enter a number and store the number across the
following structure and print the content of each member of the structure.
Then aggregate each member of the structure to form the original number and
print the same.
*/
#include <stdio.h>

struct pkt {
    char ch1;
    char ch2[2];
    char ch3;
};
int main(){
    int num;
    struct pkt p;
    printf("Enter a number: ");
    scanf("%d", &num);
}