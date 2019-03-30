/*  Name: Fulton Garcia
*   PantherID: 5464347
*   This program was written to simulate a course gpa calculator
*   This shows the usage of the C language and how to make Makefile
*   I affirm that I wrote this program myself without any help form any other people or sources from the internet.
*/

#include <stdio.h>

int main(void){
    const NUM_COURSES = 6;
    //declarating an array of courses and filling it with string values
    char *course[NUM_COURSES];
    course[0] = "COP2210";
    course[1] = "ENC1101";
    course[2] = "CGS3095";
    course[3] = "COP3333";
    course[4] = "REL1000";
    course[5] = "COT3100";
    course[6] = "COT4320";
    int credits = 3;
    //another way of setting an array
    double grade[] = {2, 3, 2, 4, 3, 4};
    printf("Student Name: Fulton Garcia\n");
    printf("Panther ID: 1234565\n");
    int x;
    double totalGrade = 0;
    printf("%-7s \t%s\t  %s \t %s\n", "Course", "Credits", "Grade", "Grade Points Earned");
    //for loop to generate the courses
    for(x = 0; x < NUM_COURSES; x++){
        printf("%-7s \t   %d   \t   %4.2f \t  %18.2f \n", course[x], credits, grade[x], credits * grade[x]);
        totalGrade += credits * grade[x];
    }
    //printing total
    printf("%-7s \t   %d   \t  \t  %18.2f \n", "Total", credits * NUM_COURSES, totalGrade);
    printf("Current GPA: %1.2f\n", totalGrade/(credits * NUM_COURSES));
    return 0;
}
