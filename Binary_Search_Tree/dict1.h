/* This program is header for dict1.c
 * Write by Dechao Sun (student id: 980546).
 */

#define ARRAYROW 18     /* number of attributes in one line input data */
#define CHARNUM 10000   /* length of combined string */
#define CHARLENGTH 127	/* length of single sub string */








/* create_node function create a new node and make data equal to info(input) */
struct node *create_node(char *info);

/* insert node into binary search tree */
struct node *insert(struct node* node, char *data);

/*search funtion is used to search input with data in bst if they are the same 
print it in the file and count the number of comparisons */
struct node *search(struct node *root, FILE *fp2, char *input, int *count, 
	int *n);

/* compare funtion for insert and search */
int compare_str(char *array_1, char *array_2);

/* combine each data as a long string */	
char *data_combine(char **word);

/* read data from input file,  sepearate data by strlok method and then store 
into a array */
struct node *read_file(FILE *fp, struct node* root);

/* free node in bst and free bst */
void free_tree(struct node *node_b);
