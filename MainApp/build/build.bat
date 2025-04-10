
@echo off

if not exist obj md obj
if not exist output md output
cd obj
if not exist image md image
cd ..

set PATH=D:\Environment\make381;D:\Environment\RISCVGCC810\bin
set TOOLS_DIR=D:\Environment\RISCVGCC810

set NAME=LiteMApiTestApp
set VER=1.12

make -s %1 all

mkimage sign rsa.private 1 0xA7260102 ./output/%NAME%_%VER%.bin ./output/%NAME%_sign.bin


set SRC_BIN_PATH=%cd%

copy /Y %SRC_BIN_PATH%\output\%NAME%_sign.bin %SRC_BIN_PATH%\..\..\release\

REM set FILE1=./../../release/LiteMApiTestApp_sign.bin
REM set FILE2=./../../release/SubApp_EMV_sign.bin

REM for %%A in (%FILE1%) do set fileSize=%%~zA
REM REM echo fileSize = %fileSize%
REM set /a fileSize+=512

REM set code=0123456789ABCDEF
REM set var=%fileSize%
REM set hexlen=0
REM :again
REM set /a tra=%var%%%16
REM call,set tra=%%code:~%tra%,1%%
REM set /a var/=16
REM set str=%tra%%str%
REM if %var% geq 16 goto again
REM set hexlen=%var%%str%
REM REM echo HEX = %hexlen%
REM REM mkimage merge_bin 0xFF "%FILE1% 0x94000 %FILE2% 0x00" ./../../release/ota.asc
REM mkimage merge_bin 0xFF "%FILE1% %hexlen% %FILE2% 0x00" ./../../release/ota.asc

pause

