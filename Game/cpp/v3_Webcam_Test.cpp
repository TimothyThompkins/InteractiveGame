//g++ v3_Webcam_Test.cpp -o v3_Webcam_Test `pkg-config --cflags --libs opencv`

//Written by  Kyle Hounslow, December 2013
//Modified by Timothy Mitchell Thompkins April 2014

//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software")
//, to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
//and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//IN THE SOFTWARE.

#include <cv.h>
#include <highgui.h>

#include "../opencv2/opencv.hpp"

using namespace std;
using namespace cv;

//our sensitivity value to be used in the absdiff() function
const static int SENSITIVITY_VALUE = 50;
//size of blur used to smooth the intensity image output from absdiff() function
const static int BLUR_SIZE = 10;

const static int RANGE_HSV = 10;

// const int MAX_NUM_OBJECTS=50;
// //minimum and maximum object area
// const int MIN_OBJECT_AREA = 20*20;
// const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH/1.5;

//we'll have just one object to search for
//and keep track of its position.
int theObject[2] = {0,0};
int frameWidth;
int frameHeight;

int H_MIN = 0;
int S_MIN = 0;
int V_MIN = 0;

int H_MAX = 0;
int S_MAX = 0;
int V_MAX = 0;
//bounding rectangle of the object, we will use the center of this as its position.
Rect objectBoundingRectangle = Rect(0,0,0,0);

bool systemCalib = false;
bool showHist = false;


//int to string helper function
string intToString(int number){

    //this function has a number input and string output
    std::stringstream ss;
    ss << number;
    return ss.str();
}

void searchForMovement(Mat thresholdImage, Mat &cameraFeed){
    //notice how we use the '&' operator for objectDetected and cameraFeed. This is because we wish
    //to take the values passed into the function and manipulate them, rather than just working with a copy.
    //eg. we draw to the cameraFeed to be displayed in the main() function.
    bool objectDetected = false;
    Mat temp;
    thresholdImage.copyTo(temp);
    //these two vectors needed for output of findContours
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
    //find contours of filtered image using openCV findContours function
    //findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );// retrieves all contours
    findContours(temp,contours,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE );// retrieves external contours

    //if contours vector is not empty, we have found some objects
    if(contours.size()>0)objectDetected=true;
    else objectDetected = false;

    if(objectDetected){
        //the largest contour is found at the end of the contours vector
        //we will simply assume that the biggest contour is the object we are looking for.
        vector< vector<Point> > largestContourVec;
        largestContourVec.push_back(contours.at(contours.size()-1));
        //make a bounding rectangle around the largest contour then find its centroid
        //this will be the object's final estimated position.
        objectBoundingRectangle = boundingRect(largestContourVec.at(0));
        int xpos = objectBoundingRectangle.x+objectBoundingRectangle.width/2;
        int ypos = objectBoundingRectangle.y+objectBoundingRectangle.height/2;

        //update the objects positions by changing the 'theObject' array values
        theObject[0] = xpos , theObject[1] = ypos;
    }
    //make some temp x and y variables so we dont have to type out so much
    int x = theObject[0];
    //int y = theObject[1];
    int y = 650;
    //draw some crosshairs around the object
    circle(cameraFeed,Point(x,y),20,Scalar(0,255,0),2);
    line(cameraFeed,Point(x,y),Point(x,y-25),Scalar(0,255,0),2);
    line(cameraFeed,Point(x,y),Point(x,y+25),Scalar(0,255,0),2);
    line(cameraFeed,Point(x,y),Point(x-25,y),Scalar(0,255,0),2);
    line(cameraFeed,Point(x,y),Point(x+25,y),Scalar(0,255,0),2);

    //write the position of the object to the screen
    putText(cameraFeed,"Tracking object at (" + intToString(x)+","+intToString(y)+")",Point(x,y),1,1,Scalar(255,0,0),2);

}

void morphOps(Mat &thresh){

    //create structuring element that will be used to "dilate" and "erode" image.
    //the element chosen here is a 3px by 3px rectangle

    Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
    //dilate with larger element so make sure object is nicely visible
    Mat dilateElement = getStructuringElement( MORPH_RECT,Size(8,8));

    erode(thresh,thresh,erodeElement);
    //erode(thresh,thresh,erodeElement);
    //erode(thresh,thresh,erodeElement);


    dilate(thresh,thresh,dilateElement);
    dilate(thresh,thresh,dilateElement);
    dilate(thresh,thresh,dilateElement);
    //dilate(thresh,thresh,dilateElement);
    //dilate(thresh,thresh,dilateElement);

}

// void trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed){

//     Mat temp;
//     threshold.copyTo(temp);
//     //these two vectors needed for output of findContours
//     vector< vector<Point> > contours;
//     vector<Vec4i> hierarchy;
//     //find contours of filtered image using openCV findContours function
//     findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
//     //use moments method to find our filtered object
//     double refArea = 0;
//     bool objectFound = false;
//     if (hierarchy.size() > 0) {
//         int numObjects = hierarchy.size();
//         //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
//         if(numObjects<MAX_NUM_OBJECTS){
//             for (int index = 0; index >= 0; index = hierarchy[index][0]) {

//                 Moments moment = moments((cv::Mat)contours[index]);
//                 double area = moment.m00;

//                 //if the area is less than 20 px by 20px then it is probably just noise
//                 //if the area is the same as the 3/2 of the image size, probably just a bad filter
//                 //we only want the object with the largest area so we safe a reference area each
//                 //iteration and compare it to the area in the next iteration.
//                 if(area>MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea){
//                     x = moment.m10/area;
//                     y = moment.m01/area;
//                     objectFound = true;
//                     refArea = area;
//                 }else objectFound = false;


//             }
//             //let user know you found an object
//             if(objectFound ==true){
//                 putText(cameraFeed,"Tracking Object",Point(0,50),2,1,Scalar(0,255,0),2);
//                 //draw object location on screen
//                 drawObject(x,y,cameraFeed);}

//         }else putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
//     }
// }

void calibColor(Mat HSVimage, Mat &cameraFeed){
    //Mat currentFrame;
    Mat tempFrame;
    Mat threshBox;

    int cornerH_Value = 0;
    int cornerS_Value = 0;
    int cornerV_Value = 0;

    int pixelCenterH_Average = 0;
    int pixelCenterS_Average = 0;
    int pixelCenterV_Average = 0;

    int pixelCenterH_Sum = 0;
    int pixelCenterS_Sum = 0;
    int pixelCenterV_Sum = 0;

    int pixelCenterRangeHigh[3];
    int pixelCenterRangeLow[3];

    //These define the parameters of the calibration box
    int boxScale = 16;
    int boxCenterWidth = frameWidth/2;
    //int boxCenterHeight = frameHeight/2;
    int boxCenterHeight = frameHeight - (frameHeight/8);
    int boxWidthLeft = boxCenterWidth - boxScale;
    int boxWidthRight = boxCenterWidth + boxScale;
    int boxHeightTop = boxCenterHeight + boxScale;
    int boxHeightBottom = boxCenterHeight - boxScale;
    //   int boxHeight = frameHeight/100;
    int boxWidthTotal = boxWidthRight - boxWidthLeft;
    int boxHeightTotal = boxHeightTop - boxHeightBottom;
    //These define the box that is drawn
    int thickness = 2;
    int connectivity = 8;

    // Tp boxScale(25);
    // Tp boxCenterWidth = frameWidth/2;
    // Tp boxCenterHeight = frameHeight/2;
    // Tp boxWidthLeft = boxCenterWidth - boxScale;
    // Tp boxWidthRight = boxCenterWidth + boxScale;
    // Tp boxHeightTop = boxCenterHeight + boxScale;
    // Tp boxHeightBottom = boxCenterHeight - boxScale;
    // //Tp boxHeight = frameHeight/100;
    // Tp boxWidthTotal = boxWidthRight + boxWidthLeft;
    // Tp boxHeightTotal = boxHeightTop - boxHeightBottom;
    // //These define the box that is drawn
    // Tp thickness = 2;
    // Tp connectivity = 8;

    //This defines the threshold box in which we are interested
    //CvRect threshBox = cvRect(boxWidthLeft, boxHeightTop, boxWidthLeft + boxWidthRight, boxHeightBottom + boxHeightTop);

    //copy frame 1 into tempFrame
    HSVimage.copyTo(tempFrame);

        // select a ROI
        //Mat roi(tempFrame, Rect(boxWidthLeft, boxHeightTop, boxWidthTotal, boxHeightTotal));
        Mat tmp(tempFrame, Rect(boxWidthLeft, boxHeightTop, boxWidthTotal, boxHeightTotal));
        Mat hsvROI;
        tmp.copyTo(hsvROI);

        imshow("hsvROI", hsvROI);

        vector<Mat> hsv_planes;
        split( hsvROI, hsv_planes );

        int histSize = 256; //from 0 to 255

        /// Set the ranges ( for H,S,V) )
        float range[] = { 0, 256 } ; //the upper boundary is exclusive
        const float* histRange = { range };
        bool uniform = true;
        bool accumulate = false;
        Mat h_hist, s_hist, v_hist;

        /// Compute the histograms:
        calcHist( &hsv_planes[0], 1, 0, Mat(), h_hist, 1, &histSize, &histRange, uniform, accumulate );
        calcHist( &hsv_planes[1], 1, 0, Mat(), s_hist, 1, &histSize, &histRange, uniform, accumulate );
        calcHist( &hsv_planes[2], 1, 0, Mat(), v_hist, 1, &histSize, &histRange, uniform, accumulate );

        double h_minVal;
        double h_maxVal;
        Point h_minLoc;
        Point h_maxLoc;

        double s_minVal;
        double s_maxVal;
        Point s_minLoc;
        Point s_maxLoc;

        double v_minVal;
        double v_maxVal;
        Point v_minLoc;
        Point v_maxLoc;

        minMaxLoc( h_hist, &h_minVal, &h_minVal, &h_minLoc, &h_maxLoc );
        minMaxLoc( s_hist, &s_minVal, &s_minVal, &s_minLoc, &s_maxLoc );
        minMaxLoc( v_hist, &v_minVal, &v_minVal, &v_minLoc, &v_maxLoc );

        if (showHist == true){

          /// Draw the histograms for H, S, V
          int hist_w = 512; int hist_h = 400;
          int bin_w = cvRound( (double) hist_w/histSize );

          Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );



          /// Normalize the result to [ 0, histImage.rows ]
          normalize(h_hist, h_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
          normalize(s_hist, s_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
          normalize(v_hist, v_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

          /// Draw for each channel
          for( int i = 1; i < histSize; i++ )
          {
              line( histImage, Point( bin_w*(i-1), hist_h - cvRound(h_hist.at<float>(i-1)) ) ,
                               Point( bin_w*(i), hist_h - cvRound(h_hist.at<float>(i)) ),
                               Scalar( 255, 0, 0), 2, 8, 0  );
              line( histImage, Point( bin_w*(i-1), hist_h - cvRound(s_hist.at<float>(i-1)) ) ,
                               Point( bin_w*(i), hist_h - cvRound(s_hist.at<float>(i)) ),
                               Scalar( 0, 255, 0), 2, 8, 0  );
              line( histImage, Point( bin_w*(i-1), hist_h - cvRound(v_hist.at<float>(i-1)) ) ,
                               Point( bin_w*(i), hist_h - cvRound(v_hist.at<float>(i)) ),
                               Scalar( 0, 0, 255), 2, 8, 0  );
          }

          // Display
           namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
           imshow("calcHist Demo", histImage );

       }


        CvScalar color = CV_RGB(111, 242, 68);

        //These lines draw the top and bottom of the calib box
        line(cameraFeed,Point(boxWidthLeft,boxHeightTop),Point(boxWidthRight,boxHeightTop),color,thickness,connectivity);
        line(cameraFeed,Point(boxWidthLeft,boxHeightBottom),Point(boxWidthRight,boxHeightBottom),color,thickness,connectivity);
        //These lines draw the left and right of the calib box
        line(cameraFeed,Point(boxWidthLeft,boxHeightBottom),Point(boxWidthLeft,boxHeightTop),color,thickness,connectivity);
        line(cameraFeed,Point(boxWidthRight,boxHeightBottom),Point(boxWidthRight,boxHeightTop),color,thickness,connectivity);
    // }
    switch(waitKey(20)){
    case 99: //'c' has been pressed. this will enter calib mode
    cout<<"Calibration began, press 'c' to lock in color"<<endl;

        H_MIN = h_maxLoc.y-30;
        H_MAX = h_maxLoc.y;

        S_MIN = s_maxLoc.y-30;
        S_MAX = s_maxLoc.y;

        V_MIN = v_maxLoc.y-30;
        V_MAX = v_maxLoc.y;

        cout<<H_MAX<<endl;
        cout<<S_MAX<<endl;
        cout<<V_MAX<<endl;

    //Determine Color Calibration Here

    systemCalib = true;
    break;
    }

}


int main(){

    //some boolean variables for added functionality
    bool objectDetected = false;
    //these two can be toggled by pressing 'd' or 't'
    bool debugMode = false;
    bool trackingEnabled = false;
    bool calibMode = false;
    bool useMorphOps = true;
    //pause and resume code
    bool pause = false;

    int x=0, y=0;
    //set up the matrices that we will need
    //Matrix to store each frame of the webcam feed
    Mat currentFrame;
    //Fame seen in Calibration
    Mat calibFrame;
    //matrix storage for HSV image
    Mat HSV;
    //matrix storage for binary threshold image
    Mat threshold;
    //resulting difference image
    Mat differenceImage;
    //thresholded difference image (for use in findContours() function)
    Mat thresholdImage;
    //video capture object.
    VideoCapture capture;

    capture.open(0);
    //capture.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
    //capture.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);
    //Get size of window
    frameWidth = capture.get(CV_CAP_PROP_FRAME_WIDTH);
    frameHeight = capture.get(CV_CAP_PROP_FRAME_HEIGHT);

    if(!capture.isOpened()){
        cout<<"ERROR ACQUIRING VIDEO FEED\n";
        getchar();
        return -1;
    }

    while(1){
             //some boolean variables for different functionality within this program

            //Store camrea feed to matrix
            capture.read(currentFrame);

            cvtColor(currentFrame,HSV,COLOR_BGR2HSV);

            //checks to see if system has been calibrated
            if(systemCalib == false){

                //capture.read(currentFrame);
                calibColor(HSV, currentFrame);

                if(calibMode == true){
                    //show the difference image and threshold imatge
                    cv::imshow("Calibrated Image", HSV);
                }
                else{
                    //if not in debug mode, destroy the windows so we don't see them anymore
                    cv::destroyWindow("Calibrated Image");
                }

            }

            else{
            //convert frame from BGR to HSV colorspace
            //cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
            //filter HSV image between values and store filtered image to
            //threshold matrix
            //inRange(HSV,Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX),threshold);
            //perform frame differencing with the sequential images. This will output an "intensity image"
            //do not confuse this with a threshold image, we will need to perform thresholding afterwards.

            //if system has not yet been calibrated, i.e. a color chosen, do that now

            inRange(HSV,Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX),threshold);
            //perform morphological operations on thresholded image to eliminate noise
            //and emphasize the filtered object(s)
            if(useMorphOps)
            morphOps(threshold);
            //pass in thresholded frame to our object tracking function
            //this function will return the x and y coordinates of the
            //filtered object

           if(trackingEnabled)
           searchForMovement(threshold,currentFrame);

            if(debugMode==true){
                //show the difference image and threshold image
                //cv::imshow("HSV Image",HSV);
                cv::imshow("Threshold Image", threshold);
            }
            else{
                //if not in debug mode, destroy the windows so we don't see them anymore
                //cv::destroyWindow("Difference Image");
                cv::destroyWindow("Threshold Image");
            }
            //blur the image to get rid of the noise. This will output an intensity image
            // cv::blur(thresholdImage,thresholdImage,cv::Size(BLUR_SIZE,BLUR_SIZE));
            // //threshold again to obtain binary image from blur output
            // cv::threshold(thresholdImage,thresholdImage,SENSITIVITY_VALUE,255,THRESH_BINARY);
            // if(debugMode==true){
            //     //show the threshold image after it's been "blurred"

            //     imshow("Final Threshold Image",thresholdImage);

            // }
            // else {
            //     //if not in debug mode, destroy the windows so we don't see them anymore
            //     cv::destroyWindow("Final Threshold Image");
            // }

            //if tracking enabled, search for contours in our thresholded image
            // if(trackingEnabled){

            //     searchForMovement(thresholdImage,frame1);
            // }

            //show our captured frame
            //imshow("Frame1",frame1);
            //check to see if a button has been pressed.
            //this 10ms delay is necessary for proper operation of this program
            //if removed, frames will not have enough time to referesh and a blank
            //image will appear.

        }

            imshow("Current Frame", currentFrame);

            switch(waitKey(20)){

            case 27: //'esc' key has been pressed, exit program.
                return 0;
            case 113: //'q' has been pressed. this will toggle tracking
                systemCalib = false;
                break;
            case 114: //'r' has been pressed. this will re-calibrate the system
                systemCalib = false;
                break;
            case 116: //'t' has been pressed. this will toggle tracking
                trackingEnabled = !trackingEnabled;
                if(trackingEnabled == false) cout<<"Tracking disabled."<<endl;
                else cout<<"Tracking enabled."<<endl;
                break;
            case 100: //'d' has been pressed. this will debug mode
                debugMode = !debugMode;
                if(debugMode == false) cout<<"Debug mode disabled."<<endl;
                else cout<<"Debug mode enabled."<<endl;
                break;
            case 112: //'p' has been pressed. this will pause/resume the code.
                pause = !pause;
                if(pause == true){ cout<<"Code paused, press 'p' again to resume"<<endl;
                while (pause == true){
                    //stay in this loop until
                    switch (waitKey()){
                        //a switch statement inside a switch statement? Mind blown.
                        case 112:
                        //change pause back to false
                        pause = false;
                        cout<<"Code Resumed"<<endl;
                        break;
                    }
                }
                }

            }
    }

    return 0;

}
