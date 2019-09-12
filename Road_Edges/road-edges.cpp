#include "utils.h"
using namespace cv  ;
using namespace std ;


road_edges::road_edges(Mat x)
{   
	namedWindow("driver assistance", CV_WINDOW_AUTOSIZE); //create a window called "Control"
    namedWindow("Computer vision", CV_WINDOW_AUTOSIZE);
    moveWindow("Control", 4000, 0);
    moveWindow("Computer vision", 4000, 500);

	frame_width  = x.cols ;
    frame_height = x.rows ; 
	half_height  = frame_height/2 ;
    half_width   = frame_width /2 ;
	
    static int half = 200 ;
	cvCreateTrackbar("half_height ", "Control", &half , frame_height);

    cout <<"New  Size  : ("<< frame_width <<" x "<< frame_height <<")" << endl;// x.cols << x.rows 

	while (true)
    {   
		raw_frame = x.clone();
		half_height = half ;  
		rect_raw = Rect(0, (frame_height-half_height), frame_width, half_height);
	 
	    raw_frame = raw_frame(rect_raw); // rectangle the raw frame
     
        eyeshot();
        Mat bird_view = eyebird(raw_frame); // bird view need to initialise by eyshot
        
		Mat test ;
		draw_eyeshot(raw_frame);
		vconcat(raw_frame,bird_view,test);
        imshow("driver assistance",test);

		char c = (char)cv::waitKey(30);

		if (c == ' ')	
            break;

		rect_seg = Rect(frame_width/2-50, half_height/2, 100, 40);//segmentatin rectangle 

    }

    mask       = imread("pic/mask.png"       );
    resize(mask      ,mask      , Size(frame_width, half_height), 0, 0, INTER_CUBIC) ;
   
    Mat red(half_height, frame_width , CV_8UC3, cv::Scalar(0,0,255));
    red_mask = red.clone();
	 
}

Mat road_edges::boost(Mat in) // in should be gray pic
{

	//dilate(out, out, getStructuringElement(CV_SHAPE_RECT, Size(3, 3)));
    //erode(pic, pic, getStructuringElement(MORPH_ELLIPSE, Size(3, 3))); // Eliminate noise and smaller objects
    static int min   =  115; //50
	static int max   = 200;
	cvCreateTrackbar("min canny      ", "Computer vision", &min, 300);
	cvCreateTrackbar("max canny      ", "Computer vision", &max, 300);



    Mat out;
	GaussianBlur(in,in, Size(5,5), 0, 0); // smoothing
    cvtColor(in, in, CV_BGR2GRAY);   
	in.convertTo(out, CV_32F, 1.0 / 255.0);
	pow(out, 3.0, out);
	out *= 3.0;
	out.convertTo(out, CV_8U, 255.0);
	dilate(out, out, getStructuringElement(CV_SHAPE_RECT, Size(3, 3)));
	Canny(out, out, min, max, 3);
	//	cvtColor(out, out, COLOR_BGR2GRAY);

	return out;
}

Mat road_edges::proba_hougline( Mat canny_in,  Mat outt  ) //  Mat outt to print on it 
{  
	
	Mat out = outt.clone();

	static int threshold =  50;
	static int minLineLength  =  10;
	static int maxLineGap =  91;//50
	cvCreateTrackbar("threshold      ", "Computer vision", &threshold, 200);
	cvCreateTrackbar("minLineLength  ", "Computer vision", &minLineLength , 200);
	cvCreateTrackbar("maxLineGap     ", "Computer vision", &maxLineGap, 200);

	vector<Vec4i> lines;
    //HoughLinesP(out, lines, 1, CV_PI / 180, threshold, 30, 10); //(in, lines, 1, CV_PI / 180, 15, 15, 2);
	HoughLinesP(canny_in, lines, 1, CV_PI / 180, threshold, minLineLength, maxLineGap); 

 //------------------------------------ Draw lines ----------------------------------------------------------------------
    
	//cvtColor(out, out, CV_GRAY2BGR);  
	for (size_t i = 0; i < lines.size(); i++) //lines.size()
	{
		Vec4i l = lines[i];
		if (l[1] > 70 or l[3] > 70)
			line(out, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 2, CV_AA); // draw the line
	}

	//string t = "proba Hough time : " + to_string(0);
	//putText(out, "edge-HoughlinesP", Point(20, 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1.5, 8);
	return out;
}



Mat road_edges::eyebird(Mat in)
{
	Mat src = in.clone();
	
	Mat dst(half_height, frame_width , CV_8UC3);
	warpPerspective(src, dst, bird_Mat, dst.size(), INTER_LINEAR, BORDER_CONSTANT);
	
	//putText(dst, "Bird View", Point(20, 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 1.5, 8);

	return dst;
}



void road_edges::eyeshot()
{
	static int upy = 50 ;
	static int down = half_height;

	static int p00x = frame_width/2-50 ; // up
	static int p10x = frame_width/2+50 ;

	static int p01x = 0;   // down
	static int p11x = frame_width ;

	cvCreateTrackbar("up          ", "Control", &upy , frame_height);
	cvCreateTrackbar("down        ", "Control", &down, frame_height);
	cvCreateTrackbar("point 00    ", "Control", &p00x, frame_width);
	cvCreateTrackbar("point 10    ", "Control", &p10x, frame_width);
	cvCreateTrackbar("point 01    ", "Control", &p01x, frame_width);
	cvCreateTrackbar("point 11    ", "Control", &p11x, frame_width);

	p11.y = p01.y = down;
	p10.y = p00.y = upy;

	p00.x = p00x;  //up left
	p10.x = p10x;  //up right

	p01.x = p01x;  //down_left
	p11.x = p11x;  //down_right	

	//----------------------------- bird_view ----------------------------------
	Point2f src_vertices[4];
    src_vertices[0] = p00;
    src_vertices[1] = p10;
    src_vertices[2] = p11;
    src_vertices[3] = p01;

	Point2f dst_vertices[4];
	dst_vertices[0] = Point(0, 0); 
	dst_vertices[1] = Point(frame_width, 0);
	dst_vertices[2] = Point(frame_width, half_height);
	dst_vertices[3] = Point(0,   half_height);

	bird_Mat = getPerspectiveTransform(src_vertices, dst_vertices);
}

void road_edges::draw_eyeshot(Mat in)
{
	line(in, p00, p01, Scalar(0, 255, 0), 1);
	line(in, p10, p11, Scalar(0, 255, 0), 1);	
	line(in, p00, p10, Scalar(0, 255, 0), 1);	
}



Mat road_edges::hsvDetect(Mat x,  bool press)
{
	Mat in = x.clone();
	Mat out;

	cvtColor(in, in, COLOR_BGR2HSV);

	static int hue = 19; //4
	static int HUE = 120;
	static int saturation = 0;
	static int SATURATION = 75;
	static int value = 50;
	static int VALUE = 165;

	cvCreateTrackbar("Low-Hue        ", "Computer vision", &hue, 255);
	cvCreateTrackbar("High-Hue       ", "Computer vision", &HUE, 255);
	cvCreateTrackbar("Low-Saturation ", "Computer vision", &saturation, 255); //Saturation (0 - 255)
	cvCreateTrackbar("High-Saturation", "Computer vision", &SATURATION, 255);
	cvCreateTrackbar("Low-Value      ", "Computer vision", &value, 255); //Value (0 - 255)
	cvCreateTrackbar("High-Value     ", "Computer vision", &VALUE, 255);

	inRange(in, Scalar(hue, saturation, value), Scalar(HUE, SATURATION, VALUE), out);

	dilate(out, out, getStructuringElement(MORPH_ELLIPSE, Size(15, 15)));
	erode(out, out, getStructuringElement(MORPH_ELLIPSE, Size(8, 8))); // Eliminate noise and smaller objects

	vector<Mat> bgr_planes;
	split(in(rect_seg), bgr_planes); // split tranch in(mask)  to 3 matrix red blue green
	static double minh, maxh, mins, maxs, minv, maxv; // all function use double ther is no int

	if (press)
	{
		minMaxLoc(bgr_planes[0], &minh, &maxh);
		minMaxLoc(bgr_planes[1], &mins, &maxs);
		minMaxLoc(bgr_planes[2], &minv, &maxv);

		hue = minh - 5;
		HUE = maxh + 5;
		saturation = mins - 10;
		SATURATION = maxs + 10;
		value = minv - 15;
		VALUE = maxv + 15;
	}
	
	bitwise_not(out,out ); //reverse color white and black
    cvtColor(out,out,COLOR_GRAY2BGR);
    bitwise_and(out,red_mask,out);   //add red mask 
	
	return out; // return a gray pic
}

Mat road_edges::white (Mat in) //return an rgb matrix
{  
    Mat  white, white_rgb;
	white = in .clone();

    inRange(white, Scalar(100, 100, 200), Scalar(255, 255, 255), white); //

    dilate(white, white, getStructuringElement(CV_SHAPE_RECT, Size(5, 5))); // fill pixell
    erode (white, white, getStructuringElement(MORPH_ELLIPSE, Size(3, 3))); // Eliminate noise and smaller objects
    
	cvtColor(white, white_rgb,CV_GRAY2BGR);
    bitwise_and(white_rgb,red_mask,white_rgb);   //add red ma
    
	white_rgb = proba_hougline(white,white_rgb  ); // can in should be gray ,raw image should be rgb
   
        
    return white_rgb ;
}








