/*
	Compiles and run with command lines
			gcc -o hbl height_balance_tree.c
			./hbl
*/
#include<stdio.h>
#include<stdlib.h>

typedef struct{
	int key;
	struct tree_node *left;
	struct tree_node *right;
	int height;
}tree_node;

int main(){
	return 0;
}