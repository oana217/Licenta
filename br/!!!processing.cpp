#include "!!!processing.h"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;
ImageProcess::ImageProcess()
{

}

ImageProcess::ImageProcess(Mat &img_thresholded)
{
	this->img_thresholded = img_thresholded;
}

void ImageProcess::ExtractContour()
{
	findContours(img_thresholded, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
}

void ImageProcess::ExtractApproxContour()
{
	ExtractContour();
	/*int indexOfBiggestContour = IndexOfBiggestContour();
	if (indexOfBiggestContour > -1)
	{
		//approxPolyDP(Mat(contours[indexOfBiggestContour]), contours_poly[indexOfBiggestContour], 3, true);

		approxPolyDP(contours[indexOfBiggestContour], contours_poly[indexOfBiggestContour], 3, true);
	}*/
}

int ImageProcess::IndexOfBiggestContour()
{
	int indexOfBiggestContour = -1;
	int sizeOfBiggestContour = 0;
	for (int i = 0; i < contours.size(); i++) {
		if (contours[i].size() > sizeOfBiggestContour) {
			sizeOfBiggestContour = contours[i].size();
			indexOfBiggestContour = i;
		}
	}
	return indexOfBiggestContour;
}

