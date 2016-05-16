/*
	Homework 2
	
	Compiles and run with command lines
			gcc -o hw2 hw2.c
			./hw2
*/
#include <stdio.h>
#include <stdlib.h>

#define BLOCKSIZE 256
#define STACKSIZE 1000000

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

typedef struct { 
	struct seg_t_2d_t *node1;
	struct seg_t_2d_t *node2;
	int number;
} stack_item;

typedef struct rec_intv{
	int x_min; int x_max; 
	int y_min; int y_max;
}intervals;

intervals rect_x_interval[STACKSIZE]; //stores x rectangles intervals
intervals rect_y_interval[STACKSIZE]; //stores y rectangles intervals


seg_tree_2d_t *current_block = NULL;
seg_tree_2d_t *free_list = NULL;
int size_left;

int rec_number_x, rec_number_y;

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


	stack_item current, left, right;
	stack_item stack[1000];
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
			(current.node1)->right = NULL;
			(current.node1)->key = list->key;

			if (current.node2 != NULL){
				//insert comparison key in the interrior node
				(current.node2)->key = list->key;
			}

			//unlink first item from the list, content has been copied to leaf
			//and return node
			temp = list;
			list = list->right;
			return_node(temp);
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
	
	if (tree == NULL){
		return NULL;
	}
	else if(tree->left == NULL){
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
		return result_list;
	}
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

	//printf("INSERT_INTERVAL()\n");
	if (tree->left == NULL){
		//tree is incorrect
		//printf("EMPTY TREE\n");
		exit(-1);
	}
	else{
		//printf("NOT EMPTY TREE\n");
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

		//printf("AFTER WHILE LOOP1\n");

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

			//left leaf needs to be selected  if reached in descent 
			if (left_path->right == NULL && left_path->key == xmin){
				attach_intv_node(left_path, xmin, xmax, ymin, ymax);
			}
		}

		//printf("AFTER LEFT_PATH IF\n");

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
		//printf("AFTER RIGHT_PATH IF\n");
	}
}


/*
int compint(int *a, int *b){
	return *a>*b;
}*/


int compint( const void *a_, const void *b_){ 
	int a = *(int*) a_;
	int b = *(int*) b_;
	return (a-b);
}



rect_list_t * query_seg_tree_2d(seg_tree_2d_t *tree, int x, int y){
	seg_tree_2d_t *current_node;
	rect_list_t *result_list, *current_list, *new_result;

	if (tree == NULL){
		return NULL;
	}
	else if (tree->left == NULL){
		//empty tree
		return NULL;
	}
	else{
		current_node = tree;
		result_list = NULL;

		while (current_node->right != NULL){
			if (x < current_node->key){
				//go left of current tree
				current_node = current_node->left;
			}
			else{
				current_node = current_node->right;
			}

			current_list  = find_intervals( current_node->tree, y);

			while(current_list != NULL){
				//going through and copying current_list to new_list
				new_result = get_list_node();

				new_result->next = result_list;

				//updating x_min, y_min, x_max, and y_max values
				new_result->x_min = current_list->y_min;
				new_result->x_max = current_list->y_max;
				new_result->y_min = current_list->x_min;
				new_result->y_max = current_list->x_max;

				//set new_result to result_list
				result_list = new_result;
				//updating to next in list
				current_list = current_list->next;
			}

		}
		return result_list;
	}

}


seg_tree_2d_t *build_y_segment_tree(rect_list_t * rect_list){
	//build y segment tree
	rect_list_t *temp_list;
	seg_tree_2d_t *list;

	int keys[STACKSIZE];
	int j, k, prev_key, i = 0;

	int *m = (int *) malloc(sizeof(int));
	*m = 42;

	temp_list = rect_list;

	//initialize rect_y_intervals to 0
	for (k=0; k<rec_number_y; k++){
		rect_y_interval[k].x_min = rect_y_interval[k].x_max = 0;
		rect_y_interval[k].y_min = rect_y_interval[k].y_max = 0;
	}

	while(temp_list != NULL){
		keys[2*i] = temp_list->y_min;
		keys[2*i+1] = temp_list->y_max;

		rect_y_interval[i].x_min = temp_list->y_min;
		rect_y_interval[i].x_max = temp_list->y_max;
		rect_y_interval[i].y_min = temp_list->x_min;
		rect_y_interval[i].y_max = temp_list->x_max;
		temp_list = temp_list->next;
		i++;
	}

	rec_number_y = i;

	qsort(keys, 2*i, sizeof(int), compint);

	seg_tree_2d_t *tmp;

	list = get_node();
	list->right = NULL;
	prev_key = list->key = keys[2*i-1];

	list->left = (seg_tree_2d_t *) m;

	
	for (j=2*i-2; j>=0; j--){
		if (keys[j] != prev_key){
			tmp = get_node();
			prev_key = tmp->key = keys[j];
			tmp->right = list;
			tmp->left = (seg_tree_2d_t *) m;
			list = tmp;
		}
	}

	tmp = get_node();
	tmp->key = -1000;
	tmp->right = list;
	tmp->left = (seg_tree_2d_t *) m;
	list = tmp;

	tmp = list;

	return list;
}

seg_tree_2d_t *build_x_segment_tree(rect_list_t * rect_list){
	//build x segment tree
	rect_list_t *temp_list;
	seg_tree_2d_t *list;

	int keys[STACKSIZE];
	int j, prev_key,  i = 0;

	int *m = (int *) malloc(sizeof(int));
	*m = 42;

	temp_list = rect_list;

	//build x segment tree
	while(temp_list != NULL){
		keys[2*i] = temp_list->x_min;
		keys[2*i+1] = temp_list->x_max;

		rect_x_interval[i].x_min = temp_list->x_min;
		rect_x_interval[i].x_max = temp_list->x_max;
		rect_x_interval[i].y_min = temp_list->y_min;
		rect_x_interval[i].y_max = temp_list->y_max;
		temp_list = temp_list->next;
		i++;
	}

	rec_number_x = i;

	qsort(keys, 2*i, sizeof(int), compint);
	
	seg_tree_2d_t *tmp;

	list = get_node();
	list->right = NULL;
	prev_key = list->key = keys[2*i-1];
	list->left = (seg_tree_2d_t *) m;

	
	for (j=2*i-2; j>=0; j--){
		if (keys[j] != prev_key){
			tmp = get_node();
			prev_key = tmp->key = keys[j];
			tmp->right = list;
			tmp->left = (seg_tree_2d_t *) m;
			list = tmp;
		}
	}

	tmp = get_node();
	tmp->key = -1000;
	tmp->right = list;
	tmp->left = (seg_tree_2d_t *) m;
	list = tmp;

	tmp = list;

	return list;
}


seg_tree_2d_t *create_seg_tree_2d(rect_list_t *list){

	seg_tree_2d_t *x_list, *y_list, *seg_tree, *temp_node;
	seg_tree_2d_t *stack[1000];
	rect_list_t *current_list;
	int i, stack_ptr = 0;

	//build x segment tree
	x_list = build_x_segment_tree(list);

	//make seg tree
	seg_tree = make_tree(x_list);

	empty_tree(seg_tree);

	for (i = rec_number_x-1; i>= 0; i--){
		insert_interval(seg_tree, rect_x_interval[i].x_min, rect_x_interval[i].x_max, rect_x_interval[i].y_min, rect_x_interval[i].y_max );
	}
	
	stack[stack_ptr++] = seg_tree;

	while(stack_ptr > 0){
		
		temp_node = stack[--stack_ptr];

		if (temp_node->rect_interval != NULL){
			//build y segment tree
			current_list = temp_node->rect_interval;
			y_list = build_y_segment_tree(current_list);
			temp_node->tree = make_tree(y_list);
			empty_tree(temp_node->tree);
			

			for(i=rec_number_y-1; i>=0; i--){
				insert_interval(temp_node->tree, rect_y_interval[i].x_min, rect_y_interval[i].x_max, rect_y_interval[i].y_min, rect_y_interval[i].y_max );
			}
		}

		if (temp_node->right != NULL){
			stack[stack_ptr++] = temp_node->left;
			stack[stack_ptr++] = temp_node->right;
		}
	}


	return seg_tree;
}

int main(){
	int i, j, x, y, l,m; 
	rect_list_t rectangles[50000];
	rect_list_t * tmp;
	seg_tree_2d_t *tr;
	
	for( i=0; i<50000; i++){  
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
	tr = create_seg_tree_2d( rectangles );
	printf("Created 2d segment tree\n"); fflush(stdout);


	/* test 1 */
	for( i= 0; i<1000; i++ ){  
		x = 500000 + 400*i +30;
		y = ((1379*i)%400000) + 10;
		tmp = query_seg_tree_2d( tr, x,y);
		if( tmp != NULL ){  
			printf("point %d,%d should not be covered by any rectangle.\n",x,y);
			printf(" instead reported as covered by [%d,%d]x[%d,%d]\n", tmp->x_min, tmp->x_max, tmp->y_min, tmp->y_max); 
			fflush(stdout);
			exit(0);
		}
	}
	printf("Passed Test 1.\n");


	/* test 2 */
	for( i= 0; i<1000; i++ ){  
		x = 500000 + 400*i +5;
		y = ((3791*i)%400000) + 10;
		tmp = query_seg_tree_2d( tr, x,y);
		if( tmp == NULL ){  
			printf("point %d,%d should be covered by a rectangle, not found\n" ,x,y);
			fflush(stdout);
			exit(0);
		}
		if( x< tmp->x_min || x > tmp->x_max || y< tmp->y_min || y > tmp->y_max ){  
			printf("rectangle [%d,%d]x[%d,%d] does not cover point %d,%d\n",tmp->x_min, tmp->x_max, tmp->y_min, tmp->y_max,x,y); 
			fflush(stdout); 
			exit(0);
		} 
	}
	printf("Passed Test 2.\n");


	/* test 3 */
	for( i= 0; i<10; i++ ){  
		x = 300000 + 20000*i + 3;
		y = 400001;
		tmp = query_seg_tree_2d( tr, x,y);
		if( tmp == NULL ){  
			printf("point %d,%d should be covered by a rectangle, not found\n" ,x,y);
			exit(0);
		}
		while( tmp != NULL ){  
			if( x< tmp->x_min|| x > tmp->x_max|| y< tmp->y_min|| y > tmp->y_max ){  
				printf("rectangle [%d,%d]x[%d,%d] does not cover point %d,%d\n", tmp->x_min, tmp->x_max, tmp->y_min, tmp->y_max,x,y); 
				exit(0);
			}
			tmp = tmp->next;
		}
	}
	printf("Passed Test 3.\n");


	/* test 4 */
	for( i= 0; i<10; i++ ){  
		x = 10* (rand()%100000) +4;
		y = 10* (rand()%100000) +4;
		m=0;
		for(j=0; j<50000; j++ ){  
			if( rectangles[j].x_min < x && rectangles[j].x_max > x && rectangles[j].y_min < y && rectangles[j].y_max > y )
				m +=1;
		}
		tmp = query_seg_tree_2d( tr, x,y);
		l=0;
		while( tmp != NULL ){  
			if( x< tmp->x_min|| x > tmp->x_max|| y< tmp->y_min|| y > tmp->y_max ){  
				printf("rectangle [%d,%d]x[%d,%d] does not cover point %d,%d\n", tmp->x_min, tmp->x_max, tmp->y_min, tmp->y_max,x,y); 
				exit(0);
			}
			l+= 1;
			tmp = tmp->next;
		}
		if( l!= m ){  
			printf("test 4.%d: Point %d,%d should be in %d rectangles, but %d found\n", i, x, y, m, l);
			fflush(stdout); 
		}
	}
	printf("Passed Test 4.\n");
	
	printf("End of tests\n");
	
	return 0;
}
