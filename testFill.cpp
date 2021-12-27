#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include "opencv2/imgproc.hpp"
#include <opencv2/highgui.hpp>

#include <opencv2/imgproc/types_c.h>

using namespace cv;

void rounded_rectangle(Mat &src, Point topLeft, Point bottomRight, const Scalar lineColor, const int thickness, const int lineType, const int cornerRadius)
{
    /* corners:
     * p1 - p2
     * |     |
     * p4 - p3
     */
    Point p1 = topLeft;
    Point p2 = Point(bottomRight.x, topLeft.y);
    Point p3 = bottomRight;
    Point p4 = Point(topLeft.x, bottomRight.y);

    // draw straight lines
    line(src, Point(p1.x + cornerRadius, p1.y), Point(p2.x - cornerRadius, p2.y), lineColor, thickness, lineType);
    line(src, Point(p2.x, p2.y + cornerRadius), Point(p3.x, p3.y - cornerRadius), lineColor, thickness, lineType);
    line(src, Point(p4.x + cornerRadius, p4.y), Point(p3.x - cornerRadius, p3.y), lineColor, thickness, lineType);
    line(src, Point(p1.x, p1.y + cornerRadius), Point(p4.x, p4.y - cornerRadius), lineColor, thickness, lineType);

    // draw arcs
    ellipse(src, p1 + Point(cornerRadius, cornerRadius), Size(cornerRadius, cornerRadius), 180.0, 0, 90, lineColor, thickness, lineType);
    ellipse(src, p2 + Point(-cornerRadius, cornerRadius), Size(cornerRadius, cornerRadius), 270.0, 0, 90, lineColor, thickness, lineType);
    ellipse(src, p3 + Point(-cornerRadius, -cornerRadius), Size(cornerRadius, cornerRadius), 0.0, 0, 90, lineColor, thickness, lineType);
    ellipse(src, p4 + Point(cornerRadius, -cornerRadius), Size(cornerRadius, cornerRadius), 90.0, 0, 90, lineColor, thickness, lineType);
}

int main(int argc, char **argv)
{
    cv::Mat menu(cv::Size(200, 150), CV_8UC3, cv::Scalar(10, 10, 1));
    cv::circle(menu, cv::Point(50, 50), 30, cv::Scalar(220, 220, 220), -1);
    cv::circle(menu, cv::Point(50, 50), 25, cv::Scalar(60, 150, 0), -1);
    // circle(menu, Point(50,50), 25, Scalar(60, 0, 150), -1);
    // circle(menu, Point(50,50), 25, Scalar(30, 190, 190), -1);
    rounded_rectangle(menu, cv::Point(10, 100), cv::Point(190, 130), cv::Scalar(255, 255, 255), 1, 8, 10);

    cv::putText(menu,               // target image
                "State",            // text
                cv::Point(100, 65), // top-left position
                cv::FONT_HERSHEY_DUPLEX,
                1,
                CV_RGB(220, 220, 220), // font color
                2);

    // cv::putText(menu,               // target image
    //             "Show Area",        // text
    //             cv::Point(30, 122), // top-left position
    //             cv::FONT_HERSHEY_DUPLEX,
    //             0.8,
    //             CV_RGB(220, 220, 220), // font color
    //             2);

    // Create simple input image
    cv::Point seed(20, 120);

    cv::floodFill(menu, seed, cv::Scalar(0,0,255), 0, Scalar(5, 5,5,5), Scalar(5, 5,5,5));

    imshow("menu", menu);
    waitKey(0);

    return 0;
}