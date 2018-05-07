#define WINVER 0x0500
#include <windows.h>
#include "!!!decision.h"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Decision::Decision(ImageProcess &img_process) {
	this->img_process = img_process;
	debug = Mat::zeros(img_process.img_thresholded.size(), CV_8UC3);
	dbpt.x = 10;
	dbpt.y = 300;
	gesture.resize(10);
	gesture.push_back({  3,'W' });	//New piece				W 0x57
	gesture.push_back({  7,'Q' });	//Start Game/Game Over	Q 0x51
	gesture.push_back({ 61,'E' });	//Rotate left			E 0x45
	gesture.push_back({ 62,'R' });	//Rotate right			R 0x52
	gesture.push_back({ 43,'A' });	//Translate left		A 0x41
	gesture.push_back({ 45,'D' });	//Translate right		D 0x44
	gesture.push_back({ 58,'S' });	//Fall one square		S 0x53
	gesture.push_back({ 59,'X' });	//Fall down				X 0x58	
}

void Decision::DecideGesture(int gestureIndex) {
	string txt = to_string(gestureIndex);
	putText(debug, txt, dbpt, FONT_HERSHEY_COMPLEX_SMALL, 1, (255, 255, 255), 2, 8);
	INPUT ip;

	// Set up a generic keyboard event.
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0; // hardware scan code for key
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	// Press the "UP" key
	ip.ki.wVk = 0x26; // virtual-key code for the "UP" key
	ip.ki.dwFlags = 0; // 0 for key press
	SendInput(1, &ip, sizeof(INPUT));
	// Pause for 1 second.
	Sleep(500);
	// Release the "UP" key
	ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
	SendInput(1, &ip, sizeof(INPUT));
	//Sleep(1000);
}

void Decision::CalculateGesture() {
	gestureIndex = 0;
	int i = img_process.IndexOfBiggestContour();
	if (img_process.isValid == true) {
		
		float box_ratio = ((float)img_process.GetBoundingBox()[i].height / (float)img_process.GetBoundingBox()[i].width);
		if (img_process.GetNoOfDefects() == 0) {
			int box_area = img_process.GetBoundingBox()[i].area();
			double contour_area = contourArea(img_process.GetHulls()[i]);
			double  area_ratio = contour_area / box_area;
			//string txt = to_string(area_ratio);
			//putText(debug, txt, dbpt, FONT_HERSHEY_COMPLEX_SMALL, 1, (255, 255, 255), 2, 8);	
			//if area < 50% bounding box
			if (area_ratio < 0.5)
			{
				int y = img_process.GetBoundingBox()[i].tl().y; //these coordinates are for finding out the top right corner of the bounding box
				int x = img_process.GetBoundingBox()[i].br().x;

				double r_distance = pointPolygonTest(img_process.GetHulls()[i], Point(x, y), true);
				double l_distance = pointPolygonTest(img_process.GetHulls()[i], img_process.GetBoundingBox()[i].tl(), true);

				// if left corner is covered by hand
				if (l_distance > -5 && l_distance < 5)
				{
					gestureIndex = 1;
					//putText(drawing, "ROTATION - LEFT", pt, FONT_HERSHEY_COMPLEX_SMALL, 1, (255, 255, 255), 2, 8);
				}

				// if right corner is covered by hand
				if (r_distance > -5 && l_distance < 5)
				{
					gestureIndex = 2;
					//putText(drawing, "ROTATION - RIGHT", pt, FONT_HERSHEY_COMPLEX_SMALL, 1, (255, 255, 255), 2, 8);
				}
			}

			else
			{
				if (box_ratio > 0.9 && box_ratio < 1.3)
				{
					gestureIndex = 3;
					//putText(drawing, "FIST", pt, FONT_HERSHEY_COMPLEX_SMALL, 1, (255, 255, 255), 2, 8);
				}

				if (box_ratio > 1.3 && box_ratio < 1.8)
				{
					gestureIndex = 4;
					//putText(drawing, "CLAW", pt, FONT_HERSHEY_COMPLEX_SMALL, 1, (255, 255, 255), 2, 8);
				}

				if (box_ratio > 1.8 && box_ratio < 2.4)
				{
					gestureIndex = 5;
					//putText(drawing, "EXTENSION - CLOSED", pt, FONT_HERSHEY_COMPLEX_SMALL, 1, (255, 255, 255), 2, 8);
				}

				if (box_ratio > 2.4 && box_ratio < 4.5)
				{
					gestureIndex = 6;
					//putText(drawing, "ROTATION - UP", pt, FONT_HERSHEY_COMPLEX_SMALL, 1, (255, 255, 255), 2, 8);
				}
			}
		}

		if (img_process.GetNoOfDefects() == 1) {
			double angle = img_process.GetAngleXAxis(img_process.GetFingersPoints()[0][1], img_process.GetFingersPoints()[0][0]);
			//string txt = to_string(angle);
			//putText(debug, txt, dbpt, FONT_HERSHEY_COMPLEX_SMALL, 1, (255, 255, 255), 2, 8);			
			if (angle > 70 && angle < 110)
			{
				gestureIndex = 7;
				//putText(drawing, "THUMBS UP", pt, FONT_HERSHEY_COMPLEX_SMALL, 1, (255, 255, 255), 2, 8);
			}
			if (angle > 165 && angle < 225)
			{
				gestureIndex = 8;
				//putText(drawing, "L SIGN", pt, FONT_HERSHEY_COMPLEX_SMALL, 1, (255, 255, 255), 2, 8);
			}
		}

		if (img_process.GetNoOfDefects() == 4) {
			gestureIndex = 9;
			//putText(drawing, "EXTENSION - PARTED", pt, FONT_HERSHEY_COMPLEX_SMALL, 1, (255, 255, 255), 2, 8);
		}
	}
}