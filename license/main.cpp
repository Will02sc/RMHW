#include<opencv2/highgui/highgui.hpp>
#include<opencv2/opencv.hpp>
#include<opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include<iostream>
#include<vector>
using namespace std;
using namespace cv;
void on_mouse(int event,int x,int y,int flag, void*);
Point2f srcTri[4],dstTri[4];
Mat srcImage,warp_image,warp_mat,tmpImage;
int cnt=0;
int main()
{
    srcImage=imread("../../RMdata/data_pack3/car.jpg");
    tmpImage=srcImage.clone();
    warp_image=Mat::zeros(srcImage.rows,srcImage.cols,srcImage.type());
    dstTri[0]=Point2f(0,0);
    dstTri[1]=Point2f(400,0);
    dstTri[2]=Point2f(400,100);
    dstTri[3]=Point2f(0,100);
    imshow("src",tmpImage);
    setMouseCallback("src",on_mouse,0);
    waitKey(0);
}
void on_mouse(int event,int x,int y,int flag, void*)
{
    if(event==EVENT_LBUTTONDOWN)
    {
        srcTri[cnt++]=Point2f(x,y);
        if(cnt==1)
            tmpImage=srcImage.clone();
        if(cnt>1)
            line(tmpImage,srcTri[cnt-2],srcTri[cnt-1],Scalar(0,0,255),2,5,0);
        if(cnt==4)
        {
            line(tmpImage,srcTri[3],srcTri[0],Scalar(0,0,255),2,5,0);
            cnt=0;
            warp_mat=getPerspectiveTransform(srcTri,dstTri);
            warpPerspective(srcImage,warp_image,warp_mat,Size(400,100));
            imshow("output",warp_image);
        }
        imshow("src",tmpImage);
    }
}