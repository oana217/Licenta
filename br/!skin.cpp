#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <Windows.h>
using namespace cv;
using namespace std;

int findBiggestContour(vector<vector<Point> > contours);
float getDistance(Point a, Point b);
float getAngle(Point s, Point f, Point e);
void getThreshold(vector<Vec3b> vect);
vector<Vec3b> analyzeHand();

Mat img_raw;
Mat img_converted, img_convertedd;
Mat img_thresholded;
int lowY, highY, lowCr, highCr, lowCb, highCb;
uchar lowB=255, highB=0, lowG=255, highG=0, lowR=255, highR=0;
vector<Vec3b> analyzeHand() {
	//show palm circles
	int w = img_raw.size().width;
	int h = img_raw.size().height;

	Point up(w / 2, h / 2 - 0.35 * h);
	Point down(w / 2, h / 2 + 0.35 * h);
	Point left(w / 2 - 0.175 * h, h / 2);
	Point right(w / 2 + 0.175 * h, h / 2);

	circle(img_raw, up, 10, Scalar(255, 255, 255), 4, 8, 0);
	circle(img_raw, down, 10, Scalar(255, 255, 255), 4, 8, 0);
	circle(img_raw, left, 10, Scalar(255, 255, 255), 4, 8, 0);
	circle(img_raw, right, 10, Scalar(255, 255, 255), 4, 8, 0);
	vector<Vec3b> colour_vector;
	cv::imshow("image", img_raw);

	//for (int i = 0; i < 10; i++)
	{
		cout << "Analyze Hand!" << endl;
		waitKey(0);
		//bool read = camera.read(img_raw);
		Vec3b colour_up = img_raw.at<Vec3b>(up);
		Vec3b colour_down = img_raw.at<Vec3b>(down);
		Vec3b colour_left = img_raw.at<Vec3b>(left);
		Vec3b colour_right = img_raw.at<Vec3b>(right);
		colour_vector.push_back(colour_up);
		colour_vector.push_back(colour_down);
		colour_vector.push_back(colour_left);
		colour_vector.push_back(colour_right);
	}

	return colour_vector;
}

void getThreshold(vector<Vec3b> vect) {	
	cout << "Threshold!" << endl;
	vector<Vec3b>::iterator it;
	for (it = vect.begin(); it != vect.end(); ++it) {
		Vec3b& v = (*it);

		cout << *it << endl;
		if (v[0] >= highB)
		{
			highB = v[0];
		}
		if (v[0] < lowB)
		{
			lowB = v[0];
		}

		if (v[1] >= highG)
		{
			highG = v[1];
		}
		if (v[1] < lowG)
		{
			lowG = v[1];
		}

		if (v[2] >= highR) 
		{
			highR = v[2];
		}
		if (v[2] < lowR)
		{
			lowR = v[2];
		}

	}
	cout << (int)lowB << endl;
	cout << (int)highB << endl;
	cout << (int)lowG << endl;
	cout << (int)highG << endl;
	cout << (int)lowR << endl;
	cout << (int)highR << endl;
}
int main() {
	VideoCapture camera(0);

	//VideoCapture camera("C:\\Users\\osaman\\Desktop\\licenta\\2.jpg");
	if (!camera.isOpened()) {
		cout << "Could not open camera." << endl;
		return -1;
	}
	string s = "\0";
	
	/*namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

	int iLowH = 0;
	int iHighH = 179;

	int iLowS = 0;
	int iHighS = 255;

	int iLowV = 0;
	int iHighV = 255;

	//Create trackbars in "Control" window
	cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
	cvCreateTrackbar("HighH", "Control", &iHighH, 179);

	cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
	cvCreateTrackbar("HighS", "Control", &iHighS, 255);

	cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
	cvCreateTrackbar("HighV", "Control", &iHighV, 255);*/


	

	while (true) {
		bool read = camera.read(img_raw);
		if (!read) {
			cout << "Could not read from camera." << endl;
			return -1;
		}

		blur(img_raw, img_raw, Size(3, 3));

		cvtColor(img_raw, img_converted, CV_BGR2HSV);
		inRange(img_converted, Scalar(0, 10, 60), Scalar(20, 150, 255), img_converted); //HSV skin
		cvtColor(img_converted, img_converted, CV_HSV2BGR);
		cvtColor(img_converted, img_converted, CV_BGR2YCrCb);

		inRange(img_converted, Scalar(80, 133, 77), Scalar(179, 173, 127), img_thresholded); //YCrCb skin

/*
		int w = img_raw.size().width;
		int h = img_raw.size().height;

		Point up(w / 2, h / 2 - 0.35 * h);
		Point down(w / 2, h / 2 + 0.35 * h);
		Point left(w / 2 - 0.175 * h, h / 2);
		Point right(w / 2 + 0.175 * h, h / 2);

		circle(img_raw, up, 10, Scalar(255, 255, 255), 4, 8, 0);
		circle(img_raw, down, 10, Scalar(255, 255, 255), 4, 8, 0);
		circle(img_raw, left, 10, Scalar(255, 255, 255), 4, 8, 0);
		circle(img_raw, right, 10, Scalar(255, 255, 255), 4, 8, 0);
		cv::imshow("image", img_raw);

		if (waitKey(30) == 13) {
			//vector<Vec3b> vect = analyzeHand();
			//show palm circles

			vector<Vec3b> colour_vector;

			//for (int i = 0; i < 10; i++)
			{
				cout << "Analyze Hand!" << endl;
				waitKey(0);
				//bool read = camera.read(img_raw);
				Vec3b colour_up = img_converted.at<Vec3b>(up);
				Vec3b colour_down = img_converted.at<Vec3b>(down);
				Vec3b colour_left = img_converted.at<Vec3b>(left);
				Vec3b colour_right = img_converted.at<Vec3b>(right);
				colour_vector.push_back(colour_up);
				colour_vector.push_back(colour_down);
				colour_vector.push_back(colour_left);
				colour_vector.push_back(colour_right);
			}
			getThreshold(colour_vector);
		}

		*/


		//threshold image

		//inRange(img_converted, Scalar(0, 0, 0), Scalar(180, 255, 50), img_thresholded); //HSV black
		//inRange(img_converted, Scalar(0, 0, 0), Scalar(30, 255, 255), img_thresholded); //YCrCb black

		//inRange(img_converted, Scalar(lowB, lowG, lowR), Scalar(highB, highG, highR), img_thresholded); //controls

		//morphological opening (remove small objects from the foreground)
		erode(img_thresholded, img_thresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(img_thresholded, img_thresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		//morphological closing (fill small holes in the foreground)
		dilate(img_thresholded, img_thresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(img_thresholded, img_thresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		findContours(img_thresholded, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

		vector<vector<Point>> hulls(contours.size());
		vector<vector<int>> hullsI(contours.size()); //indices to contour points 
		vector<vector<Vec4i>> defects(contours.size());

		vector<Rect> boundRect(contours.size());
		vector<vector<Point>> contours_poly(contours.size());
		vector<Point> finger_points;

		for (size_t i = 0; i < contours.size(); i++)
		{
			convexHull(Mat(contours[i]), hulls[i], false);
			convexHull(Mat(contours[i]), hullsI[i], false);

			if (hullsI[i].size() > 3)
				convexityDefects(contours[i], hullsI[i], defects[i]);

			approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
			boundRect[i] = boundingRect(Mat(contours_poly[i]));
		}

		int indexOfBiggestContour = findBiggestContour(contours);
		int noOfDefects = 0;

		Mat drawing = Mat::zeros(img_raw.size(), CV_8UC3);
		

		//draw defect points
		for (int i = 0; i < contours.size(); i++)
		{
			size_t count = contours[i].size();
			//std::cout << "Count : " << count << std::endl;
			if (count < 300)
				continue;

			vector<Vec4i>::iterator d = defects[i].begin();
			int j = 0;
			while (d != defects[i].end())
			{
				Vec4i& v = (*d);
				if (indexOfBiggestContour == i)
				{
					float depth = v[3] / 256; // distance between the farthest point and the convex hull
					if (depth > 20)
					{
						//s = j;
						int startidx = v[0];
						Point ptStart(contours[i][startidx]); // point of the contour where the defect begins
						int endidx = v[1];
						Point ptEnd(contours[i][endidx]); // point of the contour where the defect ends
						int faridx = v[2];
						Point ptFar(contours[i][faridx]); // the farthest from the convex hull point within the defect

						if (getDistance(ptStart, ptFar) > boundRect[i].height / 5 //finger length must be bigger than 20% of the box
							&& getDistance(ptEnd, ptFar) > boundRect[i].height / 5 
							&& getAngle(ptStart, ptFar, ptEnd) < 95) //angle between fingers must be less than 90 degrees
							{ 
								line(drawing, ptStart, ptFar, CV_RGB(0, 255, 0), 2);
								line(drawing, ptEnd, ptFar, CV_RGB(0, 255, 0), 2);
								circle(drawing, ptStart, 4, Scalar(100, 0, 255), 2);
								circle(drawing, ptFar, 4, Scalar(255, 0, 100), 2);
								circle(drawing, ptEnd, 8, Scalar(100, 0, 255), 2);

								noOfDefects++;
								if (noOfDefects == 1)
								{
									finger_points.push_back(ptStart);
									finger_points.push_back(ptFar);
									finger_points.push_back(ptEnd);
								}
								else
								{
									finger_points.clear();
								}
							}						
						}
					}
					d++; 
					//j++;
				}
			}
		
		if (indexOfBiggestContour > -1)  //if valid contour exists
		{
			drawContours(drawing, contours, indexOfBiggestContour, Scalar(255, 255, 255), 2, 8, hierarchy, 0, Point());
			drawContours(drawing, hulls, indexOfBiggestContour, Scalar(178, 34, 34), 2, 8, vector<Vec4i>(), 0, Point());
			rectangle(drawing, boundRect[indexOfBiggestContour].tl(), boundRect[indexOfBiggestContour].br(), Scalar(178, 34, 34), 2, 8, 0);

			float box_ratio = ((float)boundRect[indexOfBiggestContour].width / (float)boundRect[indexOfBiggestContour].height);
			string txt = to_string(noOfDefects);
			Point pt(10, 300);

			//gesture detection
			//putText(drawing, txt, pt, FONT_HERSHEY_COMPLEX_SMALL, 2, (255, 255, 255), 2, 8);

			if (noOfDefects == 0) {
				if (box_ratio > 1.8 && box_ratio < 2.5)
				{
			//		putText(drawing, "EXTENSION - CLOSE", pt, FONT_HERSHEY_COMPLEX_SMALL, 2, (255, 255, 255), 2, 8);

				}
				if (box_ratio > 1.4 && box_ratio < 1.8)
				{
				//	putText(drawing, "CLAW", pt, FONT_HERSHEY_COMPLEX_SMALL, 2, (255, 255, 255), 2, 8);

				}
				if (box_ratio > 0.9 && box_ratio < 1.4)
				{
					//putText(drawing, "FIST", pt, FONT_HERSHEY_COMPLEX_SMALL, 2, (255, 255, 255), 2, 8);

				}
			}

			if (noOfDefects == 1) {
				float d1 = getDistance(finger_points[0], finger_points[1]); //thumb
				float d2 = getDistance(finger_points[1], finger_points[2]); //index
				string txt = to_string(getAngle(finger_points[0], finger_points[1], finger_points[2]));
				//putText(drawing, txt, pt, FONT_HERSHEY_COMPLEX_SMALL, 2, (255, 255, 255), 2, 8);
				if (getAngle(finger_points[0], finger_points[1], finger_points[2])> 70 && getAngle(finger_points[0], finger_points[1], finger_points[2]) < 100)
				{
					float finger_ratio = (float)(d2 / d1);
					

					if (finger_ratio > 0.7 && finger_ratio < 1.1) //THUMBS UP
					{
						putText(drawing, "THUMBS UP", pt, FONT_HERSHEY_COMPLEX_SMALL, 2, (255, 255, 255), 2, 8);

					}
					if (finger_ratio > 1.4 && finger_ratio < 1.8) //LOSER SIGN
					{
						putText(drawing, "LOSER SIGN", pt, FONT_HERSHEY_COMPLEX_SMALL, 2, (255, 255, 255), 2, 8);
						//algoritm rotire. miscare deget aratator->pumn->calculare unghi-> decizie daca s-a rotit destul
					}
				}
			}
			if (noOfDefects == 4) {
				putText(drawing, "EXTENSION - FAR", pt, FONT_HERSHEY_COMPLEX_SMALL, 2, (255, 255, 255), 2, 8);
			}
		}

		cv::imshow("drw", drawing);
		cv::imshow("image", img_raw);
		cv::imshow("converted", img_converted);		

		if (waitKey(30) == 27) {
			cout << "User has pressed Esc." << endl;
			return -1;
		}
		//waitKey(0);
	}
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


float getDistance(Point a, Point b) {
	return sqrt(fabs(pow(a.x - b.x, 2) + pow(a.y - b.y, 2)));
}

float getAngle(Point s, Point f, Point e) {
	float l1 = getDistance(f, s);
	float l2 = getDistance(f, e);
	float dot = (s.x - f.x)*(e.x - f.x) + (s.y - f.y)*(e.y - f.y);
	float angle = acos(dot / (l1*l2));
	angle = angle * 180 / 3.14;
	return angle;
}
