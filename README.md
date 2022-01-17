# Dead By Daylight Auto Skill Check Presser
DBD Auto Skill Checker is a popular Dead By Daylight game cheat, which can automaticly do skill cheks when you are fixing a generator. Cheat is based on screen capturing and is depended from screen resolution and ratio
## Instalation

- Download zip archive from release with last version of program
- Extract content of archive into a directory
- Run a exe file
- You will show this interface

![Program Interface](https://i.ibb.co/ZdFv6vn/Screenshot-2022-01-17-195036.png)

Big green circle indicates program state if it is **Green** it shows that program is working, if it is **Yellow** this indicates that programm captured white collor. And **Red** when programm perform a keyboard press.

If in your PC program does not work, you can press **Show Area** button to enable capture area output

![Program Interface](https://i.ibb.co/kxgKDrr/v.png)

Now you can see which part of screen captures program and adjust it settings.

## Source code compilation
If our program does not act corectly on you device you can try to build it yourself, adjusting some settings. To do that you need to install [mingw64] compiler, [cmake] and [opencv] library, but to make opencv work with mingw64 you need to download additional files from [here](you need to chose you opencv library version). And than copy all files in **opencv/build** folder(do not forget to add opencv\build\x64\mingw\bin and opencv\build\x64\mingw\lib in enviroment variables).

When you have all needed software installed you can open up a Visual Studio Code editor and open my program folder. Also yoo may need to install cmake extension in vscode.
After that choose mingw64 as compiler for program and now you can build it by pressing build button.

![kit image](https://i.ibb.co/8rWXwDz/image-2022-01-17-202705.png)

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
![scale](https://i.ibb.co/9WZYMz6/image-2022-01-17-203348.png)

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
