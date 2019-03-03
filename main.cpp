
#include "utils.h"
// col width <--->
// rows height |


int main(int argc, char **argv)
{
    namedWindow("driver assistance", CV_WINDOW_AUTOSIZE); //create a window called "Control"
    namedWindow("Control", CV_WINDOW_AUTOSIZE);

    //--------------------------------------road camera------------------------
    /*VideoCapture cap("/dev/video2");
    VideoCapture road("../My-Package/videos/manhattan.mp4");

    double fps = road.get(CV_CAP_PROP_FPS);
    cout << fps << endl;

    float ratio = road.get(CV_CAP_PROP_FRAME_WIDTH) / road.get(CV_CAP_PROP_FRAME_HEIGHT);
    cout << "width <---> " << road.get(CV_CAP_PROP_FRAME_WIDTH) << "  height " << road.get(CV_CAP_PROP_FRAME_HEIGHT) << "  New width :" << endl; // col width
    int new_width, new_height;
    cin >> new_width;
    new_height = new_width / ratio;
    cout << new_height << "/" << new_width << endl;
*/
    //--------------------------------------face camera-------------------------------
    VideoCapture face_cap("/dev/video0");

   // VideoCapture face_cap(0);
    double fps = face_cap.get(CV_CAP_PROP_FPS);
    cout << fps << endl;

    //face_cap.set(CV_CAP_PROP_FRAME_WIDTH, 700);
    //face_cap.set(CV_CAP_PROP_FRAME_HEIGHT, 300);

    if (!face_cap.isOpened())
    {
        throw "Error when reading steam_avi"; //  cout << obj.safe_dist << endl;

        return -1;
    }
    //------------------------------------------job-------------------------------------

    VideoWriter video;
    video.open("out_vid.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, Size(face_cap.get(CV_CAP_PROP_FRAME_WIDTH), face_cap.get(CV_CAP_PROP_FRAME_HEIGHT)));

    while (1)
    {

        char c = (char)waitKey(30);
        if (c == 27)
            break;

        Mat face;
        face_cap >> face;

        /* Mat road_cut,  resize_cut, pyr;
        road >> road_cut; // cut a frame from the video

        resize(road_cut, resize_cut, Size(new_width, new_height), 0, 0, INTER_LINEAR);
*/
        imshow("driver assistance", face);

        //j += 1;
        //String g = "pic/" + to_string(j) + ".jpg";
        //imwrite(g, road_cut);
        // imshow("driver assistance", road_cut);
        video.write(face);
    }

    video.release();

    waitKey(0); // key press to close window
}