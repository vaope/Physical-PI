#include <opencv2/opencv.hpp>
#include "v4l2_camera.h"
#include <vector>
#include <string>
int main(int argc, char** argv){
    
    char *camera_name = argv[1];
    if(init_camera(camera_name)<0){     // 打开默认摄像头
        printf("init camera error\n");
        return 0;
    }

    cv::Mat frame, gray, edges, _;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    std::vector<cv::Vec3f> circles;
    char *image_path = "temp_image.jpeg";
    std::string save_path = "temp_result/pic";
    // char *save_path = "save_image.jpeg";
    int i = 50;
    while (true) {
        // if (get_image_from_camera(image_path) <0){
        //     printf("get image error");
        //     return 0;
        // }
        std::string image_name = "/home/root/getnfs/temp_pic/pic" + std::to_string(i) + ".jpeg" ;
        frame = cv::imread(image_name);
        // cap >> frame; // 捕获一帧图像
        if (frame.empty()) break;
        
        // 转换为灰度图
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        // cv::equalizeHist(gray,gray);

        // cv::GaussianBlur(gray,gray,cv::Size(5, 5), 0);
        cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();


        clahe->setClipLimit(2.0);
        clahe->setTilesGridSize(cv::Size(8, 8));

        // 应用CLAHE
        cv::Mat clahe_img;
        // clahe->apply(gray , gray);


        // double otsu_thresh_val = cv::threshold(gray, _, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);
        // cv::threshold(gray, gray, otsu_thresh_val, 255, cv::THRESH_BINARY);



        

        // double otsu_thresh_val = cv::threshold(gray, gray, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);
        cv::Canny(gray, edges, 30, 70, 3);

        cv::findContours(edges, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        int maxContourIndex = -1;
        double maxContourArea = 0.0;

        for (int i = 0; i < contours.size(); i++) {
            double area = cv::contourArea(contours[i]);
            if (area > maxContourArea) {
                maxContourArea = area;
                maxContourIndex = i;
            }
        }

        if (maxContourIndex != -1) {
            // 绘制最大轮廓
            // cv::drawContours(image, contours, maxContourIndex, cv::Scalar(0, 0, 255), 2);

            // 计算最大轮廓的中点
            cv::Moments moments = cv::moments(contours[maxContourIndex]);
            cv::Point2f centroid;
            centroid.x = moments.m10 / moments.m00;
            centroid.y = moments.m01 / moments.m00;

            // 输出中点坐标
            std::cout << "Centroid: (" << centroid.x << ", " << centroid.y << ")" << std::endl;
        }
            cv::drawContours(frame, contours, -1, cv::Scalar(0, 255, 0), 2);
            std::string pic_name = save_path + std::to_string(i++) + ".jpeg";
            cv::imwrite(pic_name,frame);
        // printf("1");
        
    }

    // 释放资源
    // cap.release();
    cv::destroyAllWindows();
    return 0;
}
