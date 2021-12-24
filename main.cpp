#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/imgproc/types_c.h>
#include <Windows.h>
#include <iostream>
#include <ctime>
#include <cstdio>
#include <time.h>
#include <stdio.h>
#include <vector>
#include <stdlib.h> /* atexit */
#include <stack>
#include <map>
#include <windows.h>
#include <gdiplus.h>
#include <time.h>

#define WINVER 0x0500

// #define RELEASE_MODE

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

struct SizeScreenToCaptureArea
{
    unsigned int screnSize, radius;
};

const SizeScreenToCaptureArea widthRad = {1280, 43};
const SizeScreenToCaptureArea heightRad = {720, 43};

// const double heightIndex = 7.5;
// const double widthIndex = 11.7;
const SizeScreenToCaptureArea heightSquare = {720, 100};
const SizeScreenToCaptureArea widthSquare = {1280, 100};

// const double radiusIndex = 30;
const double spacing = 10;

Mat ShowBlackCircle(const cv::Mat &img, cv::Point cp, int radius, int thik)
{
    cv::circle(img, cp, radius, Scalar(0, 0, 0), thik);
    return img;
}

std::vector<PointWithColor> saveWhitePixels(Mat img)
{
    std::vector<PointWithColor> whitePixels;
    for (int y = 0; y < img.rows; y++)
    {
        for (int x = 0; x < img.cols; x++)
        {
            Vec3b color = img.at<Vec3b>(Point(x, y));
            if (color[0] == 255 && color[1] == 255 && color[2] == 255)
            {
                whitePixels.push_back({Point(x, y), color});
                // std::cout << "WHITE" << std::endl;
            }
        }
    }
    return whitePixels;
}

bool compareWhitePixels(std::vector<PointWithColor> &first, std::vector<PointWithColor> &second)
{
    if (first.size() != second.size())
    {
        return false;
    }
    return true;
}

BOOL onConsoleEvent(DWORD event)
{

    switch (event)
    {
    case CTRL_C_EVENT:
    case CTRL_CLOSE_EVENT:
        // do stuff
        std::cout << "CTRL_CLOSE_EVENT" << std::endl;
        break;
    }

    return TRUE;
}

int calculateRadius(int screenWidth, int screenHeight, SizeScreenToCaptureArea widthrad, SizeScreenToCaptureArea heightrad)
{
    double x = double(screenWidth) * double(widthrad.radius) / double(widthrad.screnSize);
    double y = double(screenHeight) * double(heightrad.radius) / double(heightrad.screnSize);
    return int((x + y) / 2.0);
}

int calculateSquare(int screenWidth, int screenHeight, SizeScreenToCaptureArea widthrad, SizeScreenToCaptureArea heightrad)
{
    double x = double(screenWidth) * double(widthrad.radius) / double(widthrad.screnSize);
    double y = double(screenHeight) * double(heightrad.radius) / double(heightrad.screnSize);
    auto res = int((x + y) / 2.0);

    if (y > x)
        return int((res + y) / 2.0);
    else
        return int((res + x) / 2.0);
}

void press()
{
    INPUT ip;

    // Pause for 5 seconds.

    // Set up a generic keyboard event.
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0; // hardware scan code for key
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;

    // Press the "SPACE" key
    ip.ki.wVk = 0x20;  // virtual-key code for the "SPACE" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "SPACE" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
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
    cv::cvtColor(image, thresholded, CV_RGB2GRAY);
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
    double whiteCount = 0, allPixelsCount = img.rows * img.cols;
    for (int y = 0; y < img.rows; y++)
    {
        for (int x = 0; x < img.cols; x++)
        {
            auto color = img.at<Vec3b>(Point(x, y));
            if (color[0] > 250 && color[1] > 250 && color[2] > 250)
            {
                whiteCount++;
            }
        }
    }
    return whiteCount * 100 / allPixelsCount;
}

cv::Rect findMOstWhiteRect(std::vector<cv::Rect> rects, cv::Mat img)
{
    double maxWhitePercentage = 0;
    cv::Rect maxWhiteRect;
    for (auto &rect : rects)
    {
        double percentage = whitePercentage(img(rect));
        if (percentage > maxWhitePercentage)
        {
            maxWhitePercentage = percentage;
            maxWhiteRect = rect;
        }
    }
    return maxWhiteRect;
}

Mat takeScreenshot()
{
    Mat h;
    RECT rc;
    // HWND hwnd = GetShellWindow();
    HWND hShellWnd = GetShellWindow();
    HWND hDefView = FindWindowEx(hShellWnd, NULL, _T("SHELLDLL_DefView"), NULL);
    HWND hwnd = FindWindowEx(hDefView, NULL, _T("SysListView32"), NULL);
    if (hwnd == NULL)
    {
        cout << "it can't find any 'note' window" << endl;
        return h;
    }
    GetClientRect(hwnd, &rc);

    // create
    HDC hdcScreen = GetDC(NULL);
    HDC hdc = CreateCompatibleDC(hdcScreen);
    HBITMAP hbmp = CreateCompatibleBitmap(hdcScreen,
                                          rc.right - rc.left, rc.bottom - rc.top);
    SelectObject(hdc, hbmp);

    // Print to memory hdc
    PrintWindow(hwnd, hdc, PW_CLIENTONLY);

    // copy to clipboard
    OpenClipboard(NULL);
    EmptyClipboard();
    SetClipboardData(CF_BITMAP, hbmp);
    CloseClipboard();

    // release
    DeleteDC(hdc);
    DeleteObject(hbmp);
    ReleaseDC(NULL, hdcScreen);

    cout << "success copy to clipboard, please paste it to the 'mspaint'" << endl;
    return h;
}

int main(int argc, char **argv)
{

    HWND hwndDesktop = GetDesktopWindow();
    Mat srcConfig = hwnd2mat(hwndDesktop);
    const int squareSideLength = calculateSquare(srcConfig.size().width, srcConfig.size().height, widthSquare, heightSquare);
    const int realRadius = calculateRadius(srcConfig.size().width, srcConfig.size().height, widthRad, heightRad);

    const Rect crop_region((srcConfig.size().width / 2) - (squareSideLength / 2), srcConfig.size().height / 2 - (squareSideLength / 2), squareSideLength, squareSideLength);
    auto cropedConfig = srcConfig(crop_region);
    const Point CircleCenter = Point(cropedConfig.size().width / 2, cropedConfig.size().height / 2);

    Mat img;
    std::vector<PointWithColor> lastWhite;
    bool checkBoxAppear = false;

    bool created = false;
    bool first2 = true;
    cv::Rect mostWhite;
    unsigned long long lastCheckBox = 0;

    // namedWindow("config", WINDOW_NORMAL);
    while (true)
    {
        auto t1 = clock();
        // hwndDesktop = GetDesktopWindow();
        // img = hwnd2mat(hwndDesktop);

        img = takeScreenshot();
        imshow("config", img);

        std::cout << "Time: " << (clock() - t1) << std::endl;
        waitKey(10000000);
        exit(0);

        img = img(crop_region);

        img = ShowBlackCircle(img, CircleCenter, realRadius, FILLED);
        img = ShowBlackCircle(img, CircleCenter, realRadius + spacing + 50, 100);

        if (!created)
        {
            auto currentPixels = saveWhitePixels(img);
            if (!currentPixels.empty())
            {
                std::cout << "CheckBox appear" << std::endl;

                std::vector<cv::Rect> h;
                FindBlobs(img, h, 3);

                mostWhite = findMOstWhiteRect(h, img);

                created = true;
                lastCheckBox = clock();
            }
        }
        else
        {
            if (clock() - lastCheckBox >= 3000)
            {
                std::cout << "TIMER" << std::endl;
                created = false;
            }
            else
            {
                Mat submat = img(mostWhite);
                cv::MatConstIterator_<cv::Vec3b> it; // = src_it.begin<cv::Vec3b>();
                for (it = submat.begin<cv::Vec3b>(); it != submat.end<cv::Vec3b>(); ++it)
                {

                    if ((*it)[0] < 20 && (*it)[1] < 20 && (*it)[2] > 90)
                    {
                        std::cout << "RED" << std::endl;
                        press();
                        waitKey(1000);
                        created = false;
                        break;
                    }
                }
                // std::cout << std::endl;
                // imshow("config", submat);
                // waitKey(1);
            }
        }
        // imshow("output", img);
        // waitKey(1);
    }

    return 0;
}

// Mat croped = src(Range(src.size().width / 2 - squareSide/2, src.size().width / 2 + squareSide/2),Range(src.size().height/2,src.size().height/2 + squareSide)); // Slicing to crop the image
// std::vector<cv::Rect> h;
// FindBlobs(img, h, 3);

// // for(int i = 0; i < h.size(); i++)
// // {
// //     rectangle(img, h[i], Scalar(255,0,0), 2, 8, 0);
// // }

// cv::Rect mostWhite = findMOstWhiteRect(h, img);
// // rectangle(img, mostWhite, Scalar(0,255,0), 2, 8, 0);
// auto newImg = img(mostWhite);
// // specifies the region of interest in Rectangle form
// map<Vec3b, int, lessVec3b> palette = getPalette(newImg);
// int areanewImg = img.rows * img.cols;

// for (auto color : palette)
// {
//     cout << "Color: " << color.first << " \t - Area: " << 100.f * float(color.second) / float(areanewImg) << "%" << endl;
// }

// auto croped = src(crop_region);
// // croped = constrast(alpha, beta, croped);

// auto currentPixels = safeWhitePixels(croped);
// if (!currentPixels.empty())
// {
//     if (first)
//     {
//         lastWhite = currentPixels;
//         first = false;
//     }
//     else
//     {
//         if (!compareWhitePixels(lastWhite, currentPixels))
//         {
//             if (whitePixelsChangeCount > 0)
//             {
//                 std::cout << "Press" << std::endl;
//                 press();
//                 lastPressTime = clock();

//                 whitePixelsChangeCount = 0;
//                 pressed = true;
//             }
//             else
//             {
//                 whitePixelsChangeCount++;
//             }
//         }

//         lastWhite = currentPixels;
//     }
// }

// if (clock() - lastPressTime > 1500 && pressed)
// {
//     whitePixelsChangeCount = 0;
//     pressed = false;
// }

// imshow("Full image", img);
// imshow("Croped image", newImg);
// int k = waitKey(0); // Wait for a keystroke in the window