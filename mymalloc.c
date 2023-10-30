#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

struct mm_block_def
{
    struct mm_block_def *next;
    struct mm_block_def *prev;
    int size;
    unsigned char *buffer;
    int isFree; // 1: free, 0: allocated
};

unsigned char malloc_size[MAX_MALLOC_SIZE];
static struct mm_block_def *head = NULL;

void initmyMalloc()
{
    head = (struct mm_block_def*)malloc_size; 
    if (head == NULL) {
        return;
    }
    head->prev = NULL;
    head->next = NULL;
    head->size = sizeof(malloc_size) - sizeof(struct mm_block_def);
    head->buffer = (unsigned char *)(head + sizeof(struct mm_block_def)); 
    head->isFree = 1;
}

void *mymalloc(int size)
{ 
    if (size <= 0) {
        return NULL;
    }

    struct mm_block_def *current = head;
    while(current != NULL && current->size <= size){
        current = current->next;
    }
    if(current == NULL){
        return NULL; 
    }

    //bos alani bol
    if(current->size >= size + sizeof(struct mm_block_def)){ 
        struct mm_block_def *newBlock = (struct mm_block_def *)(current->buffer + size);
        newBlock->buffer = current->buffer + size + sizeof(struct mm_block_def); 
		newBlock->size = current->size - size - sizeof(struct mm_block_def);
		newBlock->prev = current->prev;
		newBlock->next = current->next;
        newBlock->isFree = 1;
		
		current->size = size;
        current->isFree = 0;

		if(current == head){
			head = newBlock;
		}
		else if(current->prev!= NULL){
			current->prev->next = newBlock;
		}
		if(current->next != NULL){
			current->next->prev = newBlock;
		}

		current->next = newBlock;
	}

	return current;
}


void myfree(void *buffer)
{
    if (buffer == NULL) {
        return;
    }

    struct mm_block_def *block = (struct mm_block_def *)buffer;
    block->buffer = (unsigned char *)buffer + sizeof(struct mm_block_def);
    block->isFree = 1;

    struct mm_block_def *current = head;
    while (current != NULL && current->buffer <= block->buffer) {
        current = current->next;
    }

    //buffer degerlerinde sorun var
    //solda bos blok varsa mergele 
    if (current != NULL && current->prev != NULL && current->prev->buffer + current->prev->size + sizeof(struct mm_block_def) == block->buffer) { //current boş değil ve current head değil ve önceki boş blok merge edilebilir
        current->prev->size += block->size + sizeof(struct mm_block_def);
        current->prev->next = block->next;
    }

    //sagda bos blok varsa mergele
    if (current != NULL && block->buffer + block->size + sizeof(struct mm_block_def) == current->buffer) { //current boş değil ve sonraki boş blok merge edilebilir
        block->size += current->size + sizeof(struct mm_block_def);
        block->next = current->next;
    }

    block->next = head;
    block->prev = NULL;
    head = block;
}

void printmyMallocFreeList() {
    struct mm_block_def *current = head;
    while (current != NULL) {
        if (current->isFree) {
            printf("block: %p\tsize: %d\n", current, current->size);
            current = current->next;
        }   
    }
    printf("---------------\n");
}

int main()
{
    initmyMalloc();
	//head değerlerini printle
    printf("head:\t%x\n", head);
    printf("head->size:\t%d\n", head->size);
    printf("head->next:\t%x\n", head->next);
    printf("head->prev:\t%x\n", head->prev);
    printf("head->buffer:\t%x\n", head->buffer);
    printf("-------------------------------\n");

	char *a = mymalloc(200);
	printmyMallocFreeList();
    myfree(a);
    printmyMallocFreeList();
	char *c = mymalloc(50);
	mymalloc(80);
	char *b = mymalloc(20);
	printmyMallocFreeList();
	myfree(b);
    mymalloc(800);
    mymalloc(300);
    myfree(c);
	printmyMallocFreeList();

    return 0;
}