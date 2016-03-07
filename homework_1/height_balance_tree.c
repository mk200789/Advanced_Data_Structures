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

void right_rotation(tree_node *n){
	tree_node *temp_node;
	int temp_key;

	temp_node = n->right;
	temp_key = n->key;

	n->right = n->left;
	n->key = n->left->key;

	n->left = n->right->left;

	n->right->left = n->right->right;

	n->right->right = temp_node;
	n->right->key = temp_key;
}

void left_rotation(tree_node *n){
	tree_node *temp_node;
	int temp_key;

	temp_node = n->left;
	temp_key = n->key;

	n->left = n->right;
	n->key  = n->right->key;

	n->right = n->left->right;

	n->left->right = n->left->left;
	
	n->left->left  = temp_node;
	n->left->key   = temp_key;
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
		//empty tree
		tree->left = (tree_node *) new_object;
		tree->key = new_key;
		tree->height = 0;
		tree->right = NULL;
	}
	else{
		tree_node *path_stack[100];
		int path_st_ptr = 0;
		while(temp_node->right != NULL){
			path_stack[path_st_ptr++] = temp_node;
			if (new_key < temp_node->key){
				temp_node = temp_node->left;
			}
			else{
				temp_node = temp_node->right;
			}
		}

		//leaf is found, check see if it's distinct
		if (temp_node->key == new_key){
			//exists
			return -1;
		}
		else{
			//key is distinct, perform INSERT
			tree_node *new_leaf, *old_leaf;
			
			old_leaf = get_node();
			old_leaf->left = temp_node->left;
			old_leaf->key = temp_node->key;
			old_leaf->right = NULL;
			old_leaf->height = 0;

			new_leaf = get_node();
			new_leaf->left = (tree_node *) new_object;
			new_leaf->key = new_key;
			new_leaf->right = NULL;
			new_leaf->height = 0;

			if (temp_node->key < new_key){
				temp_node->left = old_leaf;
				temp_node->right = new_leaf;
				temp_node->key = new_key;
			}
			else{
				temp_node->left = new_leaf;
				temp_node->right = old_leaf;
			}
			temp_node->height= 0;
		}

		//REBALANCE tree
		finished = 0;
		while(path_st_ptr > 0  && !finished){
			int temp_height , old_height;
			temp_node = path_stack[path_st_ptr--];
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
					temp_node->right->height = temp_height + 1;
					temp_node->left->height = temp_height + 1;
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

int * delete(tree_node *tree, int delete_key){
	tree_node *temp_node, *upper_node, *other_node;
	int *deleted_object;
	int finished;
	if (tree->left == NULL){
		//empty tree
		return NULL;
	}
	else if (tree->right == NULL){
		//object is on the left of the tree
		if (delete_key == tree->key){
			deleted_object = (int *) tree->left;
			tree->left = NULL;
			return deleted_object;
		}
		else{
			return NULL;
		}
	}
	else{
		//both side of the tree isn't empty.
	}
	return deleted_object;
}


int main(){
	return 0;
}