#pragma once
#include <opencv2/opencv.hpp>

cv::Mat hwnd2mat(HWND hwnd, const cv::Rect &crop_region)
{
    HDC hwindowDC, hwindowCompatibleDC;

    // int height, width, srcheight, srcwidth;
    HBITMAP hbwindow;
    // cv::Mat src;
    BITMAPINFOHEADER bi;

    hwindowDC = GetDC(hwnd);
    hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
    SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

    // cv::Rect windowsize; // get the height and width of the screen
    // GetClientcv::Rect(hwnd, &windowsize);

    // srcheight = windowsize.bottom;
    // srcwidth = windowsize.right;
    // height = windowsize.bottom / 1; // change this to whatever size you want to resize to
    // width = windowsize.right
    const int width = crop_region.width;
    const int height = crop_region.height;

    // src.create(height, width, CV_8UC4); // BUG
    cv::Mat src(height, width, CV_8UC4);

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
    BitBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, crop_region.x, crop_region.y, SRCCOPY);

    GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO *)&bi, DIB_RGB_COLORS); // copy from hwindowCompatibleDC to hbwindow

    // avoid memory leak
    DeleteObject(hbwindow);
    DeleteDC(hwindowCompatibleDC);
    ReleaseDC(hwnd, hwindowDC);

    return src;
}
