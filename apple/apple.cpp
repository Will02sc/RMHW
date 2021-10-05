#include<iostream>
#include<vector>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include<assert.h>
int main()
{
    cv::Mat src=cv::imread("/home/will02/Documents/apple.png");
    //读入图像
    assert(src.channels()==3);
    cv::Mat hsv,hsv1,hsv2,hsv3,hsv4,hsv5,result;
    cv::cvtColor(src,hsv,cv::COLOR_BGR2HSV);
    src.copyTo(result);
    cv::inRange(hsv,cv::Scalar(0,130,120),cv::Scalar(10,255,255),hsv1);
    cv::inRange(hsv,cv::Scalar(156,130,100),cv::Scalar(180,255,255),hsv2);
    cv::inRange(hsv,cv::Scalar(15,100,100),cv::Scalar(21,255,255),hsv4);
    cv::bitwise_or(hsv1,hsv2,hsv3);
    cv::bitwise_or(hsv3,hsv4,hsv5);
    // cv::Mat blured_img;
    // cv::medianBlur(hsv5,blured_img,7);
    //提取红色
    cv::imshow("hsv",hsv5);
    // cv::imshow("blur",blured_img);
    std::vector<std::vector<cv::Point>> contour;
    std::vector<cv::Vec4i> hierachy;
    cv::findContours(hsv5,contour,hierachy,cv::RETR_TREE,cv::CHAIN_APPROX_NONE);
    for(int i=0;i<contour.size();++i)
    {
        cv::Rect rect=cv::boundingRect(contour[i]);
        if(rect.area()>5000)
            cv::rectangle(result,rect,cv::Scalar(0, 0, 255),2,8,0);
    }
    cv::imshow("ans",result);
    cv::waitKey(0);
}