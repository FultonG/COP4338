/*  Name: Fulton Garcia
*   PantherID: 5464347
*   This program was written manipulate words from a file
*   This shows the usage of the C language and how to use getopt
*   I affirm that I wrote this program myself without any help form any other people or sources from the internet.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <getopt.h>

int main(int argc, char **argv){
	extern char *optarg;
	extern int optind;
	int c, err = 0;
	int cflag=0, sflag=0, fflag = 0;
  char *substring;

	static char usage[] = "Usage: mywords[-cs][-f substring]filename\n";

	while ((c = getopt(argc, argv, "csf:")) != -1)
		switch (c) {
		case 's':
			sflag = 1;
			break;
		case 'c':
			cflag = 1;
			break;
		case 'f':
			fflag = 1;
      substring = strdup(optarg);
			break;
		case '?':
			err = 1;
			break;
		}
		if ((optind+1) > argc) {
		/* need at least one argument*/

		printf("optind = %d, argc=%d\n", optind, argc);
		fprintf(stderr, "%s: missing value\n", argv[0]);
		fprintf(stderr, usage, argv[0]);
		exit(1);
	} else if (err) {
		fprintf(stderr, usage, argv[0]);
		exit(1);
	}

	//since we already have a condition that checks for the extra value we don't need to
	//do another check here a it would have already exited the program with an error
  char words[100][50];
  int wordCount = 0;
	//writing the ifs in this order allows for me to do the correct order
	//without needing extra conditional statements
  if(!(sflag || cflag || fflag)){
    FILE *file = fopen(argv[optind], "r");
    c = fgetc(file); 
    while (c != EOF) 
    { 
        printf ("%c", c); 
        c = fgetc(file); 
    } 
    printf("\n");
    fclose(file); 
  }
  else{
    char character;
    char word[50];
    memset(word, 0, 50);
    int index = 0;
    FILE *file = fopen(argv[optind], "r");
    character = (char)fgetc(file); 
    while (character != EOF) 
    { 
      //to make sure I wasnt getting blank spaces I had to do the index check
        if((character == ' ' || character == '\n' || character == '\0' || character == '\t' || character == ',' || character == '.' || character == '?' || character == '!') && index > 0) {
          strcpy(words[wordCount], word);
          wordCount++;
          memset(word, 0, 50);
          index = 0;
        } else {
          word[index++] = character;
        } 
        character = (char)fgetc(file); 
    } 
    printf("\n");
    fclose(file);
  }
	if(sflag){
    //I ran out of time to sort. i was using a bubble sort using strcmp but was getting segfaults so I erased it
		printf("here we will sort\n");
    int x = 0;
    for(x = 0; x < wordCount; x ++){
      printf("%s \n", words[x]);
    }
    printf("\n");
	}
	if(cflag){
    //word count was done while splitting the text into the array
		printf("The word count is: %d\n", wordCount);
	}
	if(fflag){
    //using strstr we can get if it is a substring
    int count = 0;
    int x;
    for(x = 0; x < wordCount; x ++){
      if(strstr(words[x], substring) != NULL){
        count++;
      }
    }
    printf("%s appears %d times\n", substring, count);
	}

	exit(0);
}
