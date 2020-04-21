#include <stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>
#include"util/task_helper.h"
int isLetter(char ch)
{
    return (ch>='a'&&ch<='z' || ch>='A'&&ch<='Z');

}
int isLower(char ch)
{
    return(ch>='a'&&ch<='z');
}
void uppercase(char *s)
{
    int i=0;
    s[i]=toupper(s[i]);
    i++;
    while(i<strlen(s))
    {
        if(isLetter(s[i])&&!isLower(s[i]))
            s[i]=tolower(s[i]);
        if(s[i-1]=='\n')
            s[i]=toupper(s[i]);
        i++;
    }
}

void trimming(char *s)
{
    int i=0, j=0, whiteSpace=0;
    while(i<strlen(s))
    {
        if(isLetter(s[i])||s[i]=='\n')
        {
            whiteSpace=0;
            s[j]=s[i];
            j++;
        }
        else
            if(whiteSpace==0&&s[i]==' ')
            {
                whiteSpace=1;
                s[j]=s[i];
                j++;
            }
        i++;
    }
    s[j]='\0';
	return;
}

void rhimy()
{
	return;
}

void friendly(char* s)
{
    char *str=malloc(1001*sizeof(char));
    char *friendlyWord=NULL;
    strcpy(str,s);
	char* ptr=strtok(str,", ?\n");
	while(ptr!=NULL)
    {

        //printf("%s\n",ptr);
        get_friendly_word(ptr,&friendlyWord);
        if(friendlyWord!=NULL)
        {
            char ptr2=strstr(s,&ptr);
            strncpy(pch,friendlyWord,sizeof(friendlyWord));
        }
        ptr=strtok(NULL,", ?.\r\n");
    }
    printf("textAfter:\n%s\n",str);
    free(str);
}

void print(char* ch)
{
    printf("%s",ch);
}

void makeItSilly(char *ch,float p)
{
    int i=0;

    while(i<strlen(ch))
    {
        if(isLetter(ch[i]))
        {
            float prob = rand() % 100 / 99.0;
            if(prob<p)
                if(isLower(ch[i]))
                    ch[i]=toupper(ch[i]);
                else
                    ch[i]=tolower(ch[i]);
        }
        i++;
    }
}

int main(void)
{
    srand(42);
    char ch[10001];
    char str[1001];
    scanf("%s",&str);
    while(strcmp(str,"quit")!=0)
    {
        if(strcmp(str,"uppercase")==0)
        {
            uppercase(&ch);
        }
        else if(strcmp(str,"load")==0)
        {
            char path[1001],aux[1001];
            scanf("%s",&path);
            FILE*f=fopen(path,"r");
            ch[0]='\0';
            while(fgets(aux,1001,f)){
                strcat(ch,aux);
            }
            fclose(f);
            //load_file(path,&ch);
        }
        else if(strcmp(str,"print")==0)
        {
            print(ch);
        }
        else if(strcmp(str,"trimming")==0)
        {
            trimming(&ch);
        }
        else if(strcmp(str,"make_it_silly")==0)
        {
            char temp[100];
            scanf("%s",&temp);
            makeItSilly(&ch,atof(temp));
        }
        else if(strcmp(str,"make_it_friendlier")==0)
        {
            friendly(ch);
        }
        scanf("%s",str);
    }
	return 0;
}
