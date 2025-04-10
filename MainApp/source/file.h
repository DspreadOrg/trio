#ifndef __FILE_H__
#define __FILE_H__

#define RET_FILE_OK             0
#define RET_FILE_BASE           -400
#define RET_FILE_PARAM          (RET_FILE_BASE-1)
#define RET_FILE_FILE_OPEN      (RET_FILE_BASE-2)
#define RET_FILE_FILE_READ      (RET_FILE_BASE-3)
#define RET_FILE_FILE_WRITE     (RET_FILE_BASE-4)
#define RET_FILE_FILE_SEEK      (RET_FILE_BASE-5)
#define RET_FILE_FILE_SIZE      (RET_FILE_BASE-6)
#define RET_FILE_NO_RECORD      (RET_FILE_BASE-7)
#define RET_FILE_NOT_FOUND      (RET_FILE_BASE-8)
#define RET_FILE_DATA           (RET_FILE_BASE-9)
#define RET_FILE_REMOVE         (RET_FILE_BASE-10)
#define RET_FILE_NOT_MATCH      (RET_FILE_BASE-11)
#define RET_FILE_END            (RET_FILE_BASE-12)
#define RET_FILE_TIMEOUT        (RET_FILE_BASE-13)


typedef int (*RecodeForEach)(int no, byte * data, uint len, void * param);

int File_GetRecordNum(char * name, uint recSize);
int File_Read(char * name, int offset, byte * data, uint len);
int File_Write(char * name, int offset, byte * data, uint len);
int File_Append(char * name, byte * data, uint len);
int File_ReadRecord(char * name, uint no, uint recSize, byte * data);
int File_UpdateRecord(char * name, uint no, uint recSize, byte * data );
int File_ForEachRecord(char * name, uint recSize, byte * data, RecodeForEach each, void * param);
int File_Size(char * name);
int File_Remove(char * name);
int File_IsExist(char * name);

#endif

