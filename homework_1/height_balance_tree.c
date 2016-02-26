/*
	Compiles and run with command lines
			gcc -o hbl height_balance_tree.c
			./hbl
*/
#include<stdio.h>
#include<stdlib.h>

#define BLOCKSIZE 256

//structure of a node
typedef struct{
	int key;
	struct tree_node *left;
	struct tree_node *right;
	int height;
}tree_node;

tree_node *current_block = NULL;
tree_node *free_list = NULL;
int size_left;

//returns a node
tree_node *get_node(){
	tree_node *tmp;
	if (free_list != NULL){

	}
	else{

	}
	return tmp;
}


int main(){
	return 0;
}