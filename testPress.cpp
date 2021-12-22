#define WINVER 0x0500
#include <Windows.h>
#include <iostream>

BOOL WINAPI ConsoleHandlerRoutine(DWORD dwCtrlType)
{
    if (dwCtrlType == CTRL_CLOSE_EVENT or dwCtrlType == CTRL_C_EVENT)
    {
        std::cout << "CTRL_CLOSE_EVENT" << std::endl;
        return TRUE;
    }
    return FALSE;
}


int main()
{
    BOOL ret = SetConsoleCtrlHandler(ConsoleHandlerRoutine, TRUE);

    while(true);

    // Exit normally
    return 0;
}