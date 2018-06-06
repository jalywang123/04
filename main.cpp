#include <QCoreApplication>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>


#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <algorithm>
#include "codebook.h"
//#include "track.hpp"
#include "socket.h"
#include "./kcf/fhog.hpp"
#include "./kcf/kcftracker.hpp"
using namespace std;
using namespace cv;


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Mat im;
    Mat imoriginal;
    int fnum=0,tnum=0,rnum=0,i;
   // int tx1=0,ty1=0,tx2=0,ty2=0;
    Tag tags[200];
    Tag truetags[100];
    int flag=1;
    int dd[4];
    int tbx,tby;
    int centX,centY;
    char data[8];
    const int W=600,H=400;
    Rect TarLoc;
   // char *filename="rtsp://admin:admin12345@192.168.31.62:554/h264/ch1/main/av_stream";
    char *filename="/home/userver/Qt5.5.1/detect_track_kcf_1023/video/result0824080201.avi";
    //char *filename="rtsp://admin:Admin123@172.16.1.32:554/h264/ch1/main/av_stream";
   // char *filename="/home/userver/Videos/result133201.avi";
    //char *filename="/home/userver/Qt5.5.1/kcf_multicar/video/result082401_0002.avi";
    socketinit();
    codeBook *cb;

    VideoCapture cap = cv::VideoCapture(filename);
    if(!cap.isOpened())
    {
        cout<<"12345566"<<endl;
        return -1;
    }

  //  cap>>im;

    cb=(codeBook*)calloc(W*H,sizeof(codeBook));
    Mat ret(H,W,CV_8UC1);

    for(i=0;i<W*H;i++)
        ret.data[i]=0;

    dd[0]=250;
    dd[1]=290;
    dd[2]=20;
    dd[3]=20;

    while(true)
    {
        cap>>im;

      //  im=Mat(im,detect_rect);
        cv::resize(im,im,cv::Size(W,H),0,0,CV_INTER_LINEAR);
        imoriginal = im;
        cvtColor(im,im,CV_BGR2GRAY);
        if(revdata(data,1)!=-1)
        {
            if(data[0]==1)
            flag=1;
            else if(data[0]==2)
            {
                flag=2;
                fnum=0;
                memset(cb,0,sizeof(cb)*W*H);
                memset(truetags,0,sizeof(Tag)*100);
                memset(ret.data,0,W*H*sizeof(char));
                //jj: ------------>need remake backgroud!
            }
        }
        if(flag==1)
        {
            codebook(im.data,W,H,cb,fnum,ret.data);
             tnum=getarea(ret.data,W,H,tags);
             rnum=savetag(tags,tnum,truetags,rnum,fnum-20);
             arrayarea(rnum,truetags,rnum);
             for(i=0;i<rnum;i++)

             if(truetags[i].tp>10)
             {
                 flag=0;
                 fnum=-1;
                 break;
             }

            fnum++;
        }
        else if(flag==0)
        {

            TarLoc.x = truetags[i].x1; // xmin
            TarLoc.y = truetags[i].y1; // ymin
            TarLoc.width = (truetags[i].x2-truetags[i].x1) + 1;   // width
            TarLoc.height = (truetags[i].y2-truetags[i].y1) + 1;  // height
            fnum = KCFtrack_han(imoriginal,fnum,TarLoc);
            rectangle( imoriginal, Point(TarLoc.x, TarLoc.y), Point(TarLoc.x+TarLoc.width,TarLoc.y+TarLoc.height), Scalar(0,255,255), 1, 8 );
            if(fnum == 0)  // track error,begin detect
            {
                flag = 1;
                fnum=0;
                memset(cb,0,sizeof(cb)*W*H);
                memset(truetags,0,sizeof(Tag)*100);
                memset(ret.data,0,W*H*sizeof(char));
            }
            else  // fnum = 1 conti1nue track
            {
                flag = 0;
            }


        }
        imshow("1",imoriginal);
        imshow("2",ret);

        cv::waitKey(10);
    }
    return a.exec();
}





