#ifndef   	_MEM_ALLOC_H_
#define   	_MEM_ALLOC_H_


/* Allocator functions, to be implemented in mem_alloc.c */
void memory_init(void); 
char *memory_alloc(int size); 
void memory_free(char *p);

/* Logging functions */
void print_info(void); 
int check_for_loop(void); 
//stores the sum of requested memory by the user
int requested_mem;
//stores the sum of used memory to fill the requests
int used_mem;
void add_frag(int size);
void del_frag(int size);
void frag_alloc(int size,int size_alloc);
void frag_free(int size);
void merge_free_blocks(void);
void print_alloc_info(char *addr, int size); 
void print_free_info(char *addr); 
void print_free_blocks(void); 
char *heap_base(void); 

#include <stdlib.h>
//void *malloc(size_t size); 
//void free(void *p); 
//void *realloc(void *ptr, size_t size); 


#endif 	    /* !_MEM_ALLOC_H_ */
