#include <stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>
#include "util/task_helper.h"
int isLetter(char ch)
{
    return (ch>='a'&&ch<='z' || ch>='A'&&ch<='Z');

}
int isLower(char ch)
{
    return(ch>='a'&&ch<='z');
}
void uppercase(char **s)
{
    int i=0;
    (*s)[i]=toupper((*s)[i]);
    i++;
    while(i<strlen(*s))
    {
        if(isLetter((*s)[i])&&!isLower((*s)[i]))
            (*s)[i]=tolower((*s)[i]);
        i++;
    }
}

void trimming()
{
    char s[1001];
    FILE *f = fopen("poems/test1/test1","r");
    FILE *g=fopen("poems/test1/test1.out","w");
    while(fgets(s,1001,f))
    {
        int i=0;
        while(i<strlen(s)&&!isLetter(s[i])&&s[i]!='\n')
            i++;
        int whiteSpace=0;
        while(i<strlen(s))
        {
            if(isLetter(s[i])||s[i]=='\n')
            {
                whiteSpace=0;
                fprintf(g,"%c",s[i]);
            }
            else
                if(whiteSpace==0&&s[i]==' ')
                {
                    whiteSpace=1;
                    fprintf(g,"%c",s[i]);
                }
            i++;
        }
    }
	return;
}

void rhimy()
{
	return;
}

void friendly()
{
	return;
}

void print()
{
	return;
}

int main(void)
{
    srand(42);
    char ch[10001];
    char str[1001];
    scanf("%s",&str);
    while(str!="quit")
    {
        if(strcmp(str,"uppercase")==0)
        {
            uppercase(&ch);
        }
        else if(strcmp(str,"load")==0)
        {
            char path[1001];
            scanf("%s",&path);

        }
        scanf("%s",&str);
    }




	return 0;
}
