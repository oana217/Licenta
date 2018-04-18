#ifndef DRAWING_H
#define DRAWING_H

#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include "!!!processing.h"

using namespace cv;
using namespace std;

class Draw {
public:
	Draw(ImageProcess &img_process);
	void DrawContour();
	Mat GetDrawing() { return drawing; }
private:
	ImageProcess img_process;
	Mat drawing;
	
};

#endif //!DRAWING_H