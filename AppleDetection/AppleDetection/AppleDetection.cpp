#include <iostream>
#include <stdio.h>
#include <opencv2/core/core.hpp> 
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp> 

using namespace cv;
using namespace std;

int main()
{
    // 导入并显示原始图像
    Mat img = imread("./Apples.bmp");
    int height = img.rows;
    int width = img.cols;
    namedWindow("Original", WINDOW_KEEPRATIO);
    resizeWindow("Original",cvRound(width/3), cvRound(height/3));
    imshow("Original", img);
    waitKey(0);
    destroyAllWindows();

    // 转换为HSV图像
    Mat hsv;
    cvtColor(img, hsv, COLOR_BGR2HSV);

    // 阈值化图像，只获取红色

    Mat grey, median, erosion, dilation, result;
    Mat kernel = getStructuringElement(0, Size(1, 1), Point(0, 0));
    inRange(hsv, Scalar(0, 100, 0), Scalar(17, 255, 200), grey);
    medianBlur(grey, median, 21);
    erode(median, erosion, kernel);
    dilate(erosion, dilation, kernel);

    // 边缘检测
    Mat edge;
    Canny(dilation, edge, 50, 100);
    dilate(edge, edge, Mat(), Point(-1,-1));

    // 提取轮廓
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(edge, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE); // 只提取外部轮廓

    // 标记苹果位置
    vector<Point2f> center(contours.size());
    vector<float> radius(contours.size());
    vector<Rect> rects(contours.size());
    Mat apple = img.clone();
    Scalar bound_color = (153, 255, 204);
    Scalar font_color = (153, 255, 204);
    float font_size = 1.2;
    int line_thickness = 2;
    int line_type = 8;
    int font_thickness = 2;
    int font_type = 8;
    string text = "apple";

    for (size_t i = 0; i < contours.size(); i++)
    {
        minEnclosingCircle(contours[i], center[i], radius[i]);
        rects[i] = boundingRect(contours[i]);
        circle(apple, center[i], cvRound(radius[i]), bound_color, line_thickness, line_type);
        rectangle(apple, rects[i], bound_color, line_thickness, line_type);
        putText(apple, text, center[i], FONT_HERSHEY_SIMPLEX, font_size, font_color, font_thickness, font_type);
    }

    namedWindow("Apple", WINDOW_KEEPRATIO);
    resizeWindow("Apple", 864, 648);
    imshow("Apple", apple);
    waitKey(0);
}