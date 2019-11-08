/* Program name: mian.c
 * This program is part of COMP20003 assignment 1 soultion.
 * Write by Dechao Sun (student id: 980546).
 * This program runs the method read file put into bst, search the key and
 * print it out (mian funtion of my program)
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "dict1.h"
#include "dict2.h"



/*****************************************************************************/

int 
main(int agrc, char *argv[]){
	char *input = NULL;
	size_t len = 0;
	ssize_t read;
	struct node *root = NULL;
	
	input = (char*) malloc(sizeof(char)*CHARLENGTH);
	assert(input);
    /* create two file point, fp for read file and fp2 for write to a new file
    */
	FILE *fp  = fopen(argv[1], "r"); 
    FILE *fp2 = fopen(argv[2], "w"); 
    
    if (fp == NULL) { 
               printf("Error! Could not open file\n"); 
               exit(-1);
    }
    /* using read_file funtion to read file into array */
    root = read_file(fp, root);
    /* read input put it into input line by line */
    while((read = getline(&input, &len, stdin)) != -1){
   	   	   int count = 0;
   	   	   int n = 0;
   	   	   int input_len = 0;
   	   	   int i = 0;
   	   	   input_len = strlen(input);
   	   	   /* remove last newline character */
   	   	   for( i = 0; i < input_len ; i++){
   	   	   	   if(input[i] =='\n'){
   	   	   	   	   input[i] = '\0';
   	   	   	   }
   	   	   }
   	   	   //inorder(root, fp2, input);
   	   	   /* using search funtion to find matched keys and write into file */
   		   search(root, fp2, input, &count, &n);
   		   /* print out stdout result */
   		   printf("%s --> %d\n",input, count);
   }
   /* free memory for input */
   if(input){
   	   free(input);
   }
   /* close two files */
   fclose(fp);
   fclose(fp2);
   return 0;
}


