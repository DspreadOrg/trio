
## **How to download firmwre**
##### **Install USB driver**

Unzip [zadig-2.4.zip](https://gitlab.com/dspread/trio/-/blob/main/tools/zadig-2.4.zip), execute zadig-2.4.exe to start installing the driver tool.
    
###### 1. Install SBI driver

- open the Device menu and select `Create New Device`

    ![](https://i.postimg.cc/RVKdsVM1/install-driver1.png)

- type the following information

    `Name filed: A726SBI`
    `USB ID: A726 0101`
    `Driver : WinUSB`

    Then click `Install Driver` button to start the installation. The success dialog box will pop up if the installation is successful.

    ![](https://i.postimg.cc/YSYmkjs8/install-driver1-OK.png)

##### 2. Install OS driver
- open the Device menu and select `Create New Device`

    ![](https://i.postimg.cc/RVKdsVM1/install-driver1.png)

- type the following information

    `Name filed: A726OS`
    `USB ID: A726 0102`
    `Driver : WinUSB`

    Then click `Install Driver` button to start the installation. The success dialog box will pop up if the installation is successful.

    ![](https://i.postimg.cc/TwC2XWnz/install-driver2-OK.png)

##### **Download the Firmware/Application**

Just unzip [DownloadFW-V3.0.6.zip](https://gitlab.com/dspread/trio/-/blob/main/tools/DownloadFW-V3.0.6.zip) file, execute DownloadFW.exe to start Download.


##### 1. Download Tool Operations

- step 1:Select Download configuration file¨[file.cfg](https://gitlab.com/dspread/trio/-/blob/main/release/file.cfg).

- step 2:Select the programs and files you want to download (all are selected by default).
- step 3:  Select communication type.(default is USB)
- step 4:open the start button. now waiting for download.

    ![](https://i.postimg.cc/1RLq4Lkf/downloadtools-oprate.png)

##### 2. device operations

The procedure is as follows:
- step 1: Unplug the USB, power off the device, and make sure the device is not charged.
- step 2: Press and hold the backspace key , then press and hold power key, finally, these two buttons are pressed at the same time.
- step 3: Insert USB cabel.

   After performing the third step, the download tool shows that it is in the process of downloading, and the interface prompt is as follows.

   ![](https://i.postimg.cc/2yVjw8gn/downloadtools-loading.jpg)

When the download is complete, the Download tool interface prompts are as follows,and the download is completed here.

![](https://i.postimg.cc/mg2znMKG/downloadtools-load-OK.jpg)

## **How to debug**
- Unzip the [LogViewer_UsbHid.Zip](https://gitlab.com/dspread/trio/-/blob/main/tools/LogViewer_UsbHid.zip) file
- Open the LogViewer_UsbHid.exe tool and plug in the USB cable to output log directly,you can see log when a program that can output log is running.
- Open the `APP_DEBUG` macro in [app.h](https://gitlab.com/dspread/trio/-/blob/main/MainApp/source/app.h) of demo, the demo APP program can output log.