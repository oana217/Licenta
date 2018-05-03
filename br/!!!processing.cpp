#include "!!!processing.h"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <chrono>
#include <ctime>
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
	hulls.resize(contours.size());
	hullsI.resize(contours.size()); //indices to contour points 
	defects.resize(contours.size());

	boundRect.resize(contours.size());
	contours_poly.resize(contours.size());
}

void ImageProcess::ProcessContours()
{
	int i = IndexOfBiggestContour();
	if (i > -1)
	{
		size_t count = contours[i].size(); 
		if (count > 100) {
			//approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
			convexHull(Mat(contours[i]), hulls[i], false);
			convexHull(Mat(contours[i]), hullsI[i], false);

			if (hullsI[i].size() > 3)
				convexityDefects(contours[i], hullsI[i], defects[i]);

			approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
			boundRect[i] = boundingRect(Mat(contours_poly[i]));
			isValid = true;
		}
	}
}


void ImageProcess::FilterDefects() {
	int i = IndexOfBiggestContour();
	noOfDefects = 0;
	if (i > -1) {
		size_t count = contours[i].size();
		if (count > 100) {
			//std::cout << "Count : " << count << std::endl;


			vector<Vec4i>::iterator d = defects[i].begin();
			while (d != defects[i].end())
			{
				Vec4i& v = (*d);
				float depth = v[3] / 256; // distance between the farthest point and the convex hull
				if (depth > 20)
				{
					int startidx = v[0];
					Point ptStart(contours[i][startidx]); // point of the contour where the defect begins
					int endidx = v[1];
					Point ptEnd(contours[i][endidx]); // point of the contour where the defect ends
					int faridx = v[2];
					Point ptFar(contours[i][faridx]); // the farthest from the convex hull point within the defect

					if (GetDistance(ptStart, ptFar) > boundRect[i].height / 4 //finger length must be bigger than 20% of the box
						&& GetDistance(ptEnd, ptFar) > boundRect[i].height / 4
						&& GetAngle(ptStart, ptFar, ptEnd) < 135) //angle between fingers must be less than 120 degrees
					{
						vector<Point> finger_points;
						finger_points.push_back(ptStart);
						finger_points.push_back(ptFar);
						finger_points.push_back(ptEnd);

						//add valid vector to fingers_points vector
						fingers_points.push_back(finger_points);

						noOfDefects++;
					}
				}
				d++;
			}
		}
	}
}

void ImageProcess::GetData() {
	ExtractContour();
	ProcessContours();
	FilterDefects();
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

float ImageProcess::GetDistance(Point a, Point b)
{
	return sqrt(fabs(pow(a.x - b.x, 2) + pow(a.y - b.y, 2)));
}

float ImageProcess::GetAngle(Point s, Point f, Point e)
{
	float l1 = GetDistance(f, s);
	float l2 = GetDistance(f, e);
	float dot = (s.x - f.x)*(e.x - f.x) + (s.y - f.y)*(e.y - f.y);
	float angle = acos(dot / (l1*l2));
	angle = angle * 180 / 3.14;
	return angle;
}

float ImageProcess::GetAngleXAxis(Point s, Point e)
{
	float angle = atan2(s.y - e.y, e.x - s.x);
	angle = angle * 180 / 3.14;
	if (angle < 0) {
		angle = angle + 360;
	}
	return angle;
}
