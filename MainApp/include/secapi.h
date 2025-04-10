#ifndef _SEC_API_H_
#define _SEC_API_H_

#ifndef u8
#define u8    unsigned char
#endif
#ifndef u16
#define u16    unsigned short
#endif
#ifndef u32
#define u32    unsigned int
#endif


#define SEC_ENC 1
#define SEC_DEC 0

#define SEC_MAX_MKSK 48
#define SEC_MAX_DUKPT 10
#define SEC_MAX_RSA 10


#define KCV_MODE_NOCHK   0
#define KCV_MODE_CHK0    1
#define KCV_MODE_CHKFIX  2
#define KCV_MODE_CHKMAC  3

/**
 * "key usage"  value
 */
#define SEC_KU_BDK                      ( (u16) (('B'<<8)|'0') )// 对应 TR-31 中的 "B0". 
                                            // reference " ANS X9 TR-31 (2010)", "A.5.1 Key Usage", "Table A-3. Defined Key Usage Values".
#define SEC_KU_DUKPT_INIT_KEY           ( (u16) (('B'<<8)|'1') ) // "B1". DUKPT init key.
#define SEC_KU_DATA_KEY                 ( (u16) (('D'<<8)|'0') )// "D0". This key can be used to encipher or decipher data, 
#define SEC_KU_MASTER_KEY               ( (u16) (('K'<<8)|'0') )// "K0". 包含 MK.
#define SEC_KU_KBPK                     ( (u16) (('K'<<8)|'1') )// "K1". KBPK : TR-31 Key Block Protection Key.
#define SEC_KU_MAC_KEY                  ( (u16) (('M'<<8)|'Z') )// "MZ"
#define SEC_KU_PIN_KEY                  ( (u16) (('P'<<8)|'0') )// "P0". PIN Key, WK.
/**
 *  " ANS X9 TR-31 (2010)", "A.5.2 Algorithm".
 */
#define SEC_KA_AES                        ( (char)('A') )
#define SEC_KA_DEA                        ( (char)('D') )
#define SEC_KA_ELLIPTIC_CURVE             ( (char)('E') )
#define SEC_KA_HMAC_SHA_1                 ( (char)('H') )
#define SEC_KA_RSA                        ( (char)('R') )
#define SEC_KA_DSA                        ( (char)('S') )
#define SEC_KA_TDEA                       ( (char)('T') )      // Triple DEA
#define SEC_KA_SM4                          ( (char)('M') )      // SM2 DEA

/**
 * "mode of use" value 
" ANS X9 TR-31 (2010)", "A.5.3 Mode of Use". 
 */
#define SEC_MOU_ENC_DEC_WRAP_UNWRAP  ( (char)('B') )    // Both Encrypt & Decrypt / Wrap &Unwrap. "KUM" : Key Use Mode.
#define SEC_MOU_GENERATE_AND_VERIFY  ( (char)('C') )    // Both Generate & Verify : Generate MAC or verify MAC.
#define SEC_MOU_DEC_OR_UNWRAP_ONLY   ( (char)('D') )    // Decrypt / Unwrap Only : ex.MK.
#define SEC_MOU_ENC_OR_WRAP_ONLY     ( (char)('E') )    // Encrypt / Wrap Only : ex.PIN key. 
#define SEC_MOU_GENERATE_ONLY        ( (char)('G') )    // Generate Only : 
#define SEC_MOU_NO_RESTRICTIONS      ( (char)('N') )    // No special restrictions (other than restrictions implied by the Key Usage)
#define SEC_MOU_SIGNATURE_ONLY       ( (char)('S') )    // Signature Only
#define SEC_MOU_VERIFY_ONLY          ( (char)('V') )    // Verify Only : 
#define SEC_MOU_DERIVE_KEYS          ( (char)('X') )    // Key used to derive other key(s) : 

/**
 *  "key version number" value
 * "Table A-6. Key Version Number definition"
 */
#define SEC_KV_NOT_USED          ( (u16)(('0'<<8)|'90') )

/**
 * " ANS X9 TR-31 (2010)", "A.5.5 Exportability", "Table A-7. Defined Values for Exportability Byte".
 */
/** "Exportable under a KEK in a form meeting the requirements of X9.24 Parts 1 or 2." */
#define SEC_KE_EXPORTABLE_UNDER_KEK           ( (char)('E') )
#define SEC_KE_NON_EXPORTABLE                 ( (char)('N') )       
/** "Sensitive, Exportable under a KEK in a form not necessarily meeting the requirements of X9.24 Parts 1 or 2.." */
#define SEC_KE_SENSITIVE                      ( (char)('S') )


/****macrodef of optional BLOCK ID**********************************/
#define SEC_OPT_KEY_BLOCK_ID_KSN ((u16)('K'<<8|'S'))//‘KS’ 0x4B53 Key Set Identifier,
#define SEC_ENC_KEY_FMT_TR31_MAC_LEN 8 

#define DATA_DEC        0   
#define DATA_ENC        1   

#define DATA_MODE_ECB   0   
#define DATA_MODE_CBC   1   
#define DATA_MODE_OFB   2   
#define DATA_MODE_CFB   3   

#define MAC_GENERATE_MAC       (0)
#define MAC_VERIFY_MAC         (1)

////for mac_alg
#define MAC_ALG_ISO_9797_1_MAC_ALG1     1001
#define MAC_ALG_ISO_9797_1_MAC_ALG3     1003
#define MAC_ALG_ISO_16609_MAC_ALG1      2000    //same to 9797-1-alg3
#define MAC_ALG_FAST_MODE                3000    //All XOR reencryption
#define MAC_ALG_X9_19                    3001    //same to 9797-1-alg3
#define MAC_ALG_CBC                        3002    //TDES/DES cbc encryption and output the last block
#define MAC_ALG_CUP_SM4_MAC_ALGORITHM_0 3003

//for mac_padding
/**ISO 9797-1 filling algrithm 1，followed by 0x00*/
#define MAC_PADDING_MODE_1     (1)     
/**ISO 9797-1 filling algrithm 2，followed by 0x80，again followed by0x00*/
#define MAC_PADDING_MODE_2     (2)      

// for  pinblock_fmt
#define SEC_PIN_BLK_ISO_FMT0     0
#define SEC_PIN_BLK_ISO_FMT1     1
#define SEC_PIN_BLK_ISO_FMT2     2
#define SEC_PIN_BLK_ISO_FMT3     3
#define SEC_PIN_BLK_EPS             4
#define SEC_PIN_BLK_IBM_3621     5
#define SEC_PIN_BLK_IBM_3624     6
typedef enum
{
    SEC_MKSK=0,
    SEC_DUKPT,
    SEC_RSA_KEY,
}SEC_KEY_TYPE;//the key_type for the API



typedef struct
{
    u16  key_usage;
    u8   key_alg;
}ST_KEY_Attr;

typedef struct
{
    u16 keylen;//SEC_KEY_TYPE
    u8 *keydata;
    u16    kcvmode;//0=no kcv,1-standard kcv
    u16 kcvlen;
    u8     *kcv;
    u8  *iksn;
}ST_API_FMT_NORMAL_KEY_Block;

typedef enum{KEY_FMT_NORMAL=0,KEY_FMT_TR31}KEY_BLOCK_FMT;

/**DUKPT工作密钥选择*/
typedef enum 
{
    DUKPT_KEY_PIN=0,        /**DUKPT PIN key*/
    DUKPT_KEY_MAC_BOTH,     /**DUKPT request and response MAC key，It can generate and verify MAC key*/
    DUKPT_KEY_MAC_RSP,      /**DUKPT response mac key，only verify MAC key*/
    DUKPT_KEY_DATA_BOTH,    /**DUKPT Request and response data keys that both encrypt and decrypt*/
    DUKPT_KEY_DATA_RSP      /**DUKPT Request and response data keys that can only decrypt data*/
}SEC_DUKPT_KEY_SELECT;

typedef enum
{ 
    SEC_CTRL_GETKCV=0,            //Get KCV
    SEC_CTRL_DUKPT_ADD_KSN=1,   //DUKPT KSN+1
    SEC_CTRL_DUKPT_GET_KSN=2,   //get DUKPT current KSN
    SEC_CTRL_MAX,
}EM_SEC_CTRL_CODE;
typedef struct
{
    u32 version;//fixed as 0x00 
    u16 depend_type;//SEC_KEY_TYPE
    u16 depend_id;
    u16 id;
    KEY_BLOCK_FMT format;//0-FMT_WITHOUT_KCV,1-FMT_WITHKCV， 2-FMT_TR31
    ST_KEY_Attr * keyattr;//formatis not 2-FMT_TR31 need
}ST_API_Store_Key;

typedef struct 
{
    u32 version;
    u32 keyindex;
    u8 *iv;
    u32 enc_or_dec;
    u32 mode;//CBC/ECB/OFB/CFB....
    u32 dukpt_key_select;//:SEC_DUKPT_KEY_SELECT only for dukpt
}ST_API_DataCalc_Key;

typedef struct 
{
    u32 version;
    u32 keyindex;
    u32 gen_or_verify;//MAC_GENERATE_MAC or MAC_VERIFY_MAC
    u32 mac_alg;
    u32 mac_padding;
    u8* mac_iv;
    u8  mac_in[8];//only for mac verify;padding 0x00 on the right for masked bytes;
    u32 dukpt_key_select;//:SEC_DUKPT_KEY_SELECT,only for dukpt key
}ST_API_MAC_KEY;




typedef struct
{
    u32 version;
    u32 keyindex;
    char*len_sets;
    u32 pinblock_fmt;
    u32 timeoutms;
    u8 * datain;
}ST_API_PIN_KEY;

typedef struct 
{
    u32 version;
    u32 ctrl_code;
    u32 keyindex;
}ST_API_CTRL_KEY;
typedef struct
{
     u32  ModLen;  
     u8   Mod[256]; 
     u8   Exp[4];    
     u8   randLen;   /*random data len*/
     u8   rand[8];   /*random data*/
} ST_API_RSA_ICC;

#define SEC_RET_OK                        (0)  
#define SEC_RET_ERROR                   (-1)        
#define SEC_ROOTKEY_ERROR            (-1000)                
#define SEC_ERR_LOCKED              (SEC_ROOTKEY_ERROR-1)  /*Security System locked*/    
#define SEC_ERR_DATA                (SEC_ROOTKEY_ERROR-2)  /*SEC file data FsRead/FsWrite error*/
#define SEC_ERR_KEYINDEX            (SEC_ROOTKEY_ERROR-3)  
#define SEC_ERR_KEYFAIL             (SEC_ROOTKEY_ERROR-4)  /*Ker verify fail*/ 
#define SEC_ERR_NOPIN               (SEC_ROOTKEY_ERROR-5)  /*Not input PIN*/
#define SEC_ERR_INPUT_CANCEL        (SEC_ROOTKEY_ERROR-6)  /*Cancel PIN Enter*/
#define SEC_ERR_PIN_TIMEOUT         (SEC_ROOTKEY_ERROR-7)  /*Input PIN timeout*/
#define SEC_ERR_SMALL_INTERVAL      (SEC_ROOTKEY_ERROR-8)  /*Call Get PIN function is less then small interval time*/
#define SEC_ERR_KCV_MODE            (SEC_ROOTKEY_ERROR-9) 
#define SEC_ERR_KCV_FAIL            (SEC_ROOTKEY_ERROR-10) /*KCV verify fail*/
#define SEC_ERR_KCV_ODD             (SEC_ROOTKEY_ERROR-11) 
#define SEC_ERR_NO_MATCH            (SEC_ROOTKEY_ERROR-12) /*No right to use the key*/
#define SEC_ERR_KEYTPYE             (SEC_ROOTKEY_ERROR-13) 
#define SEC_ERR_KEYLEN                (SEC_ROOTKEY_ERROR-14) 
#define SEC_ERR_EXPLEN              (SEC_ROOTKEY_ERROR-15) /**Expect len is not match*/
#define SEC_ERR_DSTKEY_INDEX        (SEC_ROOTKEY_ERROR-16) 
#define SEC_ERR_SRCKEY_INDEX        (SEC_ROOTKEY_ERROR-17) 
#define SEC_ERR_SRCKEY_TYPE            (SEC_ROOTKEY_ERROR-18)

#define SEC_ERR_GROUP_INDEX         (SEC_ROOTKEY_ERROR-19)
#define SEC_ERR_NULL_PTR            (SEC_ROOTKEY_ERROR-20)
#define SEC_ERR_NOKCV               (SEC_ROOTKEY_ERROR-21) 
#define SEC_ERR_DUKPT_OVERFLOW      (SEC_ROOTKEY_ERROR-22)
#define SEC_ERR_DUKPT_KEYTYPE       (SEC_ROOTKEY_ERROR-23)
#define SEC_ERR_NEED_ADD_KSN        (SEC_ROOTKEY_ERROR-24) 


#define SEC_ERR_KEY_USAGE         (SEC_ROOTKEY_ERROR-25)   /**  An attempt was made to use the key outside of its "usage".                                            */
#define SEC_ERR_MODE_OF_KEY_USE   (SEC_ROOTKEY_ERROR-26)   /**  The use of key is incorrect. For example, decrypting data with a key that is restricted to encryption only                  */
#define SEC_ERR_NOT_SUPPORT       (SEC_ROOTKEY_ERROR-27)
#define SEC_ERR_KEYATTR_NOT_MATCH (SEC_ROOTKEY_ERROR-28)
#define SEC_ERR_NO_AUTH           (SEC_ROOTKEY_ERROR-29)  /*Security System is not authentication*/    

#endif

