/*
********************************************************************************
*
*   File Name:
*       test_tls.c
*   Author:
*       SW R&D Department
*   Version:
*       V1.0
*   Description:
*
*
********************************************************************************
*/

#define _TEST_TLS_MODULE
#define _TEST_TLS_C

/*-----------------------------------------------------------------------------
|   Includes
+----------------------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>
#include "dsp_lib_api.h"


/*-----------------------------------------------------------------------------
|   Macros
+----------------------------------------------------------------------------*/
#define SOCKET_WIFI             (1)

//#define AP_NAME                 "Dsp-Guest"
//#define AP_PASSWORD             "12345678"
#define AP_NAME                 "LJZHB1002"
#define AP_PASSWORD             "123456LJ"

#define REQ_CA                  \
"-----BEGIN CERTIFICATE-----\r\n" \
"MIIFVTCCAz2gAwIBAgIJALE+9MSvKIILMA0GCSqGSIb3DQEBCwUAMFUxCzAJBgNV\r\n" \
"BAYTAmJqMQswCQYDVQQIDAJiajELMAkGA1UEBwwCYmoxCzAJBgNVBAoMAmRoMQsw\r\n" \
"CQYDVQQLDAJkaDESMBAGA1UEAwwJbG9jYWxob3N0MB4XDTIxMDUyODAzMDgxMVoX\r\n" \
"DTMxMDUyNjAzMDgxMVowVTELMAkGA1UEBhMCYmoxCzAJBgNVBAgMAmJqMQswCQYD\r\n" \
"VQQHDAJiajELMAkGA1UECgwCZGgxCzAJBgNVBAsMAmRoMRIwEAYDVQQDDAlsb2Nh\r\n" \
"bGhvc3QwggIiMA0GCSqGSIb3DQEBAQUAA4ICDwAwggIKAoICAQC8xdVsZxxGDHud\r\n" \
"SfHpjSqyXHC/JprCFFZylCIuQpNg8t6ZmNYfaJaUcHmXO4QfHQwt1jZJ04oDNx79\r\n" \
"dZ4cO2hj2Jw/h8d6GwsfZiv0N7AFVCcxndv0JHHwUypR3Ryq/ZcvUdZXDA/0VjAa\r\n" \
"90uT2g6tFSO52TgeiPL9PNnALnNsABc8qF4+iz8yDuAfsqpd6fSzoUNlA86Bs20+\r\n" \
"9Nmd3JiH9P07cJiDvs6Ov5+qSmLBsOoBmkHaXOT0HMjHaVL7IdmlZItG5aQnRfbg\r\n" \
"jwgjlA4AS/tYieJnGycIqctPyixXPcfsH8Q9VrFXW6cQpynYkSxu1IYf9pcMZuGP\r\n" \
"R4WOeZuHEKAzKkJjJfIBpHQunzxAhjeCYWkGc+xGXQ9BG+DlJkpdIGzlAIS3lXIY\r\n" \
"XBTvhnaONR0pK2oQA0rny5+El2cO2v6NkuyaYwEJixkl6z1a0I1921qR0sB9v7Pb\r\n" \
"5KkySbWjgmBV9qt0ZvHwnX7t0SoB4vpBDjs/qcpGEWYokpruDK91vUQ700YriS7u\r\n" \
"bozk+v2kJxlEF6hkA/JgsJQP5X1hvsJzL5dAs0OYyqkEGoZVTS/3BfDIfDME+MO+\r\n" \
"uBjB1VxtU6QNbYv9+/MsqWy3qyq0nCusUpmHBlkP5nwydztXiw85zjDp97ygnkEx\r\n" \
"AIEyh0lCqwzbQGVGg5QIb7AKleammwIDAQABoygwJjAkBgNVHREEHTAbgglsb2Nh\r\n" \
"bGhvc3SCDmNzLmRzcHJlYWQubmV0MA0GCSqGSIb3DQEBCwUAA4ICAQCS8V8VvNYk\r\n" \
"zm3BUS3JjaNuzx4JI2WCZxzrlHRL+gtASklM4VF2H2/8yv5aKd9aGew0HcKu+YhY\r\n" \
"DWvZGFGkMTEqHWePaESV+141aaBHP3Y0qLeTSU5P1m/ydPWKEEkR2kBzSP8Wfw7k\r\n" \
"L698s30KbXsEsVLQgov0e/UdCyYjqiX9VL1piLpvlHRTvD1hVV8LTdvPXuP4RV3t\r\n" \
"kki3iyulBmFWReeE+1+EcRUbdujDEiFzeOcaEhb8VxHoxn57/eCTazsJULVfKfgz\r\n" \
"s2meBaP/u2ux6YzO4SaXhEOLCIo8ThPNt3xpGELBEk9Kwm3rZSuli9N5+IibBFBn\r\n" \
"fighUCkegPAY2MczVh3bJNqRqcSZIfCTT3UNd12bmBWAn+4i3ySCG+2RgAYS9Ekc\r\n" \
"EGmUKCu2ujQlFn7qO8TXRfBTalAKBeukZ6wbqov7hrQP0d5UAckC+IWfYHldH4Ez\r\n" \
"xidJr7qIZmlwpf9IYL77bt8l+QgHvn8TvwLvMSyc6CLSd8CgIPjFZxBxdCTugBGD\r\n" \
"TAlCYnwrhEjD4lOSDCgrQJgZ5VlS9BU1qFm4Ql4/z/AqiZ7wKN74n95HOoVDjyrF\r\n" \
"XoVDXVSIBS5HO2iLbrYV8S/RqPQbAKcTuGdvGGD77Jx2kOZoWJh88OCbPsQpwan/\r\n" \
"2EUvASWeP0TPn4S4BLybf0seCeRqhY5+Qw==\r\n" \
"-----END CERTIFICATE-----\r\n""\x00"

#define REQ_KEY                 \
"-----BEGIN RSA PRIVATE KEY-----\r\n" \
"MIIJKQIBAAKCAgEAuKE96Rgkg6OGizcPxIFKPbRa9BQYhBlc8ifAD32Cvtc4piV9\r\n" \
"YCBEPmCn4Xw0P+/INhOdetnaux729uSVWx4Pln3sNTArCMUkbSCPJhzY97qaNnae\r\n" \
"lg33b2f9nqUPOfS8GfKnk5E7+v/TATx8dwk5F1UdixMSyC7EhxxTvnzLkxUuDzzX\r\n" \
"hjydwm83ECr0GGotmDtoNSEh4SgwMJwS2OHVsH4FME4pUQmIqOVA4cQ5IU1/Ol+T\r\n" \
"oL7PZwUPV7Bd5Br7yDT4VBUHNxjJwneZkjMx0FcO/L84WDrrKzjg3+jWqhSCIxp2\r\n" \
"CZykdntg3oNeCbNHjqBxKXQay5aiF9qlExFBM4ofWL6gQSxgWo1LuDxnmAhvttSp\r\n" \
"U9YI/9nfuy5m873MxbtV4UprmNALrJ41LFiQRuRN6Tgz31f75bwADICkNok26kcv\r\n" \
"m+/anYqE+kt1FEBNnGpsEpxGvKTaQK1JWHux83ajJu0lp28Hl50ZbALwkj2KEIEE\r\n" \
"Aqs35a1ua667WwALCUobHa7vbQDzeCUH/8YSamvLykwtLKJN2rd0I8aMD1b0I6ee\r\n" \
"q6h5ul74fyNuggCkGRnkU37d6N0/yAev+dv+H2lS7Hlf9vBp6GOqyadej8KsK6Wg\r\n" \
"m0XmrsJMD0xb0Fav3iyA8x6s/C1SVj7Lphrvgv6IOlAuuoZmpI2FANne6LUCAwEA\r\n" \
"AQKCAgEAsZWFsvohxOw4sRbY961T913OTg42NnkeyVwqg7u5Uk44krZkjVz7xvO5\r\n" \
"Bk0tBIJiFIFxa4S3ulviNz9N2UJkw9+CnvYhrmHQ05YJmiMNHdpCEjit+K6eXG2t\r\n" \
"qDYjGx+m/V5EiM+c3aPLFNlU7jnUy/h2XhqOajVxmnNIqWPneyVYFBf1RYxwQWAv\r\n" \
"mkF2XzVhfZdGeDPSHntFPrwf9LPjkcrkur2zR9rfs3Qun/jY6otb4I2ltWtSmkhn\r\n" \
"pXRiGtFJdBxe7qD7O6eACKh48gKD21yhccGLNHIXWI9w3G1cbGMleRYuirg8CCpL\r\n" \
"1jqM7M+f+DOiJLkkPMY6Sq9nL5vdFU9cXpLpRhFdxEBiixO2kcXDRqvl/k01YbpV\r\n" \
"TjJ6HlYj0fmjNDpOrAFXa8Un8XS1Md9hqUb5sOD107RgMPAjPa1pUVYpw7wmqQO9\r\n" \
"09L0cd4jU/xNyGvwkWBMuY//eAvqUA45XNgTSAgQB6HTO8RvZX0kK/BbFWkmECZE\r\n" \
"S25JBSzdvc6X20SQpDb2x6CUISTM4z+nei6QeTU8Iued0RDXImfL7iWaAVfXSPai\r\n" \
"zilCbie8Ifa+kZTDGzWUWKh33jiKjqfatuZdQROireJ6Ynl62KP9GzY15ngyCOn+\r\n" \
"Qgn7nBbrWhe4C88YdFyiXXQ10v12H5UkTijrkzgO03bmInFw9NECggEBAN8PeM5o\r\n" \
"pqnDBcQuT9scZKGIW4gLhlW/1C4wsEFJpvKj8bd4ButIDYbS3hMmXn3fHaBY3igf\r\n" \
"ZV3vGTjvURQ+4q4YJP8wCiDj1e3sU15/n4V9K9xWNiU+Z21CSgAXg4Rno3y9GNzn\r\n" \
"2GVhSYTi1t+D2/GHX+qVhRb1+zu+p/VCs3yY/kFEH6TWPNBXJD3cX+pFU82aDpKk\r\n" \
"CUFwKM8Wo+Rhx95lDlgOYK3i7LprPFwzcbk8RcPOs/kRFM1pV31BdCLiWRL9zS+B\r\n" \
"J2Zytg9x64OmRpRg/wFkYKI7uljKj0ogarzGjfZh++iqtSM4ykhaoz9dsH3C5jbl\r\n" \
"myTzLChscbsTrZMCggEBANPk8s0lQmNowb8IJUfERDKXcoADDAOPdzWnX383xoe8\r\n" \
"O2Pu0jXuODoVjSPGeOAsBogv0FXvsvf1rhOku2wNz2C8oBVA6gqzxAMKPJWw9wzk\r\n" \
"zUf7fj0h6bTQuy+BXZ5n8V1skJFBl+PAIqYVKSlK2bEgfpFupVQ08oAHYak9tF8g\r\n" \
"c5FWT4VlWz1qYY0YOKb/Zd+iDs7aq9OpZxoWWFVGMMw1pfrQgy2P9gTutaC+obV0\r\n" \
"OsVPRjIjHsjdzjyeF5Uba66d50nrIF5uS+S3r6XVnXqvcJiI2jwfQx4VzDIAPdcX\r\n" \
"VJXCz71oQ4rcPz6rEqqrKOUt/vBsVyqZxvakToAqvZcCggEAPPHLPgoVzzg7sRg+\r\n" \
"NU2SqRfKemc7st4QqPYgkhX/iTgF6OzgZxRurbGbf3vyQU3emFrvh+LcHUggig/J\r\n" \
"BVEROiPap+aGPepCHxl8xcr02NIUU8UyUga014NPGKtUAa8XWhOIYYLu+vQT1Dzu\r\n" \
"HG+4fktQh5WGoXlfxTkYRsiMpbRfc5GbW7w1LW9Bta0qf6Wlef1nkeVDqh9s5vf2\r\n" \
"m4WjyD2wJ5r2C0x/BYgbG83qeoPfQ9qMDbb/CgiIIO7rPkS9tZSmpR4Sl8kruRiV\r\n" \
"Eokxq4S857J3PJw5E910PPqSbzWIwBKRc0aA6ziu/AnXpI6ClDnWEJW3R+cw33KR\r\n" \
"uF1uBQKCAQAQXdC7M7OLBh5g6PooLuqtlbEMXi/hXoVwqXrg0qgM9+cGcv3dmIbT\r\n" \
"RbLfHodbmUrv3fGTka4bJOjLL6AiWna4HBmpSphJN1Vefr3rOf37CvkoJC69/g6V\r\n" \
"HsI2iyF2CMVF2ArajVx0BPUoKt5BCD0ER2Q/S3KNrIFGjktmzRNPfawvf6Eoxolj\r\n" \
"lwHt6f8GNxfhTB0ksRMwiBcklrE/9Onz+/jUBKL9M+9mtiFlRxQqdc3MS7E6Uigg\r\n" \
"j7FMUVG0/Kc2l4+zz+tyH8bQ5bc1ceh1/3MM9RUWK7v9aOWavxYHtQ+deayeDp2Y\r\n" \
"7iYcaw5IDGxeN01HuLaK3dHWXx6JG8QFAoIBAQCyyKI8M5gKD62732bu14xSNDly\r\n" \
"SUePmzi9aocqvtU21UQJdhuvn1n+onE9GAxO6CFzCpadYfloxiJQQOeitNJhzDwu\r\n" \
"xcS5RSgd4dhFW9TjrMZ2dWPfelB1f4J0IQXWKtsmGJOZ/5gfh1Z8HhDceYoWa96t\r\n" \
"pY9uGXm0aV+jGDLMhjYzGB/j7QKeO9bEUfCkW2iOxs46A3fuAvTTb5xG6kNQaiMI\r\n" \
"HKUXh9fuC6BVm7bQJNaTyHKwyei5gjvYH4z1GAHBEA3JLV99enwp6wspN89r4WyF\r\n" \
"5NzE+F2N7IJmlFK1JL3xsJcdu7dNtuYpHxn3SWZAJTAw+6cP0kgqyUA0wcD1\r\n" \
"-----END RSA PRIVATE KEY-----\r\n""\x00"

#define REQ_CERT                \
"-----BEGIN CERTIFICATE-----\r\n" \
"MIIFVTCCAz2gAwIBAgIJAIFMucxEEhbZMA0GCSqGSIb3DQEBCwUAMFUxCzAJBgNV\r\n" \
"BAYTAmJqMQswCQYDVQQIDAJiajELMAkGA1UEBwwCYmoxCzAJBgNVBAoMAmJqMQsw\r\n" \
"CQYDVQQLDAJiajESMBAGA1UEAwwJbG9jYWxob3N0MB4XDTIxMDUyODAzMTA1OFoX\r\n" \
"DTMxMDUyNjAzMTA1OFowVTELMAkGA1UEBhMCYmoxCzAJBgNVBAgMAmJqMQswCQYD\r\n" \
"VQQHDAJiajELMAkGA1UECgwCYmoxCzAJBgNVBAsMAmJqMRIwEAYDVQQDDAlsb2Nh\r\n" \
"bGhvc3QwggIiMA0GCSqGSIb3DQEBAQUAA4ICDwAwggIKAoICAQC4oT3pGCSDo4aL\r\n" \
"Nw/EgUo9tFr0FBiEGVzyJ8APfYK+1zimJX1gIEQ+YKfhfDQ/78g2E5162dq7Hvb2\r\n" \
"5JVbHg+Wfew1MCsIxSRtII8mHNj3upo2dp6WDfdvZ/2epQ859LwZ8qeTkTv6/9MB\r\n" \
"PHx3CTkXVR2LExLILsSHHFO+fMuTFS4PPNeGPJ3CbzcQKvQYai2YO2g1ISHhKDAw\r\n" \
"nBLY4dWwfgUwTilRCYio5UDhxDkhTX86X5Ogvs9nBQ9XsF3kGvvINPhUFQc3GMnC\r\n" \
"d5mSMzHQVw78vzhYOusrOODf6NaqFIIjGnYJnKR2e2Deg14Js0eOoHEpdBrLlqIX\r\n" \
"2qUTEUEzih9YvqBBLGBajUu4PGeYCG+21KlT1gj/2d+7LmbzvczFu1XhSmuY0Aus\r\n" \
"njUsWJBG5E3pODPfV/vlvAAMgKQ2iTbqRy+b79qdioT6S3UUQE2camwSnEa8pNpA\r\n" \
"rUlYe7HzdqMm7SWnbweXnRlsAvCSPYoQgQQCqzflrW5rrrtbAAsJShsdru9tAPN4\r\n" \
"JQf/xhJqa8vKTC0sok3at3QjxowPVvQjp56rqHm6Xvh/I26CAKQZGeRTft3o3T/I\r\n" \
"B6/52/4faVLseV/28GnoY6rJp16PwqwrpaCbReauwkwPTFvQVq/eLIDzHqz8LVJW\r\n" \
"PsumGu+C/og6UC66hmakjYUA2d7otQIDAQABoygwJjAkBgNVHREEHTAbgglsb2Nh\r\n" \
"bGhvc3SCDmNzLmRzcHJlYWQubmV0MA0GCSqGSIb3DQEBCwUAA4ICAQAtWUAOzjib\r\n" \
"4Ilv38FcXAyLN+SkxfhO3J4crlHZ7st1XTPlExhR76wln11WFjPFWHh8KVj5uIHC\r\n" \
"LluZLjufNri3S0huR4KamF6ywQbDyX338Xi7+lQV/NN/XVHapCZ7q9otGV3AoSz/\r\n" \
"IQJI25FTMGlEWKvLtqjy8CAolwBPEhtCcymprfLSjZ1RQeDmtN1cwp5xXxSRAzUP\r\n" \
"wwi7nKBUjkeJkAR5D1GXuB9wW7bc5h7tkEoYLM6JL2Sn4mIEKDYviXSS3F8s97Q8\r\n" \
"BXAuGtSSM0t/OEiLUhoU6VIJT2DmGMLrQ3yt4Qx/sOkjDuMR+ZY9bDQsywBvOqAK\r\n" \
"B7jxEFmyxz9wbdk3vAMIUo2+J8CMzyhpvwBi39lDJsd48zB99VqUvl9cBMJNCNln\r\n" \
"GRvnpc1aLXnyRvhqUNm6pa+iwgHmd1jY7PlL69Nrak+l3mw+wqaNxqJJ36JkxNDh\r\n" \
"2YWDZixSQ83YtofwPGr+lwhi/5c49KPJaeGAtZxXeqgIZRUVMCIN6c6fadR3Cdg1\r\n" \
"6Jnrr25ifH3qb3HnOWoV4sNswc0Dif9BTAQCzB+Bd7K7QqEAJotXkundgV1HIjwB\r\n" \
"KOdAMrMcGVq68LtQyg81N7keBAJiQCuSvdUkxgulMAyNA2cvnSjFCoXC8U49xhtu\r\n" \
"pF1zxkU0dTvyQhGJ++i1+a/8UHVHRTqUlA==\r\n" \
"-----END CERTIFICATE-----\r\n""\x00"

//#define REQ_IP                  "44.206.149.45"
//#define REQ_PORT                "443"
#define REQ_IP                  "cs.dspread.net"//"211.157.135.52"
#define REQ_PORT                "9044"
#define REQ_API                 "GET /test/1 HTTP/1.0\r\n\r\n"


#define dsp_malloc(x)   malloc(x)
#define dsp_free(x)     free(x)
#define TRACE(lv, fmt, ...)             usb_debug(fmt, ##__VA_ARGS__);
#define TRACE_DUMP(lv, pbuf, cnt)       Utils_PrintHex(pbuf, cnt);
/*-----------------------------------------------------------------------------
|   Enumerations
+----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
|   Typedefs
+----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
|   Variables
+----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
|   Constants
+----------------------------------------------------------------------------*/


#include "app.h"
#include "ui.h"
#include "utils.h"

extern int set_common_type(void *pxPARAM);
extern int setcomm_wlan(void *pxPARAM);;
extern int WlanGetSign(void);

/*-----------------------------------------------------------------------------
|   Functions
+----------------------------------------------------------------------------*/

/*--------------------------------------
|	Function Name:
|		test_tls
|	Description:
|	Parameters:
|	Returns:
+-------------------------------------*/
int test_tls(void *argv)
{
    int ret = 0;
    int socket_id = 0;
    char *buf = 0;
    char dispbuf[64] = { 0 };
    static int wlan_set_flag = 0;

    if (WifiCheck(NULL) <= 0)
    {
        if (set_common_type(NULL) < 0)
            return -1;
    }

    memset(dispbuf, 0, sizeof(dispbuf));
    sprintf(dispbuf, "TLS TEST %s", RouteGetDefault()==WIFI_ROUTE?"WiFi":"Wlan");
    Ui_ClearLine(0, 9);
    Ui_DispTitle(dispbuf);
    dsp_tls_init();
    buf = dsp_malloc(2048);
    do
    {
        ret = dsp_tls_cacert((const unsigned char*)REQ_CA, strlen(REQ_CA) + 1);
        if(ret < 0)
        {
            TRACE(TRACE_LVL_DEFAULT, "FAIL: %s,%d ret=%d\r\n", __FUNCTION__, __LINE__, ret);
            break;
        }
        TRACE(0, "Read Cacert Success\r\n");

        ret = dsp_tls_clikey((const unsigned char *)REQ_KEY, strlen(REQ_KEY) + 1);
        if (ret < 0)
        {
            TRACE(TRACE_LVL_DEFAULT, "FAIL: %s,%d ret=%d\r\n", __FUNCTION__, __LINE__, ret);
            break;
        }
        TRACE(0, "Read Clikey Success\r\n");

        ret = dsp_tls_clicert((const unsigned char *)REQ_CERT, strlen(REQ_CERT) + 1);
        if(ret < 0)
        {
            TRACE(TRACE_LVL_DEFAULT, "FAIL: %s,%d ret=%d\r\n", __FUNCTION__, __LINE__, ret);
            break;
        }
        TRACE(0, "Read Clicert Success\r\n");
        
        ret = dsp_tls_socket();
        if (ret < 0)
        {
            TRACE(TRACE_LVL_DEFAULT, "FAIL: %s,%d\r\n", __FUNCTION__, __LINE__);
            break;
        }
        socket_id = ret;
        TRACE(0, "dsp_tls_socket Success\r\n");
 
        ret = dsp_tls_vfymode(socket_id, SSL_VERIFY_OPTIONAL);
        if(ret < 0)
        {
            TRACE(TRACE_LVL_DEFAULT, "FAIL: %s,%d\r\n", __FUNCTION__, __LINE__);
            dsp_tls_socketclose(socket_id);
            break;
        }
        ret = dsp_tls_timeout(socket_id, 10000);
        if(ret < 0)
        {
            TRACE(TRACE_LVL_DEFAULT, "FAIL: %s,%d\r\n", __FUNCTION__, __LINE__);
            dsp_tls_socketclose(socket_id);
            break;
        }
                
        ret = dsp_tls_hostname(socket_id, REQ_IP);
        if(ret < 0)
        {
            TRACE(TRACE_LVL_DEFAULT, "FAIL: %s,%d\r\n", __FUNCTION__, __LINE__);
            dsp_tls_socketclose(socket_id);
            break;
        }


        TRACE(0, "Shaking hands...\r\n");
        Ui_ClearLine(1, 6);
        Ui_DispTextLineAlign(1, DISPLAY_CENTER, "Shaking hands...", 0);

        ret = dsp_tls_connect(socket_id, REQ_IP, REQ_PORT);
        if (ret < 0)
        {
            TRACE(TRACE_LVL_DEFAULT, "FAIL: %s,%d\r\n", __FUNCTION__, __LINE__);
            break;
        }
        TRACE(0, "Shake Hands Success\r\n");
        Ui_ClearLine(1, 2);
        Ui_DispTextLineAlign(1, DISPLAY_CENTER, "Shake Hands Success", 0);
        Ui_DispTextLineAlign(2, DISPLAY_CENTER, "Data Sending...", 0);

        ret = dsp_tls_send(socket_id, REQ_API, strlen(REQ_API), 0);
        if (ret != strlen(REQ_API))
        {
            TRACE(TRACE_LVL_DEFAULT, "FAIL: %s,%d\r\n", __FUNCTION__, __LINE__);
            break;
        }
        TRACE(1500, "Data Send Success\r\n");
        Ui_ClearLine(2, 3);
        Ui_DispTextLineAlign(2, DISPLAY_CENTER, "Data Send Success", 0);
        Ui_DispTextLineAlign(3, DISPLAY_CENTER, "Data Receing....", 0);

        memset(buf, 0x00, 2048);
        ret = dsp_tls_recv(socket_id, buf, 1265, 0);
        if (ret != 1265)
        {
            TRACE(TRACE_LVL_DEFAULT, "FAIL: %s,%d\r\n", __FUNCTION__, __LINE__);
            break;
        }
        
        Ui_ClearLine(3, 3);
        Ui_DispTextLineAlign(3, DISPLAY_CENTER, "Data Rece Success", 0);

        TRACE(0, "Data Rece Success\r\nThe data received is:\r\n");
        TRACE_DUMP(TRACE_LVL_DEFAULT, buf, ret);

        ret = dsp_tls_socketclose(socket_id);
        if (ret < 0)
        {
            TRACE(TRACE_LVL_DEFAULT, "FAIL: %s,%d\r\n", __FUNCTION__, __LINE__);
            break;
        }

        TRACE(TRACE_LVL_DEFAULT, "PASS: %s,%d\r\n", __FUNCTION__, __LINE__);
    }while(0);
    
    dsp_free(buf);

    if (ret < 0)
    {
        Ui_ClearLine(0, 9);
        Ui_DispTitle(dispbuf);
        Ui_DispTextLineAlign(3, DISPLAY_CENTER, "TLS Test Error", 0);
        Ui_DispTextLineAlign(4, DISPLAY_CENTER, "Pls Check Network", 0);
    }
    Ui_WaitKey(5 * 1000);

    TRACE(TRACE_LVL_DEFAULT, "---->%s|%d end\r\n", __FUNCTION__, __LINE__);
}



