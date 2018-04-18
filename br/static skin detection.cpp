#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

int main() {
	Mat src = imread("C:\\Users\\osaman\\Desktop\\licenta\\hand.jpg");
	if (src.empty())
		return -1;
	blur(src, src, Size(3, 3));
	Mat hsv;
	cvtColor(src, hsv, CV_BGR2HSV);
	Mat bw;
	inRange(hsv, Scalar(0, 10, 60), Scalar(50, 150, 255), bw);
	imshow("src", src);
	imshow("dst", bw);
	waitKey(0);
	return 0;
}