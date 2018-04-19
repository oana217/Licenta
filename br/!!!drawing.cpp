#include "!!!drawing.h"

#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Draw::Draw(ImageProcess &img_process) {
	 this->img_process = img_process; 
	 drawing = Mat::zeros(MyImage::A.getImgsize(), CV_8UC3);

}

void Draw::DrawContour() {
	int a = img_process.IndexOfBiggestContour();
	if (a > -1)
		//polylines(ken, img_process.GetBiggestContour(), true, Scalar(255, 255, 255), 2, 8, 0);
		drawContours(drawing, img_process.contours, a, Scalar(255, 255, 255), 2, 8, img_process.hierarchy, 0, Point());

}

