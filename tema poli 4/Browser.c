#include"Browser.h"


Browser* createBrowser() {
	Browser* b = calloc(1, sizeof(Browser));
	if (!b) {
		return NULL;
	}
	//open only one tab and alocate it
	b->tabs = calloc(1, sizeof(LinkedList));
	if (!b->tabs) {
		free(b);
		return NULL;
	}
	b->tabs->head = calloc(1, sizeof(node));
	if (!b->tabs->head) {
		free(b->tabs);
		free(b);
		return NULL;
	}
	b->tabs->tail = b->tabs->head;
	b->tabs->head->data = createTab();
	if (!b->tabs->head->data || !b->tabs->tail->data) {
		free(b->tabs->head);
		free(b->tabs->tail);
		free(b->tabs);
		free(b);
		return NULL;
	}
	b->tabs->tail->data = b->tabs->head->data;
	b->tabs->size = 1;
	//initialize global history
	b->globalHistory = calloc(1, sizeof(LinkedList));
	if (!b->globalHistory) {
		free(b->tabs->head->data);
		free(b->tabs->tail->data);
		free(b->tabs->head);
		free(b->tabs->tail);
		free(b->tabs);
		free(b);
		return NULL;
	}
	//initialize downloads
	b->downloads = calloc(1, sizeof(LinkedList));
	if (!b->downloads) {
		free(b->globalHistory);
		free(b->tabs->head->data);
		free(b->tabs->tail->data);
		free(b->tabs->head);
		free(b->tabs->tail);
		free(b->tabs);
		free(b);
		return NULL;
	}
	b->bandwidth = 1024;//setam bandwidth ul impleicit la 1KBps
	return b;
}

void destroyBrowser(Browser* browser) {
	//delete global history
	destroyGlobalHistory(browser);
	free(browser->globalHistory);

	//delete all tabs
	node* aux = browser->tabs->head;
	while (aux != NULL) {
		node* temp = aux;
		aux = aux->next;
		destoryTab(temp->data);
		free(temp);
	}
	free(browser->tabs);


	//	delete downloads
	aux = browser->downloads->head;
	while (aux != NULL) {
		node* temp = aux;
		aux = aux->next;
		free(temp->data);
		free(temp);
	}
	free(browser->downloads);


	free(browser);
}


void setBandwidth(Browser* b, unsigned long size) {
	b->bandwidth = size;
}

void newTab(Browser* b) {
	node* newTab = calloc(1, sizeof(node));
	newTab->data = createTab();
	b->tabs->tail->next = newTab;
	b->tabs->tail = newTab;
	b->currentTab = b->tabs->size;
	b->tabs->size++;
}


void delTab(Browser* b) {
	node* aux = b->tabs->tail;
	node* anterior = b->tabs->head;
	while (anterior->next != aux) {
		anterior = anterior->next;
	}
	b->tabs->tail = anterior;
	anterior->next = NULL;
	b->tabs->size--;
	if (b->currentTab == b->tabs->size)
		b->currentTab--;
	destoryTab(aux->data);
	free(aux);
}

void changeTab(Browser* b, int indexTab) {
	b->currentTab = indexTab;
}

void printOpenTabs(Browser* b) {
	node* i = b->tabs->head;
	int nr = 0;
	for (; i != NULL; i = i->next,nr++) {
		fprintf(out,"(%d: ",nr);
		if (!((Tab*)i->data)->currentPage->url)
			fprintf(out,"empty");
		else
			fprintf(out,"%s", ((Tab*)i->data)->currentPage->url);
		fprintf(out,")\n");
	}
}

Tab* getTab(Browser* b, int index){
	node* i = b->tabs->head;
	int contor = 0;
	while (contor < index) {
		i = i->next;
		contor++;
	}
	return ((Tab*)i->data);
}

void openPage(Browser* b, char* url) { //goto
	Tab* actTab = getTab(b, b->currentTab);
	//adauga tabul actual la backstack

	Page* page = createPage(url);
	addToHistory(b, url);
	if (!actTab->currentPage) {
		// daca tabul este gol
		actTab->currentPage = page;
		return;
	}

	node* aux = calloc(1, sizeof(node));
	aux->data = actTab->currentPage;
	//daca avem bacstack nevid
	if (actTab->backStack->size != 0) {
		aux->next = actTab->backStack->head;
		actTab->backStack->head = aux;
	}
	else {
		actTab->backStack->head= actTab->backStack->tail= aux;
	}
	actTab->backStack->size++;
	//cream pagina noua si o punem in currentPage
	actTab->currentPage = page;
	//stergem stiva forward
	distrugeStiva(actTab->forwardStack);
}

void back(Browser *b) {
	Tab *actTab = getTab(b, b->currentTab);
	prevPage(actTab);

}

void forward(Browser* b) {
	Tab* actTab = getTab(b, b->currentTab);
	nextPage(actTab);

}


void addToHistory(Browser* b, char* adr) {
	node* aux = calloc(1, sizeof(node));
	if (aux == NULL)
		return;
	aux->data = calloc(25, sizeof(char));
	strcpy(aux->data, adr);
	if (b->globalHistory->head== NULL) {
		//daca lista e vida initializam headul si taill cu prima adresa adaugata
		b->globalHistory->head = b->globalHistory->tail = aux;
	}
	else{
		//altfel adaugam la final
		b->globalHistory->tail->next = aux;
		b->globalHistory->tail = aux;
	}
	b->globalHistory->size++;
}

void printHistory(Browser* b) {
	node* aux = b->globalHistory->head;
	while (aux!= NULL) {
		fprintf(out,"%s\n", (char *)aux->data);
		aux = aux->next;
	}
}


void destroyGlobalHistory(Browser* b) {
	node* aux = b->globalHistory->head;
	while (aux != NULL) {
		node* temp = aux;
		aux = aux->next;
		free(temp->data);
		free(temp);
	}
	b->globalHistory->head = b->globalHistory->tail = NULL;
	b->globalHistory->size = 0;
}

void delHistory(Browser* b, int nr) {
	if (!nr||b->globalHistory->size==nr) {
		//daca nr este 0 stergem tot istoricul
		destroyGlobalHistory(b);
		return;
	}
	int nract = 1;
	node* aux = b->globalHistory->head;
	for (; nract <= nr && aux !=NULL ;nract++) {
		node* temp = aux;
		aux = aux->next;
		free(temp->data);
		free(temp);
	}
	b->globalHistory->head = aux;
	b->globalHistory->size = b->globalHistory->size - nr;
}

void listDl(Browser* b) {
	Tab* actTab = getTab(b, b->currentTab);
	printResoucres(actTab);
}

//downloads


void printDownloads(Browser* b) {
	node* aux = b->downloads->head;
	while (aux != NULL) {
		Resource* res = aux->data;
		fprintf(out,"[\"%s\" : ", res->name);
		if (res->dimension == res->currently_downloaded)
			fprintf(out,"completed]\n");
		else
			fprintf(out,"%lu/%lu]\n", getRemainingDownload(res), res->dimension);
		aux = aux->next;
	}
}

void addToDownload(Browser* b, Resource* res) {
	//cream un nod
	node* aux = calloc(1, sizeof(node));
	if (aux == NULL)
		return;
	//alocam memorie si copiem in aux->data resursa de descarcat
	aux->data = calloc(1, sizeof(Resource));
	strcpy(((Resource*)aux->data)->name, res->name);
	((Resource*)aux->data)->dimension = res->dimension;
	((Resource*)aux->data)->currently_downloaded = res->currently_downloaded;

	if (!b->downloads->size) {
		//daca lista de descarcari este goala adagam prima resursa de descarcat
		b->downloads->head = b->downloads->tail = aux;
		b->downloads->size++;
		return;
	}
	//adaugam in lista in crescator dupa dimensiunea resursei noi
	//si cat mai este de descarcat din fiecare resursa din coada
	
	// verificam daca resursa de adaugat este descarcata complet caz in care il adaugam la final
	if (!getRemainingDownload(aux->data)) {
		//verificam daca trebuie adaugat primul
		
		//adaugam la final
		b->downloads->tail->next = aux;
		b->downloads->tail = aux;

		b->downloads->size++;
		return;
	}




	//verificam daca trebuie adaugata prima in  lista
	if (getRemainingDownload((Resource*)b->downloads->head->data) >=((Resource*)aux->data)->dimension) {
		aux->next = b->downloads->head;
		b->downloads->head = aux;
		b->downloads->size++;
		return;
	}
	node* prev =NULL;
	node* iterator = b->downloads->head;
	//parcurgem pentru adaugate in lista

	while (iterator!=NULL&& getRemainingDownload(iterator->data)!=0&&getRemainingDownload(iterator->data)<getRemainingDownload((Resource*)aux->data))
	{
		prev = iterator;
		iterator = iterator->next;
	}
	prev->next = aux;
	aux->next = iterator;
	//daca a fost adaugat la final actualizam downloads->tail
	if (iterator == NULL)
		b->downloads->tail = aux;
	b->downloads->size++;
}

void wait(Browser* b, int seconds) {
	unsigned long remainingBytes = seconds * b->bandwidth;
	node* prev = NULL;
	node* aux = b->downloads->head;
	while (remainingBytes > 0 && aux != NULL ) {
		if (getRemainingDownload(aux->data) <= remainingBytes&& getRemainingDownload(aux->data)) {
			//s-a terminat de downloadat o resursa
			node* temp = aux;
			aux = aux->next;
			remainingBytes -= getRemainingDownload(temp->data);
			((Resource*)temp->data)->currently_downloaded = ((Resource*)temp->data)->dimension;
			//il scoatem din lista
			if (prev == NULL) {
			//verificam daca resursa este prima din coada
				b->downloads->head = b->downloads->head->next;
				b->downloads->size--;
			}
			else {
				prev->next = temp->next;
			}
			temp->next = NULL;
			//il adaugam in coada la final
			addToDownload(b, temp->data);
			free(temp->data);
			free(temp);
		}
		else {
			((Resource*)aux->data)->currently_downloaded += remainingBytes;
			remainingBytes = 0;
		}
		
	}
}