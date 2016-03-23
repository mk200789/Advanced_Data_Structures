/*
	Compiles and run with command lines
			gcc -o hw1 hw1.c
			./hw1
*/
#include<stdio.h>
#include<stdlib.h>

#define BLOCKSIZE 256
#define STACK_MAX 100000

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

tree_node *create_text(void){
	tree_node *temp_node;
	temp_node = get_node();
	temp_node->left = NULL;
	return temp_node;
}

void append_line(tree_node *tree, char *new_line){
	//appends `new_line` as a new last line
	tree_node *temp_node;
	int finished;

	if(tree->left == NULL){
		//empty tree
		tree->left = (tree_node *) new_line;
		tree->key += 1;
		tree->right = NULL;
		tree->height = 0;
	}
	else{
		temp_node = tree;
		tree_node *stack[100];
		int stack_ptr = 0;

		while (temp_node->right != NULL){
			stack[stack_ptr++] = temp_node;
			temp_node->key += 1;
			temp_node =  temp_node->right;
		}

		tree_node *left_child, *right_child;
		
		left_child = get_node();
		right_child = get_node();

		left_child->left = temp_node->left;
		left_child->key = temp_node->key;

		right_child = (tree_node *)new_line;
		right_child->key += 1;

		temp_node->left = left_child;
		temp_node->right = right_child;
		temp_node->key += 1;
		temp_node->height = 1;

		//rebalance tree
		finished = 0;

		while (stack_ptr > 0 && !finished){
			int temp_height, old_height;
			temp_node = stack[--stack_ptr];
			old_height = temp_node->height;

			if(temp_node->left->height - temp_node->right->height == 2){

				if (temp_node->left->left->height - temp_node->right->height == 1){
					right_rotation(temp_node);
					temp_node->right->height = temp_node->right->left->height + 1;
					temp_node->height = temp_node->right->height + 1;
				}
				else{
					left_rotation(temp_node->left);
					right_rotation(temp_node);
					temp_height = temp_node->left->left->height;
					temp_node->left->height = temp_height + 1;
					temp_node->right->height = temp_height + 1;
					temp_node->height = temp_height + 2;
				}

			}
			else if (temp_node->left->height - temp_node->right->height == -2){

				if (temp_node->right->right->height - temp_node->left->height == 1){
					left_rotation(temp_node);
					temp_node->left->height = temp_node->left->right->height + 1;
					temp_node->height = temp_node->left->height + 1;
				}
				else{
					right_rotation(temp_node->right);
					left_rotation(temp_node);
					temp_height = temp_node->right->right->height;
					temp_node->left->height = temp_height + 1;
					temp_node->right->height = temp_height + 1;
					temp_node->height = temp_height + 2;
				}

			}
			else{
				//if no rotation needed, update height
				if (temp_node->left->height > temp_node->right->height){
					temp_node->height = temp_node->left->height + 1;
				}
				else{
					temp_node->height = temp_node->right->height + 1;
				}

			}

			if (temp_node->height == old_height){
				finished = 1;
			}

		}
	}
}


int insert_line(tree_node *tree, int new_key, char *new_line){
	//Inserts the line before the line of the number `new_key`, if such
	//a line exists, to `new_line` , renumbering all lines after that line.
	//If no such line exists, it `appends(new_line)` as last line.

	tree_node *temp_node;
	int finished;

	if(tree->left == NULL){
		//empty tree. no children
		tree->left = (tree_node *) new_line;
		//tree->key = new_key;
		tree->key +=1;
		tree->height = 0;
		tree->right = NULL;
	}

	if(tree->key == 1){
		//the root has one child
		tree_node *left_child, *right_child;
		
		left_child = get_node();
		right_child = get_node();

		left_child->left = tree->left;
		left_child->key = tree->key;

		right_child->left = (tree_node *) new_line;
		right_child->key += 1;

		tree->left = left_child;
		tree->right = right_child;
		tree->key += 1;
	}

	if (tree->key < new_key){
		//go to the left branch
		append_line(tree, new_line);
	}
	else{
		//go to the right branch
		tree_node *stack[100];
		int stack_ptr = 0;
		temp_node = tree;

		while(temp_node->right != NULL){
			stack[stack_ptr++] = temp_node;
			temp_node->key += 1;
			if (new_key <= temp_node->left->key){
				temp_node = temp_node->left;
			}
			else{
				temp_node = temp_node->right;
				new_key -= temp_node->left->key;
			}
		}

		tree_node *left_child, *right_child;
		left_child = get_node();
		right_child = get_node();

		left_child->left = temp_node->left;
		left_child->key = temp_node->key;

		right_child->left = (tree_node *) new_line;
		right_child->key += 1;

		temp_node->left = left_child;
		temp_node->right = right_child;
		temp_node->key += 1;
		temp_node->height = 1;

		//rebalance tree
		finished = 0;
		while(stack_ptr > 0  && !finished){
			int temp_height , old_height;
			temp_node = stack[--stack_ptr];
			old_height = temp_node->height;

			if(temp_node->left->height - temp_node->right->height == 2){

				if (temp_node->left->left->height - temp_node->right->height == 1){
					right_rotation(temp_node);
					temp_node->right->height = temp_node->right->left->height + 1;
					temp_node->height = temp_node->right->height + 1;
				}
				else{
					left_rotation(temp_node->left);
					right_rotation(temp_node);
					temp_height = temp_node->left->left->height;
					temp_node->left->height = temp_height + 1;
					temp_node->right->height = temp_height + 1;
					temp_node->height = temp_height + 2;
				}

			}
			else if (temp_node->left->height - temp_node->right->height == -2){

				if (temp_node->right->right->height - temp_node->left->height == 1){
					left_rotation(temp_node);
					temp_node->left->height = temp_node->left->right->height + 1;
					temp_node->height = temp_node->left->height + 1;
				}
				else{
					right_rotation(temp_node->right);
					left_rotation(temp_node);
					temp_height = temp_node->right->right->height;
					temp_node->left->height = temp_height + 1;
					temp_node->right->height = temp_height + 1;
					temp_node->height = temp_height + 2;
				}

			}
			else{
				//if no rotation needed, update height
				if (temp_node->left->height > temp_node->right->height){
					temp_node->height = temp_node->left->height + 1;
				}
				else{
					temp_node->height = temp_node->right->height + 1;
				}

			}

			if (temp_node->height == old_height){
				finished = 1;
			}
		}

	}
	return 0;
}

char * delete(tree_node *tree, int delete_key){
	//Deletes the line of the number `delete_key`, renumbering all lines
	//after that line, and returns a pointer to the deleted line.

	tree_node *temp_node, *upper_node, *other_node;
	char *deleted_object;
	int finished;

	if (tree->left == NULL){
		//empty tree
		return NULL;
	}
	else{
		//both side of the tree isn't empty.
		tree_node *stack[100];
		int stack_ptr;
		temp_node = tree;

		while (temp_node->right != NULL){
			stack[stack_ptr++] = temp_node;
			upper_node = temp_node;
			if (delete_key <= temp_node->left->key){
				temp_node = upper_node->left;
				other_node = upper_node->right;
			}
			else{
				temp_node = upper_node->right;
				other_node = upper_node->left;
				delete_key -= temp_node->left->key;
			}
		}

		temp_node = stack[--stack_ptr];

		if (delete_key == 1){
			//perform deletion
			upper_node->key = other_node->key;
			upper_node->left = other_node->left;
			upper_node->right = other_node->right;
			upper_node->height = other_node->height;
			deleted_object = (char *) temp_node->left;


			//rebalancing tree
			finished = 0;
			stack_ptr -= 1;

			while(stack_ptr>0 && !finished){
				int temp_height, old_height;
				temp_node = stack[--stack_ptr];
				old_height = temp_node->height;

				if(temp_node->left->height - temp_node->right->height == 2){

					if (temp_node->left->left->height - temp_node->right->height == 1){
						right_rotation(temp_node);
						temp_node->right->height = temp_node->right->left->height + 1;
						temp_node->height = temp_node->right->height + 1;
					}
					else{
						left_rotation(temp_node->left);
						right_rotation(temp_node);
						temp_height = temp_node->left->left->height;
						temp_node->left->height = temp_height + 1;
						temp_node->right->height = temp_height + 1;
						temp_node->height = temp_height + 2;

					}

				}
				else if (temp_node->left->height - temp_node->right->height == -2){

					if (temp_node->right->right->height - temp_node->left->height == 1){
						left_rotation(temp_node);
						temp_node->left->height = temp_node->left->right->height + 1;
						temp_node->height = temp_node->left->height + 1;
					}
					else{
						right_rotation(temp_node->right);
						left_rotation(temp_node);
						temp_height = temp_node->right->right->height;
						temp_node->left->height = temp_height + 1;
						temp_node->right->height = temp_height + 1;
						temp_node->height = temp_height + 2;
					}

				}
				else{
					if (temp_node->left->height > temp_node->right->height){
						temp_node->height = temp_node->left->height + 1;
					}
					else{
						temp_node->height = temp_node->right->height + 1;
					}

				}

				if (old_height == temp_height){
					finished = 1;
				}
			}
			return deleted_object;
		}
		else{
			return NULL;
		}
	}
}


char *set_line(tree_node *tree, int new_key, char *new_line){
	//sets the line of number `new_key`, if such a line exists, to `new_line`, and returns a 
	//pointer to the previous line of that number. If no line of that number exists, it does
	//not change the structure and returns NULL.
	tree_node *temp_node;
	char *new_object;

	if(tree->left == NULL){
		return NULL;
	}
	else if (tree->right == NULL){
		return (char *)tree->left;
	}
	else{
		temp_node = tree;
		while(temp_node->right != NULL){
			if (new_key <= temp_node->left->key){
				temp_node = temp_node->left;
			}
			else{
				temp_node = temp_node->right;
				new_key -= temp_node->left->key;
			}
		}
	}

	if (new_key == 1){
		new_object = (char *)temp_node->left;
		temp_node->left = (tree_node *) new_line;
		return new_object;
	}
	else{
		return NULL;
	}
}

int length_text(tree_node *tree){
	//returns the number of lines of the current text/tree.
	if (tree->left == NULL){
		return 0;
	}
	else{
		return tree->key;
	}
}


int main(){
	return 0;
}