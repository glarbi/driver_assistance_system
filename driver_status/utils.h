#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <iostream>

#define DLIB_JPEG_SUPPORT

//#include <bits/stdc++.h> // include all standard libraries.
//#include <CL/cl.h>  // opencl

using namespace std;
using namespace cv;
using namespace dlib;

class dlib_face
{

public:
	image_window win;
	shape_predictor sp;
	std::vector<cv::Point> righteye;
	std::vector<cv::Point> lefteye;
	cv::Point p;
    float ear_threshold;
	
	full_object_detection shape;
	frontal_face_detector detector;
	Mat frame;



	dlib_face(string,float);  //float ear threshold
	bool blink(std::vector<cv::Point> vec);
	
};
