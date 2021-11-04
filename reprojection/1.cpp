#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <Eigen/Dense>
#include <opencv2/core/eigen.hpp>
#include <iostream>
#include <cstdio>
#include <cstring>

int main()
{
    const int COLS = 2208, ROWS = 1242;

    cv::Mat depth = cv::imread("../1_dpt.tiff", cv::IMREAD_ANYDEPTH);
    cv::Mat img = cv::imread("../1.jpg");
    cv::FileStorage reader("../data.yml", cv::FileStorage::READ);
    cv::Mat c_mat, p_mat;
    Eigen::Matrix<double, 3, 3> C;
    Eigen::Matrix<double, 4, 4> P;
    reader["C"] >> c_mat;
    reader["D"] >> p_mat;

    // std::cout << c_mat << std::endl << p_mat << std::endl << depth << std::endl;

    cv::cv2eigen(c_mat, C);
    cv::cv2eigen(p_mat, P);

    std::vector<cv::Point2f> pt_u;
    std::vector<cv::Point2f> pt_c;


    for (int i = 0; i < img.rows; i++)
        for (int j = 0; j < img.cols; j++)
            pt_u.push_back(cv::Point2f{j, i});
    cv::undistortPoints(pt_u, pt_c, c_mat, cv::noArray());

    pt_u.clear();

    std::vector<cv::Point2f> pt_c2;
    // std::cout << pt_c.size() << " : " << depth.rows * depth.cols << std::endl;
    for (int i = 0; i < pt_c.size(); i++)
    {
        Eigen::Matrix<double, 4, 1> pt_w, tmp;
        pt_w << pt_c[i].x, pt_c[i].y, 1, 1;
        pt_w *= depth.at<float>(i / depth.cols, i % depth.cols);
        pt_w(3, 0) = 1;
        Eigen::Matrix<double, 3, 1> pt, result;
        tmp = P * pt_w;
        pt << tmp(0, 0) / tmp(2, 0), tmp(1, 0) / tmp(2, 0), 1;
        result = C * pt;
        // std::cout << i << ":\n" << result << std::endl;
        pt_c2.push_back({result(0, 0) / result(2, 0), result(1, 0) / result(2, 0)});
    }
    cv::Mat result = cv::Mat::zeros(cv::Size(img.cols, img.rows), CV_8UC3);
    for (int i = 0; i < depth.rows; i++)
        for (int j = 0; j < depth.cols; j++)
        {
            cv::Point2f pt = pt_c2[i * img.cols + j];
            int x = (int)pt.x, y = (int)pt.y;
            if (x < 0 || x > result.cols - 1 || y < 0 || y > result.rows - 1)
                continue;
            result.at<cv::Vec3b>(y, x) = img.at<cv::Vec3b>(i, j);
        }
    cv::imshow("result", result);
    cv::waitKey(0);

    return 0;
}