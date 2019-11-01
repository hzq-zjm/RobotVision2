#ifndef EYE_STATUE_H
#define EYE_STATUE_H
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <math.h>
#include <iostream>
#include <sys/time.h>
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <fstream>
#include "net.h"

using namespace std;
using namespace cv;

class eye_statue                        //2018.6.8 By hzq
{
public:
    eye_statue();
    int detect_lenet_eye(const cv::Mat& bgr, std::vector<float>& cls_scores);


private:

    ncnn::Net lenet;

};


#endif // EYE_STATUE_H
