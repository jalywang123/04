#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "./kcf/kcftracker.hpp"

#include <dirent.h>

using namespace std;
using namespace cv;


bool HOG = true;
bool FIXEDWINDOW = false;
bool MULTISCALE = true;
bool SILENT = true;
bool LAB = false;


// Create KCFTracker object
KCFTracker tracker(HOG, FIXEDWINDOW, MULTISCALE, LAB);

// Frame readed
cv::Mat frame;

// Tracker results
cv::Rect result;


float xMin,yMin,width,height;

// Frame counter
int nFrames = 0;

//Display framenum
char str[25];

char KCFtrack_han(cv::Mat frame,char o,cv::Rect &tarLoc)
{
    nFrames++;
    printf("nFrames = %d,", nFrames);

    // Initation
    if(o == 0)  // First frame: init
    {
        // First frame, given the target location
        xMin =(float)tarLoc.x;
        yMin =(float)tarLoc.y;
        width =(float)tarLoc.width;
        height =(float)tarLoc.height;

        // track parameters init
        //tracker.init( Rect(xMin, yMin, width, height), frame );
        tracker.init( tarLoc, frame );
        //rectangle( frame, Point( xMin, yMin ), Point( xMin+width, yMin+height), Scalar( 0, 255, 255 ), 1, 8 );

    }

    // Begin Tracking
    else
    {
        printf("nFrames = %d,", nFrames);
        result = tracker.update(frame);
        //rectangle( frame, Point( result.x, result.y ), Point( result.x+result.width, result.y+result.height), Scalar( 0, 255, 255 ), 1, 8 );
        tarLoc = result;
    }



    // Track Fail : target occlusion , disappear , track error.
    if ( tracker.flag == 1 )  // track error
        return 0;
    else
        return 1;

}




