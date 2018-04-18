#include<opencv2\opencv.hpp>
#include "Skin_Detector.h"
using namespace std;
using namespace cv;

int main()
{
	VideoCapture capture;
	//open capture object at location zero (default location for webcam)
	capture.open(0);

	//set height and width of capture frame
	//capture.set(CV_CAP_PROP_FRAME_WIDTH, 320);
	//capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	Mat cameraFeed;

	SkinDetector mySkinDetector;

	Mat skinMat;

	//start an infinite loop where webcam feed is copied to cameraFeed matrix
	//all of our operations will be performed within this loop
	while (1) {

		//store image to matrix
		capture.read(cameraFeed);

		//show the current image
		imshow("Original Image", cameraFeed);

		skinMat = mySkinDetector.getSkin(cameraFeed);

		imshow("Skin Image", skinMat);

		waitKey(30);
	}
	return 0;
}