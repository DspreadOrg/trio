## **How to download files using tool**



This is a hidden function. press the key `3` continuously during the APP startup process can enter the interface of the download function. At this time use the 10P_ota_Tool.exe tool can download files to the device.The interface of the device to enter the download function is as below, in this interface, press `1` to enter the program normally .



​                                        ![](https://i.postimg.cc/JzCXS1ZS/image.png)

  



Use the [10P_ota_Tool](https://gitlab.com/dspread/trio/-/blob/main/tools/10p_OTAtool.zip) tool and follow the steps in the screenshot.  



​                                        ![](https://i.postimg.cc/HjDF4XT0/t-QBBJDXg-XCw-Hj3s-QQg-RV2vg-100.png)



**Note**: 



**1.** if the name of the downloaded file is the same as that already in the device, the file in the original device will be replaced with new file.

**2.** When using this function, you need to block the `testPngFileLoad` function (just change the variable `loadFlag` in the function to 1).



​                                        ![](https://i.postimg.cc/FRqPBRKP/20230629211240.png)