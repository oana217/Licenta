#include "!!!drawing.h"

#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Draw::Draw(ImageProcess &img_process) {
	 this->img_process = img_process; 
	 drawing = Mat::zeros(img_process.img_thresholded.size(), CV_8UC3);

}

void Draw::DrawData() {
	int index = img_process.IndexOfBiggestContour();
	if (index > -1) {
		//draw biggest contour and its hull

		drawContours(drawing, img_process.GetContours(), index, Scalar(255, 255, 255), 2, 8, img_process.GetHierarchy(), 0, Point());
		drawContours(drawing, img_process.GetHulls(), index, Scalar(124, 100, 233), 2, 8, vector<Vec4i>(), 0, Point());

		//draw the bounding rectangle
		rectangle(drawing, img_process.GetBoundingBox()[index].tl(), img_process.GetBoundingBox()[index].br(), Scalar(178, 34, 34), 2, 8, 0);
	}
}

