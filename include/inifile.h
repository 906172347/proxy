#ifndef _INIFILE_H
#define _INIFILE_H

/***************************************
������ReadLine
���������
	int iFd,�ļ�������
���������
	char *psLine,������
	int *iLineLen,�г���
	int *iOffset���н���ƫ����
���ܣ���ȡ�ļ�ĳһ��
����ֵ��
	0:�ļ�����
	-0:ʧ��
	1:�ɹ�
*****************************************/
int ReadLine(int iFd,char *psLine,int *iLineLen,int *iOffset);

/***************************************
������UpdateLine
���������

	int iOffset,����ʼƫ����
	char *psLine,������
	char *psFileName�ļ���
���ܣ������ļ�ָ����
����ֵ��
	<0:ʧ��
	=0:�ɹ�
*****************************************/

int UpdateLine(int iOffSet,char *psLine,char *psFileName);
/***************************************
������ReadEntry
���������
	int iFd,�ļ�������
���������
	char *psEntry,������
	char *psValue,����ֵ
	int *iOffset���н���ƫ����
���ܣ���ȡ�����ļ��ƶ�������ֵ
����ֵ��
	n=FILE_EOF�ļ�����
	<0:ʧ��
	>0:����ʼƫ����
*****************************************/
int ReadEntry(int iFd,char *psEntry,char *psValue,int *piOffSet);
/***************************************
������ReadString
���������
	char *psFileName,�����ļ���
	char *psSection,��������
	char *psEntry,��������
���������
	char *psValue����ֵ
���ܣ���ȡ�����ļ��ƶ�������ֵ
����ֵ��
	n=FILE_EOF�ļ�����
	<0:ʧ��
	>0:����ʼƫ����
*****************************************/
int ReadString(char *psFileName,char *psSection,char *psEntry,char *psValue);

/***************************************
������ReadInteger
���������
	char *psFileName,�����ļ���
	char *psSection,��������
	char *psEntry,��������
���������
	char *psValue����ֵ
���ܣ���ȡ�����ļ��ƶ�������ֵ
����ֵ��
	n=FILE_EOF�ļ�����
	<0:ʧ��
	>0:����ʼƫ����
*****************************************/
int ReadInteger(char *psFileName,char *psSection,char *psEntry,int *piValue);
/***************************************
������WriteString
���������
	char *psFileName,�����ļ���
	char *psSection,��������
	char *psEntry,��������
	char *psValue����ֵ

���ܣ����������ļ��ƶ�������ֵ
����ֵ��
	<0:ʧ��
	=0:�ɹ�
*****************************************/
int WriteString(char *psFileName,char *psSection,char *psEntry,char *psValue);
#endif
