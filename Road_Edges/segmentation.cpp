#include "utils.h"




Mat road_edges::white_line(Mat in)
{
	Mat out ;
	//cvtColor(out, out, CV_GRAY2BGR);
	putText(out, "white line", Point(20, 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 1.5, 8);

	return out;
}


Mat road_edges::hsvDetect(Mat x, Rect mask, bool press)
{
	Mat in = x.clone();
	Mat out;

	cvtColor(in, in, COLOR_BGR2HSV);

	inRange(in, Scalar(hue, saturation, value), Scalar(HUE, SATURATION, VALUE), out);

	erode(out, out, getStructuringElement(MORPH_ELLIPSE, Size(3, 3))); // Eliminate noise and smaller objects
	dilate(out, out, getStructuringElement(MORPH_ELLIPSE, Size(8, 8)));


	cvtColor(out, out, CV_GRAY2BGR);
	putText(out, "Segmentation", Point(20, 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 1.5, 8);
	



	vector<Mat> bgr_planes;
	split(in(mask), bgr_planes); // split tranch in(mask)  to 3 matrix red blue green

	static double minh, maxh, mins, maxs, minv, maxv; // all function use double ther is no int

	if (press)
	{
		minMaxLoc(bgr_planes[0], &minh, &maxh);
		minMaxLoc(bgr_planes[1], &mins, &maxs);
		minMaxLoc(bgr_planes[2], &minv, &maxv);

		hue = minh - 5;
		HUE = maxh + 5;
		saturation = mins - 5;
		SATURATION = maxs + 5;
		value = minv - 15;
		VALUE = maxv + 15;
	}

	return out; // return a gray pic
}


Mat road_edges::contour(Mat in)
{
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(in, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	RNG rng(12345);

	/// Draw contours
	Mat drawing = Mat::zeros(in.size(), CV_8UC3);
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(in, contours, i, color, 2, 8, hierarchy, 0, Point());
	}

	return drawing;
}