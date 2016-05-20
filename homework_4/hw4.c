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

typedef struct bloom_struct{
	char bit_array1[MAX_HASH_STRINGS];
	char bit_array2[MAX_HASH_STRINGS];
	char bit_array3[MAX_HASH_STRINGS];
	char bit_array4[MAX_HASH_STRINGS];
	char bit_array5[MAX_HASH_STRINGS];
	char bit_array6[MAX_HASH_STRINGS];
	char bit_array7[MAX_HASH_STRINGS];
	char bit_array8[MAX_HASH_STRINGS];
}bf_t;

typedef struct { 
	int b;   
	int size; 
	struct htp_l_node *a_list;
} hf_param_t;

typedef struct l_node {
	char *key;
	object_t  *obj;
	struct l_node  *next; 
} list_node_t;

typedef struct htp_l_node { 
	int a; 
	struct htp_l_node *next; 
} htp_l_node_t; 


list_node_t *currentblock = NULL;
list_node_t *free_list = NULL;
int size_left;

list_node_t *get_node(){
	list_node_t *tmp;
	if (free_list != NULL){
		tmp = free_list;
		free_list = free_list->next;
	}
	else{
		if (currentblock == NULL || size_left == 0){
			currentblock = (list_node_t *)malloc(BLOCKSIZE*sizeof(list_node_t));
			size_left = BLOCKSIZE;
		}
		tmp = currentblock++;
		size_left -= 1;
	}
	return tmp;
}


bf_t *create_bf(){
	//creates a Bloom filter
	bf_t *temp = (bf_t *)malloc(sizeof(bf_t));
	int i;

	for (i=0; i <250000; i++){
		temp->bit_array1[i] = 0;
		temp->bit_array2[i] = 0;
		temp->bit_array3[i] = 0;
		temp->bit_array4[i] = 0;
		temp->bit_array5[i] = 0;
		temp->bit_array6[i] = 0;
		temp->bit_array7[i] = 0;
		temp->bit_array8[i] = 0;
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