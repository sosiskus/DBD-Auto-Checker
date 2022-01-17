# Dead By Daylight Auto Skill Check Presser
DBD Auto Skill Checker is a popular Dead By Daylight game cheat, which can automaticly do skill cheks when you are fixing a generator. Cheat is based on screen capturing and is depended from screen resolution and ratio
## Instalation

- Download zip archive from release with last version of program
- Extract content of archive into a directory
- Run a exe file
- You will show this interface
![Program Interface](https://previews.dropbox.com/p/thumb/ABamk2SEQiKFlBqmifvfnQeuUrV8IbfSO5Xqy8d6Ec0EQ0-_RIA-XrFs5aUwxrMpq9x0XUU_9pEaU2Kw3OUM1SAEyTUYUISOTIQ9Y75KPcwhxJg1KhyvjzGPf-wifPP_Qq8lPNApTzhTBWtNE7kYP6LxAOC3eEuVCr57ZnQ9cm7JglKzB7oUNDQNdL5w3aE4iM67EENUDq9eBGuTAnWCclXKkjUrNPiiwyb4NHlkOYbAU7KcoKAcjJJ4rsOZ3lOoxOQMdBAM8GU0FFdnQ84XXKeflA_r7lGYEKWJldmYQFp2KFnElK_ainml1ewMCM7ZvzeWjmIz_ioHsDXE5kOSq2689q0ytZrwv2VX-dG26whEKw/p.png)

Big green circle indicates program state if it is **Green** it shows that program is working, if it is **Yellow** this indicates that programm captured white collor. And **Red** when programm perform a keyboard press.

If in your PC program does not work, you can press **Show Area** button to enable capture area output
![Program Interface](https://previews.dropbox.com/p/thumb/ABY2jvtdRicoBcHvjmxUnzyvlJnMH0kRpd18gjDXQ5x8Wvr_YhVVzP6pt_59Gt2crxfoH6vYjdoXPHtd_oX1KXuAcYMliRJe3Z1Z5F3I8Pbmgm5nVKnXoy8yIeZInCEvlBCR5thFhmwHtF6N1PPjYblmWV0yOkViCnrXCN6WHhjespN89ut3766EhB3cHylqLwuaEWS8mTIy2yTO2DnROJB08xtpfBxfKyqSQDV8Ri-_0U3svPXwA3LArcmMxKmxGKStyLz1jKkqQxO05bxhkQaTveZNckoICFJcGR1KfpmInz-yvalSRIwAbyjsdBkW-6Y5iwRr-HycRCgj5lMh7D_G0kPHAOvtxEkIoQRojaI4Yw/p.png)
Now you can see which part of screen captures program and adjust it settings.

## Source code compilation
If our program does not act corectly on you device you can try to build it yourself, adjusting some settings. To do that you need to install [mingw64] compiler, [cmake] and [opencv] library, but to make opencv work with mingw64 you need to download additional files from [here](you need to chose you opencv library version). And than copy all files in **opencv/build** folder(do not forget to add opencv\build\x64\mingw\bin and opencv\build\x64\mingw\lib in enviroment variables).

When you have all needed software installed you can open up a Visual Studio Code editor and open my program folder. Also yoo may need to install cmake extension in vscode.
After that choose mingw64 as compiler for program and now you can build it by pressing build button.
![kit image](https://previews.dropbox.com/p/thumb/ABbdc5-1lhOhoq18-aDCrDkTax7ojYtgLBZoxqT7ETQSfiV95OEFNg0mlt2Wj7Vmxnd2TOP1V_P1lmpswE-_D9NG4w9ngXX1K2XXu807VQiCe6R3DS0ljqzNAruJpO_w6It63Ipeum_2qkkpeKwOLmA-lSKPt1tyjbPeYVHvCTcG9Hx90msLaRaGfjydiOeEjA7YskvxstS8mVbfxRWJ56-ZVQ4pZQ_tyP1N0B_dG_PFSlQBTL6rUiV8Q3p6HqPaHZoK3qp0dbG8TaXLfSQjk52kRl8y6iKfqky-dFr8IohpVwO_M9dXQVYLOCEtc6uc7OvPpXO5vG4MU1KbJtfvfgYTlrE9imiIi_WOPPFi7HYKFw/p.png)

if you recieve some errors, firstly tro to remove this lines in **CMakeLists.txt** file
```cmake
add_custom_command(
    TARGET DBD_Auto_Check
    COMMAND go run ${CMAKE_CURRENT_SOURCE_DIR}/scripts/releaseCode.go
    COMMENT "Running release code generator"
)
```
If this does not help check if you have choosen correct compiler and library version. 

After build you can find program executable in /build folder

## Configuration
When you have succesefully built a program you can now try to configure it under your system. Open a **Config.hpp** file located in root directory.
Firstly, you need to check if your screen scale and program scale is equal
```c++
const double scale = 1.0; // Screan scale
```
![scale](https://previews.dropbox.com/p/thumb/ABZ1k3vrXcotJKARRjF3hIHpn0jlQIiTVurekT_-MXHSZrE7rMqAT1p_RSYX2dVAFAUjvZyLd9eH5naWXQfs_X5Wa2LfdbRc8yGRDOaab-yERn0iGj5ukpYN5a3hAguMh8CiNe4atJ4Azmw30ogcjWcHZbhNniwVCJGWq0bfyAJ98O1ACXCaR_f-imqYXSx2yOS3OAj9m_mimwA2frCYlxxP3yPs5Kj7MEtnU5b2qlJHVEwbvRfu4VBqOX443Ji8GJGzG1qzo2-VmapnNvo-YLLPNMrCrw1LFil6-YaifYH4zLDkDVHOzLagpNHO72Thkxv0knTtew0hKs1urAQp469hGHk62Mtos6ohmDPpt8N3NA/p.png)

If this does not help and you see that either a black circle is too big or window size is to small, you can try to adjust this settings.
```c++
const SizeScreenToCaptureArea widthRad = {1280, 43};
const SizeScreenToCaptureArea heightRad = {720, 43};
```
This mean that on 1280 x 720 screen perfect capture window size is 100px, you can increase this value if your checkbox not fits in current window. If you have problems with the black circle, which overlaps a chekbox in game you can decrease it radius using this two variables.
```c++
const SizeScreenToCaptureArea widthRad = {1280, 43};
const SizeScreenToCaptureArea heightRad = {720, 43};
```
`I suggest you to change this settings in pairs`

If you notices that program haa lack of accuracy you can increase this variable. The more you increase it, earlier program will press.
```c++
const int amount = 3; // Increasing amount programm presses earlier
```
> so program see a press point and presses 3px earlier

[here]: <https://github.com/huihut/OpenCV-MinGW-Build>
[opencv]: <https://sourceforge.net/projects/opencvlibrary/files/4.5.5/opencv-4.5.5-vc14_vc15.exe/download>
[cmake]: <https://cmake.org/download/>
[mingw64]: <https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/installer/mingw-w64-install.exe/download>
