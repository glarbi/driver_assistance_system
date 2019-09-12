#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp" // canny
#include "opencv2/opencv.hpp"
#include <opencv2/objdetect/objdetect.hpp>
#include <omp.h> // after enable it from paraMatre -> c++ -> langage -> openmp oui
#include <iostream>
#include <chrono>


using namespace std;
using namespace cv;



//#define frame_width  960 //  <--600-->
//#define frame_height 540
//#define half_height  230  // the height of the half frame

class road_edges
{

  public:
	Mat raw_frame ,  mask , red_mask , bird_Mat ; // bird map for warpPerspective function parametre

	int frame_width , frame_height, half_height ,half_width  ;

	Point p00,p10,p01,p11;

	Rect rect_seg ,rect_raw;



	road_edges(Mat);

  Mat  eyebird(Mat);
	void eyeshot();
  void draw_eyeshot(Mat in);

  Mat boost(Mat); // in should be gray pic
	Mat proba_hougline(Mat,Mat);

	Mat hsvDetect(Mat, bool);  //segmentation
	Mat white(Mat); //return an rgb matrix



	Mat show(Mat, Mat, Mat,Mat,Mat,Mat);

  int find_right_side(Mat in, int stage);  
  int find_left_side (Mat in, int stage);  
	//vector <int> car_position(Mat white, Mat canny, Mat hsv, Mat out , int vertical_stage );
	String car_position(Mat white, Mat canny, Mat hsv, Mat out , int vertical_stage );


	Mat blending(Mat);
	Mat contour(Mat);

};


