#ifndef __QRENCODE_H
#define __QRENCODE_H
//#include "typedef.h"

#ifndef BOOL
typedef 	unsigned char		BOOL;			
#endif

#ifndef FALSE
#define FALSE		0
#endif

#ifndef TRUE
#define TRUE		1
#endif

#define ALIGN_LEFT   0
#define ALIGN_CENTER 1
#define ALIGN_RIGHT  2
#define ALIGN_FULL   3

#define min(a,b)	(((a) < (b)) ? (a) : (b))
#define QR_LEVEL_L	0	
#define QR_LEVEL_M	1	
#define QR_LEVEL_Q	2	
#define QR_LEVEL_H	3	

#define QR_MODE_NUMERAL		0
#define QR_MODE_ALPHABET	1
#define QR_MODE_8BIT		2
#define QR_MODE_KANJI		3
#define QR_MODE_CHINESE		4

#define QR_VRESION_S	0 
#define QR_VRESION_M	1 
#define QR_VRESION_L	2 

#define QR_MARGIN	4
#define	QR_VER1_SIZE	21

#define MAX_ALLCODEWORD		400
#define MAX_DATACODEWORD	400
#define MAX_CODEBLOCK		153	
#define MAX_MODULESIZE		53

extern int m_nSymbleSize;
extern unsigned char	 m_byModuleData[MAX_MODULESIZE][MAX_MODULESIZE];
/////////////////////////////////////////////////////////////////////////////
typedef struct 
{
	unsigned short  int	 ncRSBlock;		
	unsigned short  int	 ncAllCodeWord;	
	unsigned short  int	 ncDataCodeWord;	
}RS_BLOCKINFO;
typedef struct 
{
	unsigned short  int	 nVersionNo;			
	unsigned short  int	 ncAllCodeWord; 		

	unsigned short  int	 ncDataCodeWord[4];	
	unsigned short  int	 ncAlignPoint;		
	unsigned short  int	 nAlignPoint[6];		
	//(0=L,1=M,2=Q,3=H)
	RS_BLOCKINFO RS_BlockInfo1[4];	
	RS_BLOCKINFO RS_BlockInfo2[4];	
}QR_VERSIONINFO;

int EncodeData(char *lpsSource);

int GetEncodeVersion(int nVersion, char *lpsSource, int ncLength);
//bool EncodeSourceData(char *lpsSource, int ncLength, int nVerGroup);
int EncodeSourceData(char *lpsSource, int ncLength, int nVerGroup);

int GetBitLength(unsigned char	 nMode, int ncData, int nVerGroup);

int SetBitStream(int nIndex, unsigned short wData, int ncData);

BOOL IsNumeralData(unsigned char c);
BOOL IsAlphabetData(unsigned char c);
BOOL IsKanjiData(unsigned char c1, unsigned char c2);
BOOL IsChineseData(unsigned char c1, unsigned char c2);
	
unsigned char	 AlphabetToBinaly(unsigned char c);
unsigned short KanjiToBinaly(unsigned short wc);
unsigned short ChineseToBinaly(unsigned short wc);
void GetRSCodeWord(unsigned char	 *lpbyRSWork, int ncDataCodeWord, int ncRSCodeWord);

void FormatModule(void);

void SetFunctionModule(void);
void SetFinderPattern(int x, int y);
void SetAlignmentPattern(int x, int y);
void SetVersionPattern(void);
void SetCodeWordPattern(void);
void SetMaskingPattern(int nPatternNo);
void SetFormatInfoPattern(int nPatternNo);
int CountPenalty(void);
void Print_2DCode(void);
void GetBmpBuf(char *imageBuf, u8 qrWidth);
int SetQrcodeErrLevel(int level);
int  PrintQrcode(char* str, u32 msize, u32 alignment, u32 lMargin,u8 * outbuf);



#endif
