#ifndef _UMALLOC_H_
#define _UMALLOC_H_
#define malloc(X) umalloc(X, __FILE__, __LINE__)
#define free(Y) ufree(Y, __FILE__, __LINE__)


typedef struct _listnode{
    //Size in bytes
    size_t size;
    //if 0 free if 1 taken
    int free;
    struct _listnode *next;
    struct _listnode *prev;
} ListNode;


void *umalloc(size_t bytes, char *file, int line);
void ufree(void *ptr, char *file, int line);
// void printMemory();

#endif
