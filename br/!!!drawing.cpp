#include "!!!drawing.h"

#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Draw::Draw(ImageProcess &img_process) {
	 this->img_process = img_process; 
}

void Draw::DrawContour() {
	if (img_process.IndexOfBiggestContour() > -1)
		//polylines(drawing, img_process.GetBiggestContour(), true, Scalar(255, 255, 255), 2, 8, 0);
		drawContours(drawing, img_process.GetBiggestContour(), img_process.IndexOfBiggestContour(), Scalar(255, 255, 255), 2, 8, img_process.GetHierarchy(), 0, Point());

}

