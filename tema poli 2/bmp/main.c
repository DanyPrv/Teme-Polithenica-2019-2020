#include <stdio.h>
#include <stdlib.h>
#include"bmp_header.h"
#include<string.h>


typedef struct
{
    unsigned char b;
    unsigned char g;
    unsigned char r;
}pixel;
void readImage( bmp_fileheader* fileHeader,bmp_infoheader* infoHeader, pixel*** bitmap, FILE *f)
{
    fread(fileHeader,sizeof(bmp_fileheader),1,f);
    fread(infoHeader,sizeof(bmp_infoheader),1,f);
    *bitmap=malloc(infoHeader->height*sizeof(pixel*));
    int i,j;
    for(i=0;i<infoHeader->height;i++)
        (*bitmap)[i]=malloc(infoHeader->width*sizeof(pixel));
    for(i=infoHeader->height-1;i>=0;i--)
    {

        for(j=0;j<infoHeader->width;j++)
            fread(&(*bitmap)[i][j],sizeof(pixel),1,f);
        if((infoHeader->width*3)%4!=0)
             fseek(f,(((infoHeader->width*3/4)+1)*4-(infoHeader->width)*3),SEEK_CUR);

    }
}
void printImage(bmp_infoheader infoHeader, bmp_fileheader fileHeader, pixel** bitmap, FILE* g)
{
    fwrite(&fileHeader,sizeof(fileHeader),1,g);
    fwrite(&infoHeader,sizeof(infoHeader),1,g);
    int i,j;
    unsigned char o=0;
    //fwrite(&o,sizeof(unsigned char),1,g);
    //fwrite(&o,sizeof(unsigned char),1,g);
    for(i=infoHeader.height-1;i>=0;i--)
    {
        for(j=0;j<infoHeader.width;j++)
            fwrite(&bitmap[i][j],sizeof(pixel),1,g);
        if((infoHeader.width*3)%4!=0){
             int h;
             for(h=0;h<((infoHeader.width*3/4)+1)*4-(infoHeader.width)*3;h++)
                fwrite(&o,sizeof(unsigned char),1,g);
        }
    }
}

void blackAndWite(char *filename)
{
    bmp_infoheader infoHeader;
    bmp_fileheader fileHeader;
    int i,j;
    pixel **bitmap=NULL;

    FILE *f = fopen(filename,"rb+");
    readImage(&fileHeader,&infoHeader,&bitmap,f);

    for(i=infoHeader.height-1;i>=0;i--)
        for(j=0;j<infoHeader.width;j++)
        {
            bitmap[i][j].b=bitmap[i][j].r=bitmap[i][j].g=(bitmap[i][j].b+bitmap[i][j].r+bitmap[i][j].g)/3;
        }
    char*file=malloc(100*sizeof(char));
    strcpy(file,filename);
    file[strlen(file)-4]='\0';
    strcat(file,"_black_white.bmp");
    FILE *g=fopen(file,"wb+");
    free(file);
    printImage(infoHeader,fileHeader,bitmap,g);

    fclose(f);
    fclose(g);

    for(i=0;i<infoHeader.height;i++)
        free(bitmap[i]);
    free(bitmap);
}

void noCrop(char *filename)
{
    bmp_infoheader infoHeader;
    bmp_fileheader fileHeader;
    int i,j;
    pixel **bitmap=NULL;
    FILE *f = fopen(filename,"rb+");
    readImage(&fileHeader,&infoHeader,&bitmap,f);

    char*file=malloc(100*sizeof(char));
    strcpy(file,filename);
    file[strlen(file)-4]='\0';
    strcat(file,"_nocrop.bmp");
    FILE *g=fopen(file,"wb+");
    free(file);

    unsigned char o=0;
    pixel white;
    white.r=white.g=white.b=255;
    if(infoHeader.width!=infoHeader.height)
    {
        if(infoHeader.width>infoHeader.height)
        {
            int dif=infoHeader.width-infoHeader.height;
            infoHeader.height=infoHeader.width;
            fwrite(&fileHeader,sizeof(fileHeader),1,g);
            fwrite(&infoHeader,sizeof(infoHeader),1,g);
            int k1=0,k2=0;
            for(i=dif/2+1;i<=dif;i++)
            {
                for(j=0;j<infoHeader.width;j++)
                    fwrite(&white,sizeof(pixel),1,g);
                if((infoHeader.width*3)%4!=0){
                     int h;
                     for(h=0;h<((infoHeader.width*3/4)+1)*4-(infoHeader.width)*3;h++)
                        fwrite(&o,sizeof(unsigned char),1,g);
                }
                k1++;
            }

            for(i=infoHeader.height-dif-1;i>=0;i--)
            {
                for(j=0;j<infoHeader.width;j++)
                    fwrite(&bitmap[i][j],sizeof(pixel),1,g);
                if((infoHeader.width*3)%4!=0){
                     int h;
                     for(h=0;h<((infoHeader.width*3/4)+1)*4-(infoHeader.width)*3;h++)
                        fwrite(&o,sizeof(unsigned char),1,g);
                }
            }

            for(i=1;i<=dif/2;i++)
            {
                for(j=0;j<infoHeader.width;j++)
                    fwrite(&white,sizeof(pixel),1,g);
                if((infoHeader.width*3)%4!=0){
                     int h;
                     for(h=0;h<((infoHeader.width*3/4)+1)*4-(infoHeader.width)*3;h++)
                        fwrite(&o,sizeof(unsigned char),1,g);
                }
                k2++;
            }

            for(i=0;i<infoHeader.height-dif;i++)
                free(bitmap[i]);
            free(bitmap);
            fclose(f);
            fclose(g);
            return;
        }
        else
        {
            int dif=infoHeader.height-infoHeader.width;
            infoHeader.width=infoHeader.height;
            fwrite(&fileHeader,sizeof(fileHeader),1,g);
            fwrite(&infoHeader,sizeof(infoHeader),1,g);
            for(i=infoHeader.height-1;i>=0;i--)
            {
                for(j=1;j<=dif/2;j++)
                    fwrite(&white,sizeof(pixel),1,g);
                for(j=0;j<infoHeader.width-dif;j++)
                    fwrite(&bitmap[i][j],sizeof(pixel),1,g);
                for(j=dif/2+1;j<=dif;j++)
                    fwrite(&white,sizeof(pixel),1,g);
                if((infoHeader.width*3)%4!=0){
                     int h;
                     for(h=0;h<((infoHeader.width*3/4)+1)*4-(infoHeader.width)*3;h++)
                        fwrite(&o,sizeof(unsigned char),1,g);
                }
            }
        }
    }
    else
        printImage(infoHeader,fileHeader,bitmap,g);

    fclose(f);
    fclose(g);

    for(i=0;i<infoHeader.height;i++)
        free(bitmap[i]);
    free(bitmap);
}

void convolutionalLayers(char * filename, double** filter, int n)
{
    bmp_infoheader infoHeader;
    bmp_fileheader fileHeader;
    int i,j;
    pixel **bitmap=NULL;
    FILE *f = fopen(filename,"rb+");
    readImage(&fileHeader,&infoHeader,&bitmap,f);

    pixel** newBitMap=malloc(infoHeader.height*sizeof(pixel*));
    for(i=0;i<infoHeader.height;i++)
        newBitMap[i]=malloc(infoHeader.width*sizeof(pixel));

    for(i=0;i<infoHeader.height;i++)
        for(j=0;j<infoHeader.width;j++)
    {
        int k,l;
        double sr=0,sg=0,sb=0;
        for(k=i-n/2;k<=i+n/2;k++)
        {
            if(k<0||k>=infoHeader.height)
                continue;
            for(l=j-n/2;l<=j+n/2;l++)
            {
                if(l<0||l>=infoHeader.width)
                    continue;
                sr=sr+bitmap[k][l].r*filter[k-(i-n/2)][l-(j-n/2)];
                sg=sg+bitmap[k][l].g*filter[k-(i-n/2)][l-(j-n/2)];
                sb=sb+bitmap[k][l].b*filter[k-(i-n/2)][l-(j-n/2)];
            }
        }
        if(sr<0)
            newBitMap[i][j].r=0;
        else if(sr>255)
            newBitMap[i][j].r=255;
        else
            newBitMap[i][j].r=(unsigned char)sr;

        if(sg<0)
            newBitMap[i][j].g=0;
        else if(sg>255)
            newBitMap[i][j].g=255;
        else
            newBitMap[i][j].g=(unsigned char)sg;

        if(sb<0)
            newBitMap[i][j].b=0;
        else if(sb>255)
            newBitMap[i][j].b=255;
        else
            newBitMap[i][j].b=(unsigned char)sb;
    }

    char*file=malloc(100*sizeof(char));
    strcpy(file,filename);
    file[strlen(file)-4]='\0';
    strcat(file,"_filter.bmp");
    FILE *g=fopen(file,"wb+");
    free(file);
    printImage(infoHeader,fileHeader,newBitMap,g);
    fclose(f);
    fclose(g);
    for(i=0;i<infoHeader.height;i++)
    {
        free(bitmap[i]);
        free(newBitMap[i]);
    }
    free(bitmap);
    free(newBitMap);
}

void maxMinPooling(char * filename,int n,char Mm)
{
    bmp_infoheader infoHeader;
    bmp_fileheader fileHeader;
    int i,j;
    pixel **bitmap=NULL;
    FILE *f = fopen(filename,"rb+");
    readImage(&fileHeader,&infoHeader,&bitmap,f);

    pixel** newBitMap=malloc(infoHeader.height*sizeof(pixel*));
    for(i=0;i<infoHeader.height;i++)
        newBitMap[i]=malloc(infoHeader.width*sizeof(pixel));

    for(i=0;i<infoHeader.height;i++)
        for(j=0;j<infoHeader.width;j++)
    {
        int k,l;
        int sr=0,sg=0,sb=0;
        if(Mm=='m')
            sr=sg=sb=255;
        for(k=i-n/2;k<=i+n/2;k++)
        {
            if(k<0||k>=infoHeader.height){
                if(Mm=='m')
                    sr=sg=sb=0;
                continue;
            }
            for(l=j-n/2;l<=j+n/2;l++)
            {
                if(l<0||l>=infoHeader.width){
                    if(Mm=='m')
                        sr=sg=sb=0;
                    continue;
                }
                if(Mm=='M')
                {
                    if(sr<bitmap[k][l].r)
                        sr=bitmap[k][l].r;
                    if(sg<bitmap[k][l].g)
                        sg=bitmap[k][l].g;
                    if(sb<bitmap[k][l].b)
                        sb=bitmap[k][l].b;

                }
                else
                {
                    if(sr>bitmap[k][l].r)
                        sr=bitmap[k][l].r;
                    if(sg>bitmap[k][l].g)
                        sg=bitmap[k][l].g;
                    if(sb>bitmap[k][l].b)
                        sb=bitmap[k][l].b;
                }
            }
        }
        newBitMap[i][j].r=sr;
        newBitMap[i][j].g=sg;
        newBitMap[i][j].b=sb;
    }

    char*file=malloc(100*sizeof(char));
    strcpy(file,filename);
    file[strlen(file)-4]='\0';
    strcat(file,"_pooling.bmp");
    FILE *g=fopen(file,"wb+");
    free(file);
    printImage(infoHeader,fileHeader,newBitMap,g);
    fclose(f);
    fclose(g);
    for(i=0;i<infoHeader.height;i++)
    {
        free(bitmap[i]);
        free(newBitMap[i]);
    }
    free(bitmap);
    free(newBitMap);
}
int compare(pixel x1, pixel x2, int threshold)
{
    return abs(x1.r-x2.r)+abs(x1.g-x2.g)+abs(x1.b-x2.b)<=threshold;
}
void clustering(char* filename,int threshold)
{
    bmp_infoheader infoHeader;
    bmp_fileheader fileHeader;
    int i,j;
    pixel **bitmap=NULL;
    FILE *f = fopen(filename,"rb+");
    readImage(&fileHeader,&infoHeader,&bitmap,f);
    int** viz=calloc(infoHeader.height,sizeof(int*));
    for(i=0;i<infoHeader.height;i++)
        viz[i]=calloc(infoHeader.width,sizeof(int));

    int *quei=malloc(infoHeader.height*infoHeader.width*sizeof(int));
    int *quej=malloc(infoHeader.height*infoHeader.width*sizeof(int));

    for(i=0;i<infoHeader.height;i++)
        for(j=0;j<infoHeader.width;j++)
    {
        if(viz[i][j]==0)
        {
            viz[i][j]=1;
            int p=0,u=0;
            quei[0]=i;
            quej[0]=j;
            while(p<=u)
            {
                int k=quei[p];
                int l=quej[p];

                if(k-1>=0&&!viz[k-1][l]&&compare(bitmap[i][j],bitmap[k-1][l],threshold))
                {
                    u++;
                    viz[k-1][l]=1;
                    quei[u]=k-1;
                    quej[u]=l;
                }
                if(k+1<infoHeader.height&&!viz[k+1][l]&&compare(bitmap[i][j],bitmap[k+1][l],threshold))
                {
                    u++;
                    viz[k+1][l]=1;
                    quei[u]=k+1;
                    quej[u]=l;
                }
                if(l-1>=0&&!viz[k][l-1]&&compare(bitmap[i][j],bitmap[k][l-1],threshold))
                {
                    u++;
                    viz[k][l-1]=1;
                    quei[u]=k;
                    quej[u]=l-1;
                }
                if(l+1<infoHeader.width&&!viz[k][l+1]&&compare(bitmap[i][j],bitmap[k][l+1],threshold))
                {
                    u++;
                    viz[k][l+1]=1;
                    quei[u]=k;
                    quej[u]=l+1;
                }
                p++;
            }
            int sr=0;
            int sg=0;
            int sb=0;
            p=0;
            while(p<=u)
            {
                sr=sr+bitmap[quei[p]][quej[p]].r;
                sg=sg+bitmap[quei[p]][quej[p]].g;
                sb=sb+bitmap[quei[p]][quej[p]].b;
                p++;
            }
            sr=sr/p;
            sg=sg/p;
            sb=sb/p;
            p=0;
            while(p<=u)
            {
                bitmap[quei[p]][quej[p]].r=sr;
                bitmap[quei[p]][quej[p]].g=sg;
                bitmap[quei[p]][quej[p]].b=sb;
                p++;
            }
        }
    }

    char*file=malloc(100*sizeof(char));
    strcpy(file,filename);
    file[strlen(file)-4]='\0';
    strcat(file,"_clustered.bmp");
    FILE *g=fopen(file,"wb+");
    free(file);

    printImage(infoHeader,fileHeader,bitmap,g);
    fclose(f);
    fclose(g);
    for(i=0;i<infoHeader.height;i++)
    {
        free(bitmap[i]);
        free(viz[i]);
    }
    free(bitmap);
    free(viz);
    free(quei);
    free(quej);
}
int main()
{
    char *name=malloc(100*sizeof(char));
    FILE* in=fopen("input.txt","r");
    fscanf(in,"%s",name);
    blackAndWite(name);
    noCrop(name);

    char *filterPath=malloc(100*sizeof(char));
    fscanf(in,"%s",filterPath);
    FILE* filterFile=fopen(filterPath,"r");
    int n,i,j;
    fscanf(filterFile,"%d",&n);
    double **filter=malloc(n*sizeof(double*));
    for(i=0;i<n;i++)
        filter[i]=malloc(n*sizeof(double));
    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
            fscanf(filterFile,"%lf",&filter[i][j]);
    convolutionalLayers(name,filter,n);
    for(i=0;i<n;i++)
        free(filter[i]);
    fclose(filterFile);
    free(filter);
    free(filterPath);

    char *poolingPath=malloc(100*sizeof(char));
    fscanf(in,"%s",poolingPath);
    FILE* poolingFile=fopen(poolingPath,"r");
    char minMax;
    int sz;
    fscanf(poolingFile,"%c",&minMax);
    fscanf(poolingFile,"%d",&sz);
    maxMinPooling(name,sz,minMax);
    fclose(poolingFile);
    free(poolingPath);

    char *clusteringPath=malloc(100*sizeof(char));
    fscanf(in,"%s",clusteringPath);
    FILE* clusteringFile=fopen(clusteringPath,"r");
    int threshold;
    fscanf(clusteringFile,"%d",&threshold);
    clustering(name,threshold);
    fclose(clusteringFile);
    free(clusteringPath);
    free(name);
}
