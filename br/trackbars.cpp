#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
using namespace std;

int findBiggestContour(vector<vector<Point> >);
Mat src, bw, hsv;

/// Function header
void thresh_callback(int, void*);

int low_h = 30, low_s = 30, low_v = 30;
int high_h = 100, high_s = 100, high_v = 100;

int main(int argc, char** argv) {
	Mat src = imread("C:\\Users\\osaman\\Desktop\\claw.jpg");
	if (src.empty())
		return -1;
	blur(src, src, Size(3, 3));

	//Mat hsv;
	cvtColor(src, hsv, CV_BGR2HSV);
	
	namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

												//Create trackbars in "Control" window
	createTrackbar("LowH", "Control", &low_h, 179, thresh_callback); //Hue (0 - 179)
	createTrackbar("HighH", "Control", &high_h, 179, thresh_callback);

	createTrackbar("LowS", "Control", &low_s, 255, thresh_callback); //Saturation (0 - 255)
	createTrackbar("HighS", "Control", &high_s, 255, thresh_callback);

	createTrackbar("LowV", "Control", &low_v, 255, thresh_callback); //Value (0 - 255)
	createTrackbar("HighV", "Control", &high_v, 255, thresh_callback);

	thresh_callback(0, 0);

	waitKey(0);
	return 0;
}

int findBiggestContour(vector<vector<Point> > contours) {
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

/** @function thresh_callback */
void thresh_callback(int, void*)
{
	//Mat bw;
	inRange(hsv, Scalar(low_h, low_s, low_v), Scalar(high_h, high_s, high_v), bw);
	imshow("src", src);
	imshow("dst", bw);
    Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	findContours(bw, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	int s = findBiggestContour(contours);

	Mat drawing = Mat::zeros(src.size(), CV_8UC1);
	drawContours(drawing, contours, s, Scalar(255), -1, 8, hierarchy, 0, Point());

	imshow("drw", drawing);
}