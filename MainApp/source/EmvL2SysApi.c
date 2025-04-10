#include "app.h"
#include "utils.h"

static int s_L2_FileID =-1;
static int s_L2_FileOpen=-1;

int EmvL2FileRead(u32 index, u32 offset, u8 *data, u32 len)
{
    int iret=0;

    if(-1==s_L2_FileOpen){
        iret = open("L2_kernel.dat",O_CREATE|O_RDWR);/*read/write file*/
        s_L2_FileOpen = 1;
        s_L2_FileID = iret;        
    }

    seek(s_L2_FileID, index*128+offset, 1);/*from file startpoint*/
    iret = read(s_L2_FileID, data, len);

    return iret;
}
int EmvL2FileWrite(u32 index, u32 offset, u8 *data, u32 len)
{
    int iret=0;

    if(-1==s_L2_FileOpen){
        iret = open("L2_kernel.dat",O_CREATE|O_RDWR);/*read/write file*/
        s_L2_FileOpen = 1;
        s_L2_FileID = iret;        
    }

    seek(s_L2_FileID, index*128+offset, 1);/*from file startpoint*/

    iret = write(s_L2_FileID, data, len);

    return iret;
}
int EmvL2FileDel(u32 startindex, u32 endindex)
{
    int iret=0;
    u8 buf[128];

    if(-1==s_L2_FileOpen){
        iret = open("L2_kernel.dat",O_CREATE|O_RDWR);/*read/write file*/
        s_L2_FileOpen = 1;
        s_L2_FileID = iret;        
    }

    seek(s_L2_FileID, startindex*128, 1);/*from file startpoint*/    
    memset(buf, 0xff, 128);

    do{
        write(s_L2_FileID, buf, 128);
        startindex++;
    }while(endindex>startindex);
    
    return 0;
}

int EmvL2GetSysTime(SYS_TIME * time)
{
    u8  buf[32];

	memset(buf,0,sizeof(buf));
	GetTime(buf);
    time->year = Utils_Bcd2Int(&buf[0], 1);  // Year

    if(time->year>=50) time->year += 1900;
    else time->year += 2000;
    time->mon = Utils_Bcd2Int(&buf[1], 1);  // Month
    time->day = Utils_Bcd2Int(&buf[2], 1);  // Day
    time->hour = Utils_Bcd2Int(&buf[3], 1);  // hour
    time->min = Utils_Bcd2Int(&buf[4], 1);  // minute
    time->sec = Utils_Bcd2Int(&buf[5], 1);  // second
    return 0;
}  

int EmvL2SetSysTime(SYS_TIME * time)
{
    u8  buf[32];
    memset(buf, 0x00, sizeof(buf));
    if(time->year<2000) time->year -= 1900;
    else time->year -= 2000;

    Utils_Int2Bcd(time->year, &buf[0], 1);
    Utils_Int2Bcd(time->mon, &buf[1], 1);
    Utils_Int2Bcd(time->day, &buf[2], 1);
    Utils_Int2Bcd(time->hour, &buf[3], 1);
    Utils_Int2Bcd(time->min, &buf[4], 1);
    Utils_Int2Bcd(time->sec, &buf[5], 1);
    Utils_Int2Bcd(time->wday, &buf[6], 1);
	return SetTime(buf);
}

int EmvL2GetSysTimer(ST_Timer_UNIT *timer,u32 timeout)
{	
	TimerSet(0,timeout/100);
    return 0;
}

int EmvL2SysTimerLeft(ST_Timer_UNIT *timer)
{
	return TimerCheck(0);
}  

u32 EmvL2GetSysTick()
{
    return GetTimerCount();
}
void EmvL2DelayMs(u32 ms)
{
    DelayMs(ms);
}  
void EmvL2Des(unsigned char *input,unsigned char *output,unsigned char *key, u32 mode)
{
    des(input, output, key, mode);
}
int EmvL2Aes(unsigned char *Input, unsigned char *Output, unsigned char *Aeskey, u32 keyLen, u32 Mode)
{
    return AES((u8*)Input, Output, (u8*)Aeskey, keyLen, Mode);
}

int EmvL2Rsa(u32 bitsize, const u8 *input, u8 *output, u8 *mod, u32 expLen,u8 *exp)
{
    int iret;
	
    iret = RSARecover(mod, bitsize/8, exp, expLen, (u8*)input, output);
    return iret;
}

int EmvL2Sha(u32 type,u8 *hash, u32 dataLen, u8 *data)
{
    int iret;
    iret = SHA(type, data, dataLen, hash);
    return iret;
}

int EmvL2ScExchange(const char *DeviceId, const C_APDU *c_apdu, R_APDU *r_apdu)
{
    int iret, i;
	APDU_SEND send_apdu;
	APDU_RESP rsp_apdu;

	memset(&send_apdu,0,sizeof(APDU_SEND));
	memset(&rsp_apdu,0,sizeof(APDU_RESP));
	send_apdu.Lc = c_apdu->Lc;
	send_apdu.Le = c_apdu->Le;
	memcpy(send_apdu.Command,c_apdu->Command,4);
	memcpy(send_apdu.DataIn,c_apdu->indata,256);

	/*if(EMV_GetDebug()) {
		DBG_STR("=======================");
		DBG_STR("IFM->");
		DBG_STR("-----------------------");
		DBG_STR("COMMAND : %02x %02x %02x %02x", c_apdu->Command[0], c_apdu->Command[1], c_apdu->Command[2], c_apdu->Command[3]);
		DBG_STR("LC : %d", c_apdu->Lc);
		DBG_STR("DATA : ");
		if(c_apdu->Lc >256) {
			DBG_STR("c_apdu->Lc err:%d", c_apdu->Lc);
		}
		DBG_HEX("",c_apdu->indata,c_apdu->Lc);
		DBG_STR("LE : %d", c_apdu->Le);
		DBG_STR("-----------------------");
	}*/
	
    if(strcmp(DeviceId, "SCC")==0) {
		iret =IccIsoCommand(0x00,&send_apdu,&rsp_apdu);
	}
    else if(strcmp(DeviceId, "SCCL")==0) {
		iret =PiccIsoCommand(0x00,&send_apdu,&rsp_apdu);
	}
    else return -1000;

	r_apdu->SWA= rsp_apdu.SWA;
	r_apdu->SWB= rsp_apdu.SWB;
	r_apdu->outlen= rsp_apdu.LenOut;
	memcpy(r_apdu->outdata,rsp_apdu.DataOut,256);

	/*if(EMV_GetDebug()) {
		DBG_HEX("APDU REQ COMMAND: ", (byte *)c_apdu->Command, 4);
		if(c_apdu->Lc) {
			DBG_HEX("APDU REQ DATA: ", (byte *)c_apdu->indata, c_apdu->Lc);
		}
		DBG_HEX("APDU RSP DATA: ", (byte *)r_apdu->outdata, r_apdu->outlen);
		DBG_STR("APDU RSP SW: %02X%02X", r_apdu->SWA, r_apdu->SWB);

			if(r_apdu->outlen >256) {
		        DBG_STR("outlen err:%d", r_apdu->outlen);
		        r_apdu->outlen = 256;
			}
			DBG_HEX("",r_apdu->outdata,r_apdu->outlen);
	        DBG_STR("SWA=%02x, SWB=%02x", r_apdu->SWA, r_apdu->SWB);
	        DBG_STR("-----------------------");
	    }
	}*/
	if(EMV_GetDebug()) {
		DBG_HEX("APDU REQ COMMAND: ", (byte *)c_apdu->Command, 4);
		if(c_apdu->Lc) {
			DBG_HEX("APDU REQ DATA: ", (byte *)c_apdu->indata, c_apdu->Lc);
		}
		DBG_HEX("APDU RSP DATA: ", (byte *)r_apdu->outdata, r_apdu->outlen);
		DBG_STR("APDU RSP SW: %02X%02X", r_apdu->SWA, r_apdu->SWB);
	}
		
    return iret;
}

int EmvL2GetRandom(u8 *rand,u32 len)
{
	u8 tmp[8];
	int off=0;
	
	while(1){
		PCIGetRandom(tmp);
		if((len-off) > 8){
			memcpy(rand+off,tmp,8);
			off+=8;
		}else{
			memcpy(rand+off,tmp,len-off);
			break;
		}
	}
    return 0;
}  

int EmvL2OfflinePinVerify(u32 offline_type, ST_SCPINKEY *RsaKey, u8 *status_word)
{
	return PedVerifyOfflinePin(offline_type, RsaKey, status_word);
}


