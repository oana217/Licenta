#define WINVER 0x0500
#include <windows.h>
#include "!!!image.h"
#include "!!!processing.h"
#include "!!!drawing.h"
#include "!!!decision.h"

#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int main() {
	Point p1(15, 15);
	Point p2(15, 15);
	int count = 0;
	int prevGestureIndex = 0;
	int validGestureIndex = 0, prevValidGestureIndex = 0, combo = 0;
	bool first = true;
	VideoCapture camera(0);
	if (!camera.isOpened()) {
		cout << "Could not open camera." << endl;
		return -1;
	}

	MyImage img;
	img.SetCamera(camera);

	while (true) {
		img.Setup();
		Mat img_roi = img.GetROI();
		Mat img_converted = img.GetConvertedImage(); //these are just for imshow
		Mat img_thresholded = img.GetThresholdedImage();

		ImageProcess img_processed(img_thresholded);
		img_processed.GetData();

		Draw img_draw(img_processed);

		Decision decision(img_processed);
		decision.CalculateGesture();
		int gestureIndex = decision.GetGestureIndex();
		if (gestureIndex >= 1) {
			if (gestureIndex == prevGestureIndex) {
				count++;				
			}
			else {
				count = 0;
			}
			prevGestureIndex = gestureIndex;

		}
		else {
			validGestureIndex = 0;
			count = 0;
		}
		if (count == 48) {
			validGestureIndex = gestureIndex;
			count = 0;
		}

		if (first) {
			if (validGestureIndex == 3 || validGestureIndex == 7) {
				decision.DecideGesture(gestureIndex);
				validGestureIndex = 0;
			}
			else
				if (validGestureIndex == 4 || validGestureIndex == 5 || validGestureIndex == 6) {
					prevValidGestureIndex = validGestureIndex;
					first = false;
			}
		}

		else {
			if ((prevValidGestureIndex != 5) && (validGestureIndex  == 1 || validGestureIndex  == 2 || validGestureIndex  == 3 || validGestureIndex  == 5 || validGestureIndex  == 8 || validGestureIndex  == 9)) {
				combo = prevValidGestureIndex * 10 + validGestureIndex;
				decision.DecideGesture(combo);
				validGestureIndex = 0;
				prevValidGestureIndex = 0;
				first = true;
			}
		}

		//draw progress line
		p2.x = count * 6;
		line(img_roi, p1, p2, Scalar(0, 255, 0), 3, 8);

		//draw gesture name
		img_draw.DrawGesture(gestureIndex);
		//img_draw.DrawGesture(validGestureIndex);

		//draw gesture
		img_draw.DrawData();

		Mat drw = img_draw.GetDrawing();
		imshow("roi", img_roi);
		imshow("converted", img_converted);
		imshow("thresholded", img_thresholded);
		imshow("drw", drw);
		imshow("db", decision.debug);
		if (waitKey(30) == 27) {
			cout << "User has pressed Esc." << endl;
			return -1;
		}
	}
}