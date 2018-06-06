#include <QCoreApplication>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>


#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <algorithm>
#include "codebook.h"
#include "track.hpp"

using namespace std;
using namespace cv;
 //const Rect detect_rect(1920/2-1280/2, 1080/2-960/2, 1280, 960);
 // const Rect detect_rect(1920/2-1912/2, 1080/2-860/2, 1912, 860);
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    cv::Mat im;

    int fnum=0,tnum=0,rnum=0,i,tx1=0,ty1=0,tx2=0,ty2=0;
    Tag tags[200];
    Tag truetags[100];
    int flag=1;
    int dd[4];
   // char *filename="rtsp://admin:admin12345@192.168.31.62:554/h264/ch1/main/av_stream";
   // char *filename="/home/nvidia/Video/1/0016.avi";
    char *filename="rtsp://admin:Admin123@172.16.1.32:554/h264/ch1/main/av_stream";
    //char *filename="/home/userver/Videos/kcf_Sky.mp4";
    codeBook *cb;

    VideoCapture cap = cv::VideoCapture(filename);
    if(!cap.isOpened())
    {
        cout<<"12345566"<<endl;
        return -1;
    }

  //  cap>>im;

    cb=(codeBook*)calloc(720*576,sizeof(codeBook));
    Mat ret(576,720,CV_8UC1);

    for(i=0;i<720*576;i++)
        ret.data[i]=0;

    dd[0]=250;
    dd[1]=290;
    dd[2]=20;
    dd[3]=20;

    while(true)
    {
        cap>>im;
      //  im=Mat(im,detect_rect);
        cv::resize(im,im,cv::Size(720,576),0,0,CV_INTER_LINEAR);
        cvtColor(im,im,CV_BGR2GRAY);
        if(flag)
        {


            codebook(im.data,720,576,cb,fnum,ret.data);
             tnum=getarea(ret.data,720,576,tags);
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
        else
        {
            dd[0]=(truetags[i].x1+truetags[i].x2)/2;
            dd[1]=(truetags[i].y1+truetags[i].y2)/2;
            dd[2]=(truetags[i].x2-truetags[i].x1);
            dd[3]=(truetags[i].y2-truetags[i].y1);

            fnum=track(im.data,fnum,dd);
            tx1=dd[0]-dd[2]/2;
            ty1=dd[1]-dd[3]/2;

            tx2=dd[0]+dd[2]/2;
            ty2=dd[1]+dd[3]/2;

            rectangle(im,Point(tx1,ty1),Point(tx2,ty2),Scalar(255,0,0));
//            if(fnum == 0) // track error
//                flag = 1;
//            else  // continue tracking
//            {
//                tx1=dd[0]-dd[2]/2;
//                ty1=dd[1]-dd[3]/2;

//                tx2=dd[0]+dd[2]/2;
//                ty2=dd[1]+dd[3]/2;
//                rectangle(im,Point(tx1,ty1),Point(tx2,ty2),Scalar(255,0,0));
//            }


        }
        imshow("1",im);
        imshow("2",ret);

        cv::waitKey(40);
    }
    return a.exec();
}

