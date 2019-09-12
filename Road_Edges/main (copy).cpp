#include "utils.h"
#include "../connection/utils.h"
#include <stdio.h>                                                    //  ( blue , green , red )

static int waikeyspeed = 1000;

Mat road_cut,half_road_cut; 

static void onMouse( int event, int x, int y, int f, void* ){
 Mat image=half_road_cut .clone();
 Vec3b rgb=image.at<Vec3b>(y,x);
 int B=rgb.val[0];
 int G=rgb.val[1];
 int R=rgb.val[2];

  Mat HSV;
  Mat RGB=image(Rect(x,y,1,1));
  cvtColor(RGB, HSV,CV_BGR2HSV);

    Vec3b hsv=HSV.at<Vec3b>(0,0);
    int H=hsv.val[0];
    int S=hsv.val[1];
    int V=hsv.val[2];

    char name[30];
    sprintf(name,"B=%d",B);
    putText(image,name, Point(150,40) , FONT_HERSHEY_SIMPLEX, .7, Scalar(0,255,0), 2,8,false );

    sprintf(name,"G=%d",G);
    putText(image,name, Point(150,80) , FONT_HERSHEY_SIMPLEX, .7, Scalar(0,255,0), 2,8,false );

    sprintf(name,"R=%d",R);
    putText(image,name, Point(150,120) , FONT_HERSHEY_SIMPLEX, .7, Scalar(0,255,0), 2,8,false );

    sprintf(name,"H=%d",H);
    putText(image,name, Point(25,40) , FONT_HERSHEY_SIMPLEX, .7, Scalar(0,255,0), 2,8,false );

    sprintf(name,"S=%d",S);
    putText(image,name, Point(25,80) , FONT_HERSHEY_SIMPLEX, .7, Scalar(0,255,0), 2,8,false );

    sprintf(name,"V=%d",V);
    putText(image,name, Point(25,120) , FONT_HERSHEY_SIMPLEX, .7, Scalar(0,255,0), 2,8,false );

    sprintf(name,"X=%d",x);
    putText(image,name, Point(25,300) , FONT_HERSHEY_SIMPLEX, .7, Scalar(0,0,255), 2,8,false );

    sprintf(name,"Y=%d",y);
    putText(image,name, Point(25,340) , FONT_HERSHEY_SIMPLEX, .7, Scalar(0,0,255), 2,8,false );

 //imwrite("hsv.jpg",image);
 imshow( "Self-Drive", image );
}

int main(int argc, char **argv)
{

    //VideoCapture road("../Package/videos/road_turn.mp4");

    // connection arduino('7');

    namedWindow("driver assistance", CV_WINDOW_AUTOSIZE); //create a window called "Control"
    namedWindow("Control", CV_WINDOW_AUTOSIZE);
    moveWindow("Control", 4000, 0);
    cvCreateTrackbar("speed ", "Control", &waikeyspeed, 2000);

    //Mat mask = imread("My-Package/pictures/road.jpg", CV_LOAD_IMAGE_GRAYSCALE); // upload gray image , without parametre rgb
    // Mat old_pic = Mat::zeros(150, 600, CV_8UC3);
    
   // road >> road_cut;
    //road_edges obj(road_cut);

   int frameNum = 0;
    
	for( ; ; )
	{
        char c = (char)waitKey(2001 - waikeyspeed);
        
        
         road_cut = imread("in.jpg"); // upload gray image , without parametre rgb
         road_edges obj(road_cut);


      //  road >> road_cut;
        
        resize(road_cut, road_cut, Size(frame_width, frame_height), 0, 0, INTER_CUBIC);
        half_road_cut = road_cut(Rect(0, 187, frame_width, 150));
        
        obj.eyeshot(half_road_cut);
        Mat bird_view = obj.eyebird(half_road_cut);

        //-----------------------------------------------segmentation--------------------------------------------------

        //  road_edges obj(half_road_cut); // create  a road_edges object
        Mat seg_out;
        Rect rect_seg = Rect(270, 80, 60, 50);

        if (c == ' ')
            seg_out = obj.hsvDetect(half_road_cut, rect_seg, 1);
        else
            seg_out = obj.hsvDetect(half_road_cut, rect_seg, 0);

        //------------------------------------------whiteline-----------------------------------------------------

        Mat y = obj.white_line(half_road_cut);
        y = obj.proba_hougline(y);
        
        //-------------------------------------------------edges --------------------------------------------------------

        Mat edg = obj.canny(half_road_cut);


        setMouseCallback( "Self-Drive", onMouse, 0 );

        rectangle(half_road_cut, rect_seg, Scalar(0, 255, 0), 1, 1);   
        // obj.show(half_road_cut, seg_out, y, edg);
        Mat ful = obj.show(half_road_cut, bird_view, edg, y ,seg_out);



        frameNum++;
        String in = "pic/"+to_string(frameNum)+".jpg";
        String full = "pic/ful"+to_string(frameNum)+".jpg";

       /* if (c = 's'){
        imwrite(in,half_road_cut);
       // imwrite(full,ful);

        }
        */
    }

    waitKey(0); // key press to close windowse
}
