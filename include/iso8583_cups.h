#ifndef _ISO8583_CUPS_H
#define _ISO8583_CUPS_H

#include "head.h"
#include "errcode.h"


#define NUMERIC         0x01	//ѹ��������
#define TRACKX          0x02	//ѹ�������ּ���ĸ
#define ALPHANUM        0x03	//��ѹ��������
#define ALPHANUMS       0x04	//��ѹ�������ּ���ĸ
#define BITS            0x05	//��������

#define LLVAR           0x01	//һ�ֽ�ѹ��BCD��ʾ����,�����Ҷ���
#define LLLVAR          0x02	//���ֽ�ѹ��BCD��ʾ����,�����Ҷ���
#define MMDDhhmmss      0x03	//ѹ��������, רָMMDDhhmmss��ʽ��ʱ��
#define YYMMDD          0x04	//ѹ��������, רָYYMMDD��ʽ��ʱ��
#define YYMM            0x05	//ѹ��������, רָYYMM��ʽ��ʱ��
#define hhmmss          0x06	//ѹ��������, רָhhmmss��ʽ��ʱ��

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

uchar   sLocalSerial[15];   /*������ˮ��YYMMDD+8λ�������*/
uchar   sLocalTime[15];     /*����ʱ��*/
// The next is 8583 message
uchar   sMsgId[5];          /*��Ϣ���� ��0��*/
uchar   sBitMap[16+1];          /*λͼ��Ϣ ��1��*/
uchar   sCardNo[21];        /*���� ��2��*/
uchar   sProcessCode[7];    /*���״����� ��3��*/
uchar   sAmount[13];        /*���׽�� ��4��*/
uchar   sTermSerial[7];     /*�ն���ˮ�� ��11��*/
uchar   sTermBatchNo[7];    /*�ն����κ�*/
uchar   sNetWorkCode[4];    /*���������Ϣ��*/
uchar   sTimeTrn[11];       /*���״���ʱ�� ��7��*/    

uchar   sFrontTime[7];      /*POS����ʱ�� ��12��*/
uchar   sFrontDate[5];      /*POS�������� ��13��*/
uchar   sExpiredDate[5];    /*����Ч�� ��14��*/
uchar   sSettleDate[5];     /*�������� ��15��*/
uchar   sMchtType_18[5];  /*18��  �̻�����*/

uchar   sEntryMode[5];      /*��������뷽ʽ�� ��22��*/
uchar   sCardSerial[4];      /*�����к�   ��23��*/

uchar   sConditionMode[3];  /*����������� ��25��*/
uchar   sPinCapture[3];     /*�����PIN��ȡ�� ��26��*/
uchar   sPosCenterCode[13]; /*������ʶ��(POS���Ĵ���) ��32��*/
uchar   sSendCode[13]; /*���ͻ�����(POS���Ĵ���) ��33��*/

uchar   sTrack2Data[93];     /*�� �ŵ��������� ��35��*/
uchar   sTrackData[93];     /*�ŵ��������� ��36��*/
uchar   sFrontSerial[13];   /*POS������ˮ�� ��37��*/
uchar   sAuthCode[7];       /*��Ȩ�� ��38��*/
uchar   sReturnCode[3];     /*������ ��39��*/
uchar   sTermNumber[9];     /*�ն˺� ��41��*/
uchar   sMerchantID[16];    /*�̻��� ��42��*/
uchar   sAcquirer[26];      /*�����б�ʶ��+�յ�������ʶ�� ��44��*/
uchar   sField48[162];      /*�����ܶ������ϸ������ϸ�ܱ��� ��48��*/
uchar   sCurrencyCode[4];   /*���׻��Ҵ��� ��49��*/
uchar   sPinData[13];       /*�������� ��52��*/
uchar   sSafeControl[17];   /*��ȫ������Ϣ ��53��*/
uchar   sBalance[13];       /*����� ��54��*/
uchar   sField60[13];       /*�Զ����� ��60�� ����������+���κ�+���������*/
uchar   sField61[15];       /*ԭ������Ϣ(�������������˻�����ʱ��ԭʼ��������) ��61��*/
uchar   sField62[41];       /*�Զ����� ��62��*/
uchar   sField63[82];       /*�Զ����� ��63��*/
uchar   sMac[20];           /*MAC ��64��*/

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


