/*
	Homework 4
	
	Compiles and run with command lines
			gcc -o hw4 hw4.c
			./hw4
*/
#include <stdio.h>
#include <stdlib.h>

#define BLOCKSIZE 256
#define MAXP 2278320 
#define MAX_HASH_STRINGS 250000

typedef char object_t;

typedef struct { 
	int b;   
	int size; 
	struct htp_l_node *a_list;
} hf_param_t;

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

int universal_hash_function(char *key, hf_param_t hfp){  
	int sum;
	htp_l_node_t *al;
	sum = hfp.b;
	al = hfp.a_list;

	while( *key != '\0' ){  
		if( al->next == NULL ){   
			al->next = (htp_l_node_t *) get_node();
			al->next->next = NULL;
			al->a = rand()%MAXP;
		}	
		sum += abs( (al->a)*((int) *key))%MAXP;
		key += 1;
		al = al->next;
   }

   return( sum%2000000 );
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

void insert_bf(bf_t *b, char *s){
	//Inserts the string `*s` into the Bloom filter *b

	int i;
	if (b == NULL){
		return;
	}

	for(i=0; i<8; i++){
		int j = universal_hash_function(s, b->bf_params[i]);
		(b->bits[i])[j/8] = (b->bits[i])[j/8] | hexref[j%8];
	}
}

int is_element(bf_t *b, char *q){
	//Returns 1 if string *q is accepted by the Bloom filter, and 0 else

	if (b == NULL){
		printf("ERROR\n");
		exit(-1);
	}

	int i;

	for(i=0; i<8; i++){
		int j = universal_hash_function(q, b->bf_params[i]);
		
		if (((b->bits[i])[j/8] & hexref[j%8]) == 0x00){
			return 0;
		}
	}
	return 1;
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

	for( i= 0; i< 1450000; i++ ){  
		char s[8];
		sample_string_A(s,i);
		insert_bf( bloom, s );
	}

	for( i= 0; i< 500000; i++ ){  
		char s[7];
		sample_string_B(s,i);
		insert_bf( bloom, s );
	}

	for( i= 0; i< 50000; i++ ){  
		char s[6];
		sample_string_C(s,i);
		insert_bf( bloom, s );
	}
	
	printf("inserted 2,000,000 strings of length 8,7,6.\n");

	for( i= 0; i< 1450000; i++ ){  
		char s[8];
		sample_string_A(s,i);
		if( is_element( bloom, s ) != 1 ){  
			printf("found negative error (1)\n"); exit(0); }
	}


	for( i= 0; i< 500000; i++ ){  
		char s[7];
		sample_string_B(s,i);
		if( is_element( bloom, s ) != 1 ){  
			printf("found negative error (2)\n"); exit(0); 
		}
	}

	for( i= 0; i< 50000; i++ ){  
		char s[6];
		sample_string_C(s,i);
		if( is_element( bloom, s ) != 1 ){  
			printf("found negative error (3)\n"); exit(0); 
		}
	}

	j = 0;
	for( i= 0; i< 500000; i++ ){  
		char s[8];
		sample_string_D(s,i);
		if( is_element( bloom, s ) != 0 )
			j+=1;
	}

	for( i= 0; i< 500000; i++ ){  
		char s[7];
		sample_string_E(s,i);
		if( is_element( bloom, s ) != 0 )
			j+=1;
	}

	printf("Found %ld positive errors out of 1,000,000 tests.\n",j);
	printf("Positive error rate %f.\n", (float)j/10000.0);

	return 0;
}