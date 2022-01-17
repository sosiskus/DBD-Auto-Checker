#pragma once
#include <opencv2/highgui.hpp>

#include <iostream>
#include <atomic>

std::atomic<bool> buttonClicked;
void CallBackFunc(int event, int x, int y, int flags, void *userdata)
{
    if (event == 1 /*EVENT_LBUTTONDOWN*/)
    {
        std::cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << std::endl;
        if (x >= 10 && x <= 190 && y >= 100 && y <= 130)
        {
            std::cout << "You have clicked on the Button" << std::endl;
            buttonClicked = true;
        }
    }
}

void press()
{
    INPUT ip;

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

BOOL onConsoleEvent(DWORD event)
{

    switch (event)
    {
    case CTRL_C_EVENT:
    case CTRL_CLOSE_EVENT:
        std::cout << "CTRL_CLOSE_EVENT" << std::endl;
        break;
    }

    return TRUE;
}

void mouseDown()
{
    INPUT Inputs[2] = {0};

    Inputs[0].type = INPUT_MOUSE;
    Inputs[0].mi.dx = 10;
    Inputs[0].mi.dy = 10;
    Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

    Inputs[1].type = INPUT_MOUSE;
    Inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;

    SendInput(2, Inputs, sizeof(INPUT));
}

void mouseUp()
{
    INPUT Inputs[2] = {0};

    Inputs[0].type = INPUT_MOUSE;
    Inputs[0].mi.dx = 10;
    Inputs[0].mi.dy = 10;
    Inputs[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

    Inputs[1].type = INPUT_MOUSE;
    Inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;

    SendInput(2, Inputs, sizeof(INPUT));
}

void pressD_A()
{
    INPUT ip;

    // Set up a generic keyboard event.
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0; // hardware scan code for key
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;

    // Press the "SPACE" key
    ip.ki.wVk = 0x41;  // virtual-key code for the "SPACE" key
    ip.ki.dwFlags = 0; // 0 for key pressaa
    SendInput(1, &ip, sizeof(INPUT));

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    // Press the "SPACE" key
    ip.ki.wVk = 0x44;  // virtual-key code for the "SPACE" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    ip.ki.wVk = 0x41;                // virtual-key code for the "SPACE" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    ip.ki.wVk = 0x44;                // virtual-key code for the "SPACE" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}