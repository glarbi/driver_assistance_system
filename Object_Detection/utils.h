#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp> // canny
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/dnn.hpp>

#include <omp.h>   // after enable it from paraMatre -> c++ -> langage -> openmp oui
#include <fstream> // fstream to read write create files
#include <time.h>
#include <iostream>
#include <chrono>

//#include <bits/stdc++.h> // include all standard libraries.
//#include <CL/cl.h>  // opencl
//#define CL_TARGET_OPENCL_VERSION 220




#define frame_width 960 //  <--600-->
#define frame_height 540



using namespace std;
using namespace cv;
using namespace dnn;

class yolo
{

  public:
    //int frame_height , frame_width ;
	float confThreshold, nmsThreshold; // Confidence threshold

	vector<string> classes;
	vector<String> output_layer;

	Point p00,p01,p10,p11;

	yolo(string, string, string, vector<String>, float, float, Mat);


	int left_vect  [frame_height] = {0};
	int right_vect [frame_height] = {0};

	//int left_vect  [] ;
	//int right_vect [] ;


    int safe_dist, risk_dist;

	Net net;


	bool drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat &frame);
	bool postprocess(Mat &pic, const vector<Mat> &outs); // Remove the bounding boxes with low confidence using non-maxima suppression

	void eyeshot(Mat in, int[], int[] ,bool );
	Mat safe_road(Mat);
	int car_position(Mat in);
};
