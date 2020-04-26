#include "Page.h"

Page* createPage(char* url) {
	if(url == NULL) return NULL;
	Page* page = calloc(1, sizeof(Page));
	if (page == NULL) return NULL;
	strcpy(page->url, url);
	page->resources=get_page_resources(url, &page->nrRes);
	return page;
}
void destroyPage(Page* page) {
	if (page->resources) {free(page->resources);}
	free(page);
}

void distrugeStiva(LinkedList* stack) {
	node* aux = stack->head, *anterior =NULL ;
	while (aux != NULL) {
		anterior = aux;
		aux = aux->next;
		destroyPage(anterior->data);
		free(anterior);
	}
	stack->head = stack->tail = NULL;
	stack->size = 0;
}