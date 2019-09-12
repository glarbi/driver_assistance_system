#include "utils.h"
#include <opencv2/opencv.hpp>
using namespace cv;

 // 'R' forward   'T' back   'Q' left   'S' right

int main()

{
    namedWindow("hfhhfhfh",1);

   connection arduino("/dev/ttyACM0");  // "/dev/rfcomm0"
   //arduino.sendData("hello");
    
    while (true)
    {
        fflush(stdout);
        char c = (char)waitKey(1000);
        
        if ( c =='x') 
             arduino.~connection () ;
        else    
             arduino.sendData(c);   // send a char 
        
        
        string out = arduino.reciveData() ;

        if (out != ""){
             printf ("direction : %s \r", out.c_str());
        }     
    }
}
