#include "utils.h"
#include "opencv2/imgproc/imgproc_c.h"
void cam_info ( VideoCapture road_camera){

		cout << "Input video: (" << road_camera.get(CAP_PROP_FRAME_WIDTH) // show video info
         <<"x"               << road_camera.get(CAP_PROP_FRAME_HEIGHT) 
         <<") at "           << road_camera.get(CAP_PROP_FPS)
         << endl;
}


Mat raw_image ;

static void onMouse( int event, int x, int y, int f, void* ){
 Mat image = raw_image .clone();

 Vec3b rgb=image.at<Vec3b>(y,x);
 int B=rgb.val[0];
 int G=rgb.val[1];
 int R=rgb.val[2];

  Mat HSV;
  Mat RGB=image(Rect(x,y,1,1));
  cvtColor(RGB, HSV,COLOR_BGR2HSV);

    Vec3b hsv=HSV.at<Vec3b>(0,0);
    int H=hsv.val[0];
    int S=hsv.val[1];
    int V=hsv.val[2];

    char name[30];

    sprintf(name,"B=%d",B);
    putText(image,name, Point(100,20) , FONT_HERSHEY_SIMPLEX, .5, Scalar(255,0,0), 2,8,false );

    sprintf(name,"G=%d",G);
    putText(image,name, Point(100,40) , FONT_HERSHEY_SIMPLEX, .5, Scalar(0,255,0), 2,8,false );

    sprintf(name,"R=%d",R);
    putText(image,name, Point(100,60) , FONT_HERSHEY_SIMPLEX, .5, Scalar(0,255,0), 2,8,false );

    sprintf(name,"H=%d",H);
    putText(image,name, Point(25,20) , FONT_HERSHEY_SIMPLEX, .5, Scalar(0,255,0), 2,8,false );

    sprintf(name,"S=%d",S);
    putText(image,name, Point(25,40) , FONT_HERSHEY_SIMPLEX, .5, Scalar(0,255,0), 2,8,false );

    sprintf(name,"V=%d",V);
    putText(image,name, Point(25,60) , FONT_HERSHEY_SIMPLEX, .5, Scalar(0,255,0), 2,8,false );

    sprintf(name,"X=%d",x);
    putText(image,name, Point(25,90) , FONT_HERSHEY_SIMPLEX, .5, Scalar(0,0,255), 2,8,false );

    sprintf(name,"Y=%d",y);
    putText(image,name, Point(25,110) , FONT_HERSHEY_SIMPLEX, .5, Scalar(0,0,255), 2,8,false );

 imshow( "pixels", image );
}

Mat road_edges::show(Mat raw ,Mat white, Mat segment, Mat can, Mat houg,Mat bird)
{
    static int s = 0 ;
	createTrackbar("show image ", "Control", &s , 4);

    putText(raw, "raw frame", Point(20, 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255), 1.5, 8);
	draw_eyeshot(raw);
    rectangle(raw,rect_seg,Scalar(0,255,0),1);

    Point p1 (frame_width/2, half_height/2);
    Point p2 (frame_width/2, 0);
    line  (raw , p1 , p2, Scalar(0, 255, 0), 1, CV_AA); // draw the vertical line
    Mat out ;


switch (s)
{
   case 0 :
    putText(white, "White lines", Point(20, 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255), 1.5, 8);
    vconcat(raw,white,out);
    break;
case 1 :
    putText(segment, "Segmentation", Point(20, 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255), 1.5, 8);
    vconcat(raw,segment,out);
    break;
case 2 :
    putText(can, "Canny", Point(20, 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255), 1.5, 8);
    vconcat(raw,can,out);
    break;
case 3 : 
    putText(houg, "HoughLines", Point(20, 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255), 1.5, 8);
    vconcat(raw,houg,out);
    break;
case 4 :
    putText(bird, "Bird view", Point(20, 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255), 1.5, 8);
    vconcat(raw,bird,out);
    break;  
default:
    break;
}
	imshow("driver assistance", out);
	return raw ;
}          

/*Mat road_edges::show(Mat raw ,Mat segment, Mat can, Mat houg)
{
    static int half = 200 ;
	cvCreateTrackbar("half_height ", "Control", &half , frame_height);

	//Mat out = Mat::zeros(600, frame_width, CV_8UC1); // gary pic
	Mat out = Mat::zeros((half_height*4), frame_width, CV_8UC3);
    putText(raw, "raw frame", Point(20, 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 1.5, 8);
	draw_eyeshot(raw);
    rectangle(raw,rect_seg,Scalar(0,255,0),1);

    Point p1 (frame_width/2, half_height/2);
    Point p2 (frame_width/2, 0);
    line  (raw , p1 , p2, Scalar(0, 255, 0), 1, CV_AA); // draw the vertical line

	raw.copyTo       (out(Rect(0,   0, frame_width, half_height)));
	

	segment.copyTo   (out(Rect(0, half_height,   frame_width, half_height)));
	can.copyTo       (out(Rect(0, half_height*2, frame_width, half_height)));
	houg.copyTo      (out(Rect(0, half_height*3, frame_width, half_height)));

	imshow("driver assistance", out);
	return out ;
}     
*/






/*void road_edges::moyen (Mat in, int vect [] ){
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
*/











    /* if (c = 's'){
        imwrite(in,half_road_cut);
        // imwrite(full,ful);
        -------------------
        frameNum++;
        String full = "pic/ful"+to_string(frameNum)+".jpg";
        imwrite("road_sides.jpg",ful);
        */



    /*if (c == 's'){
        edg.copyTo(raw_imagee (obj.rect_raw));
        imwrite("out.jpg",raw_imagee);
        }*/


         /* for (int x = 0 ; x < input.size();x++)
        {
            cout << input[x] << "|";
        }
        cout << endl ;*/


      // Mat water      = imread("pic/water.jpg"     );
   // cout <<"Water  : ("<< water.cols <<" x "<< water.rows <<")" << endl;// x.cols << x.rows 

     
    //int pix = (int)in.at<Vec3b>(u, x)[2]; // return pixel red valu
