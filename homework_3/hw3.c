/*
	Homework 3
	
	Compiles and run with command lines
			gcc -o hw3 hw3.c
			./hw2

	note to self: gcc -g -fno-stack-protector -D_FORTIFY_SOURCE=0 -o hw3 hw3.c
*/


#include <stdio.h>
#include <stdlib.h>

#define BLOCKSIZE 256

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

	temp2->left = NULL;
	temp2->key = 0;

	temp1->left = NULL;
	temp1->previous = temp2;

	return temp1;
}

int *find(o_t *tree, int key){
	o_t *temp_node;
	//find key in tree
	if (tree->left == NULL){
		//empty tree
		return NULL;
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
			return ( (int *)temp_node->left);
		}
		else{
			return NULL;
		}
	}
}


void left_rotation1(o_t *n){
	o_t *temp_node;
	o_t *temp_previous;

	temp_node = n->left;
	temp_previous = n->previous;

	n->left = n->right;
	n->key  = 0;

	n->right = n->left->right;

	n->left->right = n->left->left;
	
	n->left->left  = temp_node;
	n->left->key   = 0;

	n->previous = temp_previous;
	n->left->previous = n;
	n->right->previous = n;
	n->left->left->previous = n->left;
	n->left->right->previous = n->left;
}

void right_rotation1(o_t *n){
	o_t *temp_node;
	o_t *temp_previous;

	temp_node = n->right;
	temp_previous = 0;

	n->right = n->left;
	n->key = n->left->key;

	n->left = n->right->left;

	n->right->left = n->right->right;

	n->right->right = temp_node;
	n->right->key = 0;

	n->previous = temp_previous;
	n->left->previous = n;
	n->right->previous = n;
	n->right->left->previous = n->right;
	n->right->right->previous = n->left;
}


void insert_bottom (o_t *ord, int a){
	//inserts the key a as smallest element in the ordered set
	o_t *temp, *root1, *root2;

	o_t *object = (o_t *) (int *)malloc(sizeof(int));

	root1 = ord;
	root2 = ord;

	//going backwards with the second tree
	while(root2->right != NULL){
		root2 = root2->right;
	}
	root2 = root2->previous;

	while(root2->previous != NULL){
		root2 = root2->previous;
	}


	if (root1->left == NULL){
		//first tree is empty tree: insert into the second tree first, then the first tree.
		root2->left = object;
		root2->right = NULL;
		root2->key = 0;
		root2->height = 0;

		root1->left = object;
		root1->right = NULL;
		root1->key = a;
		root1->height = 0;

		root1->previous = root2;
	}
	else{
		if (find(ord, a) != NULL){
			//if a exist, return
			return;
		}

		o_t *stack1[100], *stack2[100];
		int stack_ptr1 = 0, stack_ptr2 =0;

		o_t *old_leaf2, *new_leaf2, *old_leaf1, *new_leaf1;

		temp = root2;
		while(temp->right != NULL){
			stack2[stack_ptr2++] = temp;
			temp = temp->right;
		}

		//first: insert second tree
		old_leaf2 = get_node();
		old_leaf2->left = temp->left;
		old_leaf2->right = NULL;
		old_leaf2->key = 0;
		old_leaf2->height = 0;

		new_leaf2 = get_node();
		new_leaf2->left = object;
		new_leaf2->right = NULL;
		new_leaf2->key = 0;
		new_leaf2->height = 0;

		temp->left = old_leaf2;
		temp->right = new_leaf2;
		old_leaf2->previous = temp;
		new_leaf2->previous = temp;
		temp->height = 1;


		//second: find insert position in the first tree
		temp = root1;
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
		old_leaf1 = get_node();
		old_leaf1->left = temp->left;
		old_leaf1->right = NULL;
		old_leaf1->key = temp->key;
		old_leaf1->height = 0;

		new_leaf1 = get_node();
		new_leaf1->left = object;
		new_leaf1->right = NULL;
		new_leaf1->key = a;
		new_leaf1->height = 0;

		if (temp->key < a){
			temp->left = old_leaf1;
			temp->right = new_leaf1;
			temp->key = a;
		}
		else{
			temp->left = new_leaf1;
			temp->right = old_leaf1;
		}

		temp->height = 1;

		//connect the two trees
		old_leaf1->previous = old_leaf2;
		new_leaf1->previous = new_leaf2;
		temp->previous = NULL;



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


		//rebalance second tree
		finished = 0;
		while (stack_ptr2 > 0 && !finished){
			int temp_height , old_height;
			temp = stack2[--stack_ptr2];
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


void insert_after(o_t *ord, int a, int b){
	//inserts key a immediately after key b in the ordered set
	o_t *temp, *insert_position;

	o_t *object = (o_t *) (int *)malloc(sizeof(int));

	if (ord->left == NULL){
		//empty tree
		return;
	}

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
		//b is found in first tree
		if (find(ord, a) != NULL){
			//a exist in tree
			return;
		}

		o_t *stack1[100], *stack2[100];
		int stack_ptr1 = 0, stack_ptr2 =0;

		o_t *old_leaf2, *new_leaf2, *old_leaf1, *new_leaf1;

		insert_position = temp->previous;

		//inserting a in second tree
		old_leaf2 = get_node();
		old_leaf2->left = insert_position->left;
		old_leaf2->right = NULL;
		old_leaf2->key = 0;
		old_leaf2->height = 0;

		new_leaf2 = get_node();
		new_leaf2->left = object;
		new_leaf2->right = NULL;
		new_leaf2->key = 0;
		new_leaf2->height = 0;

		insert_position->left = old_leaf2;
		insert_position->right = new_leaf2;
		new_leaf2->previous = insert_position;
		old_leaf2->previous = insert_position;
		insert_position->height = 1;


		//setting second stack for the second tree
		while(insert_position != NULL){
			stack1[stack_ptr1++] = insert_position;
			insert_position = insert_position->previous;
		}

		while(stack_ptr1 > 0){
			stack2[stack_ptr2++] = stack1[--stack_ptr1];
		}



		
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

		if (insert_position->key == a){
			//a exist
			return;
		}

		//inserting a in first tree
		old_leaf1 = get_node();
		old_leaf1->left = insert_position->left;
		old_leaf1->right = NULL;
		old_leaf1->key = insert_position->key;
		old_leaf1->height = 0;

		new_leaf1 = get_node();
		new_leaf1->left = object;
		new_leaf1->right = NULL;
		new_leaf1->key = a;
		new_leaf1->height = 0;

		if (insert_position->key < a){
			insert_position->left = old_leaf1;
			insert_position->right = new_leaf1;
			insert_position->key = a;
		}
		else{
			insert_position->left = new_leaf1;
			insert_position->right = old_leaf1;
		}

		insert_position->height = 1;

		//connect the two trees
		old_leaf1->previous = old_leaf2;
		new_leaf1->previous = new_leaf2;
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


		//rebalance second tree
		finished = 0;
		while (stack_ptr2 > 0 && !finished){
			int temp_height , old_height;
			temp = stack2[--stack_ptr2];
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

void insert_before(o_t *ord, int a, int b){
	//inserts the key a immediately before key b in the ordered set
	o_t *temp, *insert_position;

	o_t *object = (o_t *) (int *)malloc(sizeof(int));

	if (ord->left == NULL){
		//empty tree
		return;
	}

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
		//b is found in first tree
		if (find(ord,a) != NULL){
			//a exist in tree
			return;
		}

		o_t *stack1[100], *stack2[100];
		int stack_ptr1 = 0, stack_ptr2 =0;

		o_t *old_leaf2, *new_leaf2, *old_leaf1, *new_leaf1;

		insert_position = temp->previous;

		//inserting a in second tree
		old_leaf2 = get_node();
		old_leaf2->left = insert_position->left;
		old_leaf2->right = NULL;
		old_leaf2->key = 0;
		old_leaf2->height = 0;

		new_leaf2 = get_node();
		new_leaf2->left = object;
		new_leaf2->right = NULL;
		new_leaf2->key = 0;
		new_leaf2->height = 0;

		insert_position->left = old_leaf2;
		insert_position->right = new_leaf2;
		new_leaf2->previous = insert_position;
		old_leaf2->previous = insert_position;
		insert_position->height = 1;

		//setting second stack for the second tree
		while (insert_position != NULL){
			stack1[stack_ptr1++] = insert_position;
			insert_position = insert_position->previous;
		}

		while(stack_ptr1>0){
			stack2[stack_ptr2++] = stack1[--stack_ptr1];
		}


		insert_position = ord;
		//setting first stack for first tree
		while (insert_position->right != NULL){
			stack1[stack_ptr1++] = insert_position;
			if (a < insert_position->key){
				insert_position = insert_position->left;
			}
			else{
				insert_position = insert_position->right;
			}
		}


		//inserting a in first tree
		old_leaf1 = get_node();
		old_leaf1->left = insert_position->left;
		old_leaf1->right = NULL;
		old_leaf1->key = insert_position->key;
		old_leaf1->height = 0;

		new_leaf1 = get_node();
		new_leaf1->left = object;
		new_leaf1->right = NULL;
		new_leaf1->key = a;
		new_leaf1->height = 0;


		if (insert_position->key < a){
			insert_position->left = old_leaf1;
			insert_position->right = new_leaf1;
			insert_position->key = a;
		}
		else{
			insert_position->left = new_leaf1;
			insert_position->right = old_leaf1;
		}

		insert_position->height = 1;

		//connect the two trees
		old_leaf1->previous = old_leaf2;
		new_leaf1->previous = new_leaf2;
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


		//rebalance second tree
		finished = 0;
		while (stack_ptr2 > 0 && !finished){
			int temp_height , old_height;
			temp = stack2[--stack_ptr2];
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

void delete_o (o_t *ord, int a){
	//deletes the key a from the ordered set
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
	printf("Done loop1\n");

	
	for(i=100001; i< 300007; i+=2 ){  
		insert_after(o, p(i+1), p(i-1) );
		insert_before( o, p(i), p(i+1) );
		printf("%ld, %ld\n", i, p(i+1));
	}
	printf("inserted 300000 elements. \n");
	printf("Done loop2\n");

	/*
	for(i = 250000; i < 300007; i++ ){
		delete_o( o, p(i) );
	}
	printf("deleted 50000 elements. \n");
	printf("Done loop3\n");
	*/
	
	return 0;
}
