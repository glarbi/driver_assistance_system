
#include "utils.h" // col width <--->
#include "../connection/utils.h"

float blink(full_object_detection shape ,int begin ,int end )
{  
    std::vector<cv::Point> vec;
    cv::Point p;
    for (int b = begin; b < end; ++b) 
    {
        p.x = shape.part(b).x();
        p.y = shape.part(b).y();
        vec.push_back(p);
    }

    double a = cv::norm(cv::Mat(vec[1]), cv::Mat(vec[5])); //Calculates an absolute difference norm or a relative difference norm.
    double b = cv::norm(cv::Mat(vec[2]), cv::Mat(vec[4]));
    double c = cv::norm(cv::Mat(vec[0]), cv::Mat(vec[3]));

    float ECR = (a + b) / (2.0 * c); //compute ECR
    vec.clear();
    return ECR;
}

int main()
{   
    
    //connection arduino(" ");  // "/dev/rfcomm0"

    static bool state     = true ;
    static bool AutoPilot = false ;
    String out = " ";
    float ECR_threshold =  0.2;
    int   sleep_average = 0 ;
    float left_ecr  , right_ecr ;
    
    VideoCapture cap(0);
     
    frontal_face_detector detector = get_frontal_face_detector();       // Load face detection and deserialize  face landmarks model.
    
    shape_predictor sp;
    deserialize("../Package/Cnn/shape_predictor_68_face_landmarks.dat") >> sp;
 
    
    image_window win;
    //arduino.sendData('2');
    //cout << arduino.reciveData() << endl;


    while (!win.is_closed())        // Grab and process frames until the main window is closed by the user.
    {     

        auto t_start = chrono::high_resolution_clock::now();
        fflush(stdout);
               
        cv::Mat temp;
        
        cap.read(temp) ;
        cv_image<bgr_pixel> cimg(temp);
        full_object_detection shape;
             
        std::vector<rectangle> faces = detector(cimg); //cout << "Number of faces detected: " << faces.size() << endl;

        win.clear_overlay();
        win.set_image(cimg);
        
        if (faces.size() > 0) {              // Find the pose of each face.
            shape = sp(cimg, faces[0]);      // Work only with 1 face
            left_ecr  = blink(shape,36,42) ; // Compute Eye aspect ration 
            right_ecr = blink(shape,42,48) ;    
                    
            if (left_ecr < ECR_threshold and right_ecr < ECR_threshold  ) {    
                win.add_overlay(dlib::image_window::overlay_rect(faces[0], rgb_pixel(255, 255, 255), "Sleeping"));
                if (sleep_average < 20)
                    sleep_average += 1;
            }
            else if (sleep_average > 0)
                     sleep_average -= 1;
            
            win.add_overlay(render_face_detections(shape));
        }
       
        else // if there is no face detect
        { 
            left_ecr= 0.00 ;right_ecr= 0.00 ;
            if (sleep_average < 20)
                sleep_average += 1; // if there is no face detected 
        }  
       
        auto t_end = chrono::high_resolution_clock::now();
        int time  =  chrono::duration<double, milli>(t_end - t_start).count() ;
      

      
/*
       if (!AutoPilot  and sleep_average > 7 ) {
            AutoPilot = true ;
            arduino.sendData('A');}        // AutoPilot Activate 
              
        if (AutoPilot and sleep_average < 2) {
            AutoPilot = false ;    
            arduino.sendData('a');}        // AutoPilot desactivat
        
        if (!state and sleep_average < 4 ) {
            state = true ;                 // Awaking case 
            arduino.sendData('9'); }

        if (state and sleep_average > 4 ) {
            state = false ;                // sleeping case 
            arduino.sendData('8');}
      
        out = arduino.reciveData();*/
        printf("time :%u ||left :%4.2f ||right :%4.2f ||State :%u ||Arduino : %s \r", time,left_ecr ,right_ecr, sleep_average,  out.c_str() );
       
    }
}
