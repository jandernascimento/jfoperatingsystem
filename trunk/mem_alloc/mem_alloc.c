#include "mem_alloc.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

/* memory */
char memory[MEMORY_SIZE]; 

/* Structure declaration for a free block */
typedef struct free_block{
  int size; 
  struct free_block *next; 
} free_block_s, *free_block_t; 

/* Structure declaration for an occupied block */
typedef struct{
  int size; 
} busy_block_s, *busy_block_t; 


/* Pointer to the first free block in the memory */
free_block_t first_free; 

#define ULONG(x)((long unsigned int)(x))
#define max(x,y) (x>y?x:y)

void memory_init(void){
  /* ... */
  free_block_t pointer=(free_block_t)&memory;
  pointer->size=sizeof(char)*MEMORY_SIZE;//-sizeof(busy_block_s);
  pointer->next=NULL;
  first_free=pointer;
  //printf("Starting memory at:%i\n",(int)first_free);

  //fragmentation measures
  requested_mem = 0;
  used_mem = 0;
}

void logj(const char *vstr){

  printf("%s\n",vstr);

}

/*
* First fit algorithm implementation
*/
free_block_t algorithmFirstFit(int size,free_block_t f){
  printf("FirstFit: Begin\n");
  free_block_t bell=f;
  while(bell!=NULL){
	int raw_size=size+sizeof(busy_block_s);
	if((bell->size)>=raw_size){
  		printf("FirstFit: End\n");
		return bell;
	}
	bell=bell->next;
  }
  printf("FirstFit: End\n");
  return NULL; 
}


/*
Searches a free block according to the best fit principle
returns address of the free block, or NULL if there is no free block.
*/
free_block_t algorithmBestFit(int size,free_block_t f){

  printf("BestFit: Begin\n");
  int raw_size = size + sizeof(busy_block_s);
  free_block_t bestBlock=NULL;
  free_block_t freeBlock=f;
  printf("BestFit: Searching for free blocks with size: %d + %d = %d\n", size, sizeof(busy_block_s), raw_size);
  while(freeBlock!=NULL){
    //size of free block big enough?
    if ((freeBlock->size) >= raw_size) {
        printf("BestFit: Found a free block with size %d >= %d\n", freeBlock->size, raw_size);
        if (bestBlock == NULL) {
            printf("BestFit: Assign free block to bestBlock\n");
            bestBlock = freeBlock;
        } else {
            //size of free block even smaller (and therefore even better) than current best Block
            if ((bestBlock->size) > (freeBlock->size)) {
                printf("BestFit: Replace current bestBlock with size %d by new Free Block witz size %d\n", bestBlock->size, freeBlock->size);
                bestBlock = freeBlock;
            } else {
                printf("BestFit: Current bestBlock with size %d is smaller than free block with size %d\n", bestBlock->size, freeBlock->size);
            }
        }
    }
    printf("BestFit: Go to next free block @%d\n", (int)freeBlock->next);
    freeBlock=(freeBlock->next);
  }
  if (bestBlock == NULL) {
        printf("BestFit: No free block with appropriate size found\n");
  } else {
        printf("BestFit: Found free block with size %d at %d\n", bestBlock->size, (int)bestBlock);
  }

  printf("BestFit: End\n");
  return bestBlock;

}


/*
Searches a free block according to the worst fit principle
returns address of the free block, or NULL if there is no free block.
*/
free_block_t algorithmWorstFit(int size,free_block_t f){

  printf("WorstFit: Begin\n");
  int raw_size = size + sizeof(busy_block_s);
  free_block_t worstBlock=NULL;
  free_block_t freeBlock=f;
  printf("WorstFit: Searching for free blocks with size: %d + %d = %d\n", size, sizeof(busy_block_s), raw_size);
  while(freeBlock!=NULL){
    //size of free block big enough?
    if ((freeBlock->size) >= raw_size) {
        printf("WorstFit: Found a free block with size %d >= %d\n", freeBlock->size, raw_size);
        if (worstBlock == NULL) {
            printf("WorstFit: Assign free block to worstBlock\n");
            worstBlock = freeBlock;
        } else {
            //size of free block even bigger (and therefore even worse) than current worst Block
            if ((worstBlock->size) < (freeBlock->size)) {
                printf("WorstFit: Replace current worstBlock with size %d by new Free Block witz size %d\n", worstBlock->size, freeBlock->size);
                worstBlock = freeBlock;
            } else {
                printf("WorstFit: Current worstBlock with size %d is greater than free block with size %d\n", worstBlock->size, freeBlock->size);
            }
        }
    }
    printf("WorstFit: Go to next free block @%d\n", (int)freeBlock->next);
    freeBlock=(freeBlock->next);
  }
  if (worstBlock == NULL) {
        printf("WorstFit: No free block with appropriate size found\n");
  } else {
        printf("WorstFit: Found free block with size %d at %d\n", worstBlock->size, (int)worstBlock);
  }

  printf("WorstFit: End\n");
  return worstBlock;

}

/*
* Make sure that if the data where initialized properly or at least filling it up with some data that makes easier to detect the problem
*/
busy_block_t createBusyBlock(int addr){
  busy_block_t BB=(busy_block_t)addr; 
  BB->size=-99999;
  return BB;
}

/*
* Make sure that if the data where initialized properly or at least filling it up with some data that makes easier to detect the problem
*/
free_block_t createFreeBlock(int addr){

  if(addr>=(((int)&memory)+MEMORY_SIZE*sizeof(char))) return NULL;

  free_block_t FB=(free_block_t)addr; 
  FB->next=NULL;
  FB->size=-99999;

  return FB;
}

/*
* Usually used before and after a method execution, ensures that the current method is not letting the data inconsistent, mainly checking if the method to not produces a loop.
*/
int check_for_loop(void){
  int i=1;
  free_block_t ff=first_free;
  if(ff==NULL) {
	return 0;
  }
  for(;ff->next!=NULL;ff=ff->next,i++);
  return i;
}

int fragmentation=0;

void add_frag(int size) {

fragmentation+=size;

}


void del_frag(int size) {

fragmentation-=size;

}

//method used to get the total used memory, to measure fragmentation
void frag_measure(int isAllocation, int sizeRequested, int sizeAllocated) {
  if (isAllocation){
    requested_mem += sizeRequested;
    used_mem += sizeAllocated;
  }
  else{
    requested_mem -= sizeRequested;
    used_mem -= sizeAllocated;
  }

  double frag = used_mem/(double)requested_mem -1;

  printf("FRAG;%i;%i;%f;\n", (int)(frag*100), requested_mem, frag);
}

/*
*The main idea here is to fill the end of the list with new allocated blocks
*/
char *memory_alloc(int size){
  if(size==0) { return NULL; }
  free_block_t selectedFB=NULL;
  
  selectedFB=NULL;

  #if defined(WorstFit)
	selectedFB=algorithmWorstFit(size,first_free);
  #elif defined(BestFit)
	selectedFB=algorithmBestFit(size,first_free);
  #else
	selectedFB=algorithmFirstFit(size,first_free);
  #endif  
  check_for_loop(); 
  if(selectedFB==NULL) return NULL;

  //Calculates if its necessary to give more space than requested duo to fragmentation issues
  int remain_size=(selectedFB->size-size-sizeof(busy_block_s));
  int size_for_safety=0;
  if(remain_size<sizeof(free_block_s)){
    size_for_safety=remain_size;
  }
  int oldFBsize=selectedFB->size;
  
  //Create new busy block 
  busy_block_t BB=createBusyBlock((int)selectedFB);
  BB->size=size+sizeof(busy_block_s)+size_for_safety;

  //Just creates a free block if this one should be created (if only has available space)
  free_block_t newFB=NULL;
  if(remain_size>0){
  	newFB=createFreeBlock(((int)BB+BB->size));
  }

  //Removes old free block from the free block list, since its not going to be free anymore
  free_block_t currentSelectedFB=first_free;
  free_block_t previousSelectedFB=NULL;
    while(currentSelectedFB!=NULL){
       if(currentSelectedFB==selectedFB){
 	 if(currentSelectedFB==first_free) {
	   first_free=first_free->next;
         }
	 if(previousSelectedFB!=NULL){
	   previousSelectedFB->next=currentSelectedFB->next;
	 }
         break;
       }
       previousSelectedFB=currentSelectedFB;
       currentSelectedFB=currentSelectedFB->next;
    }

  //Setup the new free block in the proper position inside free block list
  if(newFB!=NULL){
    free_block_t currentFB=first_free;
    free_block_t previousFB=NULL;
    while(currentFB!=NULL){
       if((int)newFB<(int)currentFB){
         first_free=newFB; 
         newFB->next=currentFB;
         break;
       }
       previousFB=currentFB;
       currentFB=currentFB->next;
    }
    newFB->size=(oldFBsize-BB->size);
    //If the first free is NULL means that the previous list is empty
    if(first_free==NULL) first_free=newFB;
    //If has a previous free block, the new block should be put just after him
    if(previousFB!=NULL) previousFB->next=newFB;
  }

  //Setup the return address to the client of this method
  char *addr=(char*)((int)BB+sizeof(busy_block_s));
  print_alloc_info(addr,size); 
  if((int)addr==0) { printf ("ERROR!! The memory allocator should never return address zero\n"); exit(1); } 
  check_for_loop();
  //collects statistics
  frag_measure(1,size,BB->size);
  return addr;
}

/*
* Frees memory
*/
void memory_free(char *p){

  //This verification was done to prevent a case where the application asks to free the address "0"
  if(p==0) { return; }
  check_for_loop();
  
  //Creates a pointer to the beginning of the busy block
  busy_block_t busy=(busy_block_t)((int)p-sizeof(busy_block_s));
  int size_busy=busy->size;  
  free_block_t pFreeBlock=(free_block_t) busy;
  pFreeBlock->next=NULL;

  //We're creating a new free block, so, if there is no free block in the list, this will be the first one
  if(first_free==NULL){
   pFreeBlock->next=NULL;
   first_free=pFreeBlock;
  }
  //It's necessary to do some checkings to prevent things that the user may do
  else if (first_free!=pFreeBlock && size_busy>4){
        free_block_t current=first_free;
        free_block_t prior=NULL;
  
        //Searches the list of free blocks for the position where we must insert the new free block
        while(current!=NULL && ((int)pFreeBlock)>((int)current)){
                prior=current;
                current=current->next;
        }

	//Inserts the block before the first_free
	if(prior==NULL&&current!=NULL){
 		pFreeBlock->next=current;
		first_free=pFreeBlock;
	}
	//Inserts the block between two other blocks in the list
	else if(prior!=NULL&&current!=NULL){
		//Does a verification to prevent misuse
		if(pFreeBlock!=current)
 			pFreeBlock->next=current;		
		prior->next=pFreeBlock;
	}
	//Inserts the block in the last position
	else if(prior!=NULL&&current==NULL){
		prior->next=pFreeBlock;
		pFreeBlock->next=current;		
	}else{
		exit(1);
	}
  }
 
  pFreeBlock->size=size_busy;
  //Merges the contiguous blocks
  merge_free_blocks(); 
  print_free_info(p); 
  check_for_loop();
  frag_measure(0, size_busy-sizeof(busy_block_s), size_busy);
}

/*
* Merge coalescent free blocks
*/
void merge_free_blocks(void){
  //Searches the list of free blocks for two or more contiguous blocks, if they exist, merge them
  free_block_t bell=first_free;
  while(bell!=NULL){
	int position1=(int)bell+bell->size;
	int position2=((int)bell->next);
	if(position1==position2){
		bell->size=bell->size+bell->next->size;
		bell->next=bell->next->next;
	}else {
		bell=bell->next;
	}
  }
}

void print_info(void) {
  fprintf(stderr, "Memory : [%lu %lu] (%lu bytes)\n", (long unsigned int) 0, (long unsigned int) (memory+MEMORY_SIZE), (long unsigned int) (MEMORY_SIZE));
  fprintf(stderr, "Free block : %lu bytes; busy block : %lu bytes.\n", ULONG(sizeof(free_block_s)), ULONG(sizeof(busy_block_s))); 
}

void print_free_info(char *addr){
  if(addr)
    fprintf(stderr, "FREE  at : %lu \n", ULONG(addr - memory)); 
  else
    fprintf(stderr, "FREE  at : %lu \n", ULONG(0)); 
}

void print_alloc_info(char *addr, int size){
  if(addr){
    fprintf(stderr, "ALLOC at : %lu (%d byte(s))\n", 
	    ULONG(addr - memory), size);
  }
  else{
    fprintf(stderr, "Warning, system is out of memory\n"); 
  }
}

void print_free_blocks(void) {
  free_block_t current; 
  fprintf(stderr, "Begin of free block list :\n"); 
  for(current = first_free; current != NULL; current = current->next)
    fprintf(stderr, "Free block at address %lu, size %u\n", ULONG((char*)current - memory), current->size);
}

char *heap_base(void) {
  return memory;
}


void *malloc(size_t size){
  static int init_flag = 0; 
  if(!init_flag){
    init_flag = 1; 
    memory_init(); 
    print_info(); 
  }      
  return (void*)memory_alloc((size_t)size); 
}

void free(void *p){
  memory_free((char*)p); 
  print_free_blocks();
}

void *realloc(void *ptr, size_t size){
  if(ptr == NULL)
    return memory_alloc(size); 
  busy_block_t bb = ((busy_block_t)ptr) - 1; 
  printf("Reallocating %d bytes to %d\n", bb->size - (int)sizeof(busy_block_s), (int)size); 
  if(size <= bb->size - sizeof(busy_block_s))
    return ptr; 

  char *new = memory_alloc(size); 
  memcpy(new, (void*)(bb+1), bb->size - sizeof(busy_block_s) ); 
  memory_free((char*)(bb+1)); 
  return (void*)(new); 
}


#ifdef MAIN
int main(int argc, char **argv){
  /* The main can be changed, it is *not* involved in tests */
  memory_init();
  print_info(); 
  int i ; 
  for( i = 0; i < 10; i++){
    char *b = memory_alloc(rand()%8);
    memory_free(b); 
    print_free_blocks();
  }

  char * a = memory_alloc(15);
  a=realloc(a, 20); 
  memory_free(a);


  a = memory_alloc(10);
  memory_free(a);

  printf("%lu\n",(long unsigned int) (memory_alloc(9)));
  return EXIT_SUCCESS;
}
#endif 
