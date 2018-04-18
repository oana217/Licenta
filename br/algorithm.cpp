#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
using namespace std;

int findBiggestContour(vector<vector<Point> >);

void on_low_h_thresh_trackbar(int, void *);
void on_high_h_thresh_trackbar(int, void *);
void on_low_s_thresh_trackbar(int, void *);
void on_high_s_thresh_trackbar(int, void *);
void on_low_v_thresh_trackbar(int, void *);
void on_high_v_thresh_trackbar(int, void *);
int low_h = 30, low_s = 30, low_v = 30;
int high_h = 100, high_s = 100, high_v = 100;

int main() {
	namedWindow("Object Detection", CV_WINDOW_AUTOSIZE); //create a window called "Control"

	Mat frame_threshold;
	//-- Trackbars to set thresholds for RGB values
	createTrackbar("Low H", "Object Detection", &low_h, 255, on_low_h_thresh_trackbar);
	createTrackbar("High H", "Object Detection", &high_h, 255, on_high_h_thresh_trackbar);
	createTrackbar("Low S", "Object Detection", &low_s, 255, on_low_s_thresh_trackbar);
	createTrackbar("High S", "Object Detection", &high_s, 255, on_high_s_thresh_trackbar);
	createTrackbar("Low V", "Object Detection", &low_v, 255, on_low_v_thresh_trackbar);
	createTrackbar("High V", "Object Detection", &high_v, 255, on_high_v_thresh_trackbar);

	Mat src = imread("C:\\Users\\osaman\\Desktop\\90.jpg");
	if (src.empty())
		return -1;
	blur(src, src, Size(3, 3));

	Mat hsv;
	cvtColor(src, hsv, CV_BGR2HSV);

	Mat bw;
	inRange(hsv, Scalar(low_h, low_s, low_v), Scalar(high_h, high_s, high_v), bw);
	imshow("src", src);
	imshow("dst", bw);
	//imshow("Object Detection", bw);

	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	findContours(bw, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	int s = findBiggestContour(contours);

	Mat drawing = Mat::zeros(src.size(), CV_8UC1);
	drawContours(drawing, contours, s, Scalar(255), -1, 8, hierarchy, 0, Point());

	imshow("drw", drawing);
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

void on_low_h_thresh_trackbar(int, void *)
{
	low_h = min(high_h - 1, low_h);
	setTrackbarPos("Low H", "Object Detection", low_h);
}
void on_high_h_thresh_trackbar(int, void *)
{
	high_h = max(high_h, low_h + 1);
	setTrackbarPos("High H", "Object Detection", high_h);
}
void on_low_s_thresh_trackbar(int, void *)
{
	low_s = min(high_s - 1, low_s);
	setTrackbarPos("Low S", "Object Detection", low_s);
}
void on_high_s_thresh_trackbar(int, void *)
{
	high_s = max(high_s, low_s + 1);
	setTrackbarPos("High S", "Object Detection", high_s);
}
void on_low_v_thresh_trackbar(int, void *)
{
	low_v = min(high_v - 1, low_v);
	setTrackbarPos("Low V", "Object Detection", low_v);
}
void on_high_v_thresh_trackbar(int, void *)
{
	high_v = max(high_v, low_v + 1);
	setTrackbarPos("High V", "Object Detection", high_v);
}

