#ifndef __TRANS_H__
#define __TRANS_H__

int Trans_MagcardTest(void * pxPARAM);
int Trans_IcccardTest(void * pxPARAM);
void displayWarning(char *title, char *warning);
int Trans_GetCardInfo();
int Trans_MkskKeyInit(void * pxPARAM);
int Trans_PicccardTest(void * pxPARAM);

#endif

