#include"Browser.h"
int main(int argc, char* argv[]){
	if (argc < 3)
		return 0;
	in = fopen(argv[1], "r");
	out = fopen(argv[2], "w");
	char buffer[200]="";
	Browser* browser = createBrowser();
	while(fscanf(in, "%s", buffer)!=EOF){

		if (strcmp(buffer, "set_band") == 0) {
			unsigned long size;
			fscanf(in, "%lu", &size);
			setBandwidth(browser, size);
		}
		else if (strcmp(buffer, "newtab") == 0) {
			newTab(browser);
		}
		else if (strcmp(buffer, "deltab") == 0) {
			delTab(browser);
		}
		else if (strcmp(buffer, "change_tab") == 0) {
			int indexTab;
			fscanf(in, "%d", &indexTab);
			changeTab(browser, indexTab);
		}
		else if (strcmp(buffer, "print_open_tabs") == 0) {
			printOpenTabs(browser);
		}
		else if (strcmp(buffer, "goto") == 0) {
			char link[25];
			fscanf(in, "%s", link);
			openPage(browser, link);
			wait(browser, 1);
		}
		else if (strcmp(buffer, "back") == 0) {
			back(browser);
		}
		else if (strcmp(buffer, "forward") == 0) {
			forward(browser);
		}
		else if (strcmp(buffer, "history") == 0) {
			printHistory(browser);
		}
		else if (strcmp(buffer, "del_history") == 0) {
			int nr;
			fscanf(in, "%d", &nr);
			delHistory(browser, nr);
		}
		else if (strcmp(buffer, "list_dl") == 0) {
			listDl(browser);
		}
		else if (strcmp(buffer, "downloads") == 0) {
			printDownloads(browser);
		}
		else if (strcmp(buffer, "download") == 0) {
			int nr;
			fscanf(in, "%d", &nr);
			Tab* tab = getTab(browser, browser->currentTab);
			if(tab->currentPage!=NULL)
				addToDownload(browser, &tab->currentPage->resources[nr]);
		}
		else if (strcmp(buffer, "wait") == 0) {
			int nr;
			fscanf(in, "%d", &nr);
			wait(browser, nr);
		}
	}

	destroyBrowser(browser);
	return 0;
}