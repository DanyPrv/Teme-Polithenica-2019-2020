#include <stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>
#include"util/task_helper.h"
//functie ce returneaza 1 daca caracterul ch primit ca
//paramteru este litera si 0 in caz contrar
int isLetter(char ch)
{
    return (ch>='a'&&ch<='z' || ch>='A'&&ch<='Z');

}
//functie ce returneaza 1 daca caracterul ch primit ca
//paramteru este litera mica si 0 in caz contrar
int isLower(char ch)
{
    return(ch>='a'&&ch<='z');
}
//functie ce primeste ca parametru poezia si pune la inceputul fiecarui
//vers litera mare dupa care pe toate celelalte le face litere mici
void uppercase(char *poezie)
{
    int i=0;
    //facem prima litera a primului vers litera mare
    poezie[i]=toupper(poezie[i]);

    i++;
    //parcurgem intreaga poezie
    while(i<strlen(poezie))
    {
        //daca gasim o litera mare o transformam in litera mica
        if(isLetter(poezie[i])&&!isLower(poezie[i]))
            poezie[i]=tolower(poezie[i]);
        //daca o litera este la inceputul unui rand o facem litera mare
        if(poezie[i-1]=='\n')
            poezie[i]=toupper(poezie[i]);
        i++;
    }
}
//functie ce primeste ca parametru poezia si elimina semnele
// de punctutie si spatiile suplimentare
void trimming(char *poezie)
{
    int i=0, j=0;
    //whiteSpace=1 daca, caracterul anterior este un spatiu, 0 in caz contrar
    int whiteSpace=0;
    //parcurgem poezia si scriem la pozitia j caracterul de pe pozitia i
    while(i<strlen(poezie))
    {
        //daca intalnim o litera, un new line sau caracterul '-' intre doua litere(ex: s-au dus)
        //textul ramane neschimbat
        if(isLetter(poezie[i])||poezie[i]=='\n'|| ( i>0&&isLetter(poezie[i-1]) && isLetter(poezie[i+1]) ))
        {
            whiteSpace=0;
            poezie[j]=poezie[i];
            j++;
        }
        else
            if(whiteSpace==0&&poezie[i]==' ')
            {
                //in caz contrar, verificam daca este primul spatiu aparut caz in care trebuie adaugat
                //si whiteSpace devine 1, in caz contrar j(pozitia unde trebuie scris urmatorul caracter) nu se incrementeaza
                whiteSpace=1;
                poezie[j]=poezie[i];
                j++;
            }
        i++;
    }
    //adaugam la finalul poeziei cracterul null
    poezie[j]='\0';
}

void rhimy()
{
	return;
}

void friendly(char* poezie)
{
    //retinem in str poezia primita ca parametru intrucat functia strtok modifica sirul in care cauta
    char *str=malloc(1002*sizeof(char));
    //retinem in aux tot ce urmeaza dupa cuvantul ce trebuie inlocuit cu diminutivul sau
    char *aux=malloc(1002*sizeof(char));
    //pointer catre diminutivul din zona de memorie unde este salvata lista de diminutive
    char *friendlyWord=NULL;
    //copiem poezia in vectorul str
    strcpy(str,poezie);
    //cautam primul cuvant al poeziei
    char* ptr=strtok(str,", ?\n");
    //cat timp gasim cuvinte in poezie:
    while(ptr!=NULL)
    {
        //se cauta diminutiv pentru cuvantul actual
        friendlyWord=NULL;
        get_friendly_word(ptr,&friendlyWord);

        if(friendlyWord!=NULL)
        {
            //daca s-a gasit se seteaza pointerul ptr2 pe pozitia de la care incepe cuvantul
            //ce trebuie inlocuit cu diminutivul sau
            char *ptr2=strstr(poezie,ptr);
            //copiem in vectorul aux tot ce urmeaza dupa cuvant
            strcpy(aux,ptr2+strlen(ptr));
            //eliminam din sir tot ce exista dupa cuvant inclusiv acesta
            ptr2[0]='\0';
            //lipim la sir noul cuvant(diminutivul) si ceea ce exista dupa cuvant(aux)
            strcat(ptr2,friendlyWord);
            strcat(ptr2,aux);
	    }
	    //se trece la urmatorul cuvant
        ptr=strtok(NULL,", ?.\r\n");
    }
    free(str);
    free(aux);
}
//functie ce primeste ca parametru o poezia si o afiseaza
void print(char* poezie)
{
    printf("%s",poezie);
}
//functie ce primeste ca parametru poezia si probabilitatea
void makeItSilly(char *poezie,float p)
{
    int i=0;
    //se parcurge fiecare litera
    while(i<strlen(poezie))
    {
        if(isLetter(poezie[i]))
        {
            //la intalnirea unei litere se calculeaza probabilitatea iar daca
            //este mai mica decat probabilitatea data sa inverseaza cu litera mare (sau mica dupa caz)
            float prob = rand() % 100 / 99.0;
            if(prob<p)
                if(isLower(poezie[i]))
                    poezie[i]=toupper(poezie[i]);
                else
                    poezie[i]=tolower(poezie[i]);
        }
        i++;
    }
}

int main(void)
{
    srand(42);
    char *poezie;
    poezie=malloc(10001*sizeof(char));
    char *str;
    str=malloc(1001*sizeof(char));
    scanf("%s",str);
    while(strcmp(str,"quit")!=0)
    {
        if(strcmp(str,"uppercase")==0)
        {
            uppercase(poezie);
        }
        else if(strcmp(str,"load")==0)
        {
            char *path,*aux;
    	    path=malloc(1001*sizeof(char));
            aux=malloc(1001*sizeof(char));
            scanf("%s",path);
            load_file(path,poezie);
            strcat(poezie,"\n");
            strcat(poezie,"\n");
            free(path);
            free(aux);
        }
        else if(strcmp(str,"print")==0)
        {
            print(poezie);
        }
        else if(strcmp(str,"trimming")==0)
        {
            trimming(poezie);
        }
        else if(strcmp(str,"make_it_silly")==0)
        {
            char *temp;
            temp=malloc(101*sizeof(char));
            scanf("%s",temp);
            makeItSilly(poezie,atof(temp));
            free(temp);
        }
        else if(strcmp(str,"make_it_friendlier")==0)
        {
            friendly(poezie);
        }


        scanf("%s",str);
    }
    free(ch);
    free(str);
	return 0;
}
