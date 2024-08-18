/**
Write a C program to assign values to each member of the following structure.
Pass the populated structure to a function Using call-by-value and another
function using call-by-address and print the value of each member of the
structure.
*/
#include <stdio.h>
struct dob{
    int day;
    int month;
    int year;
};
struct student_info{
    int roll_no;
    char name [50];
    float CGPA;
    struct dob age;
};
void print_call_by_value(struct student_info student){
    printf("Roll No: %d\n", student.roll_no);
    printf("CGPA: %.2f\n", student.CGPA);
    printf("DOB: %d/%d/%d\n", student.age.day, student.age.month, student.age.year);
}
void print_call_by_reference(struct student_info *student){
    printf("Roll No: %d\n", student->roll_no);
    printf("CGPA: %.2f\n", student->CGPA);
    printf("DOB: %d/%d/%d\n", student->age.day, student->age.month, student->age.year);
}

int main(){
    struct student_info student;
    student.roll_no = 1;
    student.CGPA = 6.9;
    student.age.day = 18;
    student.age.month = 9;
    student.age.year = 2003;
    printf("Call by value:\n");
    print_call_by_value(student);
    printf("Call by reference:\n");
    print_call_by_reference(&student);
    return 0;
}