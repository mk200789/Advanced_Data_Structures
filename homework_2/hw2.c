/*
	Homework 2
	
	Compiles and run with command lines
			gcc -o hw2 hw2.c
			./hw2
*/
#include <stdio.h>
#include <stdlib.h>

#define BLOCKSIZE 256

typedef struct rect_l_t{
	struct rect_l_t *next;
	int x_min; int x_max;
	int y_min; int y_max;
}rect_list_t;

typedef struct seg_t_2d_t {
	int key;
	struct seg_t_2d_t *left;
	struct seg_t_2d_t *right;
	struct seg_t_2d_t *tree;
	struct rect_list_t *rect_interval;
}seg_tree_2d_t;

seg_tree_2d_t *current_block = NULL;
seg_tree_2d_t *free_list = NULL;
int size_left;


seg_tree_2d_t *get_node(){
	seg_tree_2d_t *tmp;
	
	if (free_list != NULL){
		tmp = free_list;
		free_list = free_list -> left;
	}
	else{
		if (current_block == NULL || size_left == 0){
			current_block = (seg_tree_2d_t *) malloc(BLOCKSIZE * sizeof(seg_tree_2d_t));
			size_left = BLOCKSIZE;
		}
		tmp = current_block++;
		size_left -= 1;
	}
	
	return tmp;
	
}

void return_node(seg_tree_2d_t *node)
{  node->left = free_list;
   free_list = node;
}

seg_tree_2d_t *make_tree(seg_tree_2d_t *list){
	//make a 2d segment tree
	seg_tree_2d_t *temp_node, *root;
	typedef struct { struct seg_tree_2d_t *node1;
					 struct seg_tree_2d_t *node2;
					 int number;} st_item;
	st_item current, left, right;
	st_item stack[1000];
	int st_ptr = 0;
	return root;
}



int main(){
	return 0;
}
