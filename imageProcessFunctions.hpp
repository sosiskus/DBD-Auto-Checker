#pragma once
#include <opencv2/opencv.hpp>

// Image count colored pixels in range [lowerBound, upperBound]
int countPixels(cv::Mat img, cv::Scalar lowerBound, cv::Scalar upperBound)
{
    int counter = 0;
    cv::MatConstIterator_<cv::Vec3b> it; // = src_it.begin<cv::Vec3b>();
    for (it = img.begin<cv::Vec3b>(); it != img.end<cv::Vec3b>(); ++it)
    {
        if ((*it)[0] >= lowerBound[0] && (*it)[0] <= upperBound[0] && (*it)[1] >= lowerBound[1] && (*it)[1] <= upperBound[1] && (*it)[2] >= lowerBound[2] && (*it)[2] <= upperBound[2])
        {
            counter++;
        }
    }
    return counter;
}

std::vector<PointWithColor> saveWhitePixels(cv::Mat img)
{
    std::vector<PointWithColor> whitePixels;
    for (int y = 0; y < img.rows; y++)
    {
        for (int x = 0; x < img.cols; x++)
        {
            cv::Vec3b color = img.at<cv::Vec3b>(cv::Point(x, y));
            if (color[0] == 255 && color[1] == 255 && color[2] == 255)
            {
                whitePixels.push_back({cv::Point(x, y), color});
                // std::cout << "WHITE" << std::endl;
            }
        }
    }
    return whitePixels;
}

void Traverse(int xs, int ys, cv::Mat &ids, cv::Mat &image, int blobID, cv::Point &leftTop, cv::Point &rightBottom)
{
    std::stack<cv::Point> S;
    S.push(cv::Point(xs, ys));

    while (!S.empty())
    {
        cv::Point u = S.top();
        S.pop();

        int x = u.x;
        int y = u.y;

        if (image.at<unsigned char>(y, x) == 0 || ids.at<unsigned char>(y, x) > 0)
            continue;

        ids.at<unsigned char>(y, x) = blobID;
        if (x < leftTop.x)
            leftTop.x = x;
        if (x > rightBottom.x)
            rightBottom.x = x;
        if (y < leftTop.y)
            leftTop.y = y;
        if (y > rightBottom.y)
            rightBottom.y = y;

        if (x > 0)
            S.push(cv::Point(x - 1, y));
        if (x < ids.cols - 1)
            S.push(cv::Point(x + 1, y));
        if (y > 0)
            S.push(cv::Point(x, y - 1));
        if (y < ids.rows - 1)
            S.push(cv::Point(x, y + 1));
    }
}

int FindBlobs(cv::Mat &image, std::vector<cv::Rect> &out, float minArea)
{
    cv::Mat ids = cv::Mat::zeros(image.rows, image.cols, CV_8UC1);
    cv::Mat thresholded;
    cv::cvtColor(image, thresholded, CV_RGB2GRAY); // bug
    const int thresholdLevel = 130;
    cv::threshold(thresholded, thresholded, thresholdLevel, 255, CV_THRESH_BINARY);
    int blobId = 1;
    for (int x = 0; x < ids.cols; x++)
        for (int y = 0; y < ids.rows; y++)
        {
            if (thresholded.at<unsigned char>(y, x) > 0 && ids.at<unsigned char>(y, x) == 0)
            {
                cv::Point leftTop(ids.cols - 1, ids.rows - 1), rightBottom(0, 0);
                Traverse(x, y, ids, thresholded, blobId++, leftTop, rightBottom);
                cv::Rect r(leftTop, rightBottom);
                if (r.area() > minArea)
                    out.push_back(r);
            }
        }
    return blobId;
}
double whitePercentage(cv::Mat img)
{
    double whiteCount = 0;
    for (int y = 0; y < img.rows; y++)
    {
        for (int x = 0; x < img.cols; x++)
        {
            auto color = img.at<cv::Vec3b>(cv::Point(x, y));
            if (color[0] > 250 && color[1] > 250 && color[2] > 250)
            {
                whiteCount++;
            }
        }
    }
    return whiteCount;
}
cv::Rect findMOstWhiteRect(std::vector<cv::Rect> rects, cv::Mat img, int minSide)
{
    double maxWhitePercentage = 0;
    cv::Rect maxWhiteRect;
    for (auto rect : rects)
    {
        if (rect.width < minSide || rect.height < minSide)
            continue;

        int currentPercentage = countPixels(img(rect), cv::Scalar(255, 255, 255), cv::Scalar(255, 255, 255)) * 100 / rect.area();
        if (currentPercentage > maxWhitePercentage)
        {
            maxWhitePercentage = currentPercentage;
            maxWhiteRect = rect;
        }
    }
    return maxWhiteRect;
}
