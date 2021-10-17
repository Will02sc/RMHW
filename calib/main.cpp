#include<iostream>
#include<vector>
#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
using namespace cv;
using namespace std;
int main()
{
    const int board_w=9,board_h=6;
    const int board_n=board_w*board_h;
    Size board_size(board_w,board_h);
    Mat gray_img,drawn_img;
    vector<Point2f> point_pix_pos_buf;
    vector<vector<Point2f> >point_pix_pos;
    int found=0,success=0;
    Size img_size;
    int cnt=0,k=0,n=0;
    for(int i=0;i<24;++i)
    {
        Mat src=imread(string("../../RMdata/calib2/").append(to_string(i).append("_orig.jpg")));
        if(!cnt)
        {
            img_size.width=src.cols;
            img_size.height=src.rows;
        }
        found=findChessboardCorners(src,board_size,point_pix_pos_buf);
        if(found&&point_pix_pos_buf.size()==board_n)
        {
            ++success;
            cvtColor(src,gray_img,COLOR_BGR2GRAY);
            find4QuadCornerSubpix(gray_img,point_pix_pos_buf,Size(5,5));
            point_pix_pos.push_back(point_pix_pos_buf);
            drawn_img=src.clone();
            drawChessboardCorners(drawn_img,board_size,point_pix_pos_buf,found);
            imshow("corners",drawn_img);
            waitKey(50);
        }
        point_pix_pos_buf.clear();
        ++cnt;
    }
    cout<<success<<" useful chess boards"<<endl;
    Size square_size(10,10);
    vector<Point3f>point_grid_pos_buf;
    vector<vector<Point3f> >point_grid_pos;
    vector<int> point_cnt;
    Mat camera_mat(3,3,CV_32FC1,Scalar::all(0));
    Mat dist_coeffs(1,5,CV_32FC1,Scalar::all(0));
    vector<Mat> rvecs,tvecs;
    for(int i=0;i<success;++i)
    {
        for(int j=0;j<board_h;++j)
            for(int k=0;k<board_w;++k)
            {
                Point3f pt;
                pt.x=k*square_size.width;
                pt.y=j*square_size.height;
                pt.z=0;
                point_grid_pos_buf.push_back(pt);
            }
        point_grid_pos.push_back(point_grid_pos_buf);
        point_grid_pos_buf.clear();
        point_cnt.push_back(board_h*board_w);
    }
    cout<<calibrateCamera(point_grid_pos,point_pix_pos,img_size,camera_mat,dist_coeffs,rvecs,tvecs)<<endl;
    cout<<camera_mat<<endl;
    cout<<dist_coeffs<<endl;
}