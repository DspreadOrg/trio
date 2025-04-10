#include "app.h"
#include "file.h"

#ifdef FILE_DEBUG
#define FILE_STRLOG DBG_STR
#define FILE_HEXLOG DBG_HEX
#else
#define FILE_STRLOG  
#define FILE_HEXLOG 
#endif

int File_GetRecordNum(char * name, uint recSize)
{
    int fileSize;
    int recordNum;

    if(name == NULL || recSize == 0) {
        return RET_FILE_PARAM;
    }

    fileSize = filesize(name);
    if(fileSize < 0) {
        FILE_STRLOG("filesize(%s):%d\r\n", name, fileSize);
        return RET_FILE_FILE_SIZE;
    }
    if(fileSize == 0){
        return RET_FILE_NO_RECORD;
    }	
	recordNum = fileSize / recSize;
    if(recordNum == 0 || (fileSize % recSize)){
        FILE_STRLOG("recordNum = %d, fileSzie = %d, recSize = %d\r\n", recordNum, fileSize, recSize);
        return RET_FILE_DATA;
    }

    return recordNum;
}

int File_Read(char * name, int offset, byte * data, uint len)
{
    int fd;
    int iRet;

    if(name == NULL || data == NULL || len == 0){
        return RET_FILE_PARAM;
    }

    fd = open(name,O_CREATE|O_RDWR);
    if(fd < 0){
        FILE_STRLOG("open(%s):%d\r\n", name, fd);
        return RET_FILE_FILE_OPEN;
    }
	
    iRet = seek(fd, offset, SEEK_SET);
    if(iRet < 0) {
        close(fd);
        FILE_STRLOG("seek(%d):%d\r\n", fd, iRet);
        return RET_FILE_FILE_SEEK;
    }

    iRet = read(fd, (byte*)data, len);
	close(fd);
    if(iRet != len){
		FILE_STRLOG("read(%s) = %d, %d\r\n", name, fd, iRet);
        return RET_FILE_FILE_READ;
    }

    return iRet;
}

int File_Write(char * name, int offset, byte * data, uint len)
{
    int fd;
    int iRet;

    if(name == NULL || data == NULL || len == 0){
        return RET_FILE_PARAM;
    }

    fd = open(name,O_CREATE|O_RDWR);
    if(fd < 0){
        FILE_STRLOG("open(%s):%d\r\n", name, fd);
        return RET_FILE_FILE_OPEN;
    }
	
    iRet = seek(fd, offset, SEEK_SET);
    if(iRet < 0) {
        close(fd);
        FILE_STRLOG("seek(%d):%d\r\n", fd, iRet);
        return RET_FILE_FILE_SEEK;
    }

    iRet = write(fd, (byte*)data, len);
	close(fd);
    if(iRet != len){
		FILE_STRLOG("write(%s) = %d, %d\r\n", name, fd, iRet);
        return RET_FILE_FILE_WRITE;
    }

    return iRet;
}


int File_Append(char * name, byte * data, uint len)
{
    int fd;
    int iRet;

    if(name == NULL || data == NULL || len == 0){
        return RET_FILE_PARAM;
    }

    fd = open(name,O_CREATE|O_RDWR);
    if(fd < 0){
        FILE_STRLOG("open(%s):%d\r\n", name, fd);
        return RET_FILE_FILE_OPEN;
    }
	
    iRet = seek(fd, 0, SEEK_END);
    if(iRet < 0) {
        close(fd);
        FILE_STRLOG("seek(%d):%d\r\n", fd, iRet);
        return RET_FILE_FILE_SEEK;
    }

    iRet = write(fd, (byte*)data, len);
	close(fd);
    if(iRet != len){
		FILE_STRLOG("write(%s) = %d, %d\r\n", name, fd, iRet);
        return RET_FILE_FILE_WRITE;
    }

    return RET_FILE_OK;
}

int File_ReadRecord(char * name, uint no, uint recSize, byte * data )
{
    int fd;
    int iRet;

    if(name == NULL || data == NULL || recSize == 0){
        return RET_FILE_PARAM;
    }

    iRet = File_GetRecordNum(name, recSize);
    if(iRet <= no) {
        return RET_FILE_NOT_FOUND;
    }

	iRet = File_Read( name, (int)(recSize * no), data, recSize);
	if(iRet != recSize) {
	    FILE_STRLOG("File_Read(%s): %d, %d\r\n", name, no, iRet);
		return RET_FILE_FILE_READ;
	}

    return RET_FILE_OK;
}

int File_UpdateRecord(char * name, uint no, uint recSize, byte * data )
{
    int fd;
    int iRet;

    if(name == NULL || data == NULL || recSize == 0){
        return RET_FILE_PARAM;
    }

    iRet = File_GetRecordNum(name, recSize);
    if(iRet <= no){
        return RET_FILE_NOT_FOUND;
    }

	iRet = File_Write(name, (int)(recSize * no), data, recSize);
	if(iRet != recSize) {
	    FILE_STRLOG("File_Write(%s): %d, %d\r\n", name, no, iRet);
		return RET_FILE_FILE_READ;
	}
	
    return RET_FILE_OK;
}

int File_ForEachRecord(char * name, uint recSize, byte * data, RecodeForEach each, void * param)
{
    int fd;
    int iRet;
    int recordNum;
    int curNum;
 
    if(name == NULL ||  data == NULL || each == NULL || recSize == 0) {
        return RET_FILE_PARAM;
    }

    recordNum = File_GetRecordNum(name, recSize);
    if(recordNum <= 0){
        return recordNum;
    }

    fd = open(name,O_CREATE|O_RDWR);
    if(fd < 0){
        FILE_STRLOG("open(%s):%d\r\n", name, fd);
        return RET_FILE_FILE_OPEN;
    }

    iRet = seek(fd, 0, SEEK_SET);
    if(iRet < 0) {
        close(fd);
        FILE_STRLOG("seek(%d):%d\r\n", fd, iRet);
        return RET_FILE_FILE_SEEK;
    }

    for(curNum = 0; curNum < recordNum; curNum ++)
    {
        iRet = read(fd, (u8*)data, recSize);
        FILE_STRLOG("read(%d):%d\r\n", fd, iRet);
        if(iRet != recSize){
            close(fd);
            return RET_FILE_FILE_READ;
        }

        iRet = each(curNum, data, recSize, param);
		if(iRet >= 0) {
			close(fd);
			return iRet;
		}
		FILE_STRLOG("each(%d):%d\r\n", curNum, iRet);
    }

    close(fd);

    return RET_FILE_END;
}

int File_Size(char * name)
{
	int iRet = filesize(name);
	if(iRet < 0){
		return RET_FILE_NOT_FOUND;
	}
	
	return iRet;
}

int File_Remove(char * name)
{
    if(remove(name) < 0) {
        return RET_FILE_REMOVE;
    }

    return RET_FILE_OK;
}

int File_IsExist(char * name)
{
	if(fexist(name) < 0){
		return RET_FILE_NOT_FOUND;
	}
	
	return RET_FILE_OK;
}


