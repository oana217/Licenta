#ifndef DECISION_H
#define DECISION_H

#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include "!!!drawing.h"

using namespace cv;
using namespace std;

class Decision {
public:
	Decision(ImageProcess &img_process);
	int GetGestureIndex() { return gestureIndex; }
	void CalculateGesture();
	void DecideGesture(int gestureIndex);
	Mat debug;
	Point dbpt;

private:
	ImageProcess img_process;
	int gestureIndex;
	struct Gesture {
		int id;
		char command;
	};
	vector<Gesture> gesture;
};

#endif //!DECISION_H