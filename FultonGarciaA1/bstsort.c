/*  Name: Fulton Garcia
*   PantherID: 5464347
*   This program was written to learn how to better use pointers and binary search trees
*   as well as reading from a file and writing to a file
*   I hope you like my VIM jokes in here, I trying to exit VIM is the bane of my existance
*   I affirm that I wrote this program myself without any help form any other people or sources from the internet.
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <getopt.h>

//"All strings will be no more than 100 characters long."
#define ARRAYSIZE 100

//Here we will define a node that will be used to make the BST
typedef struct Node
{
   char *string;
   int counter;
   struct Node * left;
   struct Node * right;

} Node;

//Creates a copy of a string by dynamically allocating it
char *copyString(char * string){

   char * strPtr = (char*)calloc(ARRAYSIZE, sizeof(char));
   if (!strPtr)
       exit(0);

   int i = 0;
   while(string[i] != '\0'){
       strPtr[i] = string[i];
       i++;
   }
   strPtr[i] = '\0';

   return strPtr;
}

//Creates Node with a specified string data
Node * createNode(char * stringData){
   Node * newNode = (Node*) malloc(sizeof(Node));
   if (!newNode)
       exit(0);
   //Stores only a copy of the string
   newNode->string = copyString(stringData);
   newNode->left = NULL;
   newNode->right = NULL;
   newNode->counter = 1;
   return newNode;

}

//Case Insensitive String Compare
int insensitiveCompare (char *string, char *string2){
   int i;

   for (i = 0;string[i] != '\0' || string2[i] != '\0'; i++)
   {
    
       //Covert all letter to lower case in order to compare
       if (tolower(string[i]) > tolower(string2[i])){
           return i + 1;
       }
       if (tolower(string[i]) < tolower(string2[i])){
           return -i-1;
       }

   }

   return 0;

}

//Returns 0 if they are equal, 1 if string > string2, -1 if string < string2
int stringCompCaseSen(char *string, char *string2){

   int i = 0;

   //Uses compare string until it find the NULL character
   //Uses ASCCII numerical values to compare them
   while(string[i] != '\0' || string2[i] != '\0'){
       if(string[i] > string2[i]){
           return i+1;
       }
       if(string[i] < string2[i]){
           return -i-1;
       }

       i++;
   }

   return 0;
}


//insert node function that checks to see if you are doing it case sensitive
Node *insertNode(Node *node, char *string, int caseFlag){
   
   if (node == NULL){
       return(createNode(string));
   }
   if (caseFlag){
       if (stringCompCaseSen(string, node->string) < 0){
           node->left = insertNode(node->left, string, caseFlag);
       }
       else if (stringCompCaseSen(string, node->string) > 0){
           node->right = insertNode(node->right, string, caseFlag);
       }
       else {
           node->counter = node->counter + 1;
       }
   }
   else
   {
       if (insensitiveCompare(string, node->string) < 0){
            node->left = insertNode(node->left, string, caseFlag);
       }
        else if (insensitiveCompare(string, node->string) > 0){
                node->right = insertNode(node->right, string, caseFlag);
        }
        else {
                node->counter = node->counter + 1;
        }
   }

   return node;  

}

//Read from the command line / terminal
Node * cmdInput (Node * root, int caseFlag){
   char string[ARRAYSIZE];
   int exitFlag = 0;
   char exit[] = ":q";
   while (!exitFlag)
   {
       //Uses ":q" to exit which is a joke because VIM
       printf("Enter a sentence or :q to exit. \n");
       //this reads the input and does not match the next line character
       scanf(" %[^\n]s",string);
       if ((insensitiveCompare(string, exit)) == 0){
           exitFlag = 1;
       }
       else{
           root = insertNode(root, string, caseFlag);
       }
   }

   return root;
}

//Reads string from a file
Node * fileInput(Node *root, char **name, int caseFlag){
   char string[ARRAYSIZE];
   FILE *file;
   file = fopen(*name, "r");

   while(fscanf(file, " %[^\n]s", string) != EOF){
       root = insertNode(root, string, caseFlag);    
   }
   
   fclose(file);

   return root;

}

//Prints Node to the terminal or cmd
void cmdPrint(Node *node){
   if (!node){
       return;
   }

    cmdPrint(node->left);

    int x;
    for (x = 0; x < node->counter; x++){
        printf("%s\n",node->string);
    }
    cmdPrint(node->right);

   return;
}

//simple function to print to file
void printToFile(Node *node, char *name){
   FILE *file;        

   if (!node){
       return;
   }

   printToFile(node->left, name);

   file = fopen(name, "a");
   int x;

   for (x = 0; x < node->counter; x++){
       fprintf(file,"%s\n",node->string);
   }

    //always make sure to close file
    fclose(file);

   printToFile(node->right, name);

   return;

}


//printing the tree in order
void inOrder (Node *node){
   if (node == NULL){
       return;
   }
   inOrder(node->left);
   int x;
   for (x = 0; x < node->counter; x++){
       printf("in order: %100s \n", node->string);
   }
   inOrder(node->right);

   return;
}

//deallocates the memory
void deallocateMem(Node *node){
   if (node == NULL){
       return;
   }

   deallocateMem(node->left);
   deallocateMem(node->right);
   //some more freeeeeee freee falling
   free(node->string);
   free(node);

   return;
}

//Helper method to print String
void printString(char *string){
   int x = 0;
   while (string[x] != '\0'){
       printf("%c", string[x]);
       x++;
   }
   printf("\n");
   return;
}

int main (int argc, char **argv){

   int option;
   int cVal = 0; 
   int oVal = 0;   
   int inputFlag = 0; 

   //output and input are the pointers to the files to read and write from
   char *output = NULL;
   char **input = NULL;

   struct Node *root = NULL ;


   while ((option = getopt(argc, argv, "co:")) != -1)
   {
       switch (option) {
           case 'c':
               cVal = 1;
               break;
           case 'o':
               output = optarg;
               oVal = 1;
               break;
           default:
               printf("USAGE: %% bstsort [-c] [-o output_file_name] [input_file_name]");
               break;

       }
   }

   if (*(input = (argv + optind ))){
       inputFlag = 1;
   }

   if (inputFlag){
       root = fileInput(root, input, cVal);
   }
   else{
       root = cmdInput(root, cVal);
   }

   if (oVal){
       printToFile(root, output);
   }
   else{
       cmdPrint(root);
   }

   //FREEEEEEEE FREEE FALLLLLINGGGG 
   deallocateMem(root);


   return 0;
}