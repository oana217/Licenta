int findBiggestContour(vector<vector<Point>> contours_poly)
{
	int indexOfBiggestContour = -1;
	int sizeOfBiggestContour = 0;
	for (int i = 0; i < contours_poly.size(); i++) {
		if (contours_poly[i].size() > sizeOfBiggestContour) {
			sizeOfBiggestContour = contours_poly[i].size();
			indexOfBiggestContour = i;
		}
	}
	return indexOfBiggestContour;
}


float getDistance(Point a, Point b)
{
	return sqrt(fabs(pow(a.x - b.x, 2) + pow(a.y - b.y, 2)));
}

float getAngle(Point s, Point f, Point e)
{
	float l1 = getDistance(f, s);
	float l2 = getDistance(f, e);
	float dot = (s.x - f.x)*(e.x - f.x) + (s.y - f.y)*(e.y - f.y);
	float angle = acos(dot / (l1*l2));
	angle = angle * 180 / 3.14;
	return angle;
}

float getAngleXAxis(Point s, Point e)
{
	float angle = atan2(s.y - e.y, e.x - s.x);
	angle = angle * 180 / 3.14;
	if (angle < 0) {
		angle = angle + 360;
	}
	return angle;
}