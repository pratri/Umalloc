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
    struct listnode *prev;
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
        ptr->prev = NULL;
        init = 'b';
    }
    //No bytes requested
    if(bytes == 0){
        return NULL;
    }

    //Iterate through all the metadata to see where there is a free one of the right size
    while(ptr!=NULL && ptr->size!=0){
        //Check if the chunk is large enough to have both metadata and bytes and is free
        if(ptr->size >= (bytes + sizeof(ListNode)) && ptr->free == 0){
            printf("ALLOCATING\n");
            
            //Split the chunk into two chunks and set the first one to the size requested and not free and the rest would be a free chunk
            ListNode *new;

            //Create new metadata after the requested number of bytes
            new = (ListNode*)((char*)ptr + bytes + sizeof(ListNode));

            //Size is that of the chunk minus the requested bytes
            new->size = ptr->size - bytes;
            
            new->free = 0;
            
            //Sets ptr pointing to new metadata, which points to the metadata after that
            if(ptr->next!=NULL){
                ptr->next->prev = new;
            }
            new->next = ptr->next;
            new->prev = ptr;
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

    //Check if pointer is in mem array
    if((char*)ptr < mem || (char*)ptr > mem + MEM_LENGTH){
        printf("ERROR: NOT IN MEM ARRAY\n");
        return;
    }
    //Check if mem is uninitialized
    if(init == 'a'){
        printf("ERROR no malloc was called");
        return;
    }

    //Given a pointer that is a pointing to metadata free it has to be subtracted by sizeof metadata to find the actual metadata pointer
    ListNode *pointer = (ListNode*)ptr - sizeof(ListNode);
    pointer->free = 0;
    printf("PTR: %p\n", pointer);

    //check adjacent regions if free combine into a free block
    //Left side
    if(pointer->prev!=NULL){
        if(pointer->prev->free == 0){
            //Previous chunk is free, so combine their sizes
            pointer->prev->size = pointer->prev->size + pointer->size;
            //Change the prev and next nodes
            pointer->prev->next = pointer->next;
            pointer->next->prev = pointer->prev;
            //set pointer to the combined node
            pointer = pointer->prev;
        }
    }
    //Right Side
    if(pointer->next!=NULL){
        if(pointer->next->free == 0){
            //Combine the sizes
            pointer->size = pointer->size + pointer->next->size;
            //Change the prev and next nodes
            pointer->next = pointer->next->next;
            pointer->next->prev = pointer;
        }
    }


    return;
}

void printMemory(){
    ListNode *ptr;
    ptr = (ListNode*)mem;

    while(ptr!=NULL){
        if(ptr->free == 0){
            //Is free
            printf("Chunk at %p of size %lu is free\n", ptr, ptr->size);
        }else{
            //Is taken
            printf("Chunk at %p of size %lu in use\n", ptr, ptr->size);
        }
        ptr = ptr->next;
    }
}


int main(int argc, char* argv[]){
    printf("Size of listnode: %lu\n", sizeof(ListNode));
    
    printf("Starting Location2: %p\n", mem);

    void* test = umalloc(800000);
    void* test2 = umalloc(198000);
    void* test3 = umalloc(1950);
    void* test4 = umalloc(15);
    printf("test3: %p\n", test3);
    free(test3);
    free(test);
    free(test2);
    free(test4);
    printMemory();
    
    return 0;
}
