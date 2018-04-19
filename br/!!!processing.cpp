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
	hulls.size = contours.size();
	hullsI.size = contours.size(); //indices to contour points 
	defects.size = contours.size();

	boundRect.size = contours.size();
	contours_poly.size = contours.size();
}

void ImageProcess::ExtractData()
{
	ExtractContour();
	int indexOfBiggestContour = IndexOfBiggestContour();
	if (indexOfBiggestContour > -1)
	{
		//approxPolyDP(Mat(contours[indexOfBiggestContour]), contours_poly[indexOfBiggestContour], 3, true);
		convexHull(Mat(contours[indexOfBiggestContour]), hulls[indexOfBiggestContour], false);
		convexHull(Mat(contours[indexOfBiggestContour]), hullsI[indexOfBiggestContour], false);

		if (hullsI[indexOfBiggestContour].size() > 3)
			convexityDefects(contours[indexOfBiggestContour], hullsI[indexOfBiggestContour], defects[indexOfBiggestContour]);

		approxPolyDP(Mat(contours[indexOfBiggestContour]), contours_poly[indexOfBiggestContour], 3, true);
		boundRect[indexOfBiggestContour] = boundingRect(Mat(contours_poly[indexOfBiggestContour]));
	}
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

