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
	seg_tree_2d_t *current_node, *right_path, *left_path;
	rect_list_t *current_list, *new_node;

	if (tree->left == NULL){
		//tree is incorrect
		exit(-1);
	}
	else{
		current_node = tree;
		right_path = left_path = NULL;

		while(current_node->right != NULL){
			//current node is not a leaf

			if (xmax < current_node->key){
				//go left
				current_node = current_node->left;
			}
			else if (current_node->key < xmin){
				//go right
				current_node = current_node->right;
			}
			else if (xmin < current_node->key && current_node->key < xmax){
				//split, going both left and right path
				right_path = current_node->right;
				left_path = current_node->left;
				break;
			}
			else if (xmin == current_node->key){
				//set right_path only, no left
				right_path = current_node->right;
				break;
			}
			else{
				//set left_path only, no right
				left_path = current_node->left;
				break;
			}

		}

		if (left_path != NULL){
			//there's  left path
			while (left_path->right != NULL){
				//follow the path of the left endpoint  xmin
				if (xmin < left_path->key){
					//right node must be selected
					attach_intv_node(left_path->right, xmin, xmax, ymin, ymax);
					left_path = left_path->left;
				}
				else if (xmin == left_path->key){
					attach_intv_node(left_path->right, xmin, xmax, ymin, ymax);
					//no further descent necessary
					break; 
				}
				else{
					//go right, no node selected
					left_path = left_path->right;
				}
			}

			/* left leaf needs to be selected  if reached in descent */
			if (left_path->right == NULL && left_path->key == xmin){
				attach_intv_node(left_path, xmin, xmax, ymin, ymax);
			}
		}

		if (right_path != NULL){
			while (right_path->right != NULL){
				if (right_path->key < xmax){
					//left node must be selected
					attach_intv_node(right_path->left, xmin, xmax, ymin, ymax);
					right_path = right_path->right;
				}
				else if (right_path->key == xmax){
					attach_intv_node(right_path->left, xmin, xmax, ymin, ymax);
					break;
				}
				else{
					right_path = right_path->left;
				}
			}
		}
	}
}

int compint(int *a, int *b){
	return *a>*b;
}

rect_list_t * query_seg_tree_2d(seg_tree_2d_t *tree, int x, int y){
	seg_tree_2d_t *current_node;
	rect_list_t *result_list, *current_list, *new_result;

	if (tree->left == NULL){
		//empty tree
		printf("Empty tree.\n");
		return NULL;
	}
	else{
		current_node = tree;
		while (current_node->right != NULL){
			if (x < current_node->key){
				//go left of current tree
				current_node = current_node->left;
			}
			else{
				current_node = current_node->right;
			}

			current_list = current_node->rect_interval;

			if (current_list != NULL){
				//if current_list not empty search current_node->tree for y
				current_list = find_intervals(current_node->tree, y);

				while(current_list != NULL){
					//going through and copying current_list to new_list
					new_result = get_list_node();

					new_result->next = result_list;

					//updating x_min, y_min, x_max, and y_max values
					new_result->x_min = current_list->x_min;
					new_result->x_max = current_list->x_max;
					new_result->y_min = current_list->y_min;
					new_result->y_max = current_list->y_max;

					//set new_result to result_list
					result_list = new_result;
					//updating to next in list
					current_list = current_list->next;
				}
			}
		}
	}
	return result_list;

}
seg_tree_2d_t *create_seg_tree_2d(rect_list_t *list){
	seg_tree_2d_t *temp, *rect_to_tree_list, *temp_list;

	//convert rectangle list to tree list: x segment tree


	return temp;
}

int main(){
	int i, j, x, y, l,m; 
	rect_list_t rectangles[50000];
	rect_list_t * tmp;
	seg_tree_2d_t *tr;
	for( i=0; i<50000; i++)
	{
		rectangles[(17*i)%50000 ].next = rectangles + ((17*(i+1))%50000);  
	}

	rectangles[(17*49999)%50000 ].next = NULL;
	i=0; tmp = rectangles;
	while(tmp->next != NULL ){  
		tmp = tmp->next; i+=1; 
	}
	printf("List of %d rectangles\n",i);

	for(i=0; i<12500; i++){  
		rectangles[i].x_min = 500000 + 40*i;
		rectangles[i].x_max = 500000 + 40*i + 20;
		rectangles[i].y_min = 0;
		rectangles[i].y_max = 1000000;
	}
	for(i=12500; i<25000; i++){  
		rectangles[i].x_min = 500000 + 40*(i-12500) + 10;
		rectangles[i].x_max = 500000 + 40*(i-12500) + 20;
		rectangles[i].y_min = 0;
		rectangles[i].y_max = 1000000;
	}
	for(i=25000; i<37500; i++){  
		rectangles[i].x_min = 20*(i-25000);
		rectangles[i].x_max = 20*(i-25000) + 250000;
		rectangles[i].y_min = 20*(i-25000);
		rectangles[i].y_max = 20*(i-25000) + 250000;
	}
	for(i=37500; i<50000; i++){  
		rectangles[i].x_min = 40*(i-37500);
		rectangles[i].x_max = 500000;
		rectangles[i].y_min = 300000;
		rectangles[i].y_max = 500000 + 40*(i-37500);
	}

	printf("Defined the 50000 rectangles\n"); fflush(stdout);
	//tr = create_seg_tree_2d( rectangles );
	return 0;
}
