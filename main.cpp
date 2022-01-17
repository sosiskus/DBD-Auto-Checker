#include "opencv2/imgproc.hpp"
#include <opencv2/highgui.hpp>

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
#include <thread>

#include "Config.hpp"
#include "KeyboardEventsM.hpp"
#include "imageProcessFunctions.hpp"
#include "gui.hpp"
#include "hwnd2mat.hpp"

using namespace std;
using namespace cv;

cv::Mat img;
cv::Mat menu;
std::mutex imageMutex;

std::atomic<bool> isRunning;
void captureScreenshot(Rect crop_region)
{
    HWND hwnd;
    Mat screenshot;
    while (isRunning)
    {
        hwnd = GetDesktopWindow();
        screenshot = hwnd2mat(hwnd, crop_region);

        imageMutex.lock();
        img = screenshot;
        imageMutex.unlock();
    }
}

void processScreenshot(const Point CircleCenter, const int realRadius)
{
    bool created = false;
    unsigned long long lastCheckBox = 0;
    Rect mostWhite;
    bool pressed = false;
    bool keyDown = false;
    while (isRunning)
    {
        if (GetKeyState('Z') < 0)
        {
            // DOWN
            keyDown = true;
        }
        if (GetKeyState('Z') >= 0 && keyDown)
        {
            // UP
            keyDown = false;
            if (!pressed)
            {
                mouseDown();
                pressed = true;
            }
            else
            {
                mouseUp();
                pressed = false;
            }
        }

        imageMutex.lock();
        img = ShowBlackCircle(img, CircleCenter, realRadius, FILLED).clone();
        img = ShowBlackCircle(img, CircleCenter, realRadius + spacing + 50, 100).clone();

        Mat screenshot = img.clone();
        imageMutex.unlock();

        if (!created)
        {
            if (countPixels(screenshot, Scalar(220, 220, 220), Scalar(255, 255, 255)) > 1)
            {

                std::vector<cv::Rect> h;
                FindBlobs(screenshot, h, 3);

                mostWhite = findMOstWhiteRect(h, screenshot, 5);

                // rectangle(img, mostWhite, Scalar(255, 0, 0), 1); // DEBUG

                if (mostWhite.area() == 0)
                    continue;
                std::cout << "CheckBox appear" << std::endl;

                imageMutex.lock();
                circle(menu, Point(50, 50), 25, Scalar(30, 190, 190), -1); // Change menu circle color to yellow
                imageMutex.unlock();

                if (mostWhite.x + mostWhite.width + amount > screenshot.size().width)
                {
                    mostWhite.width = screenshot.size().width - mostWhite.x;
                }
                else
                {
                    mostWhite.width += amount;
                }

                if (mostWhite.y + mostWhite.height + amount > screenshot.size().height)
                {
                    mostWhite.height = screenshot.size().height - mostWhite.y;
                }
                else
                {
                    mostWhite.height += amount;
                }

                created = true;
                lastCheckBox = clock();
            }
        }
        else
        {
            if (clock() - lastCheckBox >= 3000)
            {
                std::cout << "TIMER" << std::endl;

                imageMutex.lock();
                circle(menu, Point(50, 50), 25, Scalar(60, 150, 0), -1); // Change menu circle color to green
                imageMutex.unlock();

                created = false;
            }
            else
            {
                Mat submat = screenshot(mostWhite);
                if (countPixels(submat, Scalar(0, 0, 150), Scalar(70, 70, 255)) > 1)
                {
                    std::cout << "RED" << std::endl;
                    press();

                    imageMutex.lock();
                    circle(menu, Point(50, 50), 25, Scalar(60, 0, 150), -1); // Change menu.load() circle color to red
                    imageMutex.unlock();

                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

                    imageMutex.lock();
                    circle(menu, Point(50, 50), 25, Scalar(60, 150, 0), -1); // Change menu.load() circle color to green
                    imageMutex.unlock();

                    created = false;
                }
            }
        }
    }
}

void showImages()
{
    namedWindow("Main", WINDOW_AUTOSIZE);
    setMouseCallback("Main", CallBackFunc, NULL);
    buttonClicked = false;
    while (isRunning)
    {
        imageMutex.lock();
        imshow("Main", menu);
        imageMutex.unlock();

        if (buttonClicked)
        {
            imageMutex.lock();
            imshow("Capture", img);
            imageMutex.unlock();
            waitKey(1);
            if (getWindowProperty("Capture", WND_PROP_VISIBLE) < 1)
            {
                std::cout << "Window \"Capture\" is not visible" << std::endl;
                destroyWindow("Capture");
                buttonClicked = false;
            }
        }
        else
        {
            waitKey(1);
        }
        if (getWindowProperty("Main", WND_PROP_VISIBLE) < 1)
        {
            std::cout << "Window \"Main\" is not visible" << std::endl;
            isRunning = false;
        }
    }
}

void escapeKillerThread()
{
    bool spam = false;
    bool keyDown = false;
    while (isRunning)
    {
        if (GetKeyState('X') < 0)
        {
            // DOWN
            keyDown = true;
        }
        if (GetKeyState('X') >= 0 && keyDown)
        {
            // UP
            keyDown = false;
            spam = !spam;
        }
        if (spam)
        {
            pressD_A();
        }
    }
}
struct cropSettings
{
    cv::Point CircleCenter;
    int Radius;
    cv::Rect CropRegion;

};

cropSettings coefficientsConfig()
{
    HWND hwnd = GetDesktopWindow();
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    Mat config = hwnd2mat(hwnd, Rect(0, 0, x, y));
    const int squareSideLength = calculateSquare(config.size().width, config.size().height, widthSquare, heightSquare);
    const int realRadius = calculateRadius(config.size().width, config.size().height, widthRad, heightRad);

    std::cout << "Size of screen: " << config.size().width << "x" << config.size().height << std::endl;

    const Rect crop_region((config.size().width / 2) * scale - (squareSideLength / 2) * scale, config.size().height / 2 * scale - (squareSideLength / 2) * scale, squareSideLength, squareSideLength);

    Mat cropedConfig = config(crop_region);
    const Point CircleCenter = Point(cropedConfig.size().width / 2, cropedConfig.size().height / 2);

    return {CircleCenter, realRadius, crop_region};
}

cv::Mat menuConfig()
{
    Mat menuLocal(Size(200, 150), CV_8UC3, Scalar(10, 10, 1));
    circle(menuLocal, Point(50, 50), 30, Scalar(220, 220, 220), -1);
    circle(menuLocal, Point(50, 50), 25, Scalar(60, 150, 0), -1);
    rounded_rectangle(menuLocal, Point(10, 100), Point(190, 130), Scalar(255, 255, 255), 1, 8, 10);

    cv::putText(menuLocal,          // target image
                "State",            // text
                cv::Point(100, 65), // top-left position
                cv::FONT_HERSHEY_DUPLEX,
                1,
                CV_RGB(220, 220, 220), // font color
                2);

    cv::Point seed(20, 120);

    cv::floodFill(menuLocal, seed, cv::Scalar(255, 255, 255), 0, Scalar(5, 5, 5, 5), Scalar(5, 5, 5, 5));

    cv::putText(menuLocal,          // target image
                "Show Area",        // text
                cv::Point(30, 122), // top-left position
                cv::FONT_HERSHEY_DUPLEX,
                0.8,
                CV_RGB(100, 100, 100), // font color
                2);
    return menuLocal;
}

int main()
{
    isRunning = true;
    buttonClicked = false;

    cropSettings con = coefficientsConfig();
    imageMutex.lock();
    menu = menuConfig();
    imageMutex.unlock();

    std::thread process(processScreenshot, con.CircleCenter, con.Radius);
    std::thread show(showImages);
    std::thread takeScreenshot(captureScreenshot, con.CropRegion);
    std::thread escapeKiller(escapeKillerThread);

    process.join();
    show.join();
    takeScreenshot.join();
    escapeKiller.join();
    destroyAllWindows();
    return 0;
}