#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <Windows.h>
#include <iostream>
#include <ctime>
#include <cstdio>
#include <time.h>
#include <stdio.h>
#include <vector>

#define WINVER 0x0500

using namespace std;
using namespace cv;

struct PointWithColor
{
    cv::Point point;
    cv::Vec3b color;
};

Mat hwnd2mat(HWND hwnd)
{
    HDC hwindowDC, hwindowCompatibleDC;

    int height, width, srcheight, srcwidth;
    HBITMAP hbwindow;
    Mat src;
    BITMAPINFOHEADER bi;

    hwindowDC = GetDC(hwnd);
    hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
    SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

    RECT windowsize; // get the height and width of the screen
    GetClientRect(hwnd, &windowsize);

    srcheight = windowsize.bottom;
    srcwidth = windowsize.right;
    height = windowsize.bottom / 1; // change this to whatever size you want to resize to
    width = windowsize.right / 1;

    src.create(height, width, CV_8UC4);

    // create a bitmap
    hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
    bi.biSize = sizeof(BITMAPINFOHEADER); // http://msdn.microsoft.com/en-us/library/windows/window/dd183402%28v=vs.85%29.aspx
    bi.biWidth = width;
    bi.biHeight = -height; // this is the line that makes it draw upside down or not
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    // use the previously created device context with the bitmap
    SelectObject(hwindowCompatibleDC, hbwindow);
    // copy from the window device context to the bitmap device context
    StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 0, 0, srcwidth, srcheight, SRCCOPY); // change SRCCOPY to NOTSRCCOPY for wacky colors !
    GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO *)&bi, DIB_RGB_COLORS);    // copy from hwindowCompatibleDC to hbwindow

    // avoid memory leak
    DeleteObject(hbwindow);
    DeleteDC(hwindowCompatibleDC);
    ReleaseDC(hwnd, hwindowDC);

    return src;
}

const double heightIndex = 7.5;
const double widthIndex = 11.7;

const double radiusIndex = /*32*/ 26;
const double spacing = 6;

const double alpha = 5; /**< Simple contrast control */
const int beta = 0;     /**< Simple brightness control */

Mat constrast(double alpha, int beta, Mat image)
{
    Mat new_image = Mat::zeros(image.size(), image.type());
    for (int y = 0; y < image.rows; y++)
    {
        for (int x = 0; x < image.cols; x++)
        {
            for (int c = 0; c < 3; c++)
            {
                new_image.at<Vec3b>(y, x)[c] =
                    saturate_cast<uchar>(alpha * (image.at<Vec3b>(y, x)[c]) + beta);
            }
        }
    }
    return new_image;
}

Mat ShowBlackCircle(const cv::Mat &img, cv::Point cp, int radius, int thik)
{
    // int t_out = 0;
    // std::string win_name = "circle";
    cv::Scalar black(0, 0, 0);
    cv::circle(img, cp, radius, black, thik);
    // cv::imshow( win_name, img ); cv::waitKey( t_out );
    return img;
}

std::vector<PointWithColor> safeWhitePixels(Mat img)
{
    std::vector<PointWithColor> whitePixels;
    Mat only_white = Mat::zeros(img.size(), img.type());
    for (int y = 0; y < img.rows; y++)
    {
        for (int x = 0; x < img.cols; x++)
        {
            Vec3b color = img.at<Vec3b>(Point(x, y));
            if (color[0] > 250 && color[1] > 250 && color[2] > 250)
            {
                whitePixels.push_back({Point(x, y), color});
                // std::cout << "WHITE" << std::endl;
            }
        }
    }
    return whitePixels;
}

bool compareWhitePixels(std::vector<PointWithColor> first, std::vector<PointWithColor> second)
{
    if (first.size() != second.size())
    {
        std::cout << "Different size" << std::endl;
        return false;
    }
    return true;
}

int main(int argc, char **argv)
{
    HWND hwndDesktop = GetDesktopWindow();
    namedWindow("output", WINDOW_AUTOSIZE);
    int key = 0;

    // time_t start, end;
    std::vector<PointWithColor> lastWhite;
    bool first = true, checkBoxAppear = true, firstInWhiteLine = true;
    double lastPressTime = 0;
    while (key != 27)
    {
        // start=clock();
        Mat src = hwnd2mat(hwndDesktop);
        // Mat croped = src(Range(src.size().width / 2 - squareSide/2, src.size().width / 2 + squareSide/2),Range(src.size().height/2,src.size().height/2 + squareSide)); // Slicing to crop the image

        int squareHeight = int(double(src.size().height) / heightIndex);
        int squareWidth = int(double(src.size().width) / widthIndex);

        Rect crop_region((src.size().width / 2) - (squareWidth / 2), src.size().height / 2 - (squareHeight / 2), squareWidth, squareHeight);
        // specifies the region of interest in Rectangle form

        auto croped = src(crop_region);
        // croped = constrast(alpha, beta, croped);

        croped = ShowBlackCircle(croped, Point(croped.size().width / 2, croped.size().height / 2), (double(src.size().width) / radiusIndex), FILLED);

        croped = ShowBlackCircle(croped, Point(croped.size().width / 2, croped.size().height / 2), (double(src.size().width) / radiusIndex) + spacing + (100 / 2), 100);

        auto currentPixels = safeWhitePixels(croped);
        if (!currentPixels.empty())
        {
            if (first)
            {
                lastWhite = currentPixels;
                first = false;
            }
            else
            {
                if (!compareWhitePixels(lastWhite, currentPixels))
                {
                    if (checkBoxAppear)
                    {
                        std::cout << "Checkbox appear" << std::endl;
                        checkBoxAppear = false;
                    }
                    else
                    {
                        if (firstInWhiteLine)
                        {
                            std::cout << "PRESS" << std::endl;
                            lastPressTime = clock();
                            firstInWhiteLine = false;

                            INPUT ip;

                            // Pause for 5 seconds.

                            // Set up a generic keyboard event.
                            ip.type = INPUT_KEYBOARD;
                            ip.ki.wScan = 0; // hardware scan code for key
                            ip.ki.time = 0;
                            ip.ki.dwExtraInfo = 0;

                            // Press the "A" key
                            ip.ki.wVk = 0x20;  // virtual-key code for the "a" key
                            ip.ki.dwFlags = 0; // 0 for key press
                            SendInput(1, &ip, sizeof(INPUT));

                            // Release the "A" key
                            ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
                            SendInput(1, &ip, sizeof(INPUT));
                        }
                    }

                    // after 1 s of no change in the pixels checkBox = true and firstInWhiteLine = true
                }

                lastWhite = currentPixels;
            }
        }
        if (clock() - lastPressTime > 1000 && lastPressTime != 0)
        {
            firstInWhiteLine = true;
            checkBoxAppear = true;
            lastPressTime = 0;
            std::cout << "RELEASE" << std::endl;
        }
        // you can do some image processing here
        imshow("output", croped);
        key = waitKey(10); // you can change wait time
    }
}