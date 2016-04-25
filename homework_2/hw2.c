/*
	Homework 2
	
	Compiles and run with command lines
			gcc -o hw2 hw2.c
			./hw2
*/
#include<stdio.h>
#include<stdlib.h>

struct rect_list_t{
	struct rect_list_t *next;
	int x_min;
	int x_max;
	int y_min;
	int y_max;
};

struct seg_tree_2d_t {
	int key;
	struct seg_tree_2d_t *left;
	struct seg_tree_2d_t *right;
	struct seg_tree_2d_t *tree;
	struct rect_list_t *rect_interval;
};

int main(){
	return 0;
}
