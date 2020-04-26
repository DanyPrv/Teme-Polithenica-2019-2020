#include"Tab.h"
typedef struct {
	LinkedList* tabs;
	LinkedList* globalHistory;
	LinkedList* downloads;
	unsigned long bandwidth;
	int currentTab;
} Browser;

Browser* createBrowser();
void destroyBrowser(Browser*);

void setBandwidth(Browser* b, unsigned long size);
void newTab(Browser* b);
void delTab(Browser* b);
Tab* getTab(Browser* b, int index);
void changeTab(Browser* b, int indexTab);
void printOpenTabs(Browser* b);
void openPage(Browser* b, char* url);
void back(Browser *b);
void forward(Browser* b);
void addToHistory(Browser* b, char* adr);
void printHistory(Browser* b);
void delHistory(Browser* b, int nr);
void listDl(Browser* b);
void printDownloads(Browser* b);
void addToDownload(Browser* b, Resource* res);
void wait(Browser* b, int seconds);
void destroyGlobalHistory(Browser* b);