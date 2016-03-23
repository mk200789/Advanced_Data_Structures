/*
	Wan Kim Mok
	Homework 1
	
	Compiles and run with command lines
			gcc -o hw1 hw1.c
			./hw1
*/
#include<stdio.h>
#include<stdlib.h>

#define BLOCKSIZE 256
#define STACK_MAX 1000000

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
		tmp->left = NULL;
		tmp->right = NULL;
		tmp->key = 0;
		tmp->height = 0;
		size_left -= 1;
	}
	
	return tmp;
	
}


void right_rotation(tree_node *n){
	//note to self: modified for the new tree structure
	tree_node *temp_node;
	int temp_key;

	temp_node = n->right;

	n->right = n->left;

	n->left = n->right->left;

	n->right->left = n->right->right;

	n->right->right = temp_node;
	n->right->key = n->right->left->key + n->right->right->key;
}

void left_rotation(tree_node *n){
	//note to self: modified for the new tree structure
	tree_node *temp_node;
	int temp_key;

	temp_node = n->left;

	n->left = n->right;

	n->right = n->left->right;

	n->left->right = n->left->left;
	
	n->left->left  = temp_node;
	n->left->key   = n->left->key = n->left->left->key + n->left->right->key;

}

tree_node *create_text(){
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
		tree_node *stack[STACK_MAX];
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

		right_child->left = (tree_node *)new_line;
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


void insert_line(tree_node *tree, int new_key, char *new_line){
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
		tree_node *stack[STACK_MAX];
		int stack_ptr = 0;
		temp_node = tree;

		while(temp_node->right != NULL){
			stack[stack_ptr++] = temp_node;
			temp_node->key += 1;
			if (new_key <= temp_node->left->key){
				temp_node = temp_node->left;
			}
			else{
				new_key -= temp_node->left->key;
				temp_node = temp_node->right;
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
}

char * delete_line(tree_node *tree, int delete_key){
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
		tree_node *stack[STACK_MAX];
		int stack_ptr = 0;
		temp_node = tree;

		while (temp_node->right != NULL){
			stack[stack_ptr++] = temp_node;
			upper_node = temp_node;
			if (delete_key <= temp_node->left->key){
				temp_node = temp_node->left;
				other_node = upper_node->right;
			}
			else{
				delete_key -= temp_node->left->key;
				other_node = upper_node->left;
				temp_node = temp_node->right;
			}
		}

		tree_node *temp;
		//temp_node = stack[--stack_ptr];
		temp = stack[--stack_ptr];

		if (delete_key == 1){
			//perform deletion
			upper_node->left = other_node->left;
			upper_node->right = other_node->right;
			upper_node->height = other_node->height;
			upper_node->key = other_node->key;
			deleted_object = (char *) temp_node->left;

			int tempsize = stack_ptr;

			while (stack_ptr > 0){
				temp = stack[--stack_ptr];
				temp->key -= 1;
			}


			//rebalancing tree
			finished = 0;
			stack_ptr = tempsize;

			//stack_ptr -= 1;

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

	//tree_node *temp_node;
	//char *new_object;
	tree_node *temp_node = (tree_node *)malloc(sizeof(tree_node));
	char *new_object = (char *)malloc(sizeof(char)*1024);

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

char *get_line(tree_node *tree, int key){
	//gets the line of number `key`, if such a line exists, and returns NULL else.
	tree_node *temp_node;
	if (tree->left == NULL){
		//empty tree
		return NULL;
	}
	else{
		temp_node = tree;
		while(temp_node->right != NULL){
			if (key <= temp_node->left->key){
				//if the key is less than the current key in tree go left
				temp_node = temp_node->left;
			}
			else{
				key = key - temp_node->left->key;
				temp_node = temp_node->right;
			}
		}

		if (temp_node->left != NULL){
			return (char *)temp_node->left;
		}
		else{
			return NULL;
		}
	}
}


int main(){
 int i, tmp; tree_node *txt1, *txt2, *txt3; char *c;
   printf("starting \n");
   txt1 = create_text();
   txt2 = create_text();
   txt3 = create_text();
   append_line(txt1, "line one" );
   if( (tmp = length_text(txt1)) != 1)
   {  printf("Test 1: length should be 1, is %d\n", tmp); exit(-1);
   }
   append_line(txt1, "line hundred" );
   insert_line(txt1, 2, "line ninetynine" );
   insert_line(txt1, 2, "line ninetyeight" );
   insert_line(txt1, 2, "line ninetyseven" );
   insert_line(txt1, 2, "line ninetysix" );
   insert_line(txt1, 2, "line ninetyfive" );
   for( i = 2; i <95; i++ )
     insert_line(txt1, 2, "some filler line between 1 and 95" );
   if( (tmp = length_text(txt1)) != 100)
   {  printf("Test 2: length should be 100, is %d\n", tmp); exit(-1);
   }
   printf("found at line 1:   %s\n",get_line(txt1,  1));
   printf("found at line 2:   %s\n",get_line(txt1,  2));
   printf("found at line 99:  %s\n",get_line(txt1, 99));
   printf("found at line 100: %s\n",get_line(txt1,100));
   for(i=1; i<=10000; i++)
   {  if( i%2==1 )
        append_line(txt2, "A");
      else 
        append_line(txt2, "B");
   }
   if( (tmp = length_text(txt2)) != 10000)
   {  printf("Test 3: length should be 10000, is %d\n", tmp); exit(-1);
   }
   c = get_line(txt2, 9876 );
   if( *c != 'B')
     {  printf("Test 4: line 9876 of txt2 should be B, found %s\n", c); exit(-1);
   }
   for( i= 10000; i > 1; i-=2 )
   {  c = delete_line(txt2, i);
      if( *c != 'B')
      {  printf("Test 5: line %d of txt2 should be B, found %s\n", i, c); exit(-1);
      }
      append_line( txt2, c );
   }
   for( i=1; i<= 5000; i++ )
   {  c = get_line(txt2, i);
      if( *c != 'A')
      {  printf("Test 6: line %d of txt2 should be A, found %s\n", i, c); exit(-1);
      }
   }
   for( i=1; i<= 5000; i++ )
     delete_line(txt2, 1 );
   for( i=1; i<= 5000; i++ )
   {  c = get_line(txt2, i);
      if( *c != 'B')
      {  printf("Test 7: line %d of txt2 should be B, found %s\n", i, c); exit(-1);
      }
   }
   set_line(txt1, 100, "the last line");
   for( i=99; i>=1; i-- )
     delete_line(txt1, i );
   printf("found at the last line:   %s\n",get_line(txt1,  1));
   for(i=0; i<1000000; i++)
     append_line(txt3, "line" );   
   if( (tmp = length_text(txt3)) != 1000000)
   {  printf("Test 8: length should be 1000000, is %d\n", tmp); exit(-1);
   }
   for(i=0; i<500000; i++)
     delete_line(txt3, 400000 );   
   if( (tmp = length_text(txt3)) != 500000)
   {  printf("Test 9: length should be 500000, is %d\n", tmp); exit(-1);
   }
   printf("End of tests\n");
	return 0;
}