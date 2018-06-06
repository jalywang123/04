#include "codebook.h"



int xh[10]={0,0,0,0,0,0,0,0,0,0};

int savetag(Tag *tag,int num,Tag *alltag,int allnum,int f)
{
    int i,j,k;
    char flag=1;

    if(f==0)
    {
        i=0;
        for(j=0;j<num;j++)
        {
            if(tag[j].tp==0)
            {
                i++;
                alltag[i]=tag[j];
            }
        }
        return i;
    }

    for(i=0;i<allnum;i++)//curNum2
    {
        alltag[i].hold--;
        if((alltag[i].hold<=0)||(alltag[i].x1==0))
        {
            xh[alltag[i].IP-1]=0;
            alltag[i].IP=0;
        }
    }

    for(i=0;i<allnum;i++)
        for(j=0;j<num;j++)
        {
            flag=!tag[j].pf;
            flag=flag&&(!tag[j].tp);
            flag=abs(tag[j].x1-alltag[i].x1)>20?0:flag;
            flag=abs(tag[j].y1-alltag[i].y1)>20?0:flag;
            flag=abs(tag[j].num-alltag[i].num)>40?0:flag;
            if(flag)
            {
                tag[j].tp=alltag[i].tp+1;
                k=alltag[i].IP;
                alltag[i]=tag[j];
                alltag[i].IP=k;
                tag[j].pf=1;
                break;
            }
        }

    for(i=0;i<allnum;i++)
        if(alltag[i].hold<=0)
        {
            alltag[i].x1=0;
            alltag[i].y1=0;
            alltag[i].x2=0;
            alltag[i].y2=0;
            for(j=0;j<num;j++)
                if(tag[j].pf==0)
                {
                    alltag[i]=tag[j];
                    tag[j].pf=1;
                    break;
                }
        }
    for(j=0;j<num;j++)
        if(tag[j].pf==0)
        {
            alltag[allnum++]=tag[j];
            tag[j].pf=1;
        }

    return allnum;
}

int arrayarea(int num,Tag *alltag,int anum)
{
    int i=0,j;

    for(i=0;i<num;i++)
    {
        if((alltag[i].tp>2)&&(alltag[i].IP==0))
        {
            for(j=0;j<10;j++)
            {
                if(xh[j]==0)
                {

                    alltag[i].IP=j+1;
                    xh[j]=1;
                    break;
                }
            }


        }
    }
    return anum;
}

int getarea(uchar *im,int width,int height,Tag *out)
{
    int i,j,an=0,num=0;
    int *xx=(int*)calloc(width*height,sizeof(int));
    int *yy=(int*)calloc(width*height,sizeof(int));
    //num=width*height;
    for(i=0;i<height;i++)
    {
        for(j=0;j<width;j++)
        {
            if(im[i*width+j]==255)
            {
                xx[num]=j;
                yy[num]=i;
                num++;
            }
        }
    }
    for(i=0;i<num;i++)
    {
        for(j=0;j<an;j++)
        {
            if(((abs(out[j].x1-xx[i])<20||abs(out[j].x2-xx[i])<20)&&(abs(out[j].y1-yy[i])<20||abs(out[j].y2-yy[i])<20))||((out[j].x1<xx[i])&&(out[j].x2>xx[i])&&(out[j].y1<yy[i])))
            {
                out[j].x1=xx[i]>out[j].x1?out[j].x1:xx[i];
                out[j].x2=xx[i]>out[j].x2?xx[i]:out[j].x2;
                out[j].y1=yy[i]>out[j].y1?out[j].y1:yy[i];
                out[j].y2=yy[i]>out[j].y2?yy[i]:out[j].y2;
                out[j].num++;
                break;
            }
        }
        if(j==an)
        {
           out[j].x1=xx[i];
           out[j].x2=xx[i];
           out[j].y1=yy[i];
           out[j].y2=yy[i];
           out[j].num=1;
           out[j].hold=10;
           out[j].pf=0;
           out[j].tp=0;
           out[j].IP=0;
           an++;
        }
    }
    for(i=0;i<an-1;i++)
        for(j=i+1;j<an;j++)
        {
             if((out[i].tp==0)&&(out[j].tp==0))
             {
                 if((out[i].x1<=out[j].x1)&&(out[i].x2>=out[j].x2)&&(out[i].y1<=out[j].y1)&&(out[i].y2>=out[j].y2))
                     {out[j].tp=1;}
                 if((out[i].x1>=out[j].x1)&&(out[i].x2<=out[j].x2)&&(out[i].y1>=out[j].y1)&&(out[i].y2<=out[j].y2))
                     {out[i].tp=1;}
             }
         }
    free(xx);
    free(yy);
    return an;
}

