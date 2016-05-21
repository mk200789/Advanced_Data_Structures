/*
	Homework 4
	
	Compiles and run with command lines
			gcc -o hw4 hw4.c
			./hw4
*/
#include <stdio.h>
#include <stdlib.h>

#define BLOCKSIZE 256
#define MAXP 46337  /* prime, and 46337*46337 < 2147483647 */
#define MAX_HASH_STRINGS 250000

typedef char object_t;

typedef struct { 
	int b;   
	int size; 
	struct htp_l_node *a_list;
} hf_param_t;

/*
typedef struct l_node {
	char *key;
	object_t  *obj;
	struct l_node  *next; 
} list_node_t;
*/

typedef struct htp_l_node { 
	int a; 
	struct htp_l_node *next; 
} htp_l_node_t; 

typedef struct bloom_struct{
	hf_param_t bf_params[10];
	unsigned char *bits[10];
}bf_t;

char hexref[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

htp_l_node_t *current_block = NULL;
htp_l_node_t *free_list = NULL;
int size_left;

htp_l_node_t *get_node(){
	htp_l_node_t *tmp;
	if (free_list != NULL){
		tmp = free_list;
		free_list = free_list->next;
	}
	else{
		if (current_block == NULL || size_left == 0){
			current_block = (htp_l_node_t *)malloc(BLOCKSIZE*sizeof(htp_l_node_t));
			size_left = BLOCKSIZE;
		}
		tmp = current_block++;
		size_left -= 1;
	}
	return tmp;
}

void return_node(htp_l_node_t *node){
	node->next = free_list;
	free_list = node;
}


bf_t *create_bf(){
	//creates a Bloom filter
	bf_t *temp = (bf_t *)malloc(sizeof(bf_t));
	int i, j;

	for (i=0; i<8; i++){
		temp->bf_params[i].b = rand()%MAXP;
		temp->bf_params[i].a_list = get_node();
		temp->bf_params[i].a_list->next = NULL;
		temp->bits[i] = (unsigned char *) malloc(sizeof(char)*MAX_HASH_STRINGS);

		for(j=0; j<MAX_HASH_STRINGS; j++){
			(temp->bits[i])[j] = 0;
		}
	}

	return temp;

}



void sample_string_A(char *s, long i){  
	s[0] = (char)(1 + (i%254));
	s[1] = (char)(1 + ((i/254)%254));
	s[2] = (char)(1 + (((i/254)/254)%254));
	s[3] = 'a';
	s[4] = 'b';
	s[5] = (char)(1 + ((i*(i-3)) %217));
	s[6] = (char)(1 + ((17*i+129)%233 ));
	s[7] = '\0';
}

void sample_string_B(char *s, long i){  
	s[0] = (char)(1 + (i%254));
	s[1] = (char)(1 + ((i/254)%254));
	s[2] = (char)(1 + (((i/254)/254)%254));
	s[3] = 'a';
	s[4] = (char)(1 + ((i*(i-3)) %217));
	s[5] = (char)(1 + ((17*i+129)%233 ));
	s[6] = '\0';
}

void sample_string_C(char *s, long i){  
	s[0] = (char)(1 + (i%254));
	s[1] = (char)(1 + ((i/254)%254));
	s[2] = 'a';
	s[3] = (char)(1 + ((i*(i-3)) %217));
	s[4] = (char)(1 + ((17*i+129)%233 ));
	s[5] = '\0';
}

void sample_string_D(char *s, long i){  
	s[0] = (char)(1 + (i%254));
	s[1] = (char)(1 + ((i/254)%254));
	s[2] = (char)(1 + (((i/254)/254)%254));
	s[3] = 'b';
	s[4] = 'b';
	s[5] = (char)(1 + ((i*(i-3)) %217));
	s[6] = (char)(1 + ((17*i+129)%233 ));
	s[7] = '\0';
}

void sample_string_E(char *s, long i){  
	s[0] = (char)(1 + (i%254));
	s[1] = (char)(1 + ((i/254)%254));
	s[2] = (char)(1 + (((i/254)/254)%254));
	s[3] = 'a';
	s[4] = (char)(2 + ((i*(i-3)) %217));
	s[5] = (char)(1 + ((17*i+129)%233 ));
	s[6] = '\0';
}

int main(){
	long i,j; 
	bf_t * bloom;
	bloom = create_bf();
	printf("Created Filter\n");
	return 0;
}