#ifndef MYMALLOC_H
#define MYMALLOC_H
#define MAX_MALLOC_SIZE (1024*1024*16)
void initmyMalloc();
void *mymalloc(int size); 
void myfree(void *buffer);
void printmyMallocFreeList();
#endif