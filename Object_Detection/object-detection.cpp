#include "utils.h"

yolo::yolo(string classesFile, string config, string weight, vector<String> out, float conf, float nms, Mat in )
{  
    /* frame_width  = in.cols ;
    frame_height = in.rows ; 
   
    left_vect  [frame_height] = {0};
	right_vect [frame_height] = {0};*/
    cout <<"New  Size  : ("<< in.cols <<" x "<< in.rows <<")" << endl;// x.cols << x.rows 


    ifstream ifs(classesFile.c_str()); // coco classesFile
    string line;
    while (getline(ifs, line))
        classes.push_back(line);

    confThreshold = conf;
    nmsThreshold = nms;

    output_layer = out;

    net = readNetFromDarknet(config, weight);  // Give the configuration and weight
    net.setPreferableBackend(DNN_BACKEND_OPENCV);
    net.setPreferableTarget(DNN_TARGET_CPU);

    risk_dist = 300;
    safe_dist = 70 ;

    

    eyeshot(in, left_vect, right_vect,true );

}

bool yolo::drawPred(int classId, float conf, int left, int top, int right, int bottom, Mat &frame)
{
    int centre = (left + right) / 2;

    Scalar yelow(100, 255, 255);
    Scalar blue(255, 0,0);
    Scalar light_yelow(255, 255, 255);
    Scalar red(0, 0, 255);

    string label = format("%.2f"); //Get the label for the class name and its confidence
    CV_Assert(classId < (int)classes.size());
    label = classes[classId];

    //Display the label at the top of the bounding box
    int baseLine;
    Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
    top = max(top, labelSize.height);

    if ((bottom > risk_dist ) and centre > left_vect[bottom] and centre < right_vect[bottom])
    {
        rectangle(frame, Point(left, top), Point(right, bottom), red, 2); //Draw red rectangle displaying the bounding box
        rectangle(frame, Point(left, top - round(1.1 * labelSize.height)), Point(left + round(1.1 * labelSize.width), top + baseLine), red, FILLED);
        //return true;
    }

    else if ((bottom > (risk_dist- safe_dist)) and centre > left_vect[bottom] and centre < right_vect[bottom])
    {
        rectangle(frame, Point(left, top), Point(right, bottom), blue, 2); 
        rectangle(frame, Point(left, top - round(1.1 * labelSize.height)), Point(left + round(1.1 * labelSize.width), top + baseLine), blue, FILLED);
    }

    else 
    {
        rectangle(frame, Point(left, top), Point(right, bottom), Scalar(0, 255, 0), 2); 
        rectangle(frame, Point(left, top - round(1.1 * labelSize.height)), Point(left + round(1.1 * labelSize.width), top + baseLine), Scalar(0, 200, 0), FILLED);
    }
        

    putText(frame, label, Point(left, top), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 1.8);
    return false;
}

bool yolo::postprocess(Mat &frame, const vector<Mat> &outs) // Remove the bounding boxes with low confidence using non-maxima suppression
{
    vector<int> classIds;
    vector<float> confidences;
    vector<Rect> boxes;

    for (size_t i = 0; i < outs.size(); ++i)
    {
        // Scan through all the bounding boxes output from the network and keep only the
        // ones with high confidence scores. Assign the box's class label as the class
        // with the highest score for the box.
        float *data = (float *)outs[i].data;
        for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
        {
            Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
            Point classIdPoint;
            double confidence;
            // Get the value and location of the maximum score
            minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);

            if (confidence > confThreshold)
            {
                int centerX = (int)(data[0] * frame.cols);
                int centerY = (int)(data[1] * frame.rows);

                int width = (int)(data[2] * frame.cols);
                int height = (int)(data[3] * frame.rows);

                int left = centerX - width / 2;
                int top = centerY - height / 2;

                classIds.push_back(classIdPoint.x);
                confidences.push_back((float)confidence);
                boxes.push_back(Rect(left, top, width, height));
            }
        }
    }

    // Perform non maximum suppression to eliminate redundant overlapping boxes with
    // lower confidences

    vector<int> indices;
    NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);

    for (size_t i = 0; i < indices.size(); ++i)
    {
        int idx = indices[i];
        Rect box = boxes[idx];
        if (drawPred(classIds[idx], confidences[idx], box.x, box.y, box.x + box.width, box.y + box.height, frame)) // if true there a obstacle
            return true ;
    }

    return false ;
}

void yolo::eyeshot(Mat in, int left_vect[], int right_vect[] , bool draww  )
{
    /*static int p00x = frame_width/2 ;// 275;
    static int p10x = frame_width/2 ;//300;


    static int upy = frame_height/2 ;//200;*/
    static int p00x = 420;
    static int p10x = 540;
    static int upy = 330;

    static int p01x = 0;           // 120;
    static int p11x = frame_width; //470;

    cvCreateTrackbar("safe        ", "Control", &risk_dist, frame_height );
    cvCreateTrackbar("eyshot      ", "Control", &upy      , frame_height );
    cvCreateTrackbar("point 00    ", "Control", &p00x     , frame_width  );
    cvCreateTrackbar("point 10    ", "Control", &p10x     , frame_width  );
    cvCreateTrackbar("point 01    ", "Control", &p01x     , frame_width  );
    cvCreateTrackbar("point 11    ", "Control", &p11x     , frame_width  );
 
    p00.x = p00x; //up left
    p10.x = p10x; //up right
    p10.y = p00.y = upy;

    p01.x = p01x;                 //down_left
    p11.x = p11x;                 //down_right
    p11.y = p01.y = frame_height; //337

    while (draww)                  // for constructeur 
    {   p00.x = p00x; //up left
        p10.x = p10x; //up right
        p10.y = p00.y = upy;

        p01.x = p01x;                 //down_left
        p11.x = p11x;                 //down_right
        p11.y = p01.y = frame_height; //337
        Mat out = in.clone();

        line(out, p00, p01, Scalar(0, 255, 0), 2);
        line(out, p10, p11, Scalar(0, 255, 0), 2);
        line(out, Point(frame_width/4, risk_dist), Point(frame_width*3/4, risk_dist), Scalar(0, 0, 255), 3); //horizontal safe dist

        imshow("driver assistance", out);

        char c = (char)cv::waitKey(30);
        if (c == ' ')
            break;
    }


    LineIterator it1(in, p00, p01);

    LineIterator it2(in, p10, p11);

    for (int i = 0; i < it1.count; i++, ++it1)
    {
        left_vect[it1.pos().y] = it1.pos().x;
    }

    for (int i = 0; i < it2.count; i++, ++it2)
    {
        right_vect[it2.pos().y] = it2.pos().x;
    }
}


Mat yolo::safe_road(Mat in)
{ 

    line(in, p00, p01, Scalar(0, 255, 0), 2);
    line(in, p10, p11, Scalar(0, 255, 0), 2);

    line(in, Point(left_vect[risk_dist], risk_dist), Point(right_vect[risk_dist], risk_dist), Scalar(0, 0, 255), 2); // Red

    int dist = risk_dist - safe_dist;
    line(in, Point(left_vect[dist], dist), Point(right_vect[dist], dist), Scalar(255, 0, 0), 2); // yellow

    return in;
}
