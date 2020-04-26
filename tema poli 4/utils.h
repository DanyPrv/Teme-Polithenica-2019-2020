#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#pragma warning(disable:4996)
#ifndef _DOWNLOAD_RESOURCE_
#define _DOWNLOAD_RESOURCE_

typedef struct{
    char name[100];
    unsigned long dimension;
    unsigned long currently_downloaded;
} Resource;

Resource* get_page_resources(const char *URL_Name, int *n);
unsigned long getRemainingDownload(Resource* res);
#endif /* _DOWNLOAD_RESOURCE_ */


//simple linked list
typedef struct node {
	void* data;
	struct node* next;
} node;

typedef struct {
	node* head;
	node* tail;
	int size;
} LinkedList;



/* functions */

#ifndef _FUNCTIONS_FOR_STACK_
#define _FUNCTIONS_FOR_STACK_

node* popStack(LinkedList* stack);
void pushStack(LinkedList* stack, node *elem);

#endif
//output FILE POINTER
 FILE* in;
 FILE* out;