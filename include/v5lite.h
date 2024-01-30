#ifndef V5LITE_H
#define V5LITE_H




#include "layer.h"
#include "net.h"

#if defined(USE_NCNN_SIMPLEOCV)
#include "simpleocv.h"
#else
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#endif
#include <float.h>
#include <stdio.h>
#include <vector>
#include <sys/time.h>
#include <string>
// 0 : FP16
// 1 : INT8
#define USE_INT8 0

// 0 : Image
// 1 : Camera
#define USE_CAMERA 0


struct Detect_output{
    float x;
    float y;
    float width;
    float height;
};
void init_yolov5(char* param_path,char* bin_path,bool is_int8);
int detect_image(char *imagepath,pthread_mutex_t &Mutex,std::vector<struct Detect_output> &result);

#endif // !V5LITE_H