#define WINVER 0x0500
#include <windows.h>
 
int main()
{
    // This structure will be used to create the keyboard
    // input event.
    INPUT ip;
 
    // Pause for 5 seconds.
    Sleep(5000);
 
    // Set up a generic keyboard event.
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0; // hardware scan code for key
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;
 
    // Press the "A" key
    ip.ki.wVk = 0x20; // virtual-key code for the "a" key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));
 
    // Release the "A" key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
 
    // Exit normally
    return 0;
}