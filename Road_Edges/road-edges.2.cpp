#include "utils.h"
using namespace cv  ;
using namespace std ;


road_edges::road_edges(Mat x)
{   

	namedWindow("driver assistance", CV_WINDOW_AUTOSIZE); //create a window called "Control"
    namedWindow("Control", CV_WINDOW_AUTOSIZE);
    namedWindow("Computer vision", CV_WINDOW_AUTOSIZE);
    moveWindow("Control", 4000, 0);
    moveWindow("Computer vision", 4000, 500);


	frame_width  = x.cols ;
    frame_height = x.rows ; 
	half_height  = frame_height/2 ;
    half_width   = frame_width /2 ;

	
    //static int half = frame_height/2 ;
    static int half = 200 ;
	cvCreateTrackbar("half_height ", "Control", &half , frame_height);

    cout <<"New  Size  : ("<< frame_width <<" x "<< frame_height <<")" << endl;// x.cols << x.rows 

	

	while (true)
    {   
		raw_frame = x.clone();
		half_height = half ;  
		rect_raw = Rect(0, (frame_height-half_height), frame_width, half_height);
	 
	    raw_frame = raw_frame(rect_raw); // rectangle the raw frame
     
	    imshow("driver assistance",raw_frame);
       // eyeshot();
       // Mat bird_view = eyebird(raw_frame); // bird view need to initialise by eyshot
        
		//show(bird_view,bird_view,bird_view );
        
		char c = (char)cv::waitKey(30);

		if (c == ' ')	
            break;
		rect_seg = Rect(frame_width/2-50, half_height/2, 100, 40);//segmentatin rectangle 

    }

    mask       = imread("pic/mask.png"       );
    resize(mask      ,mask      , Size(frame_width, half_height), 0, 0, INTER_CUBIC) ;
   
    Mat red(half_height, frame_width , CV_8UC3, cv::Scalar(0,0,255));
    red_mask = red.clone();
    
	hsvDetect(raw_frame,  1); //initialie colore thresold limit

	 
}

Mat road_edges::boost(Mat in) // in should be gray pic
{

	//dilate(out, out, getStructuringElement(CV_SHAPE_RECT, Size(3, 3)));
    //erode(pic, pic, getStructuringElement(MORPH_ELLIPSE, Size(3, 3))); // Eliminate noise and smaller objects
    static int min   =  50;
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
	static int maxLineGap =  50;
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


void road_edges::eyeshot()
{
	static int upy = 50 ;
	static int down = half_height;

	static int p00x = frame_width/2-50 ; // up
	static int p10x = frame_width/2+50 ;

	static int p01x = 0;   // down
	static int p11x = frame_width ;

	cvCreateTrackbar("up          ", "Control", &upy , half_height);
	cvCreateTrackbar("down        ", "Control", &down, half_height);
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

Mat road_edges::show(Mat raw ,Mat segment, Mat can, Mat houg)
{
	//Mat out = Mat::zeros(600, frame_width, CV_8UC1); // gary pic
	Mat out = Mat::zeros((half_height*4), frame_width, CV_8UC3);
    putText(raw, "raw frame", Point(20, 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 1.5, 8);
	draw_eyeshot(raw);
    rectangle(raw,rect_seg,Scalar(0,255,0),1);
	
	raw.copyTo       (out(Rect(0,   0, frame_width, half_height)));
	

	segment.copyTo   (out(Rect(0, half_height,   frame_width, half_height)));
	can.copyTo       (out(Rect(0, half_height*2, frame_width, half_height)));
	houg.copyTo      (out(Rect(0, half_height*3, frame_width, half_height)));

	imshow("driver assistance", out);
	return out ;
}

Mat road_edges::hsvDetect(Mat x,  bool press)
{
	Mat in = x.clone();
	Mat out;

	cvtColor(in, in, COLOR_BGR2HSV);

	static int hue = 4;
	static int HUE = 30;
	static int saturation = 17;
	static int SATURATION = 70;
	static int value = 70;
	static int VALUE = 148;

	cvCreateTrackbar("Low-Hue        ", "Control", &hue, 255);
	cvCreateTrackbar("High-Hue       ", "Control", &HUE, 255);
	cvCreateTrackbar("Low-Saturation ", "Control", &saturation, 255); //Saturation (0 - 255)
	cvCreateTrackbar("High-Saturation", "Control", &SATURATION, 255);
	cvCreateTrackbar("Low-Value      ", "Control", &value, 255); //Value (0 - 255)
	cvCreateTrackbar("High-Value     ", "Control", &VALUE, 255);

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

void road_edges::moyen (Mat in, int vect [] ){
    int size =half_height*2 ;

	int moy_vect[size] = {0};
	
     Mat out = Mat::zeros(half_height, frame_width, CV_8UC1);

	 for (int y = 0; y < half_height; y++)  // vertical parcour
	 {
        for (int x = 0; x < half_height; x++) 
		    {   
			    int pix = (int)in.at<uchar>(y,x);// detect red line
		        if (pix)
		            {
			        vect[y] = pix;
			        break;
		        }
		    }	
	  }
}


vector <int> road_edges::car_position(Mat white, Mat canny, Mat hsv, Mat out , int vert_nbr ) //vert_nbr should be grater thatn 0 for devision
{
   vector <Point> vect_left  ;
   vector <Point> vect_right ;
   Scalar red = Scalar(0,0,255) ;
    //int pix = (int)in.at<Vec3b>(u, x)[2]; // return pixel red value

	for (int vert_pos = 1 ; vert_pos <  vert_nbr; vert_pos++) // vertical eteration 
	{	
		int stage = (half_height * vert_pos /vert_nbr) ; // vertical stage 


 	    for (int x = half_width ; x < frame_width; x++)  // horizontal eteration right side
 	    {        
			Scalar canny_pix = canny.at<Vec3b>(stage, x);
			Scalar white_pix = white.at<Vec3b>(stage, x);
			Scalar hsv_pix   = hsv.at<Vec3b>(stage, x);
			
	     	if (white_pix == red || canny_pix == red || hsv_pix == red  )  // detect red line
	    	{
		    	string m = to_string(x);
				putText(out, m, Point(x-13, stage+3 ), FONT_HERSHEY_SIMPLEX, 0.4, CV_RGB(255, 255, 255), 1.5, 8); //SIMPLEX,PLAIN, DUPLEX,COMPLEX,TRIPLEX, COMPLEX_SMALL, SCRIPT_SIMPLEX, SCRIPT_COMPLEX,		

			    Point centre (x,stage) ;
				vect_right.push_back(centre) ;

                break;
	       	}
	    }
		
		for (int x = half_width; x > 0; x--)  // LEFT  SIDES
     	{ 
			Scalar canny_pix = canny.at<Vec3b>(stage, x);
			Scalar white_pix = white.at<Vec3b>(stage, x);
			Scalar hsv_pix   = hsv.at<Vec3b>(stage, x);
			
		   if (white_pix == red || canny_pix == red || hsv_pix == red  )
		   {
			    string m = to_string(x);
			    putText(out, m, Point(x-13 , stage+3), FONT_HERSHEY_SIMPLEX, 0.4, CV_RGB(255, 255, 255), 1.5, 8);

		        Point centre (x,stage) ;
                vect_left.push_back(centre) ;

			    break ;
	    	}
    	}
	}
	
	for (int vert_pos = 1 ; vert_pos <  vert_nbr-1 ; vert_pos++) // vertical eteration to draw circle and lines
	{
     	line  (out, vect_left [vert_pos], vect_left [vert_pos-1], Scalar(0, 255, 0), 3, CV_AA); // draw the vertical line
     	line  (out, vect_right[vert_pos], vect_right[vert_pos-1], Scalar(0, 255, 0), 3, CV_AA); // draw the vertical line
	    circle(out, vect_left [vert_pos] ,15,Scalar(255, 255, 255),1.5,8);
	    circle(out, vect_right[vert_pos] ,15,Scalar(255, 255, 255),1.5,8);

	}

	vector <int> input ;
    for (int x = 0 ; x < vect_left.size();x++)
        {
            input.push_back(vect_left[x].x) ;
        }

    for (int x = 0 ; x < vect_left.size();x++)
        {
            input.push_back(vect_right[x].x) ;  
        }
        
    return input ;

}




