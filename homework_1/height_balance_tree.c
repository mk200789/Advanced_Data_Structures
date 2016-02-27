/*
	Compiles and run with command lines
			gcc -o hbl height_balance_tree.c
			./hbl
*/
#include<stdio.h>
#include<stdlib.h>

#define BLOCKSIZE 256

//structure of a node
typedef struct tr_n_t{
	int key;
	struct tr_n_t *left;
	struct tr_n_t *right;
	int height;
}tree_node;

tree_node *current_block = NULL;
tree_node *free_list = NULL;
int size_left;

//returns a node
tree_node *get_node(){
	tree_node *tmp;
	if (free_list != NULL){
		tmp = free_list;
		free_list = free_list -> left;
	}
	else{
		if (current_block == NULL || size_left == 0){
			current_block = (tree_node *) malloc(BLOCKSIZE * sizeof(tree_node));
			size_left = BLOCKSIZE;
		}
		tmp = current_block++;
		size_left -= 1;
	}
	return tmp;
}

void return_node(tree_node *node){
	node->left = free_list;
	free_list = node;
}

void right_rotation(tree_node *n){
	tree_node *temp_node;
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

void left_rotation(tree_node *n){
	tree_node *temp_node;
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

tree_node *create_tree(void){
	tree_node *temp_node;
	temp_node = get_node();
	temp_node->left = NULL;
	return temp_node;
}


int main(){
	return 0;
}