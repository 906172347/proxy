#ifndef _ISO8583_CUPS_H
#define _ISO8583_CUPS_H

#include "head.h"
#include "errcode.h"


#define NUMERIC         0x01	//压缩的数字
#define TRACKX          0x02	//压缩的数字加字母
#define ALPHANUM        0x03	//非压缩的数字
#define ALPHANUMS       0x04	//非压缩的数字加字母
#define BITS            0x05	//二进制数

#define LLVAR           0x01	//一字节压缩BCD表示长度,长度右对齐
#define LLLVAR          0x02	//两字节压缩BCD表示长度,长度右对齐
#define MMDDhhmmss      0x03	//压缩的数字, 专指MMDDhhmmss格式的时间
#define YYMMDD          0x04	//压缩的数字, 专指YYMMDD格式的时间
#define YYMM            0x05	//压缩的数字, 专指YYMM格式的时间
#define hhmmss          0x06	//压缩的数字, 专指hhmmss格式的时间

#define BIT1   1
#define BIT2   2
#define BIT3	3
#define BIT4	4
#define BIT5	5
#define BIT6	6
#define BIT7	7
#define BIT8	8
#define BIT9	9
#define BIT10	10
#define BIT11	11
#define BIT12	12
#define BIT13	13
#define BIT14	14
#define BIT15	15
#define BIT16	16
#define BIT17	17
#define BIT18	18
#define BIT19	19
#define BIT20	20
#define BIT21	21
#define BIT22	22
#define BIT23	23
#define BIT24	24
#define BIT25	25
#define BIT26	26
#define BIT27	27
#define BIT28	28
#define BIT29	29
#define BIT30	30
#define BIT31	31
#define BIT32	32
#define BIT33	33
#define BIT34	34
#define BIT35	35
#define BIT36	36
#define BIT37	37
#define BIT38	38
#define BIT39	39
#define BIT40	40
#define BIT41	41
#define BIT42	42
#define BIT43	43
#define BIT44	44
#define BIT45	45
#define BIT46	46
#define BIT47	47
#define BIT48	48
#define BIT49	49
#define BIT50	50
#define BIT51	51
#define BIT52	52
#define BIT53	53
#define BIT54	54
#define BIT55	55
#define BIT56	56
#define BIT57	57
#define BIT58	58
#define BIT59	59
#define BIT60	60
#define BIT61	61
#define BIT62	62
#define BIT63	63
#define BIT64	64
#define BIT65  65
#define BIT66  66
#define BIT67  67
#define BIT68  68
#define BIT69  69
#define BIT70  70
#define BIT71  71
#define BIT72  72
#define BIT73  73
#define BIT74  74
#define BIT75  75
#define BIT76  76
#define BIT77  77
#define BIT78  78
#define BIT79  79
#define BIT80  80
#define BIT81  81
#define BIT82  82
#define BIT83  83
#define BIT84  84
#define BIT85  85
#define BIT86  86
#define BIT87  87
#define BIT88  88
#define BIT89  89
#define BIT90  90
#define BIT91  91
#define BIT92  92
#define BIT93  93
#define BIT94  94
#define BIT95  95
#define BIT96  96
#define BIT97  97
#define BIT98  98
#define BIT99  99
#define BIT100 100
#define BIT101 101
#define BIT102 102
#define BIT103 103
#define BIT104 104
#define BIT105 105
#define BIT106 106
#define BIT107 107
#define BIT108 108
#define BIT109 109
#define BIT110 110
#define BIT111 111
#define BIT112 112
#define BIT113 113
#define BIT114 114
#define BIT115 115
#define BIT116 116
#define BIT117 117
#define BIT118 118
#define BIT119 119
#define BIT120 120
#define BIT121 121
#define BIT122 122
#define BIT123 123
#define BIT124 124
#define BIT125 125
#define BIT126 126
#define BIT127 127
#define BIT128 128




#define ASC_NUL 0x00
#define ASC_ASC 0x01
#define BCD_ASC 0x02
#define ASC_BCD 0x03
#define HEX_ASC 0x04
#define ASC_HEX 0x05

struct BITD
{
	unsigned char 	bitn;
	unsigned char  	format;
	unsigned char  	attrib;
	int 	max;
};

struct Pos
{

uchar   sLocalSerial[15];   /*本地流水号YYMMDD+8位本地序号*/
uchar   sLocalTime[15];     /*本地时间*/
// The next is 8583 message
uchar   sMsgId[5];          /*信息类型 第0域*/
uchar   sBitMap[16+1];          /*位图信息 第1域*/
uchar   sCardNo[21];        /*卡号 第2域*/
uchar   sProcessCode[7];    /*交易处理码 第3域*/
uchar   sAmount[13];        /*交易金额 第4域*/
uchar   sTermSerial[7];     /*终端流水号 第11域*/
uchar   sTermBatchNo[7];    /*终端批次号*/
uchar   sNetWorkCode[4];    /*网络管理信息码*/
uchar   sTimeTrn[11];       /*交易传输时间 第7域*/    

uchar   sFrontTime[7];      /*POS中心时间 第12域*/
uchar   sFrontDate[5];      /*POS中心日期 第13域*/
uchar   sExpiredDate[5];    /*卡有效期 第14域*/
uchar   sSettleDate[5];     /*结算日期 第15域*/
uchar   sMchtType_18[5];  /*18域  商户类型*/

uchar   sEntryMode[5];      /*服务点输入方式码 第22域*/
uchar   sCardSerial[4];      /*卡序列号   第23域*/

uchar   sConditionMode[3];  /*服务点条件码 第25域*/
uchar   sPinCapture[3];     /*服务点PIN获取码 第26域*/
uchar   sPosCenterCode[13]; /*受理方标识码(POS中心代号) 第32域*/
uchar   sSendCode[13]; /*发送机构号(POS中心代号) 第33域*/

uchar   sTrack2Data[93];     /*二 磁道加密数据 第35域*/
uchar   sTrackData[93];     /*磁道加密数据 第36域*/
uchar   sFrontSerial[13];   /*POS中心流水号 第37域*/
uchar   sAuthCode[7];       /*授权码 第38域*/
uchar   sReturnCode[3];     /*返回码 第39域*/
uchar   sTermNumber[9];     /*终端号 第41域*/
uchar   sMerchantID[16];    /*商户号 第42域*/
uchar   sAcquirer[26];      /*发卡行标识码+收单机构标识码 第44域*/
uchar   sField48[162];      /*结算总额或交易明细或交易明细总笔数 第48域*/
uchar   sCurrencyCode[4];   /*交易货币代码 第49域*/
uchar   sPinData[13];       /*密码密文 第52域*/
uchar   sSafeControl[17];   /*安全控制信息 第53域*/
uchar   sBalance[13];       /*卡余额 第54域*/
uchar   sField60[13];       /*自定义域 第60域 交易类型码+批次号+网络管理码*/
uchar   sField61[15];       /*原交易信息(冲正、撤销和退货交易时填原始交易数据) 第61域*/
uchar   sField62[41];       /*自定义域 第62域*/
uchar   sField63[82];       /*自定义域 第63域*/
uchar   sMac[20];           /*MAC 第64域*/

// The next is field length message
int     Length02;
int     Length32;
int     Length33;
int     Length35;
int     Length36;
int     Length44;
int     Length48;
int     Length54;
int     Length60;
int     Length61;
int     Length62;
int     Length63;
};


int Unpack_Iso8583_Package(uchar *rxbuf, struct Pos *optrPos);
uint Pack_Iso8583_Package(uchar *buf,struct Pos *optrPos);


#endif


