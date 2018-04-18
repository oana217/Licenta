#include <opencv2\opencv.hpp>
#include <vector>
#include <numeric>
using namespace cv;
using namespace std;


const string trackbarWindowName = "Trackbars";
int H_MIN = 0;
int H_MAX = 256;
int S_MIN = 0;
int S_MAX = 256;
int V_MIN = 0;
int V_MAX = 256;

int findBiggestContour(vector<vector<Point> > contours);
int IndexOfBiggestContour;

void on_trackbar(int, void*)
{//This function gets called whenever a
 // trackbar position is changed


}

void createTrackbars() {

	namedWindow(trackbarWindowName, 0);
	//create memory to store trackbar name on window
	char TrackbarName[50];
	sprintf(TrackbarName, "H_MIN", H_MIN);
	sprintf(TrackbarName, "H_MAX", H_MAX);
	sprintf(TrackbarName, "S_MIN", S_MIN);
	sprintf(TrackbarName, "S_MAX", S_MAX);
	sprintf(TrackbarName, "V_MIN", V_MIN);
	sprintf(TrackbarName, "V_MAX", V_MAX);
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH), 
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->      
	createTrackbar("H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar);
	createTrackbar("H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar);
	createTrackbar("S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar);
	createTrackbar("S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar);
	createTrackbar("V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar);
	createTrackbar("V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar);


}


int main(int argc, char* argv[])
{
	VideoCapture cap(0);
	Mat frame(Size(640, 420), CV_8UC3);
	Mat frame2(Size(640, 420), CV_8UC3);
	createTrackbars();
	if (!cap.isOpened())
		return -1;

	while (true) {
		cap >> frame;
		cap >> frame2;
		//blur( frame, frame, Size(3,3) );

		Size kSize;
		kSize.height = 3;
		kSize.width = 3;
		double sigma = 0.3*(3 / 2 - 1) + 0.8;
		GaussianBlur(frame, frame, kSize, sigma, 0.0, 4);

		Mat hsv(Size(640, 420), CV_8UC3);
		cvtColor(frame, hsv, CV_RGB2HSV);

		Mat bw(Size(640, 420), CV_8UC1);
		inRange(hsv, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), bw);


		vector<Vec4i> hierarchy;
		vector<vector<Point> > contours_hull;


		Mat Erode(Size(640, 420), CV_8UC1);
		cv::erode(bw, Erode, cv::Mat(), cv::Point(-1, -1));

		Mat Dialate(Size(640, 420), CV_8UC1);


		cv::dilate(Erode, Dialate, cv::Mat(), cv::Point(-1, -1), 2);


		findContours(Dialate.clone(), contours_hull, hierarchy, CV_RETR_TREE, CV_CLOCKWISE, Point(0, 0)); // CV_CHAIN_APPROX_SIMPLE

		if (contours_hull.size() > 0)
		{
			/// Find the convex hull object for each contour
			vector<vector<Point> >hull(contours_hull.size());
			//find the defects points for each contour
			vector<vector<Vec4i>> defects(contours_hull.size());

			vector<vector<int> > hullsI(contours_hull.size());

			//find the biggest contour
			IndexOfBiggestContour = findBiggestContour(contours_hull);

			Point2f rect_points[4];
			vector<RotatedRect> minRect(contours_hull.size());

			vector<vector<Point> > contours_poly(contours_hull.size());
			vector<Rect> boundRect(contours_hull.size());


			try {
				for (int i = 0; i < contours_hull.size(); i++)
				{
					convexHull(Mat(contours_hull[i]), hull[i], false);
					convexHull(Mat(contours_hull[i]), hullsI[i], false);
					convexityDefects(Mat(contours_hull[i]), hullsI[i], defects[i]);

					if (IndexOfBiggestContour == i)
					{
						minRect[i] = minAreaRect(Mat(contours_hull[i]));

						drawContours(frame2, contours_hull, IndexOfBiggestContour, CV_RGB(255, 255, 255), 2, 8, hierarchy, 0, Point());
						drawContours(frame2, hull, IndexOfBiggestContour, CV_RGB(255, 0, 0), 2, 8, hierarchy, 0, Point());

						approxPolyDP(Mat(contours_hull[i]), contours_poly[i], 3, true);
						boundRect[i] = boundingRect(Mat(contours_poly[i]));

						rectangle(frame2, boundRect[i].tl(), boundRect[i].br(), CV_RGB(0, 0, 0), 2, 8, 0);

						Point2f rect_points[4];
						minRect[i].points(rect_points);

						for (int j = 0; j < 4; j++)
						{
							line(frame2, rect_points[j], rect_points[(j + 1) % 4], CV_RGB(255, 255, 0), 2, 8);
						}

					}
				}
			}
			catch (Exception ex)
			{
				std::cout << "Object Not Found!!!" << std::endl;
			}

			for (int i = 0; i< contours_hull.size(); i++)
			{
				size_t count = contours_hull[i].size();
				std::cout << "Count : " << count << std::endl;
				if (count < 300)
					continue;

				vector<Vec4i>::iterator d = defects[i].begin();

				while (d != defects[i].end()) {
					Vec4i& v = (*d);
					if (IndexOfBiggestContour == i) {

						int startidx = v[0];
						Point ptStart(contours_hull[i][startidx]); // point of the contour where the defect begins
						int endidx = v[1];
						Point ptEnd(contours_hull[i][endidx]); // point of the contour where the defect ends
						int faridx = v[2];
						Point ptFar(contours_hull[i][faridx]); // the farthest from the convex hull point within the defect
						float depth = v[3] / 256; // distance between the farthest point and the convex hull


						if (depth > 20 && depth < 80)
						{
							line(frame2, ptStart, ptFar, CV_RGB(0, 255, 0), 2);
							line(frame2, ptEnd, ptFar, CV_RGB(0, 255, 0), 2);
							circle(frame2, ptStart, 4, Scalar(100, 0, 255), 2);
						}

					}
					d++;
				}
			}

			///imshow("Erode", Erode);
			//imshow("Dialate", Dialate);
			//imshow("Original Video", frame);
			//imshow("HSV Converted Video", hsv);
			imshow("BW Converted Video", bw);
			imshow("Frame 2", frame2);
		}
		if (cvWaitKey(15) == 27)  break;

	}

	return 0;
}

int findBiggestContour(vector<vector<Point> > contours)
{
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