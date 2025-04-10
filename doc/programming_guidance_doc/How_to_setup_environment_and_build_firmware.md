## [**How to setup environment**]

Unzip [toolchain.zip](https://gitlab.com/dspread/trio/-/blob/main/tools/toolchain.zip) file to a path in PC, such as D:\Environment.
Be careful not to modify the directory hierarchy

![Env pic](https://i.postimg.cc/mDXhjRHQ/env.png)



## **How to build firmware**

- Modify [build.bat](https://gitlab.com/dspread/trio/-/blob/main/MainApp/build/build.bat) file

  Change the path of the red part and be the same as the paths in the **Environment Setup**,As shown below.

```
@echo off
if not exist obj md obj
if not exist output md output
set PATH=D:\Environment\make381;D:\Environment\RISCVGCC810\bin;D:\Environment\mkimage
set TOOLS_DIR= D:\Environment\\RISCVGCC810
set NAME=LiteMApiTestApp
set VER=1.1
make -s %1 all
mkimage sign rsa.private 1 0xA7260102 ./output/%NAME%_%VER%.bin ./output/%NAME%_%VER%_sign.bin
set SRC_BIN_PATH=%cd%
copy /Y %SRC_BIN_PATH%\output\*.bin %SRC_BIN_PATH%\..\..\release\
pause

```
- Build Firmware
  - Double-click cmd.exe to enter the compilation window

    ![](https://i.postimg.cc/DzFVzMzs/image.png)
  - Execute build.bat file, success indicates success as a result

    ![](https://i.postimg.cc/pX4h3DL3/image.png)
