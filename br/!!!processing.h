#ifndef PROCESSING_H
#define PROCESSING_H

#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include "!!!image.h"

using namespace cv;
using namespace std;

class ImageProcess {
public:
	ImageProcess();
	ImageProcess(Mat &img_thresholded);
	void ExtractData();
	Mat img_thresholded;

	vector<vector<Point>> GetApproxContour() { return contours_poly; }
	vector<vector<Point>> GetContours() { return contours; }
	vector<Vec4i> GetHierarchy() { return hierarchy; }

	int IndexOfBiggestContour();
	vector<vector<Point>> GetHulls() { return hulls; }
	vector<vector<int>> GetHullsI() { return hullsI; }
	vector<vector<Vec4i>>  GetDefects() { return defects; }
	vector<Rect> GetBoundingBox() { return boundRect; }
	
	
private:
	vector<vector<Point>> contours;
	vector<vector<Point>> contours_poly;
	vector<Vec4i> hierarchy;

	vector<vector<Point>> hulls;
	vector<vector<int>> hullsI; //indices to contour points 
	vector<vector<Vec4i>> defects;

	vector<Rect> boundRect;

	void ExtractContour();
};

#endif // !PROCESSING_H