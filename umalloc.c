#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "umalloc.h"

//Memory length of 10 MB
#define MEM_LENGTH 1000000

static char mem[MEM_LENGTH];
char init = 'a';


// void printMemory(){
//     ListNode *ptr;
//     ptr = (ListNode*)mem;
    
//     while(ptr!=NULL){
//         if(ptr->free == 0){
//             //Is free
//             printf("Chunk at %p of size %lu is free\n", ptr, ptr->size);
//         }else{
//             //Is taken
//             printf("Chunk at %p of size %lu in use\n", ptr, ptr->size);
//         }
//         ptr = ptr->next;
//     }
//     printf("\n");
// }

void *umalloc(size_t bytes, char *file, int line){


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
        printf("Error calling malloc with 0 bytes in file %s, on line %d\n", file, line);
        return NULL;
    }

    //Iterate through all the metadata to see where there is a free one of the right size
    while(ptr!=NULL && ptr->size!=0){
        //Check if the chunk is large enough to have both metadata and bytes and is free
        if(ptr->size >= (bytes + sizeof(ListNode)) && ptr->free == 0){
            
            //Split the chunk into two chunks and set the first one to the size requested and not free and the rest would be a free chunk
            ListNode *new;
            

            //Create new metadata after the requested number of bytes
            new = (ListNode*)((char*)ptr + bytes + sizeof(ListNode));

            //Size is that of the chunk minus the requested bytes
            new->size = ptr->size - bytes - sizeof(ListNode);
            
            new->free = 0;
            
            //Sets ptr pointing to new metadata, which points to the metadata after that
            if(ptr->next!=NULL){
                ptr->next->prev = new;
            }
            new->next = ptr->next;
            new->prev = ptr;
            ptr->next = new;
            ptr->free = 1;
            // printf("SIZE: %zu, remaining, %zu\n", ptr->size, new->size);

            ptr->size = bytes + sizeof(ListNode);
            
            //Return the chunk after the metadata 
            // printf("ptr: %p\n", ptr);
            return (ptr + sizeof(ListNode));
        }
        ptr = ptr->next;
    }
    printf("Not enough space avialable in memory for this malloc call in file %s on line %d\n", file, line);
    return NULL;
}



void ufree(void* ptr, char *file, int line){

    if(ptr == NULL){
        printf("Error pointer is NULL in free call in file %s, line %d\n", file, line);
        return;
    }

    //Check if mem is uninitialized
    if(init == 'a'){
        printf("ERROR memory is unitialized in free call in file %s, line %d", file, line);
        return;
    }

    //Given a pointer that is a pointing to metadata free it has to be subtracted by sizeof metadata to find the actual metadata pointer
    ListNode *pointer = (ListNode*)ptr - sizeof(ListNode);
    pointer->free = 0;

    ListNode *main_ptr = (ListNode*)mem;
    //Check if given pointer was called by malloc and is at the start of a chunk
    int checker = 0;
    while(main_ptr!=NULL){
        if(pointer == main_ptr){
            //Is in chunk
            checker = 1;
            break;
        }
        main_ptr = main_ptr->next;
    }

    if(checker == 0){
        printf("ERROR: given pointer is not at the start of a chunk. File %s. Line: %d\n", file, line);
    }

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
            // printMemory();
        }
    }
    //Right Side
    if(pointer->next!=NULL){
        
        if(pointer->next->free == 0){
            
            //Combine the sizes
            pointer->size = pointer->size + pointer->next->size;
            //Change the prev and next nodes
            pointer->next = pointer->next->next;

            //Checks to see if the next node is NULL, if it's not then change it's prev
            if(pointer->next!=NULL){
                pointer->next->prev = pointer;
            }
            
        }
    }
    

    return;
}


