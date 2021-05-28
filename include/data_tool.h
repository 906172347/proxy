#ifndef _DATA_TOOL_H
#define _DATA_TOOL_H

#include "head.h"

int Check_Mask(char *Msg,char Mask_Type);
void xor(uchar *a,uchar *b,int lg);
void gz_xor(uchar *a,uchar *b,int lg);
uchar Conv(uchar Oct);
/*
原型: ushort checksum (uchar *StartingAddr, uchar *AfterEndingAddr, ushort ValInit) ;
停止: 是
描述： 'checksum' 的计算（数组中所有字符的总和）：
使用初始值'ValInit' 计算从数组的起始字节 'StartingAddr'累加到最后一个字节'AfterEndingAddr'；　
返回值：计算后的Checksum值；

*/
uint checksum( uchar *Pt, uchar *Ptf, uint Val );

/*
作用：转换长整形到BCD码
函数：uchar * long_bcd (uchar *ptDest, uchar lgDest, ulong *ptSrc);
等待：是
说明：将unsigned int类型（字符的10进制）数据转换为对应的BCD码字符串 (ASCII value = digit value + 0x30)； 
      目标字符串（lgDest）在转换前总是赋值为0x30 

      unsigned int类型的数据最大值有10个阿拉伯数字，因此转换后的字符串最多是5个字符的阿拉伯字符组成的BCD 字符串。

返回值：转换后的字符串地址。
例：ptSrc = 1234567890
if lgDest = 2    ptDest = 0x78/0x90
if lgDest = 7    ptDest = 0x00/0x00/0x12/0x34/0x56/0x78/0x90

*/
void *long_bcd(uchar *Ptd ,uchar Lgd,ulong *Pts);
/*
作用：转换无符号长整形到ASCII码。
函数：uchar * long_asc (uchar *ptDest, uchar lgDest, ulong *ptSrc);
等待：是
说明：将unsigned int类型（字符的10进制）数据转换为对应的ASCII 码字符串 (ASCII value = digit value + 0x30)； 
      目标字符串（lgDest）在转换前总是赋值为0x30 (字符'0') ； 

      在目标字符串的结尾会增加一个'\0'字符。 

      unsigned int类型的数据最大值有5个阿拉伯数字，因此转换后的字符串最多是5个字符的阿拉伯字符组成的ASCII 字符串。

返回值：存放转换字符串的首地址。
　
例如: ptSrc = 12345
if lgDest = 2        ptDest = '45'

if lgDest = 7        ptDest = '0012345'
*/
uchar *long_asc( uchar *Ptd, uchar Lgd, ulong *Pts );

/*
作用：返回无符号长整形数组。
函数： uchar * long_tab (uchar *ptDest, uchar lgDest, ulong *ptSrc);
等待：是
说明：返回无符号长整形数组。
例：ptSrc = 1234608850 = 0x49/0x96/0xA2/0xD2
if lgDest = 2    ptDest = 0xA2/0xD2
if lgDest = 6    ptDest = 0x00/0x00/0x49/0x96/0xA2/0xD2

*/
void *long_tab(uchar *Ptd ,uchar Lgd,ulong *Pts);
/*
原型：uchar * long_str (uchar *ptDest, uchar lgDest, ulong *ptSrc);
停止：是
描述：将unsigned  long 类型数据转换为对应的ASCII码字符串 (ASCII value = digit value + 0x30)；
　　　目标字符串（lgDest）在转换前总是赋值为0x30 (字符'0') ；
　　　在目标字符串的结尾会增加一个0x00字符。
　　　unsigned  short 类型的数据最大值有10个阿拉伯数字，因此转换后的字符串最多是10个字符的阿拉伯字符组成的ASCII字符串。
返回值：返回转换后的字符串首地址，转换后字符串以0x00字符结束；
例如：ptSrc = 1234567890

if lgDest = 2 ptDest = "90"

if lgDest = 12 ptDest = "001234567890"

*/
uchar *long_str( uchar *Ptd, uchar Lgd, ulong *Pts );
/*
作用: 把16进制数组转化成无符号的长整形数
描述：'tab_long'函数和'long_tab'函数在功能上是相反的，它将ptSrc里面的16进制的数据,转换为unsigned long类型的数据
注意：该函数目前计算出的数据 < 1000000000 (lgSrc 在1~4之间)
返回值：转换后的数据
例如 ptSrc = 0x49/0x96/0x02/0xD2 (= '1234567890')
if lgSrc = 1 ptDest = 0x000000D2
if lgSrc = 4 ptDest = 0x499602D2 = 1234567890
*/
ulong tab_long(uchar *Pts,uchar Ls);

/*
作用：转换BCD码到无符号长整形。
函数：ulong bcd_long (uchar *ptSrc, uchar lgSrc) ;
等待：是
说明:把源串的每个半字节转换为相应的数字，保存为无符号长整形目标数

    转换是将Lg的源字节串转换为Lg的半字节到目的串中

    如果lgSrc为奇数，转换将从目标串的第二个“半字节”开始（即第一字节的低4位）

返回值:返回处理过的无符号长整形数据

例：ptSrc = 0x12/0x34
if lgSrc = 1     ptDest = 2
if lgSrc = 2     ptDest = 12
if lgSrc = 3     ptDest = 234
if lgSrc = 4     ptDest = 1234

*/
ulong bcd_long( uchar *Pts, uchar Ls );
/*
作用：转换ASCII码到无符号长整形。
函数： ulong asc_long (uchar *ptSrc, uchar lgSrc) ;
等待：是
注意：该函数转换的数应小于1000000000.
描述：扩展每个ASCII源字节到它的低4位，并将其转换为相应的无符号长整形目标数数值。

      转化是将lgSrc的源字节串变为lgSrc数字并保存为目标值。

返回值：无符号长整形数
例： ptSrc = '123'
if lgSrc = 3     ptDest = 123
if lgSrc = 2     ptDest = 12
*/
ulong asc_long(uchar *Pts,uchar Ls);
/*
作用：转换ACSII码到BCD码
函数： uchar * asc_bcd (uchar *ptDest, uchar lgDest, uchar *ptSrc, uchar lgSrc) ;
等待：是
返回值：返回转换后数组的首地址。
描述：取ASCII码的各个字节低4位，置于对应目的字符串的对应的字节位置。
      转换是将lgSrc的源字节串转换为lgSrc的半字节到目的串中
      目标字符串的长度：
     如果lgDest>lgSrc/2 ，则为LgDest的字节数
     如果lgDest<lgSrc/2 且lgSrc为偶数，则为lgSrc/2字节数
     如果lgDest<lgSrc/2 且lgSrc为奇数，则为（lgSrc＋1）/2字节数
         如果lgDest>lgSrc/2，目标串在转换前被置为0x00。
      如果lgSrc为奇数，转换将从目标串的第二个半字节开始（即第一字节的低4位）
例： ptSrc = '12345' = 0x31/0x32/0x33/0x34/0x35
if lgSrc = 4 and lgDest = 3        ptDest = 0x00/0x12/0x34
if lgSrc = 4 and lgDest < lgSrc/2  ptDest = 0x12/0x34
if lgSrc = 5 and lgDest = 3        ptDest = 0x01/0x23/0x45
if lgSrc = 5 and lgDest < lgSrc/2  ptDest = 0x(?)1/0x23/0x45

*/
void *asc_bcd(uchar *Ptd ,uchar Lgd,uchar *Pts,uchar Lgs);

/*
作用：转换ASCII码到十六进制码
函数： uchar * asc_hex (uchar *ptDest, uchar lgDest, uchar *ptSrc, uchar lgSrc) ;
等待：是
返回值：返回转换后数组的首地址。
描述:根据值的范围，每个ASCII源字节（Oi）将被转换到目的串的对应的半字节
          for Oi = '0' to '9' (0x30 to 0x39) => 0 to 9
          for Oi = 'A' to 'F' (0x41 to 0x46) => A to F
     转换是将lgSrc的源字节串转换为lgSrc的半字节到目的串中
     目标串（lgDest）在转换前通常被置为0x00。
     如果lgSrc为奇数，转换将从目标串的第二个“半字节”开始（即第一字节的低4位）
例：ptSrc = '1AC45'
if lgSrc = 4 and lgDest = 3        ptDest = 0x00/0x1A/0xC4
if lgSrc = 4 and lgDest < lgSrc/2  ptDest = 0x1A/0xC4
if lgSrc = 5 and lgDest = 4        ptDest = 0x00/0x01/0xAC/0x45
if lgSrc = 5 and lgDest < lgSrc/2  ptDest = 0x01/0xAC/0x45

*/
void *asc_hex(uchar *Ptd ,uchar Ld,uchar *Pts,uchar Ls);

/*
作用：将BCD码转换为ASCII码
函数：uchar * bcd_asc (uchar *ptDest, uchar *ptSrc, uchar Lg) ;
等待：是
描述:以1字节为单位，将源字节串的每个“半字节”（Qi）转换成ASCII值 
         for Oi = '0' to '9' (0x30 to 0x39) => 0 to 9
         for Oi = 'A' to 'F' (0x41 to 0x46) => A to F
    转换是将 Lg的源字节串转换为 Lg的半字节到目的串中
    目标串（lgDest）在转换前通常被置为0x00
    如果 Lg为奇数，转换将从目标串的第二个“半字节”开始（即第一字节的低4位）
返回值：返回转换后数组的首地址(ptDest + Lg)
例： ptSrc = 0x12/0x34
if Lg = 1        ptDest = '2'
if Lg = 2        ptDest = '12'
if Lg = 3        ptDest = '234'
if Lg = 4        ptDest = '1234'

*/
uchar *bcd_asc( uchar *Ptd , uchar *Pts , uchar Lg);


/*
作用：转换十六进制数到ASCII码
函数： uchar * hex_asc (uchar *ptDest, uchar *ptSrc, uchar Lg) ;
等待：是
说明: 转换十六进制数到ASCII码
         如果0 to 9 => '0' to '9' (0x30 to 0x39)
         如果 A to F => 'A' to 'F' (0x41 to 0x46) 
返回值：存放转换后数组的首地址。
例： ptSrc = 0x1A/0xC4
if Lg = 1        ptDest = 'A'
if Lg = 2        ptDest = '1A'
if Lg = 3        ptDest = 'AC4'
if Lg = 4        ptDest = '1AC4'

*/
uchar *hex_asc( uchar *Ptd, uchar *Pts, uchar Lg );
/*
作用：转换十六进制数到ASCII码
函数： uchar * hex_str (uchar *ptDest, uchar *ptSrc, uchar Lg) ;
等待：是
说明: 转换十六进制数到ASCII码
         如果0 to 9 => '0' to '9' (0x30 to 0x39)
         如果 A to F => 'A' to 'F' (0x41 to 0x46) 
返回值：存放转换后数组的首地址。
例： ptSrc = 0x1A/0xC4
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
作用：将unsigned int类型数据 转换为 ASCII 码字符数组；
原型: uchar * int_asc (uchar *ptDest, uchar lgDest, ushort *ptSrc);
描述：将unsigned int型数据转换为ASCII码（从0~9）字符串(ASCII value = digit value + 0x30)
目标字符串（lgDest）在转换前总是赋值为0x30 (字符'0') 
unsigned int类型的数据最大值有5个阿拉伯数字，因此转换后的字符串最多是5个字符的阿拉伯字符组成的ASCII 字符串
返回值：返回转换后的字符串首地址；
例如: ptSrc = 12345
if lgDest = 2       ptDest = '45'
if lgDest = 7       ptDest = '0012345'
*/
void *int_asc(uchar *Ptd ,uchar Lgd,uint *Pts);
/*
作用：返回unsigned int 型16进制数据，此数据是一个unsigned char型的字符串
原形: uchar * int_tab (uchar *ptDest, uchar lgDest, ushort *ptSrc);
描述：返回十六进制数据数组，目标字符串（lgDest）在转换前总是赋值为0x00 ；
返回值：返回转换后的字符串首地址。
例如:ptSrc = 12345 = 0x3039
if lgDest = 1       ptDest = 0x39
*/
void *int_tab(uchar *Ptd ,uchar Lgd,uint *Pts);
/*
作用：将unsigned int类型数据,转换为 ASCII 码字符数组； 
原型: uchar * int_bcd (uchar *ptDest, uchar lgDest, ushort *ptSrc); 
停止: 是
描述：将unsigned int类型（字符的10进制）数据转 换 为 对应的BCD 码 字符串,目标字符串（lgDest）在转换前总是赋值为0x00 ； 
      unsigned int类型的数据最大值有5个阿拉伯数字，因此转换后的BCD数据最多是5个“半个字符” 组  成的字符串。 
返回值：返回转换后的字符串首地址。 
例如: ptSrc = 12345 
if lgDest = 2 ptDest = 0x23/0x45 
if lgDest = 5 ptDest = 0x00/0x00/0x01/0x23/0x45 

*/
void *int_bcd(uchar *Ptd ,uchar Lgd,uint *Pts);
/*
作用：将unsigned int类型数据 转换为 ASCII 码字符串； 
原型: uchar * int_str (uchar *ptDest, uchar lgDest, ushort *ptSrc); 
停止: 是
描述：将unsigned int类型（字符的10进制）数据转换为对应的ASCII 码字符串 (ASCII value = digit value + 0x30)； 
      目标字符串（lgDest）在转换前总是赋值为0x30 (字符'0') ； 
      在目标字符串的结尾会增加一个'\0'字符。 
      unsigned int类型的数据最大值有5个阿拉伯数字，因此转换后的字符串最多是5个字符的阿拉伯字符组成的ASCII 字符串。
返回值：返回转换后的字符串首地址，转换后字符串以'\0'字符结束； 
例如ptSrc = 12345 
if lgDest = 2 ptDest = "45" 
if lgDest = 7 ptDest = "0012345" 

*/
void *int_str(uchar *Ptd ,uchar Lgd,uint *Pts);
/*
作用: 把一个非负CHAR型数转化成ASCII型字符串
原型: uchar * char_asc (uchar *ptDest, uchar lgDest, uchar *ptSrc);
停止: 是
描述：将unsigned  char 类型（字符的10进制）数据转换为对应的ASCII 码字符串 (ASCII value = digit value + 0x30)；
      目标字符串（lgDest）在转换前总是赋值为0x30 (字符'0') ；
      unsigned  char 类型的数据最大值有3个阿拉伯数字，因此转换后的字符串最多是3个字符的阿拉伯字符组成的ASCII 字符串。
返回值：返回转换后的字符串首地址。
例如: ptSrc = 'z' = 0x7A = 122
if lgDest = 2 ptDest = '22'
if lgDest = 5 ptDest = '00122'
*/
void *char_asc( uchar *Ptd , uchar Lgd, uchar *Pts );

/*
作用:把无符号的数据转换为对应的ASCII 码字符串
原形: uchar * char_str (uchar *ptDest, uchar lgDest, uchar *ptSrc);
等待:是
描述:将unsigned  char 类型（字符的10进制）数据转换为对应的ASCII 码字符串 (ASCII value = digit value + 0x30)；
     目标字符串（lgDest）在转换前总是赋值为0x30 (字符'0') ；
     在目标字符串的结尾会增加一个'\0' 字符。
     unsigned  char 类型的数据最大值有3个阿拉伯数字，因此转换后的字符串最多是3个字符的阿拉伯字符组成的ASCII 字符串。
返回值：返回转换后的字符串首地址，转换后字符串以'\0' 字符结束；
例如: ptSrc = 'z' = 0x7A = 122
if lgDest = 2 ptDest = "22"
if lgDest = 5 ptDest = "00122"
*/
void *char_str( uchar *Ptd , uchar Lgd, uchar *Pts );

/*
作用: 把一个非负CHAR型数转化成BCD型字符串
原型: uchar * char_bcd (uchar *ptDest, uchar lgDest, uchar *ptSrc);
描述：将unsigned  char 类型（字符的10进制）数据转换为对应的BCD 码字符串，
     目标字符串（lgDest）在转换前总是赋值为0x00；     
     unsigned  char 类型的数据最大值有3个阿拉伯数字，因此转换后的BCD数据最多是3个“半个字符”组成的字符串。
返回值：返回转换后的字符串首地址。
例如: ptSrc = 'z' = 0x7A = 122
if lgDest = 1 ptDest = 0x22
if lgDest = 4 ptDest = 0x00/0x00/0x01/0x22
*/
void *char_bcd(uchar *Ptd ,uchar Lgd,uchar *Pts);
/*
作用:将ASCII码字符串转化到无符号的长整形数据中去(32位)
原型: ulong str_long (uchar *ptSrc) ;
停止:是
描述：该函数和'asc_long'函数类似，lgSrc = strlen(ptSrc)。
      它将截断每个字符的低4位，同时转换为unsigned long型对应的数据；
注意：该函数目前使用的范围是：数据 < 1000000000;
返回值：转换后的数据。
例如: ptSrc = "123" => ptDest = 123
*/
ulong str_long( uchar *Pts );

/*
作用:将ASCII码转换到BCD值中
描述：此函数和 'asc_bcd'函数类似，lgSrc = strlen(ptSrc)。
      他将截断每个ASCII 码的低4位为对应目标字符串的“半个字节”\；
     如果 lgDest > strlen(ptSrc)/2 目标字符串前补0x00;
      如果 原字符串的长度是奇数，则转换将从第二个字符的“半个字节”开始
返回值：返回转换后的字符串首地址。
例如: ptSrc = "12345" = 0x31/0x32/0x33/0x34/0x35/'\0'
(thus lgSrc = strlen(ptSrc) = 5)
if lgDest = 4 (> strlen(ptSrc)/2) ptDest = 0x00/0x01/0x23/0x45
if lgDest < strlen(ptSrc)/2 ptDest = 0x(?)1/0x23/0x45
the (?) represent the old 'half byte' value which is not modified

*/
void *str_bcd(uchar *Ptd ,uchar Lgd,uchar *Pts);
/*
原型: uchar * str_hex (uchar *ptDest, uchar lgDest, uchar *ptSrc) ;

描述:此函数和 'asc_bcd'函数类似，lgSrc = strlen(ptSrc)。

     他将截断每个ASCII 码的低4位为对应目标字符串的“半个字节”；

     如果 Oi = '0' to '9'      (0x30 to 0x39) => 0 to 9

     如果 Oi = 'A' to 'F' (0x41 to 0x46) => A to F

     目标字符串（lgDest）在转换前总是赋值为0x00 ；

     如果 原字符串的长度是奇数，则转换将从第二个字符的“半个字节”开始

返回值：返回转换后的字符串首地址

例如: ptSrc = '1AC45'
(thus lgSrc = strlen(ptSrc) = 5)

if lgDest = 4 (> strlen(ptSrc)/2) ptDest = 0x00/0x01/0xAC/0x45

if lgDest < strlen(ptSrc)/2 ptDest = 0x01/0xAC/0x45.

*/
void *str_hex(uchar *Ptd ,uchar Lgd,uchar *Pts);

/*
原型: uchar * bcd_str (uchar *ptDest, uchar *ptSrc, uchar Lg) ;
停止:是
描述：将字符串每“半个字节”转换为一个字节的ASCII码（从0x30 到0x39），并且在所得字符串的结尾以'\0'字符结束；
      这个过程是将Lg个“半个字符”转换为Lg个字节长度的字符串。
      如果Lg是奇数，转换将会从指定的第二个“半个字符”开始（从第一个字节的低4位开始）。
返回值：返回转换后的字符串首地址，转换后字符串以 '\0'字符结束；
例如: ptSrc = 0x12/0x34

if Lg = 1 ptDest = "2"

if Lg = 2 ptDest = "12"

if Lg = 3 ptDest = "234"

if Lg = 4 ptDest = "1234"

*/
uchar *bcd_str( uchar *Ptd , uchar *Pts , uchar Lg );
  /**
 * 将字符串中的小写字母转换为大写字母
 * @param eopczStr
 * @since 2009-7-10
 */
void Upper( char *eopczStr );
  /**
 * 将字符串终端大写字母转换为小写字母
 * @param eopczStr
 * @since 2009-7-10
 */
void Lower( char *eopczStr );


void des(uchar *binput, uchar *boutput, uchar *bkey);
void desm1(uchar *binput, uchar *boutput, uchar *bkey);
void doubleDecrypt(uchar *in, uchar *out, uchar *key);
void macgen(uchar *in,uint len,uchar *key,char *masterkey, uchar *out);


#endif
