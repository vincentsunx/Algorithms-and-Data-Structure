#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>


#include "ai.h"
#include "utils.h"
#include "priority_queue.h"


struct heap h;
/* explored array size */
int arrsize = 10000000;


/**
 * Function called by pacman.c
*/
void initialize_ai(){
	heap_init(&h);
}

/**
 * function to copy a src into a dst state
*/
void copy_state(state_t* dst, state_t* src){
	//Location of Ghosts and Pacman
	memcpy( dst->Loc, src->Loc, 5*2*sizeof(int) );

    //Direction of Ghosts and Pacman
	memcpy( dst->Dir, src->Dir, 5*2*sizeof(int) );

    //Default location in case Pacman/Ghosts die
	memcpy( dst->StartingPoints, src->StartingPoints, 5*2*sizeof(int) );

    //Check for invincibility
    dst->Invincible = src->Invincible;
    
    //Number of pellets left in level
    dst->Food = src->Food;
    
    //Main level array
	memcpy( dst->Level, src->Level, 29*28*sizeof(int) );

    //What level number are we on?
    dst->LevelNumber = src->LevelNumber;
    
    //Keep track of how many points to give for eating ghosts
    dst->GhostsInARow = src->GhostsInARow;

    //How long left for invincibility
    dst->tleft = src->tleft;

    //Initial points
    dst->Points = src->Points;

    //Remiaining Lives
    dst->Lives = src->Lives;   

}

node_t* create_init_node( state_t* init_state ){
	node_t * new_n = (node_t *) malloc(sizeof(node_t));
	new_n -> parent = NULL;	
	new_n -> priority = 0;
	new_n -> depth = 0;
	new_n -> num_childs = 0;
	copy_state(&(new_n->state), init_state);
	new_n -> acc_reward =  get_reward( new_n );
	return new_n;
	
}


float heuristic( node_t* n ){
	float h = 0.0;
	float i = 0.0;
	float l = 0.0;
	float g = 0.0;
	/* score for pacman eats a furit */
	i = (n -> state.Invincible) * 10;
	/* check whether pacamn loses a life and score for calculate acc_reward */
	if(n -> parent != NULL){
		if(n -> parent -> state.Lives > n -> state.Lives){
			l = 10;
		}
	}
	/* score for pacman died and game over */
	if(n -> state.Lives == 0){
		g = 100;
	}
	/* calculate the final heuristic score */
	h = i - l - g;
	return h;
}

/* funtion to calculate the reward for each node */
float get_reward ( node_t* n ){
	
	float reward = 0;
	float h_score = heuristic(n);
	float score_n = n -> state.Points;
	float score_n_p;
	if(n -> parent == NULL){
		score_n_p = score_n;
	}
	else{
		score_n_p = n -> parent -> state.Points;
	}
	reward = h_score + score_n - score_n_p;
	float discount = pow(0.99,n->depth);
   	
	return discount * reward;
}

/**
 * Apply an action to node n and return a new node resulting from executing the action
*/
bool applyAction(node_t* n, node_t** new_node, move_t action){
	bool changed_dir = false;
	*new_node = (node_t *) malloc(sizeof(node_t));
	/* copy infomation from n node to new node */
	copy_state(&((*new_node) -> state), &(n -> state));
    changed_dir = execute_move_t( &((*new_node)->state), action );
	/* set n be the parent node of new_node */
	(*new_node) -> parent = n;
	(*new_node) -> depth = n-> depth + 1;
	(*new_node) -> priority = -((*new_node) -> depth);
	(*new_node) -> num_childs = 0;
	(*new_node) -> acc_reward = get_reward(*new_node);
	/* set move for the node which depth is large than 1 the same as the parent node move 
	 * which is esay to calulate propation max and avg at next steps */
	if((*new_node) -> depth == 1){
		(*new_node) -> move = action;
	}
	else{
		(*new_node) -> move  = (*new_node) -> parent -> move;
	}
	return changed_dir;
}





/**
 * Find best action by building all possible paths up to budget
 * and back propagate using either max or avg
 */
/* adding a new struct pointer info which uses for print the infomation to the file */
move_t get_next_move( state_t init_state, int budget, propagation_t propagation, char* stats, info_t *info ){
	move_t best_action;
	/* use for getting time for running this funtion which is calculating best move time */
	clock_t begin = clock();
	float best_action_score[4];
	for(unsigned i = 0; i < 4; i++)
	    best_action_score[i] = INT_MIN;

	unsigned generated_nodes = 0;
	unsigned expanded_nodes = 0;
	unsigned max_depth = 0;
	


	//Add the initial node
	node_t* n = create_init_node( &init_state );
	/* malloc a array explore with size as arraysize */
	node_t **explored = malloc(sizeof(node_t*) * arrsize);
	/* set the first element of explored array is node n */
	explored[generated_nodes] = n;
	//Use the max heap API provided in priority_queue.h
	heap_push(&h,n);
	while(h.count > 0){
		node_t* node_prior = heap_delete(&h);
		if(expanded_nodes < budget){
			expanded_nodes++;
			for(unsigned i = 0; i < 4; i++){
				bool changed_dir = false; 
				node_t* new_node = NULL;
				changed_dir = applyAction(node_prior, &new_node, i);
				/* detect whether the move is available */
				if(changed_dir){
					/* change the max_depth */
					if(new_node -> depth > max_depth){
						max_depth = new_node ->depth;
					}
					/* add number of childss which is leaf node for the current node*/
					node_prior -> num_childs++;
					generated_nodes++;
					/* adding elements to array explored */
					explored_add(new_node, explored, generated_nodes);
					/* if losing life then do not add node to priority queue. if not
					* add it to PQ using heap_push */
					if(new_node -> state.Lives < node_prior -> state.Lives){
					}
					else{
						heap_push(&h, new_node);
					}
				} 
			}
		}
	}
	/* calculate score with propagation is max */
	if(propagation == max){
		propagate_back_max(explored, best_action_score, generated_nodes);
	}
	/* calculate score with propagation is avg */
	else if(propagation == avg){
		propagate_back_avg(explored, best_action_score, generated_nodes);
		/* in -> prop == 1 print out avg else print max in output file */
		info -> prop = 1;
	}
	best_action = select_best_action(best_action_score);
	clock_t end = clock();
	/* time spend for running this funtion once */
	float time_spent = (float) (end - begin) / CLOCKS_PER_SEC;
	
	/* update information at info_t structure */
	if(max_depth > info -> max_depth){
		info -> max_depth = max_depth;
	}
	info -> generated += generated_nodes;
	info -> expanded += expanded_nodes;
	info -> total_time += time_spent;
	/* free memory */
	free_memory(explored, generated_nodes);
	


	
	
	
	sprintf(stats, "Max Depth: %d Expanded nodes: %d  Generated nodes: %d\n",max_depth,expanded_nodes,generated_nodes);
	
	if(best_action == left)
		sprintf(stats, "%sSelected action: Left\n",stats);
	if(best_action == right)
		sprintf(stats, "%sSelected action: Right\n",stats);
	if(best_action == up)
		sprintf(stats, "%sSelected action: Up\n",stats);
	if(best_action == down)
		sprintf(stats, "%sSelected action: Down\n",stats);

	sprintf(stats, "%sScore Left %f Right %f Up %f Down %f",stats,best_action_score[left],best_action_score[right],best_action_score[up],best_action_score[down]);
	
	return best_action;
}






/* add node into explored array */
void explored_add(node_t* node, node_t **explored, int count) {

	if (count > arrsize) {
		explored = realloc(explored, (++arrsize)*sizeof(node_t*));
		if(!explored) {
			perror("realloc Node"); 
			exit (EXIT_FAILURE); 
		}
	}
	explored[count] = node; 
}



/* calculate score for max */
void propagate_back_max(node_t **explored, float* best_action_score, int generated_nodes){
	for(int i = generated_nodes; i >= 0; i--){
		node_t* temp = explored[i];
		/* find leaf node and then calculate score until it reaches to root node */
		if(temp -> num_childs == 0){
			float a_reward = 0.0;
			node_t* temp_parent = temp -> parent;
			a_reward += temp -> acc_reward;
			while(temp_parent != NULL){
				a_reward += temp_parent -> acc_reward;
				temp_parent = temp_parent -> parent;
			}
			/* if it is larger than pervious score, add to array best_action_score */
			if(a_reward > best_action_score[temp -> move]){
				best_action_score[temp -> move] = a_reward;
			}
		}
	}
	/* loop the whole explored array will get a max number for each direciton of move */
}


/* calculate score for avg */
void propagate_back_avg(node_t **explored, float* best_action_score, int generated_nodes){
	float total_reward_up = 0.0;
	float total_reward_down = 0.0;
	float total_reward_left = 0.0;
	float total_reward_right = 0.0;
	float total_child_left = 0;
	float total_child_right = 0;
	float total_child_up = 0;
	float total_child_down = 0;

	/* same as max, find a path and calculate the score, adding it to total_reward for each direction */
	for(int i = generated_nodes; i >= 0; i--){
		node_t* temp = explored[i];
		if(temp -> num_childs == 0){
			float a_reward = 0.0;
			node_t* temp_parent = temp -> parent;
			a_reward += temp -> acc_reward;
			while(temp_parent != NULL){
				a_reward += temp_parent -> acc_reward;
				temp_parent = temp_parent -> parent;
			}
			if(temp -> move == left){
				total_reward_left += a_reward;
				total_child_left+=1.0;
			}
			else if(temp -> move == right){
				total_reward_right += a_reward;
				total_child_right+=1.0;
			}
			else if(temp -> move == up){
				total_reward_up += a_reward;
				total_child_up+=1.0;
			}
			else if(temp -> move == down){
				total_reward_down += a_reward;
				total_child_down+=1.0;
			}
		}
	}
	/* compare to each diretion, and get the max score for each diretion with avg propagation */
	if((total_reward_left / total_child_left) > best_action_score[left]){
		best_action_score[left] = (total_reward_left / total_child_left);
	}
	if((total_reward_right / total_child_right) > best_action_score[right]){
		best_action_score[right] = (total_reward_right / total_child_right);
	}
	if((total_reward_up / total_child_up) > best_action_score[2]){
		best_action_score[2] = (total_reward_up / total_child_up);
	}
	if((total_reward_down / total_child_down) > best_action_score[3]){
		best_action_score[3] = (total_reward_down / total_child_down);
	}
}

/* loop the best_action array find the highest score move */
move_t select_best_action(float* best_action_score){
	float best_score = INT_MIN;
	float* same;
	move_t best_action;
	for(unsigned i = 0; i < 4; i++){
		if(best_action_score[i] > best_score){
			best_score = best_action_score[i];
			best_action = i;
		}
		/* if there is a situation which have same higest score at seveal moves */
		/* using rand() to decide the move direction */
		else if(best_action_score[i] == best_score){
			if(rand() < RAND_MAX/2){
				best_action = i;
			}
		}
	}
	return best_action;
}

/* free explored array */
void free_memory(node_t** explored, int num) {
	int i;
	for(i = 0; i < num; i++) {
		free(explored[i]);
	}
	free(explored);
}


/* print the result by using info_t structure */
void print_info(info_t *info, int score){
	 FILE *fp = fopen("output", "w");
	 
	 /* calculate expanded nodes per second */
	 int expandedSec = info -> expanded  / info -> total_time;
	 if(info -> prop == 0){
		fprintf(fp, "Propagation = Max\n");
	 }
	 else{
		fprintf(fp, "Propagation = Avg\n");
	 }
	 
	 fprintf(fp, "Budget = %d\n", info -> budget);
	 fprintf(fp, "MaxDepth = %d\n", info -> max_depth);
	 fprintf(fp, "TotalGenerated = %d\n", info -> generated);
	 fprintf(fp, "TotalExpanded = %d\n", info -> expanded);
	 fprintf(fp, "Time = %.2f seconds\n", info -> total_time);
	 fprintf(fp, "Expanded/Second = %d\n", expandedSec);
	 fprintf(fp, "Score = %d\n", score);
	 fclose(fp);
}



	
	