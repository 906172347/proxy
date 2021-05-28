#ifndef _DATA_TOOL_H
#define _DATA_TOOL_H

#include "head.h"

int Check_Mask(char *Msg,char Mask_Type);
void xor(uchar *a,uchar *b,int lg);
void gz_xor(uchar *a,uchar *b,int lg);
uchar Conv(uchar Oct);
/*
ԭ��: ushort checksum (uchar *StartingAddr, uchar *AfterEndingAddr, ushort ValInit) ;
ֹͣ: ��
������ 'checksum' �ļ��㣨�����������ַ����ܺͣ���
ʹ�ó�ʼֵ'ValInit' ������������ʼ�ֽ� 'StartingAddr'�ۼӵ����һ���ֽ�'AfterEndingAddr'����
����ֵ��������Checksumֵ��

*/
uint checksum( uchar *Pt, uchar *Ptf, uint Val );

/*
���ã�ת�������ε�BCD��
������uchar * long_bcd (uchar *ptDest, uchar lgDest, ulong *ptSrc);
�ȴ�����
˵������unsigned int���ͣ��ַ���10���ƣ�����ת��Ϊ��Ӧ��BCD���ַ��� (ASCII value = digit value + 0x30)�� 
      Ŀ���ַ�����lgDest����ת��ǰ���Ǹ�ֵΪ0x30 

      unsigned int���͵��������ֵ��10�����������֣����ת������ַ��������5���ַ��İ������ַ���ɵ�BCD �ַ�����

����ֵ��ת������ַ�����ַ��
����ptSrc = 1234567890
if lgDest = 2    ptDest = 0x78/0x90
if lgDest = 7    ptDest = 0x00/0x00/0x12/0x34/0x56/0x78/0x90

*/
void *long_bcd(uchar *Ptd ,uchar Lgd,ulong *Pts);
/*
���ã�ת���޷��ų����ε�ASCII�롣
������uchar * long_asc (uchar *ptDest, uchar lgDest, ulong *ptSrc);
�ȴ�����
˵������unsigned int���ͣ��ַ���10���ƣ�����ת��Ϊ��Ӧ��ASCII ���ַ��� (ASCII value = digit value + 0x30)�� 
      Ŀ���ַ�����lgDest����ת��ǰ���Ǹ�ֵΪ0x30 (�ַ�'0') �� 

      ��Ŀ���ַ����Ľ�β������һ��'\0'�ַ��� 

      unsigned int���͵��������ֵ��5�����������֣����ת������ַ��������5���ַ��İ������ַ���ɵ�ASCII �ַ�����

����ֵ�����ת���ַ������׵�ַ��
��
����: ptSrc = 12345
if lgDest = 2        ptDest = '45'

if lgDest = 7        ptDest = '0012345'
*/
uchar *long_asc( uchar *Ptd, uchar Lgd, ulong *Pts );

/*
���ã������޷��ų��������顣
������ uchar * long_tab (uchar *ptDest, uchar lgDest, ulong *ptSrc);
�ȴ�����
˵���������޷��ų��������顣
����ptSrc = 1234608850 = 0x49/0x96/0xA2/0xD2
if lgDest = 2    ptDest = 0xA2/0xD2
if lgDest = 6    ptDest = 0x00/0x00/0x49/0x96/0xA2/0xD2

*/
void *long_tab(uchar *Ptd ,uchar Lgd,ulong *Pts);
/*
ԭ�ͣ�uchar * long_str (uchar *ptDest, uchar lgDest, ulong *ptSrc);
ֹͣ����
��������unsigned  long ��������ת��Ϊ��Ӧ��ASCII���ַ��� (ASCII value = digit value + 0x30)��
������Ŀ���ַ�����lgDest����ת��ǰ���Ǹ�ֵΪ0x30 (�ַ�'0') ��
��������Ŀ���ַ����Ľ�β������һ��0x00�ַ���
������unsigned  short ���͵��������ֵ��10�����������֣����ת������ַ��������10���ַ��İ������ַ���ɵ�ASCII�ַ�����
����ֵ������ת������ַ����׵�ַ��ת�����ַ�����0x00�ַ�������
���磺ptSrc = 1234567890

if lgDest = 2 ptDest = "90"

if lgDest = 12 ptDest = "001234567890"

*/
uchar *long_str( uchar *Ptd, uchar Lgd, ulong *Pts );
/*
����: ��16��������ת�����޷��ŵĳ�������
������'tab_long'������'long_tab'�����ڹ��������෴�ģ�����ptSrc�����16���Ƶ�����,ת��Ϊunsigned long���͵�����
ע�⣺�ú���Ŀǰ����������� < 1000000000 (lgSrc ��1~4֮��)
����ֵ��ת���������
���� ptSrc = 0x49/0x96/0x02/0xD2 (= '1234567890')
if lgSrc = 1 ptDest = 0x000000D2
if lgSrc = 4 ptDest = 0x499602D2 = 1234567890
*/
ulong tab_long(uchar *Pts,uchar Ls);

/*
���ã�ת��BCD�뵽�޷��ų����Ρ�
������ulong bcd_long (uchar *ptSrc, uchar lgSrc) ;
�ȴ�����
˵��:��Դ����ÿ�����ֽ�ת��Ϊ��Ӧ�����֣�����Ϊ�޷��ų�����Ŀ����

    ת���ǽ�Lg��Դ�ֽڴ�ת��ΪLg�İ��ֽڵ�Ŀ�Ĵ���

    ���lgSrcΪ������ת������Ŀ�괮�ĵڶ��������ֽڡ���ʼ������һ�ֽڵĵ�4λ��

����ֵ:���ش�������޷��ų���������

����ptSrc = 0x12/0x34
if lgSrc = 1     ptDest = 2
if lgSrc = 2     ptDest = 12
if lgSrc = 3     ptDest = 234
if lgSrc = 4     ptDest = 1234

*/
ulong bcd_long( uchar *Pts, uchar Ls );
/*
���ã�ת��ASCII�뵽�޷��ų����Ρ�
������ ulong asc_long (uchar *ptSrc, uchar lgSrc) ;
�ȴ�����
ע�⣺�ú���ת������ӦС��1000000000.
��������չÿ��ASCIIԴ�ֽڵ����ĵ�4λ��������ת��Ϊ��Ӧ���޷��ų�����Ŀ������ֵ��

      ת���ǽ�lgSrc��Դ�ֽڴ���ΪlgSrc���ֲ�����ΪĿ��ֵ��

����ֵ���޷��ų�������
���� ptSrc = '123'
if lgSrc = 3     ptDest = 123
if lgSrc = 2     ptDest = 12
*/
ulong asc_long(uchar *Pts,uchar Ls);
/*
���ã�ת��ACSII�뵽BCD��
������ uchar * asc_bcd (uchar *ptDest, uchar lgDest, uchar *ptSrc, uchar lgSrc) ;
�ȴ�����
����ֵ������ת����������׵�ַ��
������ȡASCII��ĸ����ֽڵ�4λ�����ڶ�ӦĿ���ַ����Ķ�Ӧ���ֽ�λ�á�
      ת���ǽ�lgSrc��Դ�ֽڴ�ת��ΪlgSrc�İ��ֽڵ�Ŀ�Ĵ���
      Ŀ���ַ����ĳ��ȣ�
     ���lgDest>lgSrc/2 ����ΪLgDest���ֽ���
     ���lgDest<lgSrc/2 ��lgSrcΪż������ΪlgSrc/2�ֽ���
     ���lgDest<lgSrc/2 ��lgSrcΪ��������Ϊ��lgSrc��1��/2�ֽ���
         ���lgDest>lgSrc/2��Ŀ�괮��ת��ǰ����Ϊ0x00��
      ���lgSrcΪ������ת������Ŀ�괮�ĵڶ������ֽڿ�ʼ������һ�ֽڵĵ�4λ��
���� ptSrc = '12345' = 0x31/0x32/0x33/0x34/0x35
if lgSrc = 4 and lgDest = 3        ptDest = 0x00/0x12/0x34
if lgSrc = 4 and lgDest < lgSrc/2  ptDest = 0x12/0x34
if lgSrc = 5 and lgDest = 3        ptDest = 0x01/0x23/0x45
if lgSrc = 5 and lgDest < lgSrc/2  ptDest = 0x(?)1/0x23/0x45

*/
void *asc_bcd(uchar *Ptd ,uchar Lgd,uchar *Pts,uchar Lgs);

/*
���ã�ת��ASCII�뵽ʮ��������
������ uchar * asc_hex (uchar *ptDest, uchar lgDest, uchar *ptSrc, uchar lgSrc) ;
�ȴ�����
����ֵ������ת����������׵�ַ��
����:����ֵ�ķ�Χ��ÿ��ASCIIԴ�ֽڣ�Oi������ת����Ŀ�Ĵ��Ķ�Ӧ�İ��ֽ�
          for Oi = '0' to '9' (0x30 to 0x39) => 0 to 9
          for Oi = 'A' to 'F' (0x41 to 0x46) => A to F
     ת���ǽ�lgSrc��Դ�ֽڴ�ת��ΪlgSrc�İ��ֽڵ�Ŀ�Ĵ���
     Ŀ�괮��lgDest����ת��ǰͨ������Ϊ0x00��
     ���lgSrcΪ������ת������Ŀ�괮�ĵڶ��������ֽڡ���ʼ������һ�ֽڵĵ�4λ��
����ptSrc = '1AC45'
if lgSrc = 4 and lgDest = 3        ptDest = 0x00/0x1A/0xC4
if lgSrc = 4 and lgDest < lgSrc/2  ptDest = 0x1A/0xC4
if lgSrc = 5 and lgDest = 4        ptDest = 0x00/0x01/0xAC/0x45
if lgSrc = 5 and lgDest < lgSrc/2  ptDest = 0x01/0xAC/0x45

*/
void *asc_hex(uchar *Ptd ,uchar Ld,uchar *Pts,uchar Ls);

/*
���ã���BCD��ת��ΪASCII��
������uchar * bcd_asc (uchar *ptDest, uchar *ptSrc, uchar Lg) ;
�ȴ�����
����:��1�ֽ�Ϊ��λ����Դ�ֽڴ���ÿ�������ֽڡ���Qi��ת����ASCIIֵ 
         for Oi = '0' to '9' (0x30 to 0x39) => 0 to 9
         for Oi = 'A' to 'F' (0x41 to 0x46) => A to F
    ת���ǽ� Lg��Դ�ֽڴ�ת��Ϊ Lg�İ��ֽڵ�Ŀ�Ĵ���
    Ŀ�괮��lgDest����ת��ǰͨ������Ϊ0x00
    ��� LgΪ������ת������Ŀ�괮�ĵڶ��������ֽڡ���ʼ������һ�ֽڵĵ�4λ��
����ֵ������ת����������׵�ַ(ptDest + Lg)
���� ptSrc = 0x12/0x34
if Lg = 1        ptDest = '2'
if Lg = 2        ptDest = '12'
if Lg = 3        ptDest = '234'
if Lg = 4        ptDest = '1234'

*/
uchar *bcd_asc( uchar *Ptd , uchar *Pts , uchar Lg);


/*
���ã�ת��ʮ����������ASCII��
������ uchar * hex_asc (uchar *ptDest, uchar *ptSrc, uchar Lg) ;
�ȴ�����
˵��: ת��ʮ����������ASCII��
         ���0 to 9 => '0' to '9' (0x30 to 0x39)
         ��� A to F => 'A' to 'F' (0x41 to 0x46) 
����ֵ�����ת����������׵�ַ��
���� ptSrc = 0x1A/0xC4
if Lg = 1        ptDest = 'A'
if Lg = 2        ptDest = '1A'
if Lg = 3        ptDest = 'AC4'
if Lg = 4        ptDest = '1AC4'

*/
uchar *hex_asc( uchar *Ptd, uchar *Pts, uchar Lg );
/*
���ã�ת��ʮ����������ASCII��
������ uchar * hex_str (uchar *ptDest, uchar *ptSrc, uchar Lg) ;
�ȴ�����
˵��: ת��ʮ����������ASCII��
         ���0 to 9 => '0' to '9' (0x30 to 0x39)
         ��� A to F => 'A' to 'F' (0x41 to 0x46) 
����ֵ�����ת����������׵�ַ��
���� ptSrc = 0x1A/0xC4
if Lg = 1        ptDest = 'A'
if Lg = 2        ptDest = '1A'
if Lg = 3        ptDest = 'AC4'
if Lg = 4        ptDest = '1AC4'

*/
uchar *hex_str( uchar *Ptd, uchar *Pts, uchar Lg );

int rtrim( char *Pts );
int ltrim( char *Pts );
int trim(char *Pts);
/*
���ã���unsigned int�������� ת��Ϊ ASCII ���ַ����飻
ԭ��: uchar * int_asc (uchar *ptDest, uchar lgDest, ushort *ptSrc);
��������unsigned int������ת��ΪASCII�루��0~9���ַ���(ASCII value = digit value + 0x30)
Ŀ���ַ�����lgDest����ת��ǰ���Ǹ�ֵΪ0x30 (�ַ�'0') 
unsigned int���͵��������ֵ��5�����������֣����ת������ַ��������5���ַ��İ������ַ���ɵ�ASCII �ַ���
����ֵ������ת������ַ����׵�ַ��
����: ptSrc = 12345
if lgDest = 2       ptDest = '45'
if lgDest = 7       ptDest = '0012345'
*/
void *int_asc(uchar *Ptd ,uchar Lgd,uint *Pts);
/*
���ã�����unsigned int ��16�������ݣ���������һ��unsigned char�͵��ַ���
ԭ��: uchar * int_tab (uchar *ptDest, uchar lgDest, ushort *ptSrc);
����������ʮ�������������飬Ŀ���ַ�����lgDest����ת��ǰ���Ǹ�ֵΪ0x00 ��
����ֵ������ת������ַ����׵�ַ��
����:ptSrc = 12345 = 0x3039
if lgDest = 1       ptDest = 0x39
*/
void *int_tab(uchar *Ptd ,uchar Lgd,uint *Pts);
/*
���ã���unsigned int��������,ת��Ϊ ASCII ���ַ����飻 
ԭ��: uchar * int_bcd (uchar *ptDest, uchar lgDest, ushort *ptSrc); 
ֹͣ: ��
��������unsigned int���ͣ��ַ���10���ƣ�����ת �� Ϊ ��Ӧ��BCD �� �ַ���,Ŀ���ַ�����lgDest����ת��ǰ���Ǹ�ֵΪ0x00 �� 
      unsigned int���͵��������ֵ��5�����������֣����ת�����BCD���������5��������ַ��� ��  �ɵ��ַ����� 
����ֵ������ת������ַ����׵�ַ�� 
����: ptSrc = 12345 
if lgDest = 2 ptDest = 0x23/0x45 
if lgDest = 5 ptDest = 0x00/0x00/0x01/0x23/0x45 

*/
void *int_bcd(uchar *Ptd ,uchar Lgd,uint *Pts);
/*
���ã���unsigned int�������� ת��Ϊ ASCII ���ַ����� 
ԭ��: uchar * int_str (uchar *ptDest, uchar lgDest, ushort *ptSrc); 
ֹͣ: ��
��������unsigned int���ͣ��ַ���10���ƣ�����ת��Ϊ��Ӧ��ASCII ���ַ��� (ASCII value = digit value + 0x30)�� 
      Ŀ���ַ�����lgDest����ת��ǰ���Ǹ�ֵΪ0x30 (�ַ�'0') �� 
      ��Ŀ���ַ����Ľ�β������һ��'\0'�ַ��� 
      unsigned int���͵��������ֵ��5�����������֣����ת������ַ��������5���ַ��İ������ַ���ɵ�ASCII �ַ�����
����ֵ������ת������ַ����׵�ַ��ת�����ַ�����'\0'�ַ������� 
����ptSrc = 12345 
if lgDest = 2 ptDest = "45" 
if lgDest = 7 ptDest = "0012345" 

*/
void *int_str(uchar *Ptd ,uchar Lgd,uint *Pts);
/*
����: ��һ���Ǹ�CHAR����ת����ASCII���ַ���
ԭ��: uchar * char_asc (uchar *ptDest, uchar lgDest, uchar *ptSrc);
ֹͣ: ��
��������unsigned  char ���ͣ��ַ���10���ƣ�����ת��Ϊ��Ӧ��ASCII ���ַ��� (ASCII value = digit value + 0x30)��
      Ŀ���ַ�����lgDest����ת��ǰ���Ǹ�ֵΪ0x30 (�ַ�'0') ��
      unsigned  char ���͵��������ֵ��3�����������֣����ת������ַ��������3���ַ��İ������ַ���ɵ�ASCII �ַ�����
����ֵ������ת������ַ����׵�ַ��
����: ptSrc = 'z' = 0x7A = 122
if lgDest = 2 ptDest = '22'
if lgDest = 5 ptDest = '00122'
*/
void *char_asc( uchar *Ptd , uchar Lgd, uchar *Pts );

/*
����:���޷��ŵ�����ת��Ϊ��Ӧ��ASCII ���ַ���
ԭ��: uchar * char_str (uchar *ptDest, uchar lgDest, uchar *ptSrc);
�ȴ�:��
����:��unsigned  char ���ͣ��ַ���10���ƣ�����ת��Ϊ��Ӧ��ASCII ���ַ��� (ASCII value = digit value + 0x30)��
     Ŀ���ַ�����lgDest����ת��ǰ���Ǹ�ֵΪ0x30 (�ַ�'0') ��
     ��Ŀ���ַ����Ľ�β������һ��'\0' �ַ���
     unsigned  char ���͵��������ֵ��3�����������֣����ת������ַ��������3���ַ��İ������ַ���ɵ�ASCII �ַ�����
����ֵ������ת������ַ����׵�ַ��ת�����ַ�����'\0' �ַ�������
����: ptSrc = 'z' = 0x7A = 122
if lgDest = 2 ptDest = "22"
if lgDest = 5 ptDest = "00122"
*/
void *char_str( uchar *Ptd , uchar Lgd, uchar *Pts );

/*
����: ��һ���Ǹ�CHAR����ת����BCD���ַ���
ԭ��: uchar * char_bcd (uchar *ptDest, uchar lgDest, uchar *ptSrc);
��������unsigned  char ���ͣ��ַ���10���ƣ�����ת��Ϊ��Ӧ��BCD ���ַ�����
     Ŀ���ַ�����lgDest����ת��ǰ���Ǹ�ֵΪ0x00��     
     unsigned  char ���͵��������ֵ��3�����������֣����ת�����BCD���������3��������ַ�����ɵ��ַ�����
����ֵ������ת������ַ����׵�ַ��
����: ptSrc = 'z' = 0x7A = 122
if lgDest = 1 ptDest = 0x22
if lgDest = 4 ptDest = 0x00/0x00/0x01/0x22
*/
void *char_bcd(uchar *Ptd ,uchar Lgd,uchar *Pts);
/*
����:��ASCII���ַ���ת�����޷��ŵĳ�����������ȥ(32λ)
ԭ��: ulong str_long (uchar *ptSrc) ;
ֹͣ:��
�������ú�����'asc_long'�������ƣ�lgSrc = strlen(ptSrc)��
      �����ض�ÿ���ַ��ĵ�4λ��ͬʱת��Ϊunsigned long�Ͷ�Ӧ�����ݣ�
ע�⣺�ú���Ŀǰʹ�õķ�Χ�ǣ����� < 1000000000;
����ֵ��ת��������ݡ�
����: ptSrc = "123" => ptDest = 123
*/
ulong str_long( uchar *Pts );

/*
����:��ASCII��ת����BCDֵ��
�������˺����� 'asc_bcd'�������ƣ�lgSrc = strlen(ptSrc)��
      �����ض�ÿ��ASCII ��ĵ�4λΪ��ӦĿ���ַ����ġ�����ֽڡ�\��
     ��� lgDest > strlen(ptSrc)/2 Ŀ���ַ���ǰ��0x00;
      ��� ԭ�ַ����ĳ�������������ת�����ӵڶ����ַ��ġ�����ֽڡ���ʼ
����ֵ������ת������ַ����׵�ַ��
����: ptSrc = "12345" = 0x31/0x32/0x33/0x34/0x35/'\0'
(thus lgSrc = strlen(ptSrc) = 5)
if lgDest = 4 (> strlen(ptSrc)/2) ptDest = 0x00/0x01/0x23/0x45
if lgDest < strlen(ptSrc)/2 ptDest = 0x(?)1/0x23/0x45
the (?) represent the old 'half byte' value which is not modified

*/
void *str_bcd(uchar *Ptd ,uchar Lgd,uchar *Pts);
/*
ԭ��: uchar * str_hex (uchar *ptDest, uchar lgDest, uchar *ptSrc) ;

����:�˺����� 'asc_bcd'�������ƣ�lgSrc = strlen(ptSrc)��

     �����ض�ÿ��ASCII ��ĵ�4λΪ��ӦĿ���ַ����ġ�����ֽڡ���

     ��� Oi = '0' to '9'      (0x30 to 0x39) => 0 to 9

     ��� Oi = 'A' to 'F' (0x41 to 0x46) => A to F

     Ŀ���ַ�����lgDest����ת��ǰ���Ǹ�ֵΪ0x00 ��

     ��� ԭ�ַ����ĳ�������������ת�����ӵڶ����ַ��ġ�����ֽڡ���ʼ

����ֵ������ת������ַ����׵�ַ

����: ptSrc = '1AC45'
(thus lgSrc = strlen(ptSrc) = 5)

if lgDest = 4 (> strlen(ptSrc)/2) ptDest = 0x00/0x01/0xAC/0x45

if lgDest < strlen(ptSrc)/2 ptDest = 0x01/0xAC/0x45.

*/
void *str_hex(uchar *Ptd ,uchar Lgd,uchar *Pts);

/*
ԭ��: uchar * bcd_str (uchar *ptDest, uchar *ptSrc, uchar Lg) ;
ֹͣ:��
���������ַ���ÿ������ֽڡ�ת��Ϊһ���ֽڵ�ASCII�루��0x30 ��0x39���������������ַ����Ľ�β��'\0'�ַ�������
      ��������ǽ�Lg��������ַ���ת��ΪLg���ֽڳ��ȵ��ַ�����
      ���Lg��������ת�������ָ���ĵڶ���������ַ�����ʼ���ӵ�һ���ֽڵĵ�4λ��ʼ����
����ֵ������ת������ַ����׵�ַ��ת�����ַ����� '\0'�ַ�������
����: ptSrc = 0x12/0x34

if Lg = 1 ptDest = "2"

if Lg = 2 ptDest = "12"

if Lg = 3 ptDest = "234"

if Lg = 4 ptDest = "1234"

*/
uchar *bcd_str( uchar *Ptd , uchar *Pts , uchar Lg );
  /**
 * ���ַ����е�Сд��ĸת��Ϊ��д��ĸ
 * @param eopczStr
 * @since 2009-7-10
 */
void Upper( char *eopczStr );
  /**
 * ���ַ����ն˴�д��ĸת��ΪСд��ĸ
 * @param eopczStr
 * @since 2009-7-10
 */
void Lower( char *eopczStr );


void des(uchar *binput, uchar *boutput, uchar *bkey);
void desm1(uchar *binput, uchar *boutput, uchar *bkey);
void doubleDecrypt(uchar *in, uchar *out, uchar *key);
void macgen(uchar *in,uint len,uchar *key,char *masterkey, uchar *out);


#endif
