#include"utils.h"
typedef struct {
	char url[25];
	int nrRes;
	Resource* resources;
}Page;

Page* createPage(char*);
void destroyPage(Page*);
void distrugeStiva(LinkedList* stack);