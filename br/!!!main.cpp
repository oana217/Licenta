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
	int count = 0;
	int prevGestureIndex = 0;
	int validGestureIndex = 0;
	VideoCapture camera(0);
	if (!camera.isOpened()) {
		cout << "Could not open camera." << endl;
		return -1;
	}

	MyImage img;
	img.SetCamera(camera);

	while (true) {
		/*INPUT ip;

		// Pause for 5 seconds.
		Sleep(1000);

		// Set up a generic keyboard event.
		ip.type = INPUT_KEYBOARD;
		ip.ki.wScan = 0; // hardware scan code for key
		ip.ki.time = 0;
		ip.ki.dwExtraInfo = 0;

		// Press the "UP" key
		ip.ki.wVk = 0x26; // virtual-key code for the "UP" key
		ip.ki.dwFlags = 0; // 0 for key press
		SendInput(1, &ip, sizeof(INPUT));

		// Release the "A" key
		ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
		SendInput(1, &ip, sizeof(INPUT));*/

		img.Setup();
		Mat img_roi = img.GetROI();
		Mat img_converted = img.GetConvertedImage(); //these are just for imshow
		Mat img_thresholded = img.GetThresholdedImage();

		ImageProcess img_processed(img_thresholded);
		img_processed.GetData();

		Draw img_draw(img_processed);

		Decision decision(img_processed);
		decision.DecideGesture();
		int gestureIndex = decision.GetGestureIndex();
		if (gestureIndex >= 1) {
			if (gestureIndex == prevGestureIndex) {
				count++;
				//img_draw.DrawGesture(gestureIndex);
			}
			else {
				count = 0;
			}
			prevGestureIndex = gestureIndex;

		}
		if (count == 10) {
			//img_draw.SetGestureIndex(gestureIndex);
			validGestureIndex = gestureIndex;
			count = 0;
		}
		img_draw.DrawGesture(validGestureIndex);
		//validGestureIndex = 0;
		img_draw.DrawData();
		Mat drw = img_draw.GetDrawing();

		imshow("roi", img_roi);
		imshow("converted", img_converted);
		imshow("thresholded", img_thresholded);
		imshow("drw", drw);
		if (waitKey(30) == 27) {
			cout << "User has pressed Esc." << endl;
			return -1;
		}
	}
}