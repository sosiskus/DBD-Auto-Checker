#pragma once
#include <opencv2/opencv.hpp>

cv::Mat ShowBlackCircle(const cv::Mat h, cv::Point cp, int radius, int thik)
{
    cv::circle(h, cp, radius, cv::Scalar(0, 0, 0), thik);
    return h;
}

void rounded_rectangle(cv::Mat &src, cv::Point topLeft, cv::Point bottomRight, const cv::Scalar lineColor, const int thickness, const int lineType, const int cornerRadius)
{
    /* corners:
     * p1 - p2
     * |     |
     * p4 - p3
     */
    cv::Point p1 = topLeft;
    cv::Point p2 = cv::Point(bottomRight.x, topLeft.y);
    cv::Point p3 = bottomRight;
    cv::Point p4 = cv::Point(topLeft.x, bottomRight.y);

    // draw straight lines
    line(src, cv::Point(p1.x + cornerRadius, p1.y), cv::Point(p2.x - cornerRadius, p2.y), lineColor, thickness, lineType);
    line(src, cv::Point(p2.x, p2.y + cornerRadius), cv::Point(p3.x, p3.y - cornerRadius), lineColor, thickness, lineType);
    line(src, cv::Point(p4.x + cornerRadius, p4.y), cv::Point(p3.x - cornerRadius, p3.y), lineColor, thickness, lineType);
    line(src, cv::Point(p1.x, p1.y + cornerRadius), cv::Point(p4.x, p4.y - cornerRadius), lineColor, thickness, lineType);

    // draw arcs
    ellipse(src, p1 + cv::Point(cornerRadius, cornerRadius), cv::Size(cornerRadius, cornerRadius), 180.0, 0, 90, lineColor, thickness, lineType);
    ellipse(src, p2 + cv::Point(-cornerRadius, cornerRadius), cv::Size(cornerRadius, cornerRadius), 270.0, 0, 90, lineColor, thickness, lineType);
    ellipse(src, p3 + cv::Point(-cornerRadius, -cornerRadius), cv::Size(cornerRadius, cornerRadius), 0.0, 0, 90, lineColor, thickness, lineType);
    ellipse(src, p4 + cv::Point(cornerRadius, -cornerRadius), cv::Size(cornerRadius, cornerRadius), 90.0, 0, 90, lineColor, thickness, lineType);
}
