#include "utils.h" // col width <--->
#include "../connection/utils.h"
void cam_info ( VideoCapture road_camera){

		cout << "Input video: (" << road_camera.get(CV_CAP_PROP_FRAME_WIDTH) // show video info
         <<"x"               << road_camera.get(CV_CAP_PROP_FRAME_HEIGHT) 
         <<") at "           << road_camera.get(CV_CAP_PROP_FPS)
         << endl;
}
int main(int argc, char **argv)
{
    namedWindow("driver assistance", CV_WINDOW_AUTOSIZE); //create a window called "Control"
    namedWindow("Control", CV_WINDOW_AUTOSIZE);

    VideoCapture road("../Package/videos/ravin_blue.avi"); //"/dev/video0"
    cam_info(road);

    vector<Mat> outs;
    Mat road_cut, blob;

    road >> road_cut; // cut a frame from the video
    pyrDown(road_cut,road_cut);

    yolo obj("../Package/Cnn/coco.names",        // coco names
             "../Package/Cnn/yolov3.cfg",        // config
             "../Package/Cnn/yolov3.weights",    // weight
             {"yolo_82", "yolo_94", "yolo_106"}, // outputs classification layer
             0.5,                                // Confidence threshold
             0.4,
             road_cut);
 
    int frameNum = 0;


    while (1)
    {
        char c = (char)cv::waitKey(30);
        if (c == 27 )
            break;

        road >> road_cut; // cut a frame from the video
        pyrDown(road_cut,road_cut);

        frameNum++;
        fflush(stdout);
        printf("FRAME #%6d ", frameNum);

        if (c == ' ')
        {
            auto t_start = chrono::high_resolution_clock::now();

            blobFromImage(road_cut, blob, 1 / 255.0, cvSize(416, 416), Scalar(0, 0, 0), true, false);
            obj.net.setInput(blob);
            obj.net.forward(outs, obj.output_layer); // Run the forward pass to get output of the output layers
            
            obj.safe_road(road_cut);
            obj.postprocess(road_cut, outs);

            auto t_end = chrono::high_resolution_clock::now();  
            printf("   yolo :%.2f (ms)\r", chrono::duration<double, milli>(t_end - t_start).count());
           
            cv::imshow("driver assistance",road_cut);
            cv::waitKey(2000);

        }  

        else 
        {
            obj.safe_road(road_cut);
            cv::imshow("driver assistance",road_cut);
            printf("\r");
        }
    }
}
