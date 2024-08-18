/**
Write a C program to swap the content of 2 variables entered
 through command line using function and pointer.
 */
#include <stdio.h>
int swap(int *n1, int *n2) {
    *n1 = *n1 ^ *n2;
    *n2 = *n1 ^ *n2;
    *n1 = *n1 ^ *n2;
}
int main(int argc, char *argv[]) {
	printf("This prints the name of the program: %s\n", argv[0]);
	if (argc > 1) {
		int n1 = atoi(argv[1]), n2 = atoi(argv[2]);
        printf("Before swapping: n1 = %d, n2 = %d\n", n1, n2);
        swap(&n1, &n2);
        printf("After swapping: n1 = %d, n2 = %d\n", n1, n2);
	} else {
		printf("No argument provided.\n");
	}
	return 0;
}