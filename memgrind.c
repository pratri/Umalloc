#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "umalloc.h"

int main(int argc, char *argv[]){

    //Consistency
    clock_t begin1 = clock();

    int* test1 = malloc(sizeof(int));
    *test1 = 4;
    free(test1);
    // printf("test1: %p\n", test1);
    int* test2 = malloc(sizeof(int));
    *test2 = 123;
    free(test2); 
    // printf("test2: %p\n", test2);

    clock_t end1 = clock();
    double avg_time1 = (double)(end1-begin1)/CLOCKS_PER_SEC;
    printf("%f\n", avg_time1);

    //Maximazation
    clock_t begin2 = clock();

    void* vtest;
    int size = 1;
    vtest = malloc(1);
    while(vtest!=NULL){
        free(vtest);
        size = size * 2;
        vtest = malloc(size);
    }
    size = size/2;
    vtest = malloc(size);
    if(vtest!=NULL){
        // printf("%d\n", size);
    }
    free(vtest);

    clock_t end2 = clock();
    double avg_time2 = (double)(end2-begin2)/CLOCKS_PER_SEC;
    printf("%f\n", avg_time2);

    //Basic Coalescense
    clock_t begin3 = clock();

    vtest = malloc(size/2);
    void* vtest1 = malloc(size/4);
    free(vtest);
    free(vtest1);
    void* vtest3 = malloc(size);
    free(vtest3);

    clock_t end3 = clock();
    double avg_time3 = (double)(end3-begin3)/CLOCKS_PER_SEC;
    printf("%f\n", avg_time3);

    //Saturation
    clock_t begin4 = clock();

    ListNode *container[1000];
    int counter = 0;
    int c = 0;
    for(int i=0; i<9216; i++){
        vtest = malloc(1024);
        if(vtest!=NULL){
            container[counter] = vtest;
            c++;
            counter++;
        }
    }
    while(vtest1!=NULL){
        vtest1 = malloc(1);
        if(vtest1!=NULL){
         
            container[counter] = vtest1;
            counter++;
        }
    }


    clock_t end4 = clock();
    double avg_time4 = (double)(end4-begin4)/CLOCKS_PER_SEC;
    printf("%f\n", avg_time4);
    //Time Overhead

    free(container[counter-1]);
    
    clock_t begin5 = clock();

    
    vtest1 = malloc(1);

    clock_t end5 = clock();
    double avg_time5 = (double)(end5-begin5)/CLOCKS_PER_SEC;
    printf("%f\n", avg_time5);

    //Intermediate Coallescense
    clock_t begin6 = clock();

    for(int i = 0; i<counter; i++){
        free(container[i]);
    }

    vtest = malloc(size);
    free(vtest);

    clock_t end6 = clock();
    double avg_time6 = (double)(end6-begin6)/CLOCKS_PER_SEC;
    printf("%f\n", avg_time6);

}
