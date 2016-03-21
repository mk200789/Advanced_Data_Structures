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


int insert(tree_node *tree, int new_key, int *new_object){
	tree_node *temp_node;
	int finished;

	if(tree->left == NULL){
		//empty tree. no children
		tree->left = (tree_node *) new_object;
		//tree->key = new_key;
		tree->key +=1;
		tree->height = 0;
		tree->right = NULL;
	}

	if(tree->key == 1){
		//there's one child in total
	}
	return 0;
}


int main(){
	return 0;
}