@echo off

set BIN=\output
set OBJ=\obj
set ROOT_PATH=%cd%

cd %ROOT_PATH%
REM echo %BIN%
REM echo %OBJ%
REM echo %ROOT_PATH%%BIN%
rd /s /Q %ROOT_PATH%%BIN%
rd /s /Q %ROOT_PATH%%OBJ%

echo %ROOT_PATH%%BIN% deleted successful
echo %ROOT_PATH%%OBJ% deleted successful

REM make -f Makefile clean
