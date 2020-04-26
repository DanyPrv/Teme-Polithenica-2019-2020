#include"Tab.h"
Tab* createTab() {
	Tab* tab =(Tab*) calloc(1, sizeof(Tab));
	if (!tab) return NULL;
	// tab->currentPage = createPage(NULL);
	tab->backStack =(LinkedList*) calloc(1, sizeof(LinkedList));
	if (!tab->backStack) {
		free(tab);
		return NULL;
	}
	tab->forwardStack =(LinkedList*) calloc(1, sizeof(LinkedList));
	if (!tab->forwardStack) {
		free(tab->backStack); free(tab);
		return NULL;
	}
	return tab;
}

void destoryTab(Tab* tab) {
	distrugeStiva(tab->backStack);
	free(tab->backStack);
	distrugeStiva(tab->forwardStack);
	free(tab->forwardStack);
	if(tab->currentPage)
		destroyPage(tab->currentPage);
	free(tab);
}

void prevPage(Tab *tab) {
	if (!tab->backStack->size) {
		fprintf(out,"can’t go back, no pages in stack\n");
		return;
	}
	node *aux = popStack(tab->backStack);
	Page *page = tab->currentPage;
	tab->currentPage = (Page *)aux->data;
	aux->data = page;
	pushStack(tab->forwardStack, aux);
}

void nextPage(Tab* tab) {
	if (!tab->forwardStack->size) {
		fprintf(out,"can’t go forward,no pages in stack\n");
		return;
	}
	node* aux = popStack(tab->forwardStack);
	Page* page = tab->currentPage;
	tab->currentPage = (Page*)aux->data;
	aux->data = page;
	pushStack(tab->backStack, aux);
}

void printResoucres(Tab* tab) {
	int i;
	if (tab->currentPage == NULL)
		return;
	for (i = 0; i < tab->currentPage->nrRes; i++) {
		Resource res = tab->currentPage->resources[i];
		fprintf(out,"[%d - \"%s\" : %lu]\n", i, res.name, res.dimension);
	}
}