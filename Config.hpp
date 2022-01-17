#pragma once

struct PointWithColor
{
    cv::Point point;
    cv::Vec3b color;
};
struct SizeScreenToCaptureArea
{
    unsigned int screnSize, radius;
};

const SizeScreenToCaptureArea widthRad = {1280, 43};
const SizeScreenToCaptureArea heightRad = {720, 43};

const SizeScreenToCaptureArea heightSquare = {720, 100};
const SizeScreenToCaptureArea widthSquare = {1280, 100};

const double spacing = 10; // Space between circles

const int amount = 3; // Increasing amount programm presses earlier
const double scale = 1.0; // Screan scale



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