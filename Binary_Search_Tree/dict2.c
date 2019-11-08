/* Program name: dict2
 * This program is part of assignments solution.
 * Write by Dechao Sun (student id: 980546).
 * This program create, insert and search data in binary search tree.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "dict2.h" 


/*****************************************************************************/
/* for dict2 create node with three leaves and a data string */
struct node{
    char *data;
    struct node *left;
    struct node *right;
    struct node *next;
};


/* every insert or search from root */
struct node *root = NULL;


/* create_node function create a new node and make data equal to info(input) */
struct node *create_node(char *info){
	struct node *bnode = (struct node*) malloc(sizeof(struct node));
	assert(bnode);
	bnode -> data = info;
	bnode -> left = bnode -> right = bnode -> next = NULL;
	return bnode;
}



/* insert node into binary search tree */
struct node *insert(struct node* node_b, char *data){
	/* if node is empty, using create node funtion to create a new node */
	if (node_b == NULL){
		return create_node(data);
	}
	/* compare existed data and new data, if it is less than 0 (go left) 
	store new data at next left empty node */
	if (compare_str(data, node_b -> data) < 0){
		node_b -> left = insert(node_b -> left, data);
	}
	/* compare existed data and new data, if it is larger than 0 (go right) 
	store new data at next right empty node */
	else if (compare_str(data, node_b -> data) > 0){
		node_b -> right = insert(node_b -> right, data);
	}
	/* compare existed data and new data, if it is equal (go next) 
	store new data at next "next" empty node (not go to left or right, go next)
	*/
	else{
		node_b -> next = insert(node_b -> next, data);
	}
	return node_b;
}


/*search funtion is used to search input with data in bst if they are the same 
print it in the file and count the number of comparisons */
struct node *search(struct node *root, FILE *fp2, char *input, int *count , int *n) {
	if (root == NULL){
		if (*n == 0){
			fprintf(fp2, "%s --> NOTFOUND\n",input);
		}
		return root;
	}
	/* if number n euqal to 0 means search funion not go into equal to 0 
		case which will print notfound in file */
	if (compare_str2(input, root -> data) == 0){
		fprintf(fp2, "%s\n",root -> data);
		(*count)++;
		(*n)++;
		/* if there is a match, print the match in the file and number count
		plus one. And then check whether the node has a next. if it has next,
		print data from all next node directly until it reach to null. */
		if(root -> next != NULL){
			//root = root -> next;
			//fprintf(fp2, "%s\n",root -> data);
			return search(root -> next, fp2, input, count, n);
		}
		//return search(root -> right, fp2, input, count, n);
		return NULL;
	}
	/* compare input and data in node if lager than 0 (go right) and count
	number plus one */
	else if (compare_str2(input, root -> data) > 0){
		(*count)++;
		return search(root -> right, fp2, input, count, n);
	}
	/* compare input and data in node if less than 0 (go left) and count
	number plus one */
	else{
		
		(*count)++;
		return search(root -> left, fp2, input, count, n);
	}
}




/*void inorder(struct node *root, FILE *fp2, char *input) { 
    if (root != NULL){ 
        inorder(root->left, fp2, input); 
        if((compare_str2(input, root -> data)) == 0){
        	fprintf(fp2, "%s\n",root -> data); 
        }
        inorder(root->right, fp2, input); 
    }
} */


/* free node in bst and free bst */
void free_tree(struct node *node_b){
	if(root != NULL){
		/* free every node in bst */
		free_tree(node_b -> right);
		free(node_b -> data);
		free_tree(node_b -> left);
		free_tree(node_b -> next);
		free(node_b);
	}
}



/* compare funtion for insert */
int compare_str(char *array_1, char *array_2){
	int i, j;
	int len1 = 1;
	int len2 = 1;
	int array1_len = 0;
	int array2_len = 0;
	/* using idigit to check the required length */
	for(j = 0; j < COMPARELEN; j++){
		if(isdigit(array_2[j])){
				array2_len = len2++;
		}
		if(isdigit(array_1[j])){
				array1_len = len1++;
		}		
	}
	/* if array1_len is larger than array2_len (means) array1 must longer than
	array2 which will return 1 (go right in the bst). Vice versa */
	if(array1_len > array2_len){
		return 1;
	}
	else if(array1_len < array2_len){
		return -1;
	}
	/* if length of two arrays are the same compare each char(number) */
	for (i = 0; i < COMPARELEN; i++){
		
		if(array_1[i] < array_2[i]){
			return -1;
		}
		else if (array_1[i] > array_2[i]){
			return 1;
		}
	}
	/* if the program go through for loop and not return which means array1 and 
	two are the same (return 0) */
	return 0;
}
		

/* compare funtion for search */
int compare_str2(char *array_1, char *array_2){
	int i, j;
	int len = 1;
	int array1_len = strlen(array_1);
	int array2_len = 0;
	/* using is digital to get useful length for array2 which is data in node */
	for(j = 0; j < COMPARELEN; j++){
		if(isdigit(array_2[j])){
				array2_len = len++;
		}	
	}
	/* if array1_len is larger than array2_len (means) array1 must longer than
	array2 which will return 1 (go right in the bst). Vice versa */
	if(array1_len > array2_len){
		return 1;
	}
	else if(array1_len < array2_len){
		return -1;
	}
	else{
		/* if length of two arrays are the same compare each char(number) 
		and the caompaerion length depend on input len */
		for (i = 0; i < array1_len; i++){
			
			if(array_1[i] < array_2[i]){
				return -1;
			}
			else if (array_1[i] > array_2[i]){
				return 1;
			}
			
		}
		/* if the program go through for loop and not return which means array1 and 
		two are the same (return 0) */
		return 0;
	}
}


/* combine each data as a long string */
char *data_combine(char **word){
	char *str_comb;
	char sword[] = " --> ";
	int i;
	str_comb = malloc(sizeof(char)* CHARNUM);
	assert(str_comb);
	/* combine data by using strcat. Put the PUlation at front */
	for(i = 0; i < ARRAYROW; i++){
		if(i == 5){
			strcat(word[i],sword);
			strcpy(str_comb,word[i]);
		}
		else if(i == 15){
			strcat(str_comb, word[i]);
		}
	}
	return str_comb;
}


	




/* read data from input file,  sepearate data by strlok method and then store 
into a array */
struct node *read_file(FILE *fp, struct node* root){
	char line[CHARLENGTH];
    char **word = NULL;
    char *token;
    char *str_res;
    int i = 0;
    int k;
    int length;
    
    str_res = (char*) malloc(sizeof(char) * CHARNUM);
    word = (char **) malloc(sizeof(char *) * ARRAYROW);
    assert(word);
    assert(str_res);
    /* read input line by line */
    while(fgets(line, sizeof line , fp) != NULL ){
    	length = strlen(line);
    	token =strtok(line, ",");
    	/* store data */
    	while(token != NULL){
    		if( i < ARRAYROW){
    			word[i] = (char *) malloc(sizeof(char) * length);
    			strcpy(word[i],token);
    			token = strtok(NULL, ",");
    			i++;
    		}
    		else {
    			i = 0;
    		}
    	}
    	/* if there is a newline character, convert it to end of string 
    	character */
    	for(k = 0; k < length+1; k++){
    		if(word[ARRAYROW - 1][k] == '\n'){
    			word[ARRAYROW - 1][k] = '\0';
    		}
    	}
    	/* using data_combine to combine data and using insert function to
    	insert into bst */
    	str_res = data_combine(word);
    	root = insert(root, str_res);
    }
    return root;
}


