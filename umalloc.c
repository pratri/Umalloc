#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Memory length of 10 MB
#define MEM_LENGTH 1000000

static char mem[MEM_LENGTH];
char init = 'a';

typedef struct listnode{
    //Size in bytes
    size_t size;
    //if 0 free if 1 taken
    int free;
    struct listnode *next;
} ListNode;

void *umalloc(size_t bytes){


    ListNode *ptr;
    //Pointer to the start of the memory array
    //Should be the first metadata if uninitialized
    ptr = (ListNode*)mem;

    //check if memory is initialized
    if(init == 'a'){
        //Set the first metadata
        ptr->size = MEM_LENGTH;
        ptr->free = 0;
        ptr->next = NULL;
        init = 'b';
    }
    //No bytes requested
    if(bytes == 0){
        return NULL;
    }

    //Iterate through all the metadata to see where there is a free one of the right size
    while(ptr!=NULL && ptr->size!=0){
        //Check if the chunk is large enough and is free
        if(ptr->size >= (bytes + sizeof(ListNode)) && ptr->free == 0){
            printf("ALLOCATING\n");
            
            //Split the chunk into two chunks and set the first one to the size requested and not free and the rest would be a free chunk
            ListNode *new;

            //Create new metadata after the requested number of bytes
            new = (ListNode*)((char*)ptr+bytes+sizeof(ListNode));

            //Size is that of the chunk minus the requested bytes minus metadata size of new
            new->size = ptr->size - bytes - sizeof(ListNode);
            
            new->free = 0;
            
            //Sets ptr pointing to new metadata, which points to the metadata after that
            new->next = ptr->next;
            ptr->next = new;
            ptr->free = 1;
            ptr->size = bytes;
            
            //Return the chunk after the metadata 
            return (ptr + sizeof(ListNode));
        }
        ptr = ptr->next;
    }
    return NULL;
}



void free(void* ptr){

    //Given a pointer that is a pointing to metadata
    p

}

void printMemory(){
    ListNode *ptr;
    ptr = (ListNode*)mem;

    while(ptr!=NULL){
        printf("Faulr1?\n");
        if(ptr->free == 0){
            //Is free
            printf("Faulr2?\n");
            printf("Chunk at %p of size %lu is free\n", ptr, ptr->size);
        }else{
            //Is taken
            printf("Faulr3?\n");
            printf("Chunk at %p of size %lu in use\n", ptr, ptr->size);
        }
        printf("Faulr4?\n");
        ptr = ptr->next;
    }
    printf("Faulr5?\n");
}


int main(int argc, char* argv[]){
    printf("Size of listnode: %lu\n", sizeof(ListNode));
    
    printf("Starting Location2: %p\n", mem);

    int* test = umalloc(sizeof(int));
    *test = 4;
    printf("Testing %d\n", *test);
    printMemory();
    
    return 0;
}
