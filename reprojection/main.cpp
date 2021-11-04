#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <Eigen/Dense>
#include <opencv2/core/eigen.hpp>
#include <iostream>
#include <cstdio>
#include <cstring>
using namespace cv;
using namespace Eigen;
using namespace std;
int main()
{
    Mat depth=imread("../../RMdata/data_pack4/1_dpt.tiff",IMREAD_ANYDEPTH);
    Mat img=imread("../../RMdata/data_pack4/1.jpg");
    FileStorage reader("../../RMdata/data_pack4/data.yml",FileStorage::READ);
    Mat c_mat,p_mat,dst_img=Mat::zeros(Size(img.cols,img.rows),CV_8UC3);
    reader["C"]>>c_mat;
    reader["D"]>>p_mat;
    Matrix<double,3,3>C;
    Matrix<double,4,4>P;
    cv2eigen(c_mat,C);
    cv2eigen(p_mat,P);
    vector<Point2f>pt_u,pt_c,pt_dst;
    //input
    // cout<<c_mat<<endl<<p_mat<<endl<<depth<<endl;
    for(int i=0;i<img.rows;++i)
        for(int j=0;j<img.cols;++j)
            pt_u.push_back(Point2f(j,i));
    undistortPoints(pt_u,pt_c,c_mat,noArray());
    //pixel coordinate system to world coordinate system
    for(int i=0;i<pt_c.size();++i)
    {
        Matrix<double,4,1>pt_w,tmp;
        pt_w<<pt_c[i].x,pt_c[i].y,1,1;
        pt_w*=depth.at<float>(i/depth.cols,i%depth.cols);
        pt_w(3,0)=1;
        Matrix<double,3,1>pt,tmp1;
        tmp=P*pt_w;
        pt<<tmp(0,0)/tmp(2,0),tmp(1,0)/tmp(2,0),1;
        tmp1=C*pt;
        pt_dst.push_back(Point2f(tmp1(0,0)/tmp1(2,0),tmp1(1,0)/tmp1(2,0)));
    }
    for(int i=0;i<depth.rows;++i)
        for(int j=0;j<depth.cols;++j)
        {
            Point2f pt=pt_dst[i*depth.cols+j];
            if(pt.x>0&&pt.x<dst_img.cols&&pt.y>0&&pt.y<dst_img.rows)
            {
                dst_img.at<cv::Vec3b>(pt.y,pt.x)=img.at<cv::Vec3b>(i,j);
            }
        }
    imshow("dst_img",dst_img);
    imwrite("../dst_img.jpg",dst_img);
    waitKey(0);
}