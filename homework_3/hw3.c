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
	int height;
}o_t;

o_t *current_block = NULL;
o_t *free_list = NULL;
int size_left;


int main(){
	return 0;
}
