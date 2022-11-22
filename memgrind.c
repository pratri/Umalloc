#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "umalloc.h"

int main(int argc, char *argv[]){

    //Consistency

   int* test1 = malloc(sizeof(int));
   *test1 = 4;
   free(test1);
   printf("test1: %p\n", test1);
   int* test2 = malloc(sizeof(int));
   *test2 = 123;
   free(test2); 
   printf("test2: %p\n", test2);

    //Maximazation
    void* test;
    int size = 1;
    test = malloc(1);
    while(test!=NULL){
        free(test);
        size = size * 2;
        test = malloc(size);
    }
    size = size/2;
    test = malloc(size);
    if(test!=NULL){
        printf("%d\n", size);
    }
    free(test);

    //Basic Coalescense

    vtest = malloc(size/2);
    void* vtest1 = malloc(size/4);
    free(vtest);
    free(vtest1);
    void* vtest3 = malloc(size);
    free(vtest3);

    //Saturation

    //Time Overhead

    //Intermediate Coallescense



}
