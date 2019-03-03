#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp> // canny
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp>


#include <bits/stdc++.h> // include all standard libraries.
#include <omp.h>  
//#include <CL/cl.h>  // opencl


using namespace std;
using namespace cv;

class road_edges
{

public:
	Mat pic, gray_pic, canny_pic, hsv_pic, blur_pic, white_line;
	int pic_width, pic_height;

	road_edges(Mat, int);

	Mat proba_hougline(Mat);

	int car_position(Mat);
	Mat hsvDetect(Mat,Mat,bool);
	
	Mat rgbDetect(Mat);

	Mat road_line(Mat);
	Mat boost(Mat);
	Mat blending(Mat);
	Mat contour(Mat);
	Mat histo(Mat);
	Mat show(Mat, Mat, Mat);

	void timing(clock_t, clock_t, clock_t, clock_t, float);
	//static void onMouse(int, int, int, int, void *param);

	Mat GrabCut(Mat, Mat);
};

class object_detection
{

public:
	int pic_height, pic_width;
	Mat pic;

	object_detection(Mat);
	bool  detect(CascadeClassifier);
};

class face_detection
{

public:
	Mat pic;
	face_detection(Mat);
	bool  detect(CascadeClassifier,CascadeClassifier,Mat);
};
class connection
{
public:
	connection(char *);
	~connection();
	FILE *file;

    char *port ;
	void sendData(int);
};