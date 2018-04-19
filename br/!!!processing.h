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
	void ExtractApproxContour();

	//vector<vector<Point>> GetBiggestContour() { return contours_poly; }
	int IndexOfBiggestContour();
	vector<Vec4i> GetHierarchy() { return hierarchy; }
	//int GetDefects();
	//int GetBoundingBox();
	
	
private:
	Mat img_thresholded;
	vector<vector<Point>> contours_poly;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	void ExtractContour();
};

#endif // !PROCESSING_H