#include "utils.h"

Resource* get_page_resources(const char *URL_Name, int *n){
	unsigned int i, j, name_len, url_name_len = strlen(URL_Name);

	unsigned int resource_nr = 0;
	uint32_t size = 0;

	if (URL_Name == NULL) return NULL;

	for(i = 0; i < url_name_len; i++){
		resource_nr += URL_Name[i];
	}

	resource_nr %= 13;
	Resource *result = (Resource *)calloc(resource_nr, sizeof(Resource));
	if (!result) {
		return NULL;
	}
	*n = resource_nr;
	for(i = 0; i < resource_nr; i++){
		sprintf(result[i].name, "%s-(%d)", URL_Name, i);
		size = 0;
		name_len = strlen(result[i].name);

		for(j = 0; j < name_len; j++){
			size += result[i].name[j];
		}
		/* Some randomness */
		size ^= size << 3;
		size += size >> 5;
		size ^= size << 4;
		size += size >> 17;
		size ^= size << 25;
		size += size >> 6;

		/* 100MB Maximum size */
		result[i].dimension = size % 104857601;
		result[i].currently_downloaded = 0;
	}

	return result;
}

node* popStack(LinkedList* stack) {
	if (!stack->head)
		return NULL;
	node* aux=stack->head;
	stack->head = stack->head->next;
	if (!stack->head ) {
		//daca in stack mai era doar un element
		stack->tail = NULL;
	}
	stack->size--;
	aux->next = NULL;
	return aux;
}

void pushStack(LinkedList* stack, node *elem) {

	if (stack->size == 0) {
		stack->head = stack->tail = (node *)elem;
	}
	else {
		((node *)elem)->next = stack->head;
		stack->head = elem;
	}
	stack->size++;
}

unsigned long getRemainingDownload(Resource* res) {
	return res->dimension - res->currently_downloaded;
}