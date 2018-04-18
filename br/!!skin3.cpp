#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <Windows.h>
#include "!!!processing.h"

using namespace cv;
using namespace std;

int findBiggestContour(vector<vector<Point> > contours);
float getDistance(Point a, Point b);
float getAngle(Point s, Point f, Point e);
float getAngleXAxis(Point s, Point e);

vector<vector<Point>> hulls(contours.size());
vector<vector<int>> hullsI(contours.size()); //indices to contour points 
vector<vector<Vec4i>> defects(contours.size());
vector<vector<Point>> contours_poly(contours.size());

int main() {

	Mat img_thresholded;
	
	vector<Point> finger_points;
	vector<Rect> boundRect(contours.size());


	if (!camera.isOpened()) {
		cout << "Could not open camera." << endl;
		return -1;
	}

	while (true)
	{
		



		img_thresholded = processRawImage(img_raw_roi);





			for (size_t i = 0; i < contours.size(); i++)
			{

			}

			int noOfDefects = 0;
			Mat drawing = Mat::zeros(img_raw_roi.size(), CV_8UC3);

			//draw defect points
			for (int i = 0; i < contours_poly.size(); i++)
			{
				size_t count = contours_poly[i].size();
				std::cout << "Count : " << count << std::endl;
				if (count < 200)
					continue;

				vector<Vec4i>::iterator d = defects[i].begin();
				while (d != defects[i].end())
				{
					Vec4i& v = (*d);
					if (indexOfBiggestContour == i)
					{
						float depth = v[3] / 256; // distance between the farthest point and the convex hull
						if (depth > 20)
						{
							int startidx = v[0];
							Point ptStart(contours_poly[i][startidx]); // point of the contour where the defect begins
							int endidx = v[1];
							Point ptEnd(contours_poly[i][endidx]); // point of the contour where the defect ends
							int faridx = v[2];
							Point ptFar(contours_poly[i][faridx]); // the farthest from the convex hull point within the defect

							if (getDistance(ptStart, ptFar) > boundRect[i].height / 5 //finger length must be bigger than 20% of the box
								&& getDistance(ptEnd, ptFar) > boundRect[i].height / 5
								&& getAngle(ptStart, ptFar, ptEnd) < 110) //angle between fingers must be less than 90 degrees
																		  //&& cos(getAngle(ptStart, ptFar, ptEnd))<=-0.8)
							{
								line(drawing, ptStart, ptFar, CV_RGB(0, 255, 0), 2);
								line(drawing, ptEnd, ptFar, CV_RGB(0, 255, 0), 2);
								circle(drawing, ptStart, 4, Scalar(100, 0, 255), 2);
								circle(drawing, ptFar, 4, Scalar(255, 0, 100), 2);
								//circle(drawing, ptEnd, 8, Scalar(100, 0, 255), 2);

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
				}
			}

			//	if (indexOfBiggestContour > -1)  //if valid contour exists
			{
				//draw biggest contour and its hull
				drawContours(drawing, contours_poly, indexOfBiggestContour, Scalar(255, 255, 255), 2, 8, hierarchy, 0, Point());
				drawContours(drawing, hulls, indexOfBiggestContour, Scalar(124, 100, 233), 2, 8, vector<Vec4i>(), 0, Point());

				//draw the bounding rectangle
				boundRect[indexOfBiggestContour] = boundingRect(Mat(contours_poly[indexOfBiggestContour]));
				rectangle(drawing, boundRect[indexOfBiggestContour].tl(), boundRect[indexOfBiggestContour].br(), Scalar(178, 34, 34), 2, 8, 0);

				float box_ratio = ((float)boundRect[indexOfBiggestContour].height / (float)boundRect[indexOfBiggestContour].width);

				//string txt = to_string(area_ratio);
				Point pt(10, 300);
				Point pt1(10, 100);

				//putText(drawing, txt, pt1, FONT_HERSHEY_COMPLEX_SMALL, 2, (255, 255, 255), 2, 8);

				//gesture detection
				if (noOfDefects == 0)
				{
					float box_area = boundRect[indexOfBiggestContour].area();
					float contour_area = contourArea(hulls[indexOfBiggestContour]);
					float area_ratio = contour_area / box_area;

					//if area < 50% bounding box
					if (area_ratio < 0.5)
					{
						int y = boundRect[indexOfBiggestContour].tl().y; //these coordinates are for finding out the top right corner of the bounding box
						int x = boundRect[indexOfBiggestContour].br().x;

						double r_distance = pointPolygonTest(hulls[indexOfBiggestContour], Point(x, y), true);
						double l_distance = pointPolygonTest(hulls[indexOfBiggestContour], boundRect[indexOfBiggestContour].tl(), true);

						// if left corner is covered by hand
						if (l_distance > -5 && l_distance < 5)
						{
							putText(drawing, "ROTATION - LEFT", pt, FONT_HERSHEY_COMPLEX_SMALL, 1, (255, 255, 255), 2, 8);
						}

						// if right corner is covered by hand
						if (r_distance > -5 && l_distance < 5)
						{
							putText(drawing, "ROTATION - RIGHT", pt, FONT_HERSHEY_COMPLEX_SMALL, 1, (255, 255, 255), 2, 8);
						}
					}

					else
					{
						if (box_ratio > 1.8 && box_ratio < 2.4)
						{
							putText(drawing, "EXTENSION - CLOSED", pt, FONT_HERSHEY_COMPLEX_SMALL, 1, (255, 255, 255), 2, 8);
						}
						if (box_ratio > 1.3 && box_ratio < 1.8)
						{
							putText(drawing, "CLAW", pt, FONT_HERSHEY_COMPLEX_SMALL, 1, (255, 255, 255), 2, 8);
						}
						if (box_ratio > 0.9 && box_ratio < 1.3)
						{
							putText(drawing, "FIST", pt, FONT_HERSHEY_COMPLEX_SMALL, 1, (255, 255, 255), 2, 8);
						}
						if (box_ratio > 2.4 && box_ratio < 4.5)
						{
							putText(drawing, "ROTATION - UP", pt, FONT_HERSHEY_COMPLEX_SMALL, 1, (255, 255, 255), 2, 8);
						}

					}
				}

				if (noOfDefects == 1)
				{
					float angle = getAngleXAxis(finger_points[1], finger_points[0]);
					//string txt = to_string(angle);
					//string txt = to_string(getAngle(finger_points[0], finger_points[1], finger_points[2]));
					//putText(drawing, "0", finger_points[0], FONT_HERSHEY_COMPLEX_SMALL, 2, (255, 255, 255), 2, 8);
					//putText(drawing, "1", finger_points[1], FONT_HERSHEY_COMPLEX_SMALL, 2, (255, 255, 255), 2, 8);
					//putText(drawing, "2", finger_points[2], FONT_HERSHEY_COMPLEX_SMALL, 2, (255, 255, 255), 2, 8);
					//putText(drawing, txt, pt, FONT_HERSHEY_COMPLEX_SMALL, 2, (255, 255, 255), 2, 8);
					if (angle > 75 && angle < 105)
					{
						putText(drawing, "THUMBS UP", pt, FONT_HERSHEY_COMPLEX_SMALL, 1, (255, 255, 255), 2, 8);
					}
					if (angle > 165 && angle < 195)
					{
						putText(drawing, "L SIGN", pt, FONT_HERSHEY_COMPLEX_SMALL, 1, (255, 255, 255), 2, 8);
					}

					//old
					/*
					float d1 = getDistance(finger_points[0], finger_points[1]); //thumb
					float d2 = getDistance(finger_points[1], finger_points[2]); //index
					if (getAngle(finger_points[0], finger_points[1], finger_points[2])> 70 && getAngle(finger_points[0], finger_points[1], finger_points[2]) < 110)
					{
					float finger_ratio = (float)(d2 / d1);


					if (finger_ratio > 0.7 && finger_ratio < 1.1) //THUMBS UP
					{
					//	putText(drawing, "THUMBS UP", pt, FONT_HERSHEY_COMPLEX_SMALL, 2, (255, 255, 255), 2, 8);

					}
					//if (finger_ratio > 1.4 && finger_ratio < 1.8) //LOSER SIGN
					{
					//	putText(drawing, "LOSER SIGN", pt, FONT_HERSHEY_COMPLEX_SMALL, 2, (255, 255, 255), 2, 8);
					//algoritm rotire. miscare deget aratator->pumn->calculare unghi-> decizie daca s-a rotit destul
					}
					}*/
				}
				if (noOfDefects == 4) {
					putText(drawing, "EXTENSION - PARTED", pt, FONT_HERSHEY_COMPLEX_SMALL, 1, (255, 255, 255), 2, 8);
				}
			}
			cv::imshow("drw", drawing);
			cv::imshow("image", img_raw);
			cv::imshow("converted", img_thresholded);
			cv::imshow("imageraw", img_raw_roi);

		}


		if (waitKey(30) == 27) {
			cout << "User has pressed Esc." << endl;
			return -1;
		}
	}
}


