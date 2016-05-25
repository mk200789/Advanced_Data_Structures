/*
	Homework 3
	
	Compiles and run with command lines
			gcc -o hw3 hw3.c
			./hw3

*/

#include <stdio.h>
#include <stdlib.h>

#define BLOCKSIZE 256
#define STACKSIZE 100

//structure of o_t
typedef struct tr_n_t{
	int key;
	struct tr_n_t *left;
	struct tr_n_t *right;
	struct tr_n_t *previous;
	int height;
}o_t;

o_t *current_block = NULL;
o_t *free_list = NULL;
int size_left;

//returns a node
o_t *get_node(){
	o_t *tmp;
	if (free_list != NULL){
		tmp = free_list;
		free_list = free_list -> left;
	}
	else{
		if (current_block == NULL || size_left == 0){
			current_block = (o_t *) malloc(BLOCKSIZE * sizeof(o_t));
			size_left = BLOCKSIZE;
		}
		tmp = current_block++;
		size_left -= 1;
	}
	return tmp;
}

void return_node(o_t *node){
	node->left = free_list;
	free_list = node;
}

void right_rotation(o_t *n){
	o_t *temp_node;
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

void left_rotation(o_t *n){
	o_t *temp_node;
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

o_t *create_order(){
	//creates an empty linear ordered set
	o_t *temp1, *temp2;
	temp1 = get_node();
	temp2 = get_node();

	temp1->left = NULL;
	temp2->left = NULL;
	
	temp1->previous = temp2;

	return temp1;
}

int find(o_t *tree, int key){
	o_t *temp_node;
	//find key in tree
	if (tree->left == NULL){
		//empty tree
		return 0;
	}
	else{
		temp_node = tree;
		while(temp_node->right != NULL){
			if (key < temp_node->key){
				temp_node = temp_node->left;
			}
			else{
				temp_node = temp_node->right;
			}
		}

		if (temp_node->key == key){
			return 1;
		}
		else{
			return 0;
		}
	}
}


void left_rotation1(o_t *n){
	o_t *temp_node;
	o_t *temp_previous;

	temp_node = n->left;
	n->left = n->right;


	n->right = n->left->right;
	n->right->previous = n;

	n->left->right = n->left->left;
	n->left->left  = temp_node;

	n->left->left->previous = n->left;
}

void right_rotation1(o_t *n){
	o_t *temp_node;
	o_t *temp_previous;

	temp_node = n->right;
	n->right = n->left;

	n->left = n->right->left;
	n->left->previous = n;

	n->right->left = n->right->right;
	n->right->right = temp_node;

	n->right->right->previous = n->right;
}


void insert_node(o_t *node, int a){
	o_t *old_leaf, *new_leaf;
	int *insert_object = (int *) malloc (sizeof(int));
	*insert_object = a;

	old_leaf = get_node();
	old_leaf->left = node->left;
	old_leaf->right = NULL;
	old_leaf->key = node->key;
	old_leaf->height = 0;

	new_leaf = get_node();
	new_leaf->left = (o_t *) insert_object;
	new_leaf->right = NULL;
	new_leaf->key = a;
	new_leaf->height = 0;

	if (node->key < a){
		node->left = old_leaf;
		node->right = new_leaf;
		node->key = a;
	}
	else{
		node->left = new_leaf;
		node->right = old_leaf;
	}
	node->height = 1;
}

void insert_node1(o_t *node, int a, int where){
	//`where` is 1 to insert node before a, else 0
	o_t *old_leaf, *new_leaf;
	int *insert_object = (int *) malloc (sizeof(int));
	*insert_object = a;

	old_leaf = get_node();
	old_leaf->left = node->left;
	old_leaf->right = NULL;
	old_leaf->key = 0;
	old_leaf->height = 0;

	new_leaf = get_node();
	new_leaf->left = (o_t *) insert_object;
	new_leaf->right = NULL;
	new_leaf->key = 0;
	new_leaf->height = 0;

	if (where == 1){
		node->left = new_leaf;
		node->right = old_leaf;
	}
	else{
		node->left = old_leaf;
		node->right = new_leaf;
	}
	old_leaf->previous = node;
	new_leaf->previous = node;
	node->height = 1;
}


void insert_bottom (o_t *ord, int a){
	//inserts the key a as smallest element in the ordered set
	o_t *temp, *root1, *root2;
	temp = ord;
	root1 = ord;
	root2 = ord;

	o_t *object = (o_t *) (int *)malloc(sizeof(int));

	if (find(ord, a) == 1){
		//a exist in tree, return
		return;
	}
	else if (root1->left == NULL){
		//tree empty

		//first tree is empty tree: insert into the second tree first, then the first tree.
		root1->previous->left = (o_t *)object;
		root1->left = (o_t *)object;
		root1->right = NULL;
		root1->key = a;
		return;
	}
	else{

		//tree not empty.

		//going backwards with the second tree
		while(root2->right != NULL){
			root2 = root2->left;
		}
		root2 = root2->previous;

		while(root2->previous != NULL){
			root2 = root2->previous;
		}

		while(root2->right !=NULL){
			root2 = root2->left;
		}

		int insert_value = root2->left->key;

		//first: insert a in second tree (root2)
		insert_node1(root2, a, 1);

		o_t *stack1[STACKSIZE];
		int stack_ptr1 = 0;

		//second: find insert position in the first tree
		while(temp->right != NULL){
			stack1[stack_ptr1++] = temp;
			if (a < temp->key){
				temp = temp->left;
			}
			else{
				temp = temp->right;
			}
		}

		//third: insert first tree
		insert_node(temp, a);


		//connect the two trees
		if (temp->left->key == a){
			temp->left->previous = root2->left;
			temp->right->previous = temp->previous;
		}
		else{
			temp->left->previous = temp->previous;
			temp->right->previous = root2->left;
		}

		temp->previous = NULL;

		//update previous pointer
		temp = root1;
		while(temp->right != NULL){
			if (insert_value < temp->key){
				temp = temp->left;
			}
			else{
				temp = temp->right;
			}
		}
		temp->previous = root2->right;

		o_t *temp1 = root2;


		//rebalance first tree
		int finished = 0;
		while (stack_ptr1 > 0 && !finished){
			int temp_height , old_height;
			o_t *tmp = stack1[--stack_ptr1];
			old_height = tmp->height;

			if(tmp->left->height - tmp->right->height == 2){

				if (tmp->left->left->height - tmp->right->height == 1){
					right_rotation(tmp);
					tmp->right->height = tmp->right->left->height + 1;
					tmp->height = tmp->right->height + 1;
				}
				else{
					left_rotation(tmp->left);
					right_rotation(tmp);
					temp_height = tmp->left->left->height;
					tmp->left->height = temp_height + 1;
					tmp->right->height = temp_height + 1;
					tmp->height = temp_height + 2;
				}

			}
			else if (tmp->left->height - tmp->right->height == -2){

				if (tmp->right->right->height - tmp->left->height == 1){
					left_rotation(tmp);
					tmp->left->height = tmp->left->right->height + 1;
					tmp->height = tmp->left->height + 1;
				}
				else{
					right_rotation(tmp->right);
					left_rotation(tmp);
					temp_height = tmp->right->right->height;
					tmp->left->height = temp_height + 1;
					tmp->right->height = temp_height + 1;
					tmp->height = temp_height + 2;
				}

			}
			else{
				//if no rotation needed, update height
				if (tmp->left->height > tmp->right->height){
					tmp->height = tmp->left->height + 1;
				}
				else{
					tmp->height = tmp->right->height + 1;
				}

			}

			if (tmp->height == old_height){
				finished = 1;
			}
		}


		root2 = temp1;

		//rebalance second tree
		while(root2 != NULL){
			int temp_height , old_height;

			if(root2->left->height - root2->right->height == 2){

				if (root2->left->left->height - root2->right->height == 1){
					right_rotation1(root2);
					root2->right->height = root2->right->left->height + 1;
					root2->height = root2->right->height + 1;
				}
				else{
					left_rotation1(root2->left);
					right_rotation1(root2);
					temp_height = root2->left->left->height;
					root2->left->height = temp_height + 1;
					root2->right->height = temp_height + 1;
					root2->height = temp_height + 2;
				}

			}
			else if (root2->left->height - root2->right->height == -2){

				if (root2->right->right->height - root2->left->height == 1){
					left_rotation1(root2);
					root2->left->height = root2->left->right->height + 1;
					root2->height = root2->left->height + 1;
				}
				else{
					right_rotation1(root2->right);
					left_rotation1(root2);
					temp_height = root2->right->right->height;
					root2->left->height = temp_height + 1;
					root2->right->height = temp_height + 1;
					root2->height = temp_height + 2;
				}

			}
			else{
				//if no rotation needed, update height
				if (root2->left->height > root2->right->height){
					root2->height = root2->left->height + 1;
				}
				else{
					root2->height = root2->right->height + 1;
				}

			}

			root2 = root2->previous;
		}

		return;
	}
}


void insert_after(o_t *ord, int a, int b){
	//inserts key a immediately after key b in the ordered set
	o_t *temp, *insert_position, *position_in_tree, *root2;

	if (ord->left == NULL){
		//empty tree
		return;
	}
	if (find(ord, b) == 0){
		//b exists, return
		return;
	}

	o_t *stack1[STACKSIZE];
	int stack_ptr1 = 0;

	temp = ord;
	
	//find b in first tree
	while(temp->right != NULL){
		if (b < temp->key){
			temp = temp->left;
		}
		else{
			temp = temp->right;
		}
	}

	position_in_tree = temp;
	temp = temp->previous;

	//insert a in second tree
	insert_node1(temp, a, 0);

	position_in_tree->previous = temp->left;

	//use later for second tree rebalancing
	o_t *temp1 = temp; 

	insert_position = ord;

	//setting first stack for first tree
	while(insert_position->right != NULL){
		stack1[stack_ptr1++] = insert_position;
		if (a < insert_position->key){
			insert_position = insert_position->left;
		}
		else{
			insert_position = insert_position->right;
		}
	}

	//inserting a in first tree
	insert_node(insert_position, a);

	//connect the two trees
	if(insert_position->left->key == a){
		insert_position->left->previous = temp->right;
		insert_position->right->previous = insert_position->previous;
	}
	else{
		insert_position->left->previous = insert_position->previous;
		insert_position->right->previous = temp->right;
	}
	insert_position->previous = NULL;


	//rebalance first tree
	int finished = 0;
	while (stack_ptr1 > 0 && !finished){
		int temp_height , old_height;
		temp = stack1[--stack_ptr1];
		old_height = temp->height;

		if(temp->left->height - temp->right->height == 2){

			if (temp->left->left->height - temp->right->height == 1){
				right_rotation(temp);
				temp->right->height = temp->right->left->height + 1;
				temp->height = temp->right->height + 1;
			}
			else{
				left_rotation(temp->left);
				right_rotation(temp);
				temp_height = temp->left->left->height;
				temp->left->height = temp_height + 1;
				temp->right->height = temp_height + 1;
				temp->height = temp_height + 2;
			}

		}
		else if (temp->left->height - temp->right->height == -2){

			if (temp->right->right->height - temp->left->height == 1){
				left_rotation(temp);
				temp->left->height = temp->left->right->height + 1;
				temp->height = temp->left->height + 1;
			}
			else{
				right_rotation(temp->right);
				left_rotation(temp);
				temp_height = temp->right->right->height;
				temp->left->height = temp_height + 1;
				temp->right->height = temp_height + 1;
				temp->height = temp_height + 2;
			}

		}
		else{
			//if no rotation needed, update height
			if (temp->left->height > temp->right->height){
				temp->height = temp->left->height + 1;
			}
			else{
				temp->height = temp->right->height + 1;
			}

		}

		if (temp->height == old_height){
			finished = 1;
		}
	}

	root2 = temp1;
	//rebalance second tree
	while(root2 != NULL){
		int temp_height , old_height;

		if(root2->left->height - root2->right->height == 2){

			if (root2->left->left->height - root2->right->height == 1){
				right_rotation1(root2);
				root2->right->height = root2->right->left->height + 1;
				root2->height = root2->right->height + 1;
			}
			else{
				left_rotation1(root2->left);
				right_rotation1(root2);
				temp_height = root2->left->left->height;
				root2->left->height = temp_height + 1;
				root2->right->height = temp_height + 1;
				root2->height = temp_height + 2;
			}

		}
		else if (root2->left->height - root2->right->height == -2){

			if (root2->right->right->height - root2->left->height == 1){
				left_rotation1(root2);
				root2->left->height = root2->left->right->height + 1;
				root2->height = root2->left->height + 1;
			}
			else{
				right_rotation1(root2->right);
				left_rotation1(root2);
				temp_height = root2->right->right->height;
				root2->left->height = temp_height + 1;
				root2->right->height = temp_height + 1;
				root2->height = temp_height + 2;
			}

		}
		else{
			//if no rotation needed, update height
			if (root2->left->height > root2->right->height){
				root2->height = root2->left->height + 1;
			}
			else{
				root2->height = root2->right->height + 1;
			}

		}

		root2 = root2->previous;
	}
}


void insert_before(o_t *ord, int a, int b){
	//inserts the key a immediately before key b in the ordered set
	o_t *temp, *insert_position, *position_in_tree, *root2;

	if (ord->left == NULL){
		//empty tree
		return;
	}
	if (find(ord, b) == 0){
		//b exists, return
		return;
	}

	o_t *stack1[STACKSIZE], *stack2[STACKSIZE];
	int stack_ptr1 = 0, stack_ptr2 =0;

	temp = ord;
	
	//find b in first tree
	while(temp->right != NULL){
		if (b < temp->key){
			temp = temp->left;
		}
		else{
			temp = temp->right;
		}
	}

	position_in_tree = temp;
	temp = temp->previous;

	//insert a in second tree
	insert_node1(temp, a, 1);

	position_in_tree->previous = temp->right;

	//use later for second tree rebalancing
	o_t *temp1 = temp; 

	insert_position = ord;

	//setting first stack for first tree
	while(insert_position->right != NULL){
		stack1[stack_ptr1++] = insert_position;
		if (a < insert_position->key){
			insert_position = insert_position->left;
		}
		else{
			insert_position = insert_position->right;
		}
	}

	//inserting a in first tree
	insert_node(insert_position, a);

	//connect the two trees
	if(insert_position->left->key == a){
		insert_position->left->previous = temp->left;
		insert_position->right->previous = insert_position->previous;
	}
	else{
		insert_position->left->previous = insert_position->previous;
		insert_position->right->previous = temp->left;
	}
	insert_position->previous = NULL;


	//rebalance first tree
	int finished = 0;
	while (stack_ptr1 > 0 && !finished){
		int temp_height , old_height;
		temp = stack1[--stack_ptr1];
		old_height = temp->height;

		if(temp->left->height - temp->right->height == 2){

			if (temp->left->left->height - temp->right->height == 1){
				right_rotation(temp);
				temp->right->height = temp->right->left->height + 1;
				temp->height = temp->right->height + 1;
			}
			else{
				left_rotation(temp->left);
				right_rotation(temp);
				temp_height = temp->left->left->height;
				temp->left->height = temp_height + 1;
				temp->right->height = temp_height + 1;
				temp->height = temp_height + 2;
			}

		}
		else if (temp->left->height - temp->right->height == -2){

			if (temp->right->right->height - temp->left->height == 1){
				left_rotation(temp);
				temp->left->height = temp->left->right->height + 1;
				temp->height = temp->left->height + 1;
			}
			else{
				right_rotation(temp->right);
				left_rotation(temp);
				temp_height = temp->right->right->height;
				temp->left->height = temp_height + 1;
				temp->right->height = temp_height + 1;
				temp->height = temp_height + 2;
			}

		}
		else{
			//if no rotation needed, update height
			if (temp->left->height > temp->right->height){
				temp->height = temp->left->height + 1;
			}
			else{
				temp->height = temp->right->height + 1;
			}

		}

		if (temp->height == old_height){
			finished = 1;
		}
	}

	root2 = temp1;
	//rebalance second tree
	while(root2 != NULL){
		int temp_height , old_height;

		if(root2->left->height - root2->right->height == 2){

			if (root2->left->left->height - root2->right->height == 1){
				right_rotation1(root2);
				root2->right->height = root2->right->left->height + 1;
				root2->height = root2->right->height + 1;
			}
			else{
				left_rotation1(root2->left);
				right_rotation1(root2);
				temp_height = root2->left->left->height;
				root2->left->height = temp_height + 1;
				root2->right->height = temp_height + 1;
				root2->height = temp_height + 2;
			}

		}
		else if (root2->left->height - root2->right->height == -2){

			if (root2->right->right->height - root2->left->height == 1){
				left_rotation1(root2);
				root2->left->height = root2->left->right->height + 1;
				root2->height = root2->left->height + 1;
			}
			else{
				right_rotation1(root2->right);
				left_rotation1(root2);
				temp_height = root2->right->right->height;
				root2->left->height = temp_height + 1;
				root2->right->height = temp_height + 1;
				root2->height = temp_height + 2;
			}

		}
		else{
			//if no rotation needed, update height
			if (root2->left->height > root2->right->height){
				root2->height = root2->left->height + 1;
			}
			else{
				root2->height = root2->right->height + 1;
			}

		}

		root2 = root2->previous;
	}

}


void delete_o (o_t *ord, int a){
	//deletes the key a from the ordered set
	o_t *temp1, *upper_node1, *other_node1, *temp2, *upper_node2, *other_node2;
	int finished;

	if (ord->left == NULL){
		//empty tree
		return;
	}
	else if (ord->right == NULL){
		//object is on the left of the tree
		if (ord->key == a){
			ord->left = NULL;
			ord->previous->left = NULL;
			ord->previous->right = NULL;
			return;
		}
		else{
			return;
		}
	}

	else{
		//both side of the tree isn't empty.
		o_t *stack1[STACKSIZE], *stack2[STACKSIZE];
		int stack_ptr1 = 0, stack_ptr2 = 0;

		temp1 = ord;

		if(find(ord, a) == 0){
			//a exists, return
			return;
		}

		//find a in first tree
		while (temp1->right != NULL){
			stack1[stack_ptr1++] = temp1;
			upper_node1 = temp1;
			if (a < temp1->key){
				temp1 = upper_node1->left;
				other_node1 = upper_node1->right;
			}
			else{
				temp1 = upper_node1->right;
				other_node1 = upper_node1->left;
			}
		}

		if (temp1->key != a){
			return;
		}
		else{


			//perform deletion of a on second tree
			temp2 = temp1->previous;
			upper_node2 = temp2->previous;

			if (temp2 == upper_node2->left){
				other_node2 = upper_node2->right;
			}
			else{
				other_node2 = upper_node2->left;
			}

			int save_value = other_node2->left->key;

			temp1 = upper_node2;
			o_t * temp_stack[STACKSIZE];
			int temp_stack_ptr = 0;
			while (temp1 != NULL){
				temp_stack[temp_stack_ptr++] = temp1;
				temp1 = temp1->previous;
			}
			while (temp_stack_ptr > 0){
				stack2[stack_ptr2++] = temp_stack[--temp_stack_ptr];
			}

			upper_node2->left = other_node2->left;
			upper_node2->right = other_node2->right;
			upper_node2->key = other_node2->key;
			upper_node2->height = other_node2->height;

			temp1 = ord;

			while(temp1->right !=NULL){
				if (save_value <temp1->key){
					temp1 = temp1->left;
				}
				else{
					temp1 = temp1->right;
				}
			}

			temp1->previous = upper_node2;

			//perform deletion of a on first tree and update `previous` pointer
			upper_node1->left = other_node1->left;
			upper_node1->right = other_node1->right;
			upper_node1->key = other_node1->key;
			upper_node1->height = other_node1->height;
			upper_node1->previous = other_node1->previous;
			other_node1->previous = NULL;
		}

		//rebalance first tree
		int finished = 0; stack_ptr1 -=1;
		while (stack_ptr1 > 0 && !finished){
			int temp_height , old_height;
			o_t *temp = stack1[--stack_ptr1];
			old_height = temp->height;

			if(temp->left->height - temp->right->height == 2){

				if (temp->left->left->height - temp->right->height == 1){
					right_rotation(temp);
					temp->right->height = temp->right->left->height + 1;
					temp->height = temp->right->height + 1;
				}
				else{
					left_rotation(temp->left);
					right_rotation(temp);
					temp_height = temp->left->left->height;
					temp->left->height = temp_height + 1;
					temp->right->height = temp_height + 1;
					temp->height = temp_height + 2;
				}

			}
			else if (temp->left->height - temp->right->height == -2){

				if (temp->right->right->height - temp->left->height == 1){
					left_rotation(temp);
					temp->left->height = temp->left->right->height + 1;
					temp->height = temp->left->height + 1;
				}
				else{
					right_rotation(temp->right);
					left_rotation(temp);
					temp_height = temp->right->right->height;
					temp->left->height = temp_height + 1;
					temp->right->height = temp_height + 1;
					temp->height = temp_height + 2;
				}

			}
			else{
				//if no rotation needed, update height
				if (temp->left->height > temp->right->height){
					temp->height = temp->left->height + 1;
				}
				else{
					temp->height = temp->right->height + 1;
				}

			}

			if (temp->height == old_height){
				finished = 1;
			}


		}


		//rebalance second tree
		finished = 0; stack_ptr2 -=1;
		while (stack_ptr2 > 0 && !finished){
			int temp_height , old_height;
			o_t *temp = stack2[--stack_ptr2];
			old_height = temp->height;

			if(temp->left->height - temp->right->height == 2){

				if (temp->left->left->height - temp->right->height == 1){
					right_rotation1(temp);
					temp->right->height = temp->right->left->height + 1;
					temp->height = temp->right->height + 1;
				}
				else{
					left_rotation1(temp->left);
					right_rotation1(temp);
					temp_height = temp->left->left->height;
					temp->left->height = temp_height + 1;
					temp->right->height = temp_height + 1;
					temp->height = temp_height + 2;
				}

			}
			else if (temp->left->height - temp->right->height == -2){

				if (temp->right->right->height - temp->left->height == 1){
					left_rotation1(temp);
					temp->left->height = temp->left->right->height + 1;
					temp->height = temp->left->height + 1;
				}
				else{
					right_rotation1(temp->right);
					left_rotation1(temp);
					temp_height = temp->right->right->height;
					temp->left->height = temp_height + 1;
					temp->right->height = temp_height + 1;
					temp->height = temp_height + 2;
				}

			}
			else{
				//if no rotation needed, update height
				if (temp->left->height > temp->right->height){
					temp->height = temp->left->height + 1;
				}
				else{
					temp->height = temp->right->height + 1;
				}

			}

			if (temp->height == old_height){
				finished = 1;
			}
		}

	}
	

}

void insert_top(o_t *ord, int a){
	//inserts the key a as largest element in the ordered set
	o_t *temp, *root1, *root2;
	temp = ord;
	root1 = ord;
	root2 = ord;

	o_t *object = (o_t *) (int *)malloc(sizeof(int));

	if (find(ord, a) == 1){
		//a exist in tree, return
		return;
	}
	else if (root1->left == NULL){
		//tree empty

		//first tree is empty tree: insert into the second tree first, then the first tree.
		root1->previous->left = (o_t *)object;
		root1->left = (o_t *)object;
		root1->right = NULL;
		root1->key = a;
		return;
	}
	else{

		//tree not empty.

		//going backwards with the second tree
		while(root2->right != NULL){
			root2 = root2->left;
		}
		root2 = root2->previous;

		while(root2->previous != NULL){
			root2 = root2->previous;
		}

		while(root2->right !=NULL){
			root2 = root2->right;
		}

		int insert_value = root2->left->key;

		//first: insert a in second tree (root2)
		insert_node1(root2, a, 1);

		o_t *stack1[STACKSIZE];
		int stack_ptr1 = 0;

		//second: find insert position in the first tree
		while(temp->right != NULL){
			stack1[stack_ptr1++] = temp;
			if (a < temp->key){
				temp = temp->left;
			}
			else{
				temp = temp->right;
			}
		}

		//third: insert first tree
		insert_node(temp, a);


		//connect the two trees
		if (temp->left->key == a){
			temp->left->previous = root2->left;
			temp->right->previous = temp->previous;
		}
		else{
			temp->left->previous = temp->previous;
			temp->right->previous = root2->left;
		}

		temp->previous = NULL;

		//update previous pointer
		temp = root1;
		while(temp->right != NULL){
			if (insert_value < temp->key){
				temp = temp->left;
			}
			else{
				temp = temp->right;
			}
		}
		temp->previous = root2->right;

		o_t *temp1 = root2;




		//rebalance first tree
		int finished = 0;
		while (stack_ptr1 > 0 && !finished){
			int temp_height , old_height;
			o_t *tmp = stack1[--stack_ptr1];
			old_height = tmp->height;

			if(tmp->left->height - tmp->right->height == 2){

				if (tmp->left->left->height - tmp->right->height == 1){
					right_rotation(tmp);
					tmp->right->height = tmp->right->left->height + 1;
					tmp->height = tmp->right->height + 1;
				}
				else{
					left_rotation(tmp->left);
					right_rotation(tmp);
					temp_height = tmp->left->left->height;
					tmp->left->height = temp_height + 1;
					tmp->right->height = temp_height + 1;
					tmp->height = temp_height + 2;
				}

			}
			else if (tmp->left->height - tmp->right->height == -2){

				if (tmp->right->right->height - tmp->left->height == 1){
					left_rotation(tmp);
					tmp->left->height = tmp->left->right->height + 1;
					tmp->height = tmp->left->height + 1;
				}
				else{
					right_rotation(tmp->right);
					left_rotation(tmp);
					temp_height = tmp->right->right->height;
					tmp->left->height = temp_height + 1;
					tmp->right->height = temp_height + 1;
					tmp->height = temp_height + 2;
				}

			}
			else{
				//if no rotation needed, update height
				if (tmp->left->height > tmp->right->height){
					tmp->height = tmp->left->height + 1;
				}
				else{
					tmp->height = tmp->right->height + 1;
				}

			}

			if (tmp->height == old_height){
				finished = 1;
			}
		}


		root2 = temp1;

		//rebalance second tree
		while(root2 != NULL){
			int temp_height , old_height;

			if(root2->left->height - root2->right->height == 2){

				if (root2->left->left->height - root2->right->height == 1){
					right_rotation1(root2);
					root2->right->height = root2->right->left->height + 1;
					root2->height = root2->right->height + 1;
				}
				else{
					left_rotation1(root2->left);
					right_rotation1(root2);
					temp_height = root2->left->left->height;
					root2->left->height = temp_height + 1;
					root2->right->height = temp_height + 1;
					root2->height = temp_height + 2;
				}

			}
			else if (root2->left->height - root2->right->height == -2){

				if (root2->right->right->height - root2->left->height == 1){
					left_rotation1(root2);
					root2->left->height = root2->left->right->height + 1;
					root2->height = root2->left->height + 1;
				}
				else{
					right_rotation1(root2->right);
					left_rotation1(root2);
					temp_height = root2->right->right->height;
					root2->left->height = temp_height + 1;
					root2->right->height = temp_height + 1;
					root2->height = temp_height + 2;
				}

			}
			else{
				//if no rotation needed, update height
				if (root2->left->height > root2->right->height){
					root2->height = root2->left->height + 1;
				}
				else{
					root2->height = root2->right->height + 1;
				}

			}

			root2 = root2->previous;
		}

		return;
	}

}


int is_before(o_t *ord, int a, int b){
	//returns 1 if key a occurs before key b in the ordered set, 0 else
	o_t *temp, *key_a, *key_b;

	if (ord->left == NULL){
		//empty tree
		return 0;
	}
	else{
		//First: find a in tree
		temp = ord;
		while(temp->right != NULL){
			if (a < temp->key){
				temp = temp->left;
			}
			else{
				temp = temp->right;
			}
		}

		if (temp->key == a){
			//a is found, store back ptr to key_a
			key_a = temp->previous;
		}
		else{
			//a not found
			return 0;
		}

		//Second: find b in tree
		temp = ord;
		while(temp->right != NULL){
			if (b < temp->key){
				temp = temp->left;
			}
			else{
				temp = temp->right;
			}
		}

		if (temp->key == b){
			key_b = temp->previous;
		}
		else{
			return 0;
		}

		int height_a = 0, height_b = 0;
		//Third: look for height of key_a
		temp = key_a;
		while(temp != NULL){
			temp = temp->previous;
			height_a += 1;
		}

		//Fourth: look for height of key_b
		temp = key_b;
		while(temp != NULL){
			temp = temp->previous;
			height_b += 1;
		}

		//Fifth: check if heights are unequal, if not prioritize the shorter height
		if (height_a != height_b){
			if (height_a > height_b){
				while(height_a != height_b){
					key_a = key_a->previous;
					height_a -= 1;
				}
			}
			else{
				while(height_a != height_b){
					key_b = key_b->previous;
					height_b -= 1;
				}
			}
		}

		while(key_a->previous != key_b->previous){
			key_a = key_a->previous;
			key_b = key_b->previous;
		}

		if (key_a->previous->left == key_a){
			return 1;
		}
		else{
			return 0;
		}

	}
	return 0;
}



long p(long q)
{ return( (1247*q +104729) % 300007 );
}

int main(){
	long i; o_t *o; 
	printf("starting \n");
	o = create_order();
	
	for(i=100000; i>=0; i-- )
		insert_bottom( o, p(i) );
	//printf("Done loop1\n");
	
	for(i=100001; i< 300007; i+=2 ){  
		insert_after(o, p(i+1), p(i-1) );
		insert_before( o, p(i), p(i+1) );
	}
	printf("inserted 300000 elements. \n");
	//printf("Done loop2\n");

	
	for(i = 250000; i < 300007; i++ ){
		delete_o( o, p(i) );
	}
	printf("deleted 50000 elements. \n");
	//printf("Done loop3\n");
	

	insert_top( o, p(300006) );
	//printf("Done 4\n");

	for(i = 250000; i < 300006; i++ ){
		insert_before( o, p(i) , p(300006) );
	}
	printf("reinserted. now testing order\n");
	//printf("Done loop5\n");
	
	
	for( i=0; i < 299000; i +=42 ){  
		if( is_before( o, p(i), p(i+23) ) != 1 ){  
			printf(" found error (1) \n"); 
			exit(0);
		}
	}
	printf("finished. no problem found.\n");
	//printf("Done loop6\n");

	return 0;
}
