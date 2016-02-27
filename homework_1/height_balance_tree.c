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


int main(){
	return 0;
}