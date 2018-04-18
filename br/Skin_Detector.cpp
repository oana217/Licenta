#include "Skin_Detector.h"
#include"opencv2\opencv.hpp"

SkinDetector::SkinDetector(void)
{
	/*cv::namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"
	//Create trackbars in "Control" window
	cvCreateTrackbar("LowH", "Control", &Y_MIN, 179); //Hue (0 - 179)
	cvCreateTrackbar("HighH", "Control", &Y_MAX, 179);

	cvCreateTrackbar("LowS", "Control", &Cr_MIN, 255); //Saturation (0 - 255)
	cvCreateTrackbar("HighS", "Control", &Cr_MAX, 255);

	cvCreateTrackbar("LowV", "Control", &Cb_MIN, 255); //Value (0 - 255)
	cvCreateTrackbar("HighV", "Control", &Cb_MAX, 255);*/
	//YCrCb threshold
	// You can change the values and see what happens

	Y_MIN = 0;
	Y_MAX = 255;
	Cr_MIN = 133;
	Cr_MAX = 173;
	Cb_MIN = 77;
	Cb_MAX = 127;
}

SkinDetector::~SkinDetector(void)
{
}

//this function will return a skin masked image
cv::Mat SkinDetector::getSkin(cv::Mat input)
{
	cv::Mat skin;
	//first convert our RGB image to YCrCb
	cv::cvtColor(input, skin, cv::COLOR_BGR2YCrCb);

	//uncomment the following line to see the image in YCrCb Color Space
	cv::imshow("YCrCb Color Space",skin);

	//filter the image in YCrCb color space
	cv::inRange(skin, cv::Scalar(Y_MIN, Cr_MIN, Cb_MIN), cv::Scalar(Y_MAX, Cr_MAX, Cb_MAX), skin);
	cv::inRange(skin, cv::Scalar(), cv::Scalar(), skin); //Threshold the image

	return skin;
}