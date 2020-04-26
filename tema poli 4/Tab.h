#include"Page.h"
typedef struct {
	Page* currentPage;
	LinkedList* backStack;
	LinkedList* forwardStack;
}Tab;

Tab* createTab();
void destoryTab(Tab*);
void prevPage(Tab *tab);
void nextPage(Tab* tab);
void printResoucres(Tab* tab);