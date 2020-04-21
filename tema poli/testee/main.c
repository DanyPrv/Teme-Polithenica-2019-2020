#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<ctype.h>
char frWord[]="pisoias";
void get_friendly_word(char*word,char**friendlyWord)
{
    int i;
    char cuv[25];
    strcpy(cuv,word);
    for(i=0;i<strlen(cuv);i++)
        cuv[i]=tolower(cuv[i]);
    if(strcmp(cuv,"pisoi")==0)
        *friendlyWord=frWord;
    else
        *friendlyWord=NULL;
}
void friendly(char* s)
{
    char *str=malloc(1002*sizeof(char));
    char *aux=malloc(1002*sizeof(char));
    char *friendlyWord=NULL;
    strcpy(str,s);
    char* ptr=strtok(str,", ?\n");
    while(ptr!=NULL)
    {
        //printf("%s\n",ptr);
	friendlyWord=NULL;
        get_friendly_word(ptr,&friendlyWord);
        if(friendlyWord!=NULL)
        {
            char *ptr2=strstr(s,ptr);
	    strcpy(aux,ptr2+strlen(ptr));
	    ptr2[0]='\0';
	    strcat(ptr2,friendlyWord);
	    strcat(ptr2,aux);
	    printf("Text:\n%s\n",s);
            //strncpy(ptr2,friendlyWord,sizeof(friendlyWord));
        }
        ptr=strtok(NULL,", ?.\r\n");
    }
    free(str);
    free(aux);
}
int main()
{
    char ch[1001],aux[101];
    FILE *f = fopen("test.in","r");
    while(fgets(aux,101,f)){
        strcat(ch,aux);
    }
    friendly(ch);
    return 0;

}
