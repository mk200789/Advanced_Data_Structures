/*
	Homework 3
	
	Compiles and run with command lines
			gcc -o hw3 hw3.c
			./hw2
*/


#include <stdio.h>
#include <stdlib.h>

#define BLOCKSIZE 256

//structure of o_t
typedef struct tr_n_t{
	int key;
	struct tr_n_t *left;
	struct tr_n_t *right;
	struct tr_n_t *previous;
	int height;
}o_t;

o_t *current_block = NULL;
o_t *free_list = NULL;
int size_left;

//returns a node
o_t *get_node(){
	o_t *tmp;
	if (free_list != NULL){
		tmp = free_list;
		free_list = free_list -> left;
	}
	else{
		if (current_block == NULL || size_left == 0){
			current_block = (o_t *) malloc(BLOCKSIZE * sizeof(o_t));
			size_left = BLOCKSIZE;
		}
		tmp = current_block++;
		size_left -= 1;
	}
	return tmp;
}


void return_node(o_t *node){
	node->left = free_list;
	free_list = node;
}

void right_rotation(o_t *n){
	o_t *temp_node;
	int temp_key;

	temp_node = n->right;
	temp_key = n->key;

	n->right = n->left;
	n->key = n->left->key;

	n->left = n->right->left;

	n->right->left = n->right->right;

	n->right->right = temp_node;
	n->right->key = temp_key;
}

void left_rotation(o_t *n){
	o_t *temp_node;
	int temp_key;

	temp_node = n->left;
	temp_key = n->key;

	n->left = n->right;
	n->key  = n->right->key;

	n->right = n->left->right;

	n->left->right = n->left->left;
	
	n->left->left  = temp_node;
	n->left->key   = temp_key;
}

o_t *create_order(){
	//creates an empty linear ordered set
	o_t *temp1, *temp2;
	temp1 = get_node();
	temp2 = get_node();

	temp2->left = NULL;
	temp2->key = 0;

	temp1->left = NULL;
	temp1->previous = temp2;

	return temp1;
}

int main(){
	return 0;
}
