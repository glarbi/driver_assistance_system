#include "utils.h" 
#include "../connection/utils.h"
#include "tools.cpp"                   //  ( blue , green , red )
//#include "vector"             

 static int waikeyspeed = 1970;

int main(int argc, char **argv)
{
    namedWindow("Control", CV_WINDOW_AUTOSIZE);  
	//namedWindow( "pixels", CV_WINDOW_AUTOSIZE); //create a window for on mouse function
    cvCreateTrackbar("speed ", "Control", &waikeyspeed, 2000);

    VideoCapture road_camera("../Package/videos/road_turn.mp4");  //road_turn.mp4
    cam_info(road_camera);

    double fps = road_camera.get(CV_CAP_PROP_FPS);
    VideoWriter video;

    road_camera >> raw_image ;  // raw image declared in  tools.cpp 


    pyrDown( raw_image, raw_image ); 

    road_edges obj(raw_image );        // instance object
    video.open("out_vid.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, Size(obj.half_width,obj.half_height));

    Mat preprocessing ;
    
    int frames = 0 ;
    while(true)
    {  
        frames += 1 ;
        auto t_start = chrono::high_resolution_clock::now();
        fflush(stdout);

        char c = (char)waitKey(2001 - waikeyspeed);
        if (c == 27)
             break;

        road_camera >> raw_image;   // cut frame from camera
        pyrDown(raw_image,raw_image);
        
        raw_image = raw_image(obj.rect_raw );  // rectangle the raw image

        //------------------------------------------------ Pre-Processing -----------------------------------------------
         obj.eyeshot();
         Mat bird = obj.eyebird(raw_image); // bird view need to initialise by eyshot 


       // bitwise_and(raw_image, obj.mask, preprocessing); // triangle mask  
        //medianBlur (preprocessing ,preprocessing, 3);
     
        medianBlur (raw_image ,preprocessing, 3);
        //GaussianBlur(x, blur_pic, Size(5, 5), 0, 0); // smoothing

      //------------------------------------------------ Segmentation -----------------------------------------------------
        Mat seg_out, white ;

        white = obj.white(preprocessing); // white-yellow lines
        
        if (c == ' ')
            seg_out = obj.hsvDetect(preprocessing,  1); 
        else
            seg_out = obj.hsvDetect(preprocessing,  0);
                     
     //------------------------------------------------ Canny hougline --------------------------------------------------------
 
        Mat can = obj.boost(preprocessing);
        Mat can_rgb ;
        cvtColor(can, can_rgb, CV_GRAY2BGR);  

        Mat edg = obj.proba_hougline(can,can_rgb  ); // can in should be gray ,raw image should be rgb

     //------------------------------------------------- show -----------------------------------------

       Mat outt= obj.show(raw_image , white,seg_out, can_rgb,edg,bird );

        video.write(outt);

 
     //-------------------------------------------------------------------------------------------  

       // setMouseCallback( "pixels", onMouse, 0 );
       

        auto t_end = chrono::high_resolution_clock::now();
        int time = chrono::duration<double, milli>(t_end - t_start).count();

        printf("frame ::%u time :%u \r",frames, time);
    }
}
