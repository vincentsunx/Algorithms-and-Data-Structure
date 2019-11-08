#ifndef __AI__
#define __AI__

#include <stdint.h>
#include <unistd.h>
#include "node.h"
#include "priority_queue.h"


void initialize_ai();
move_t get_next_move( state_t init_state, int budget, propagation_t propagation, char* stats ,info_t *info );
void print_info(info_t *info, int score);
float get_reward( node_t* n );
void explored_add(node_t* node, node_t **explored, int count);
void propagate_back_max(node_t **explored, float* best_action_score, int generated_nodes);
void propagate_back_avg(node_t **explored, float* best_action_score, int generated_nodes);
move_t select_best_action(float* best_action_score);
void free_memory(node_t** explored, int num);
void copy_state(state_t* dst, state_t* src);
bool applyAction(node_t* n, node_t** new_node, move_t action );
float heuristic( node_t* n );
#endif
