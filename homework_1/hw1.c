/*
	Compiles and run with command lines
			gcc -o hw1 hw1.c
			./hw1
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



void right_rotation(tree_node *n){
	//note to self: modified for the new tree structure
	tree_node *temp_node;
	int temp_key;

	temp_node = n->right;
	temp_key = n->left->key;

	n->right = n->left;

	n->left = n->right->left;

	n->right->left = n->right->right;

	n->right->right = temp_node;
	n->right->key = temp_key;
	/*
		or 
		n->right->key = n->right->left->key + n->right->right->key;
	*/
}

void left_rotation(tree_node *n){
	//note to self: modified for the new tree structure
	tree_node *temp_node;
	int temp_key;

	temp_node = n->left;
	temp_key = n->right->key;

	n->left = n->right;

	n->right = n->left->right;

	n->left->right = n->left->left;
	
	n->left->left  = temp_node;
	n->left->key   = temp_key;
	/*
		or 
		n->left->key = n->left->left->key + n->left->right->key;
	*/
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