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
	rect_list_t *rect_interval;
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

void return_node(seg_tree_2d_t *node){  
	node->left = free_list;
	free_list = node;
}

seg_tree_2d_t *make_tree(seg_tree_2d_t *list){
	//make a 2d segment tree
	seg_tree_2d_t *temp, *root;

	typedef struct { struct seg_t_2d_t *node1;
					 struct seg_t_2d_t *node2;
					 int number;} st_item;
	st_item current, left, right;
	st_item stack[1000];
	int st_ptr = 0;

	//use to keep track the length of the list
	int length = 0; 

	for (temp = list; temp != NULL; temp = temp->right){
		length++;
	}

	root = get_node();

	//put root on stack
	current.node1 = root; 
	current.node2 = NULL;

	//root expands to length leaves
	current.number = length;
	stack[st_ptr++] = current;

	while (st_ptr > 0){
		//there is still items in stack
		current = stack[--st_ptr];

		if (current.number > 1){
			// create empty tree nodes
			left.node1 = get_node();
			left.node2 = current.node2;
			left.number = current.number/2;

			right.node1 = get_node();
			right.node2 = current.node1;
			right.number = current.number - left.number;

			(current.node1)->left = left.node1;
			(current.node1)->right = right.node1;

			stack[st_ptr++] = right;
			stack[st_ptr++] = left;

		}
		else{
			// reached a leaf, must filled with list item
			//fill leaf from list
			(current.node1)->left = list->left;
			(current.node1)->key = list->key;

			if (current.node2 != NULL){
				//insert comparison key in the interrior node
				(current.node2)->key = list->key;

				//unlink first item from the list, content has been copied to leaf
				//and return node
				temp = list;
				list = list->right;
				return_node(temp);
			}
		}
	}
	return root;
}

void empty_tree(seg_tree_2d_t *stree){
	stree->rect_interval = NULL;
	if (stree->right != NULL){
		empty_tree(stree->left);
		empty_tree(stree->right);
	}
}

void check_tree(seg_tree_2d_t *tree, int depth, int lower, int upper){
	if (tree->left == NULL){
		printf("Tree is empty.\n");
		return;
	}

	if (tree->key < lower || tree->key >=upper){
		printf("Wrong key order.\n");
		return;
	}

	if (tree->right == NULL){
		if ( *((int *)tree->left) == 42){
			printf("%d(%d) ", tree->key, depth);
		}
		else{
			printf("Wrong Object.\n");
		}
	}
	else{
		check_tree(tree->left, depth+1, lower, tree->key);
		check_tree(tree->right, depth+1, tree->key, upper);
	}
}

rect_list_t *get_list_node(){
	return (rect_list_t *)get_node();
}

rect_list_t *find_intervals(seg_tree_2d_t *tree, int query_key){
	rect_list_t *result_list, *new_result, *current_list;
	seg_tree_2d_t *current_node;

	if(tree->left == NULL){
		//empty tree
		return NULL;
	}
	else{
		current_node = tree;
		result_list = NULL;
		while(current_node->right != NULL){
			if (query_key < current_node->key){
				current_node = current_node->left;
			}
			else{
				current_node = current_node->right;
			}

			current_list = current_node->rect_interval;

			while (current_list != NULL){
				//copy entry from node list to result list
				new_result = get_list_node();
				new_result->next = result_list;
				
				new_result->x_min = current_list->x_min;
				new_result->x_max = current_list->x_max;

				new_result->y_min = current_list->y_min;
				new_result->y_max = current_list->y_max;

				result_list = new_result;

				current_list = current_list->next;
			}
		}
	}

	return result_list;
}


void attach_intv_node(seg_tree_2d_t *tree, int xmin, int xmax, int ymin, int ymax){
	rect_list_t *new_node;

	new_node = get_list_node();

	new_node->next = tree->rect_interval;

	new_node->x_min = xmin;
	new_node->x_max = xmax;
	new_node->y_min = ymin;
	new_node->y_max = ymax;

	tree->rect_interval = new_node;

}


void insert_interval(seg_tree_2d_t *tree, int xmin, int xmax, int ymin, int ymax){
	
}



int main(){
	return 0;
}
