/*
	Homework 2
	
	Compiles and run with command lines
			gcc -o hw2 hw2.c
			./hw2
*/
#include <stdio.h>
#include <stdlib.h>

#define BLOCKSIZE 256

struct rect_list_t{
	struct rect_list_t *next;
	int x_min; int x_max;
	int y_min; int y_max;
};

struct seg_tree_2d_t {
	int key;
	struct seg_tree_2d_t *left;
	struct seg_tree_2d_t *right;
	struct seg_tree_2d_t *tree;
	struct rect_list_t *rect_interval;
};

struct seg_tree_2d_t *current_block = NULL;
struct seg_tree_2d_t *free_list = NULL;
int size_left;

int main(){
	return 0;
}
