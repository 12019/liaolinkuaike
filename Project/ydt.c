/******************************************************************************
Copyright 2005 �����״�ͨ�������޹�˾
All rights riserved.

�ļ���	��ydt.c
ģ�����ƣ�
���ܸ�Ҫ��

ȡ���汾��0.0.1
�޸���	������
������ڣ�2005.04.06
����˵����create

******************************************************************************/
#include "main.h"


#include "Common_exp.h"

extern DEV_STAT DevStat;

extern char dbuf[];

extern INT8U Com1SndBuf[UART1_SEND_QUEUE_LENGTH];              //���ڷ��ͻ�����
extern INT8U Com1RcvBuf[UART1_RCV_QUEUE_LENGTH];               //���ڽ��ջ�����
extern INT8U Com1RcvBuf2[UART1_RCV_QUEUE_LENGTH];               //���ڽ��ջ�����

extern DevHandle hRS232Handle;

/*****************************************************************
����ԭ�ͣ�Get_Time
������������ȡʱ�䣬��ȡ���Σ�Ϊ�˷�ֹż��һ�β��ɹ�
����������
�������ƣ�	����/�����	����		����
*time_data	���   _TIME	7���ֽڳ������ݴ�ͷ��β˳�������� �� �� �� ʱ �� ��
��  ��  ֵ��ok(0)-�ɹ� 
		notok(0xFF)��ʧ��
��      �ߣ�����
��      �ڣ�2005-04-12
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
INT8U Get_Time(BUS_TIME *time_data)
{
	ET_DATETIME time1;

	(void)EA_ucGetDateTime(&time1);

	time_data->century = 0x20;
	time_data->year = hextobcd((INT8U)(time1.usYear % 2000));
	time_data->month = hextobcd(time1.ucMonth);
	time_data->day = hextobcd(time1.ucDay);
	time_data->hour = hextobcd(time1.ucHour);
	time_data->minute = hextobcd(time1.ucMinute);
	time_data->second = hextobcd(time1.ucSecond);

	return ok;
}

/*****************************************************************
����ԭ�ͣ�Modify_Time
�����������޸�ʱ�䣬�޸����Σ�Ϊ�˷�ֹż��һ�β��ɹ�
����������
�������ƣ�	����/�����	����		����
*time_data	����	_TIME	7���ֽڳ������ݴ�ͷ��β˳�������� �� �� �� ʱ �� ��
��  ��  ֵ��ok(0)-�ɹ� 
		notok(0xFF)��ʧ��
��      ��	������
��      �ڣ�2005-04-12
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
INT8U Modify_Time(BUS_TIME const * const time_data)
{
	ET_DATETIME time1;

	time1.usYear = 2000 + (INT16U)bcdtohex(time_data->year);
	time1.ucMonth = bcdtohex(time_data->month);
	time1.ucDay = bcdtohex(time_data->day);
	time1.ucHour = bcdtohex(time_data->hour);
	time1.ucMinute = bcdtohex(time_data->minute);
	time1.ucSecond = bcdtohex(time_data->second);

	EA_ucSetDateTime(&time1, EM_SYSTEM_DATE | EM_SYSTEM_TIME);

	return ok;
}

/*****************************************************************
����ԭ�ͣ�CheckTimeFormat
�������������ʱ���ʽ�Ƿ�Ϸ�
����������
�������ƣ�	����/�����	����		����
time		����  			const _TIME *	7���ֽڳ������ݴ�ͷ��β˳�������� �� �� �� ʱ �� ��

��  ��  ֵ��ok(0)-�ɹ� 
		notok(0xFF)��ʧ��
��      ��	������
��      �ڣ�2003-05-12
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
INT8U CheckTimeFormat(const BUS_TIME *time)
{
	INT8U temp;

	temp = time->month;
	if ( temp >= 0x0a )       //�·�Ӧ��1��12֮��
	{
		if ( temp >= 0x10 && temp <= 0x12 )
		{
			;
		}
		else
		{
			return notok;
		}
	}

	temp = time->day;
	if ( temp > 0x31 )                   //��Ӧ��1-31֮��
		return notok;
	temp = temp & 0x0F;
	if ( temp > 0x09 )
		return notok;

	temp = time->hour;
	if ( temp > 0x23 )                                   //СʱӦ��0-23֮��
		return notok;
	temp = temp & 0x0F;
	if ( temp > 0x09 )
		return notok;

	temp = time->minute;
	if ( temp > 0x59 )                                   //����Ӧ��0-59֮��
		return notok;
	temp = temp & 0x0F;
	if ( temp > 0x09 )
		return notok;

	temp = time->second;
	if ( temp > 0x59 )                                    //��Ӧ��0-59֮��
		return notok;
	temp = temp & 0x0F;
	if ( temp > 0x09 )
		return notok;

	return ok;
}

/******************************************************************************
 �������ƣ�ascii_to_hex
 ������������һ��0-9��ascii���룬ת��Ϊ16������
 �������ƣ� ����/����� ����        ����
 num        ����        INT8U           Ҫת��Ϊascii���������Ҫ��Χ0-9 a-f A-F

 ��  ��  ֵ��ת�����16������

 ��      �� ������
 ��      �ڣ�2004-09-02
 �޸���ʷ��
	����        �޸���      �޸�����
	------      ---------   -------------
******************************************************************************/
INT8U ascii_to_hex(INT8U num)
{
	//  ToggleWD();

	// 	return (num - 0x30);
	if ( num <= '9' )
		return ( num - 0x30 );
	else if ( num == 'a' || num == 'A' )
		return 0x0A;
	else if ( num == 'b' || num == 'B' )
		return 0x0B;
	else if ( num == 'c' || num == 'C' )
		return 0x0C;
	else if ( num == 'd' || num == 'D' )
		return 0x0D;
	else if ( num == 'e' || num == 'E' )
		return 0x0E;
	else
		return 0x0F;
}

/*****************************************************************
����ԭ�ͣ�bcdtohex
����������bcd��ת��Ϊhex��
����������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------
bcd				����			INT8U		�����bcd��

��  ��  ֵ��ת�����hex��
��      ��	������
��      �ڣ�2006-01-09
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
INT8U bcdtohex(INT8U bcd)
{
	//  ToggleWD();

	return ( (bcd >> 4) * 10 + (bcd & 0x0F) );
}

/******************************************************************************
 �������ƣ�hextobcd
 ������������һ��16��������ת��Ϊbcd��
 �������ƣ� ����/����� ����        ����
 num        ����        INT8U           Ҫת��Ϊbcd�����

 ��  ��  ֵ��ת�����bcd��

 ��      �� ������
 ��      �ڣ�2004-09-02
 �޸���ʷ��
	����        �޸���      �޸�����
	------      ---------   -------------
******************************************************************************/
INT8U hextobcd(INT8U num)
{
	return ( ((num / 10) << 4) | (num % 10) );
}

/******************************************************************************
 �������ƣ�ascii
 ������������һ��0-9����ת��Ϊascii���룬�˺���Ҳ����չ����ͨѶ�У�ͨѶ���漰��һЩA-F�ķǷ����룬��Ϊ�Ϸ�
 �������ƣ�	����/�����	����		����
 num		����		INT8U			Ҫת��Ϊascii���������Ҫ��Χ0-9
 
 ��  ��  ֵ��ת�����ascii����
 
 ��      ��	������
 ��      �ڣ�2004-09-02
 �޸���ʷ��
	����		�޸���		�޸�����
	------		---------	-------------
******************************************************************************/
INT8U ascii(INT8U num)
{
	//  ToggleWD();

	//     return (num + 0x30);
	if ( num <= 9 )
		return ( num + 0x30 );
	else if ( num == 0x0A )
		return 'A';
	else if ( num == 0x0B )
		return 'B';
	else if ( num == 0x0C )
		return 'C';
	else if ( num == 0x0D )
		return 'D';
	else if ( num == 0x0E )
		return 'E';
	else
		//      if (num == 0x0F)
		return 'F';
}

// *****************************************************************
// ���ܣ�����Ļ������ַ�
// ��ڲ�����	lb->��ʾ����ַ�
// ���ڲ�����	Null
// ���ߣ�
// ����������EH0218---COMMON.C���ӳ���
// *****************************************************************
char HextoBcd(char sour)
{
	//  ToggleWD();

	return ( ((sour / 10) << 4) | (sour % 10) );
}

/******************************************************************************
 �������ƣ�htoa
 ����������hexת��Ϊascii��
 ����������
 �������ƣ� ����/����� ����		����

 ��  ��  ֵ����

 �� 	 �� ������
 �� 	 �ڣ�2005-04-07
 �޸���ʷ��
	 ����		�޸���		�޸�����
	 ------		---------	-------------
******************************************************************************/
void htoa(void *des, INT8U *sour, int hex_len)
{
	int i = 0;
	char *dest = (char *)des;

	if ( hex_len <= 0 )
		return;

	for ( i = 0; i < hex_len; i++ )
		sprintf((char *)dest + i * 2, "%02x", sour[i]);

	dest[hex_len * 2] = '\0';
}

/******************************************************************************
 �������ƣ�atoh
 ����������asciiת��Ϊhex��
 ����������
 �������ƣ� ����/����� ����		����
		
 ��  ��  ֵ����
		   
 �� 	 �� ������
 �� 	 �ڣ�2005-04-07
 �޸���ʷ��
	����		�޸���		�޸�����
	------		---------	-------------
******************************************************************************/
void atoh(void *des, INT8U *sour, int ascii_len)
{
	int i = 0;
	char *dest = (char *)des;

	//  ToggleWD();

	if ( ascii_len <= 0 )
		return;

	for ( i = 0; i < ascii_len; i++ )
	{
		if ( i % 2 == 0 )
			continue;

		dest[i / 2] = (ascii_to_hex(sour[i - 1]) << 4) | ascii_to_hex(sour[i]);
	}
}

/******************************************************************************
 �������ƣ�ascii_comm
 ������������һ��0-9����ת��Ϊascii���룬�˺���Ҳ����չ����ͨѶ�У�ͨѶ���漰��һЩA-F�ķǷ����룬��Ϊ�Ϸ�
 �������ƣ�	����/�����	����		����
 num		����		INT8U			Ҫת��Ϊascii���������Ҫ��Χ0-9
 
 ��  ��  ֵ��ת�����ascii����
 
 ��      ��	������
 ��      �ڣ�2004-09-02
 �޸���ʷ��
	����		�޸���		�޸�����
	------		---------	-------------
******************************************************************************/
INT8U ascii_comm(INT8U num)
{
	return ( num + 0x30 );
}

/******************************************************************************
 �������ƣ�SleepMs
 ������������ʱ������11.0592M*4����ר�ã���mem����
 �������ƣ�	����/�����	����		����
 _ms	����	INT16U				Ҫ��ʱ�ı��� 1ms*?
 
 ��  ��  ֵ����
 
 ��      �ߣ�����
 ��      �ڣ�2004-09-02
 �޸���ʷ��
	����		�޸���		�޸�����
	------		---------	-------------
******************************************************************************/
void SleepMs(INT16U _ms)
{
	EA_vDelayMs(_ms);
}

/******************************************************************************
 �������ƣ�lcddisp
 ������������ʾ����
 �������ƣ�	����/�����	����		����
 line		����		INT16U		�� 1~4
 col		����		INT16U		�� 1~20
 str		����		INT8U *		Ҫ��ʾ������
 
 ��  ��  ֵ����
 
 ��      �ߣ�����
 ��      �ڣ�2012-10-29
 �޸���ʷ��
	����		�޸���		�޸�����
	------		---------	-------------
******************************************************************************/
void lcddisp(INT16U line, INT16U col, const INT8U *str)
{

	EA_vDisp((int)line, (int)col, (char *)str);
}

/******************************************************************************
 �������ƣ�browse_menu
 �����������˵�����
 �������ƣ�	����/�����	����		����
 startline	����		INT8U		�˵���ʼ��ʾ�� 1~4
 title		����		INT8U *		�˵���ʾ, ������8������, �ڵ�һ����ʾ
 p_menu		����		INT8U *		�˵���ÿ��10�����֣��������ո�  
 allow_exit	����		INT8U		�Ƿ�����ȡ�����˳�TRUE=���� FALSE=������ 
 
 ��  ��  ֵ��ѡ��Ĳ˵���  0Ϊ��һ��
		-1 = ȡ���� 
 
 ��      �ߣ�����
 ��      �ڣ�2012-10-29
 �޸���ʷ��
	����		�޸���		�޸�����
	------		---------	-------------
******************************************************************************/
INT8S browse_menu(INT8U startline, INT8U *title, INT8U *p_menu, INT8U allow_exit)
{
	INT16U len = 0;
	INT16U menu_lines = 0;
	INT16U i = 0;
#define MAX_MENU_LINES 100
	INT8U menu_text[MAX_MENU_LINES][22];
	//  INT16U j = 0;
	INT16U key = EM_key_NOHIT;
	struct _MENU_PARAM
	{
		INT16S disp_pos;
		INT16S menu_pos;
	}lmenu;
	INT16S h = 0;
	INT16U disp_pos_max = 0;
	INT16S m = 0;
	//  INT8S buf[30];


	startline = startline;              //avoid warning

	EA_vCls();
	EA_ucSetInverse(EM_lcd_INVOFF);

	//��ʾtitle
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, (char *)title);

	//�˵����ֳ�ʼ��
	len = strlen((char *)p_menu);
	menu_lines = len / 20;
	if ( menu_lines > MAX_MENU_LINES )
		menu_lines = MAX_MENU_LINES;
	//  if ( menu_lines <= 1 )
	//  	return 0;
	if ( menu_lines == 0 )
		return -1;
	memset((void *)&menu_text[0][0], 0x00, sizeof(menu_text));
	for ( i = 0; i < menu_lines; i++ )
	{
		memcpy((void *)&menu_text[i][0], (void *)&p_menu[i * 20], 20);
	}


	if ( menu_lines < 4 )
		disp_pos_max = menu_lines - 1;
	else
		disp_pos_max = 3;
	//  EA_ucSetInverse(EM_lcd_INVON);
	//  lcddisp(1, 1, &menu_text[0][0]);
	//  EA_ucSetInverse(EM_lcd_INVOFF);
	//  for ( i = 1; i < disp_pos_max; i++ )
	//  {
	//  	lcddisp(i + 1, 1, &menu_text[i][0]);
	//  }

	lmenu.disp_pos = 0;
	lmenu.menu_pos = 0;

	EA_vCls();   //�����Ļ
	EA_ucSetInverse(EM_lcd_INVON);
	lcddisp(lmenu.disp_pos + 1, 1, &menu_text[lmenu.menu_pos][0]);       //��ʾ��ǰ�˵�������
	EA_ucSetInverse(EM_lcd_INVOFF);

	m = 1;
	for ( i = lmenu.disp_pos + 1; i <= 3; i++ )
	{
		lcddisp(i + 1, 1, &menu_text[lmenu.menu_pos + m][0]);
		m++;
	}
	m = 1;
	for ( h = lmenu.disp_pos - 1; h >= 0; h-- )
	{
		lcddisp(h + 1, 1, &menu_text[lmenu.menu_pos - m][0]);
		m++;
	}


	for (;;)
	{
		key = EA_uiInkey(0);
		switch ( key )
		{
		   case EM_key_INTERPUNCTION:           //��
			   lmenu.disp_pos--;
			   if ( lmenu.disp_pos < 0 )
				   lmenu.disp_pos = 0;
			   lmenu.menu_pos--;
			   if ( lmenu.menu_pos < 0 )
				   lmenu.menu_pos = 0;


			   EA_vCls();   //�����Ļ
			   EA_ucSetInverse(EM_lcd_INVON);
			   lcddisp(lmenu.disp_pos + 1, 1, &menu_text[lmenu.menu_pos][0]);       //��ʾ��ǰ�˵�������
			   EA_ucSetInverse(EM_lcd_INVOFF);

			   m = 1;
			   for ( i = lmenu.disp_pos + 1; i <= 3; i++ )
			   {
				   lcddisp(i + 1, 1, &menu_text[lmenu.menu_pos + m][0]);
				   m++;
			   }
			   m = 1;
			   for ( h = lmenu.disp_pos - 1; h >= 0; h-- )
			   {
				   lcddisp(h + 1, 1, &menu_text[lmenu.menu_pos - m][0]);
				   m++;
			   }

			   break;

		   case EM_key_LETTER:                  //��
			   lmenu.disp_pos++;
			   if ( lmenu.disp_pos > disp_pos_max )
				   lmenu.disp_pos = disp_pos_max;
			   lmenu.menu_pos++;
			   if ( lmenu.menu_pos > menu_lines - 1 )
				   lmenu.menu_pos = menu_lines - 1;


			   EA_vCls();   //�����Ļ
			   EA_ucSetInverse(EM_lcd_INVON);
			   lcddisp(lmenu.disp_pos + 1, 1, &menu_text[lmenu.menu_pos][0]);       //��ʾ��ǰ�˵�������
			   EA_ucSetInverse(EM_lcd_INVOFF);

			   m = 1;
			   for ( i = lmenu.disp_pos + 1; i <= 3; i++ )
			   {
				   lcddisp(i + 1, 1, &menu_text[lmenu.menu_pos + m][0]);
				   m++;
			   }
			   m = 1;
			   for ( h = lmenu.disp_pos - 1; h >= 0; h-- )
			   {
				   lcddisp(h + 1, 1, &menu_text[lmenu.menu_pos - m][0]);
				   m++;
			   }
			   //  		   for ( h = lmenu.disp_pos - 1; h >= 0; h-- )
			   //  		   {
			   //  			   lcddisp(h + 1, 1, &menu_text[lmenu.menu_pos - (lmenu.disp_pos - h)][0]);
			   //  		   }


			   break;

		   case EM_key_ENTER:                   //ȷ��
			   return lmenu.menu_pos;
			   //  		   //debug
			   //  		   sprintf((void *)buf, "%d", lmenu.menu_pos + 1);
			   //  		   EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, (void *)buf);
			   //  		   break;
			   //  		   //debug end


		   case EM_key_1:
			   if ( menu_lines >= 1 )
				   return 0;
			   else
				   break;

		   case EM_key_2:
			   if ( menu_lines >= 2 )
				   return 1;
			   else
				   break;

		   case EM_key_3:
			   if ( menu_lines >= 3 )
				   return 2;
			   else
				   break;

		   case EM_key_4:
			   if ( menu_lines >= 4 )
				   return 3;
			   else
				   break;

		   case EM_key_5:
			   if ( menu_lines >= 5 )
				   return 4;
			   else
				   break;

		   case EM_key_6:
			   if ( menu_lines >= 6 )
				   return 5;
			   else
				   break;

		   case EM_key_7:
			   if ( menu_lines >= 7 )
				   return 6;
			   else
				   break;

		   case EM_key_8:
			   if ( menu_lines >= 8 )
				   return 7;
			   else
				   break;

		   case EM_key_9:
			   if ( menu_lines >= 9 )
				   return 8;
			   else
				   break;


		   case EM_key_CANCEL:
			   if ( allow_exit == TRUE )
				   return -1;
			   else
				   break;

		   default:
			   break;
		}

	}




	return 0;
}

/******************************************************************************
 �������ƣ�browse_menu2
 �����������˵�����,����ʼĬ���е�
 �������ƣ�	����/�����	����		����
 startline	����		INT16U		�˵���ʼ��ʾ�� 1~4
 title		����		INT8U *		�˵���ʾ, ������8������, �ڵ�һ����ʾ
 p_menu		����		INT8U *		�˵���ÿ��10�����֣��������ո�  
 allow_exit	����		INT8U		�Ƿ�����ȡ�����˳�TRUE=���� FALSE=������ 
 default_line����		INT16U		��ʼĬ����
 
 ��  ��  ֵ��ѡ��Ĳ˵���  0Ϊ��һ��
		-1 = ȡ���� 
 
 ��      �ߣ�����
 ��      �ڣ�2012-10-29
 �޸���ʷ��
	����		�޸���		�޸�����
	------		---------	-------------
******************************************************************************/
INT16S browse_menu2(INT16U startline, INT8U *title, INT8U *p_menu, INT8U allow_exit, INT16U default_line)
{
	INT16U len = 0;
	INT16U menu_lines = 0;
	INT16U i = 0;
#define MAX_MENU_LINES 100
	INT8U menu_text[MAX_MENU_LINES][22];
	//  INT16U j = 0;
	INT16U key = EM_key_NOHIT;
	struct _MENU_PARAM
	{
		INT16S disp_pos;
		INT16S menu_pos;
	}lmenu;
	INT16S h = 0;
	INT16U disp_pos_max = 0;
	INT16S m = 0;
	//  INT8S buf[30];


	startline = startline;              //avoid warning

	EA_vCls();
	EA_ucSetInverse(EM_lcd_INVOFF);

	//��ʾtitle
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, (char *)title);

	//�˵����ֳ�ʼ��
	len = strlen((char *)p_menu);
	menu_lines = len / 20;
	if ( menu_lines > MAX_MENU_LINES )
		menu_lines = MAX_MENU_LINES;
	if ( menu_lines == 0 )
		return -1;
	memset((void *)&menu_text[0][0], 0x00, sizeof(menu_text));
	for ( i = 0; i < menu_lines; i++ )
	{
		memcpy((void *)&menu_text[i][0], (void *)&p_menu[i * 20], 20);
	}


	if ( menu_lines < 4 )
		disp_pos_max = menu_lines - 1;
	else
		disp_pos_max = 3;
	//  EA_ucSetInverse(EM_lcd_INVON);
	//  lcddisp(1, 1, &menu_text[0][0]);
	//  EA_ucSetInverse(EM_lcd_INVOFF);
	//  for ( i = 1; i < disp_pos_max; i++ )
	//  {
	//  	lcddisp(i + 1, 1, &menu_text[i][0]);
	//  }

	lmenu.disp_pos = 0;
	lmenu.menu_pos = 0;

	//  EA_vCls();   //�����Ļ
	//  EA_ucSetInverse(EM_lcd_INVON);
	//  lcddisp(lmenu.disp_pos + 1, 1, &menu_text[lmenu.menu_pos][0]);       //��ʾ��ǰ�˵�������
	//  EA_ucSetInverse(EM_lcd_INVOFF);
	//
	//  m = 1;
	//  for ( i = lmenu.disp_pos + 1; i <= 3; i++ )
	//  {
	//  	lcddisp(i + 1, 1, &menu_text[lmenu.menu_pos + m][0]);
	//  	m++;
	//  }
	//  m = 1;
	//  for ( h = lmenu.disp_pos - 1; h >= 0; h-- )
	//  {
	//  	lcddisp(h + 1, 1, &menu_text[lmenu.menu_pos - m][0]);
	//  	m++;
	//  }


	//������ʼλ��
	if ( default_line > menu_lines - 1 )
		default_line = menu_lines - 1;

	lmenu.disp_pos = 0;
	lmenu.menu_pos = 0;

	for ( i = 0; i < default_line; i++ )
	{
		lmenu.disp_pos++;
		if ( lmenu.disp_pos > disp_pos_max )
			lmenu.disp_pos = disp_pos_max;
		lmenu.menu_pos++;
		if ( lmenu.menu_pos > menu_lines - 1 )
			lmenu.menu_pos = menu_lines - 1;
	}

	EA_vCls();   //�����Ļ
	EA_ucSetInverse(EM_lcd_INVON);
	lcddisp(lmenu.disp_pos + 1, 1, &menu_text[lmenu.menu_pos][0]);       //��ʾ��ǰ�˵�������
	EA_ucSetInverse(EM_lcd_INVOFF);

	m = 1;
	for ( i = lmenu.disp_pos + 1; i <= 3; i++ )
	{
		lcddisp(i + 1, 1, &menu_text[lmenu.menu_pos + m][0]);
		m++;
	}
	m = 1;
	for ( h = lmenu.disp_pos - 1; h >= 0; h-- )
	{
		lcddisp(h + 1, 1, &menu_text[lmenu.menu_pos - m][0]);
		m++;
	}



	for (;;)
	{
		key = EA_uiInkey(0);
		switch ( key )
		{
		   case EM_key_INTERPUNCTION:           //��
			   lmenu.disp_pos--;
			   if ( lmenu.disp_pos < 0 )
				   lmenu.disp_pos = 0;
			   lmenu.menu_pos--;
			   if ( lmenu.menu_pos < 0 )
				   lmenu.menu_pos = 0;


			   EA_vCls();   //�����Ļ
			   EA_ucSetInverse(EM_lcd_INVON);
			   lcddisp(lmenu.disp_pos + 1, 1, &menu_text[lmenu.menu_pos][0]);       //��ʾ��ǰ�˵�������
			   EA_ucSetInverse(EM_lcd_INVOFF);

			   m = 1;
			   for ( i = lmenu.disp_pos + 1; i <= 3; i++ )
			   {
				   lcddisp(i + 1, 1, &menu_text[lmenu.menu_pos + m][0]);
				   m++;
			   }
			   m = 1;
			   for ( h = lmenu.disp_pos - 1; h >= 0; h-- )
			   {
				   lcddisp(h + 1, 1, &menu_text[lmenu.menu_pos - m][0]);
				   m++;
			   }

			   break;

		   case EM_key_LETTER:                  //��
			   lmenu.disp_pos++;
			   if ( lmenu.disp_pos > disp_pos_max )
				   lmenu.disp_pos = disp_pos_max;
			   lmenu.menu_pos++;
			   if ( lmenu.menu_pos > menu_lines - 1 )
				   lmenu.menu_pos = menu_lines - 1;


			   EA_vCls();   //�����Ļ
			   EA_ucSetInverse(EM_lcd_INVON);
			   lcddisp(lmenu.disp_pos + 1, 1, &menu_text[lmenu.menu_pos][0]);       //��ʾ��ǰ�˵�������
			   EA_ucSetInverse(EM_lcd_INVOFF);

			   m = 1;
			   for ( i = lmenu.disp_pos + 1; i <= 3; i++ )
			   {
				   lcddisp(i + 1, 1, &menu_text[lmenu.menu_pos + m][0]);
				   m++;
			   }
			   m = 1;
			   for ( h = lmenu.disp_pos - 1; h >= 0; h-- )
			   {
				   lcddisp(h + 1, 1, &menu_text[lmenu.menu_pos - m][0]);
				   m++;
			   }

			   break;

		   case EM_key_ENTER:                   //ȷ��
			   return lmenu.menu_pos;
			   //  		   //debug
			   //  		   sprintf((void *)buf, "%d", lmenu.menu_pos + 1);
			   //  		   EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, (void *)buf);
			   //  		   break;
			   //  		   //debug end


		   case EM_key_1:
			   if ( menu_lines >= 1 )
				   return 0;
			   else
				   break;

		   case EM_key_2:
			   if ( menu_lines >= 2 )
				   return 1;
			   else
				   break;

		   case EM_key_3:
			   if ( menu_lines >= 3 )
				   return 2;
			   else
				   break;

		   case EM_key_4:
			   if ( menu_lines >= 4 )
				   return 3;
			   else
				   break;

		   case EM_key_5:
			   if ( menu_lines >= 5 )
				   return 4;
			   else
				   break;

		   case EM_key_6:
			   if ( menu_lines >= 6 )
				   return 5;
			   else
				   break;

		   case EM_key_7:
			   if ( menu_lines >= 7 )
				   return 6;
			   else
				   break;

		   case EM_key_8:
			   if ( menu_lines >= 8 )
				   return 7;
			   else
				   break;

		   case EM_key_9:
			   if ( menu_lines >= 9 )
				   return 8;
			   else
				   break;


		   case EM_key_CANCEL:
			   if ( allow_exit == TRUE )
				   return -1;
			   else
				   break;

		   default:
			   break;
		}

	}




	return 0;
}

/******************************************************************************
 �������ƣ�lcddisperr
 ����������Һ������ʾ����
 �������ƣ�	����/�����	����		����
 str		����		char *		Ҫ��ʾ������
 
 ��  ��  ֵ����
 
 ��      �ߣ�����
 ��      �ڣ�2012-10-29
 �޸���ʷ��
	����		�޸���		�޸�����
	------		---------	-------------
******************************************************************************/
void lcddisperr(const char *str)
{
	INT16U key = EM_key_NOHIT;

	EA_vCls();
	EA_vDisp(3, 1, (char *)str);
	key = EA_uiInkey(2);
}

/*****************************************************************
 ����ԭ�ͣ�param_factory_default()
 ���������������ָ���������
 ����������	
 �������ƣ�	����/�����	����		����
 level		����		INT8U		��ʼ���ĵȼ�0-����ʼ����¼�ͱ��ݼ�¼�ļ� 1-ֻ��ʼ����¼�ļ� 2-��¼�ͱ��ݼ�¼�ļ�����ʼ��
				 
 ����ֵ��	��
 
 ��  �ߣ�	����
 ��  �ڣ�	2004-09-17
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
*****************************************************************/
void param_factory_default(INT8U level)
{
	const char acModule[] = "ticket_printer";
	const char acFileName[] = "param";
	const char acVersion[] = "0.01";
	INT8U ucRet;
	//  INT32U uiFileSize;
	//  INT32U uiFileNumber;
	//  INT32U uiActNumber;
	//  ET_ParamFileInfo ptInfo;
	const char recFileName[] = "record";
	//  const char recFileName[] = "record";
	INT32U uiRecordLen = sizeof(TICKET_INFO);
	INT8U i = 0;


	//  // ɾ���������ļ�
	//  ucRet = EA_ucDeleteParamFile(acModule, acFileName, EM_param_USING_FILE);
	//  if ( ucRet != EM_SUCCESS )
	//  {
	//  	lcddisperr("ɾ�������ļ�ʧ��!");		// ����ʧ�ܵĴ������
	//  }

	ucRet = EA_ucCreateParamFile(acModule, acFileName, acVersion, EM_param_USING_FILE);
	if ( ucRet != EM_SUCCESS )
	{
		lcddisperr("���������ļ�ʧ��!");       // �����ļ�ʧ�ܵĴ������
	}

	DevStat.format_sign = 0x55AA;
	DevStat.line_amount = 0;
	strcpy((void *)&DevStat.pos_number[0], "000000");
	strcpy((void *)&DevStat.bus_number[0], "12345");
	DevStat.line_ptr = 0;
	strcpy((void *)&DevStat.line_number[0], "");
	strcpy((void *)&DevStat.line_menu_name[0], "");
	strcpy((void *)&DevStat.line_full_name[0], "");
	DevStat.banci = 1;
	DevStat.zhekou = 80;	//��ʼ���ۿ�����80����ʾ8��
	DevStat.cur_ticket_number = 0;

	WriteParam();



	if ( level == 0 )
	{
		return;
	}
	else if ( level == 1 )
	{
		//���´�����¼�ļ�
		i = EA_ucPFCreate((INT8U *)recFileName, uiRecordLen);
		if ( i == EM_ffs_FILEEXIST )          //�ļ��Ѿ�����
		{
			if ( EA_ucPFDel((uchar *)recFileName) != EM_ffs_SUCCESS )
			{
				lcddisperr("ɾ����¼�ļ�ʧ��");
			}
			if ( EA_ucPFCreate((uchar *)recFileName, uiRecordLen) != EM_ffs_SUCCESS )
			{
				lcddisperr("���´�����¼�ļ�ʧ��");
			}
		}
		else if ( i == EM_ffs_FILEFULL )
		{
			lcddisperr("������¼�ļ�����");
		}
		else if ( i == EM_ffs_SUCCESS )
		{
			;           //do nothing
		}
		else
		{
			lcddisperr("������¼�ļ�ʧ��");
		}

		return;
	}
	else if ( level == 2 )
	{
		//���´�����¼�ļ�
		i = EA_ucPFCreate((INT8U *)recFileName, uiRecordLen);
		if ( i == EM_ffs_FILEEXIST )          //�ļ��Ѿ�����
		{
			if ( EA_ucPFDel((uchar *)recFileName) != EM_ffs_SUCCESS )
			{
				lcddisperr("ɾ����¼�ļ�ʧ��");
			}
			if ( EA_ucPFCreate((uchar *)recFileName, uiRecordLen) != EM_ffs_SUCCESS )
			{
				lcddisperr("���´�����¼�ļ�ʧ��");
			}
		}
		else if ( i == EM_ffs_FILEFULL )
		{
			lcddisperr("������¼�ļ�����");
		}
		else if ( i == EM_ffs_SUCCESS )
		{
			;           //do nothing
		}
		else
		{
			lcddisperr("������¼�ļ�ʧ��");
		}


		//���´������ɼ�¼�ļ�
		i = EA_ucPFCreateLoop((INT8U *)"backup", uiRecordLen, RECNUM_OF_BACKUP_FILE);
		if ( i == EM_ffs_FILEEXIST )          //�ļ��Ѿ�����
		{
			if ( EA_ucPFDel((INT8U *)"backup") != EM_ffs_SUCCESS )
			{
				lcddisperr("ɾ�����ɼ�¼�ļ�ʧ��");
			}
			if ( EA_ucPFCreateLoop((INT8U *)"backup", uiRecordLen, RECNUM_OF_BACKUP_FILE) != EM_ffs_SUCCESS )
			{
				lcddisperr("���´��������ļ�ʧ��");
			}
		}
		else if ( i == EM_ffs_FILEFULL )
		{
			lcddisperr("������¼�ļ�����");
		}
		else if ( i == EM_ffs_SUCCESS )
		{
			;           //do nothing
		}
		else
		{
			lcddisperr("�������ɼ�¼�ļ�ʧ��");
		}

	}
}

/******************************************************************************
 �������ƣ�WriteParam
 �����������������
 ����������
 �������ƣ� ����/����� ����        ����

 ��  ��  ֵ����

 ��      �� ������
 ��      �ڣ�2006-08-02
 �޸���ʷ��
		����        �޸���      �޸�����
		------      ---------   -------------
******************************************************************************/
void WriteParam(void)
{
	//  INT8U i = 0;
	//  INT8U j = 0;
	const char acModule[] = "ticket_printer";
	const char acFileName[] = "param";
	//  const char acVersion[] = "0.01";
	INT8U ucRet;


	DevStat.crc = 0;

	DevStat.crc = Cal_Crc((void *)&DevStat.start, &DevStat.end - &DevStat.start - 4);


	// ������ļ�д������
	ucRet = EA_ucWriteParamFile(acModule, acFileName, &DevStat.end - &DevStat.start, (void *)&DevStat.start);
	if ( ucRet != EM_SUCCESS )
	{
		lcddisperr("д�����ļ�ʧ��!"); // д����ʧ�ܵĴ������
	}


}

/******************************************************************************
 �������ƣ�ReadParam
 ������������ȡ����
 ����������
 �������ƣ� ����/����� ����        ����

 ��  ��  ֵ��EA_ucReadParamFile �ķ��� EM_SUCCESS��ʾ�ɹ� ����˵��ʧ��

 ��      �� ������
 ��      �ڣ�2006-08-02
 �޸���ʷ��
		����        �޸���      �޸�����
		------      ---------   -------------
******************************************************************************/
INT8U ReadParam(void)
{
	const char acModule[] = "ticket_printer";
	const char acFileName[] = "param";
	//  const char acVersion[] = "0.01";
	//  INT16U crc = 0;
	INT32U uiFileSize;
	INT8U ucRet;


	// ��ȡ�����ļ�����
	ucRet = EA_ucReadParamFile(acModule, acFileName, &uiFileSize, (void *)&DevStat.start);
	if ( ucRet != EM_SUCCESS )
	{
		sprintf(dbuf, "�������ļ�ʧ��%02X", ucRet);
		lcddisperr(dbuf);
		//  	lcddisperr("�������ļ�ʧ��!");   // ��ȡ�ļ�ʧ�ܵĴ������
	}
	return ucRet;
}

/*****************************************************************
 ����ԭ�ͣ�Cal_Crc
 ��������������һ�����ݵ�CRCֵ������CCITT�㷨
 ����������
 �������ƣ�	����/�����	����		����
 data_to_cal_crc	����	INT8U const *		Ҫ����CRC������������ʼָ��
 len				 		����	INT16U				Ҫ����CRC���������ĳ���

 ��  ��  ֵ��uint�͵�CRCֵ(2�ֽ�)
 ��      ��	������
 ��      �ڣ�2004-02-04
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
*****************************************************************/
INT16U Cal_Crc(INT8U const *data_to_cal_crc, INT16U len)
{
	INT16U crc;
	INT8U i;

	crc = 0xFFFF;           //CRC��ʼֵ

	while ( len-- != 0 )
	{
		crc = crc ^ ((INT16U) * data_to_cal_crc++ << 8);

		for ( i = 0; i < 8; ++i )
			if ( crc & 0x8000 )
				crc = (crc << 1) ^ 0x1021;
			else
				crc	= crc << 1;
	}

	return crc;
}

/******************************************************************************
 �������ƣ�store_record
 ���������������¼
 �������ƣ�	����/�����	����		����
 rec		����		INT8U *		��¼
 
 ��  ��  ֵ����
 
 ��      �ߣ�����
 ��      �ڣ�2012-10-29
 �޸���ʷ��
	����		�޸���		�޸�����
	------		---------	-------------
******************************************************************************/
void store_record(INT8U *rec)
{
	const char recFileName[] = "record";
	//  INT32U uiRecordLen = sizeof(TICKET_INFO);
	//  INT8U i = 0;
	INT8U ucOpenID = 0;

	// ���ļ�
	if ( EA_ucPFOpen((uchar *)recFileName, &ucOpenID) != EM_ffs_SUCCESS )
	{
		lcddisperr("�򿪼�¼�ļ�ʧ��");
		return;
	}


	if ( EA_ucPFWriteRec(ucOpenID, 0, rec) != EM_ffs_SUCCESS )
	{
		lcddisperr("��Ӽ�¼ʧ��");
	}

	if ( EA_ucPFClose(ucOpenID) != EM_ffs_SUCCESS )
	{
		lcddisperr("�رռ�¼�ļ�ʧ��");
		return;
	}


}

///******************************************************************************
// �������ƣ�store_record
// ���������������¼
// �������ƣ� ����/����� ����        ����
// rec        ����        INT8U *     ��¼
//
// ��  ��  ֵ����
//
// ��      �ߣ�����
// ��      �ڣ�2012-10-29
// �޸���ʷ��
//    ����        �޸���      �޸�����
//    ------      ---------   -------------
//******************************************************************************/
//void get_record_amount(void)
//{
//    const char recFileName[] = "record";
////  INT32U uiRecordLen = sizeof(TICKET_INFO);
////  INT8U i = 0;
//    INT8U ucOpenID = 0;
//
//    // ���ļ�
//    if ( EA_ucPFOpen((uchar *)recFileName, &ucOpenID) != EM_ffs_SUCCESS )
//    {
//        lcddisperr("�򿪼�¼�ļ�ʧ��");
//        return;
//    }
//
//
//    if ( EA_ucPFWriteRec(ucOpenID, 0, rec) != EM_ffs_SUCCESS )
//    {
//        lcddisperr("��Ӽ�¼ʧ��");
//    }
//
//    if ( EA_ucPFClose(ucOpenID) != EM_ffs_SUCCESS )
//    {
//        lcddisperr("�رռ�¼�ļ�ʧ��");
//        return;
//    }
//
//
//}

/******************************************************************************
 �������ƣ�browse_menu_time
 �����������˵�����������ʾʱ��ģ����˵�ר��
 �������ƣ�	����/�����	����		����
 startline	����		INT8U		�˵���ʼ��ʾ�� 1~4
 title		����		INT8U *		�˵���ʾ, ������8������, �ڵ�һ����ʾ
 p_menu		����		INT8U *		�˵���ÿ��10�����֣��������ո�  
 allow_exit	����		INT8U		�Ƿ�����ȡ�����˳�TRUE=���� FALSE=������ 
 
 ��  ��  ֵ��ѡ��Ĳ˵���  0Ϊ��һ��
		-1 = ȡ���� 
 
 ��      �ߣ�����
 ��      �ڣ�2012-10-29
 �޸���ʷ��
	����		�޸���		�޸�����
	------		---------	-------------
******************************************************************************/
INT8S browse_menu_time(INT8U startline, INT8U *title, INT8U *p_menu, INT8U allow_exit)
{
	INT16U len = 0;
	INT16U menu_lines = 0;
	INT16U i = 0;
#define MAX_MENU_LINES 100
	INT8U menu_text[MAX_MENU_LINES][22];
	//  INT16U j = 0;
	INT16U key = EM_key_NOHIT;
	struct _MENU_PARAM
	{
		INT16S disp_pos;
		INT16S menu_pos;
	}lmenu;
	INT16S h = 0;
	INT16U disp_pos_max = 0;
	INT16S m = 0;
	INT8S buf[30];
	BUS_TIME ltime;


	startline = startline;              //avoid warning

	EA_vCls();
	EA_ucSetInverse(EM_lcd_INVOFF);

	//  //��ʾtitle
	//  EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, (char *)title);
	//2012-11-16 14:25
	Get_Time(&ltime);
	sprintf((void *)buf, "%02X%02X-%02X-%02X %02X:%02X",
			ltime.century, ltime.year, ltime.month, ltime.day, ltime.hour, ltime.minute);
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, (char *)buf);

	//�˵����ֳ�ʼ��
	len = strlen((char *)p_menu);
	menu_lines = len / 20;
	if ( menu_lines > MAX_MENU_LINES )
		menu_lines = MAX_MENU_LINES;
	//  if ( menu_lines <= 1 )
	//  	return 0;
	if ( menu_lines == 0 )
		return -1;
	memset((void *)&menu_text[0][0], 0x00, sizeof(menu_text));
	for ( i = 0; i < menu_lines; i++ )
	{
		memcpy((void *)&menu_text[i][0], (void *)&p_menu[i * 20], 20);
	}


	if ( menu_lines < 4 )
		disp_pos_max = menu_lines - 1;
	else
		disp_pos_max = 3;
	//  EA_ucSetInverse(EM_lcd_INVON);
	//  lcddisp(1, 1, &menu_text[0][0]);
	//  EA_ucSetInverse(EM_lcd_INVOFF);
	//  for ( i = 1; i < disp_pos_max; i++ )
	//  {
	//  	lcddisp(i + 1, 1, &menu_text[i][0]);
	//  }

	lmenu.disp_pos = 0;
	lmenu.menu_pos = 0;

	EA_vCls();   //�����Ļ
	EA_ucSetInverse(EM_lcd_INVON);
	lcddisp(lmenu.disp_pos + 1, 1, &menu_text[lmenu.menu_pos][0]);       //��ʾ��ǰ�˵�������
	EA_ucSetInverse(EM_lcd_INVOFF);

	m = 1;
	for ( i = lmenu.disp_pos + 1; i <= 3; i++ )
	{
		lcddisp(i + 1, 1, &menu_text[lmenu.menu_pos + m][0]);
		m++;
	}
	m = 1;
	for ( h = lmenu.disp_pos - 1; h >= 0; h-- )
	{
		lcddisp(h + 1, 1, &menu_text[lmenu.menu_pos - m][0]);
		m++;
	}


	for (;;)
	{
		Get_Time(&ltime);
		sprintf((void *)buf, "%02X%02X-%02X-%02X %02X:%02X",
				ltime.century, ltime.year, ltime.month, ltime.day, ltime.hour, ltime.minute);
		EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, (char *)buf);

		key = EA_uiInkey(2);
		switch ( key )
		{
		   case EM_key_INTERPUNCTION:           //��
			   lmenu.disp_pos--;
			   if ( lmenu.disp_pos < 0 )
				   lmenu.disp_pos = 0;
			   lmenu.menu_pos--;
			   if ( lmenu.menu_pos < 0 )
				   lmenu.menu_pos = 0;


			   EA_vCls();   //�����Ļ
			   EA_ucSetInverse(EM_lcd_INVON);
			   lcddisp(lmenu.disp_pos + 1, 1, &menu_text[lmenu.menu_pos][0]);       //��ʾ��ǰ�˵�������
			   EA_ucSetInverse(EM_lcd_INVOFF);

			   m = 1;
			   for ( i = lmenu.disp_pos + 1; i <= 3; i++ )
			   {
				   lcddisp(i + 1, 1, &menu_text[lmenu.menu_pos + m][0]);
				   m++;
			   }
			   m = 1;
			   for ( h = lmenu.disp_pos - 1; h >= 0; h-- )
			   {
				   lcddisp(h + 1, 1, &menu_text[lmenu.menu_pos - m][0]);
				   m++;
			   }

			   break;

		   case EM_key_LETTER:                  //��
			   lmenu.disp_pos++;
			   if ( lmenu.disp_pos > disp_pos_max )
				   lmenu.disp_pos = disp_pos_max;
			   lmenu.menu_pos++;
			   if ( lmenu.menu_pos > menu_lines - 1 )
				   lmenu.menu_pos = menu_lines - 1;


			   EA_vCls();   //�����Ļ
			   EA_ucSetInverse(EM_lcd_INVON);
			   lcddisp(lmenu.disp_pos + 1, 1, &menu_text[lmenu.menu_pos][0]);       //��ʾ��ǰ�˵�������
			   EA_ucSetInverse(EM_lcd_INVOFF);

			   m = 1;
			   for ( i = lmenu.disp_pos + 1; i <= 3; i++ )
			   {
				   lcddisp(i + 1, 1, &menu_text[lmenu.menu_pos + m][0]);
				   m++;
			   }
			   m = 1;
			   for ( h = lmenu.disp_pos - 1; h >= 0; h-- )
			   {
				   lcddisp(h + 1, 1, &menu_text[lmenu.menu_pos - m][0]);
				   m++;
			   }
			   //  		   for ( h = lmenu.disp_pos - 1; h >= 0; h-- )
			   //  		   {
			   //  			   lcddisp(h + 1, 1, &menu_text[lmenu.menu_pos - (lmenu.disp_pos - h)][0]);
			   //  		   }


			   break;

		   case EM_key_ENTER:                   //ȷ��
			   return lmenu.menu_pos;
			   //  		   //debug
			   //  		   sprintf((void *)buf, "%d", lmenu.menu_pos + 1);
			   //  		   EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, (void *)buf);
			   //  		   break;
			   //  		   //debug end


		   case EM_key_1:
			   if ( menu_lines >= 1 )
				   return 0;
			   else
				   break;

		   case EM_key_2:
			   if ( menu_lines >= 2 )
				   return 1;
			   else
				   break;

		   case EM_key_3:
			   if ( menu_lines >= 3 )
				   return 2;
			   else
				   break;

		   case EM_key_4:
			   if ( menu_lines >= 4 )
				   return 3;
			   else
				   break;

		   case EM_key_5:
			   if ( menu_lines >= 5 )
				   return 4;
			   else
				   break;

		   case EM_key_6:
			   if ( menu_lines >= 6 )
				   return 5;
			   else
				   break;

		   case EM_key_7:
			   if ( menu_lines >= 7 )
				   return 6;
			   else
				   break;

		   case EM_key_8:
			   if ( menu_lines >= 8 )
				   return 7;
			   else
				   break;

		   case EM_key_9:
			   if ( menu_lines >= 9 )
				   return 8;
			   else
				   break;


		   case EM_key_CANCEL:
			   if ( allow_exit == TRUE )
				   return -1;
			   else
				   break;

		   case EM_key_0:      //������ݷ�ʽ����0-��������Ӧ-��Ʊ��
			   return 10;

		   default:
			   break;
		}

	}




	return 0;
}

/******************************************************************************
 �������ƣ�get_input_str
 ������������ȡ�ַ���
 �������ƣ�	����/�����	����		����
 iStartLine	����		int			������ʼ��
 iStartCol	����		int			������ʼ��
 uiMinText	����		uint		�˵���ÿ��10�����֣��������ո�  
 uiMaxText	����		uint		�Ƿ�����ȡ�����˳�TRUE=���� FALSE=������ 
 
 ��  ��  ֵ��ѡ��Ĳ˵���  0Ϊ��һ��
		-1 = ȡ���� 
 
 ��      �ߣ�����
 ��      �ڣ�2012-10-29
 �޸���ʷ��
	����		�޸���		�޸�����
	------		---------	-------------
******************************************************************************/
INT8U get_input_str(int iStartLine, int iStartCol, uint uiMinText, uint uiMaxText, char *pcString)
{
	INT16U key = EM_key_NOHIT;
	char buf[100];
	char clr_buf[100];
	INT32U i = 0;

	strcpy(clr_buf, "");
	for ( i = 0; i < uiMaxText; i++ )
	{
		strcat(clr_buf, " ");
	}

	strcpy(buf, "");

	for (;;)
	{
		lcddisp(iStartLine, iStartCol, (void *)buf);
		key = EA_uiInkey(0);
		switch ( key )
		{
		   case EM_key_0:
			   if ( strlen(buf) < uiMaxText )
				   strcat(buf, "0");
			   break;

		   case EM_key_1:
			   if ( strlen(buf) < uiMaxText )
				   strcat(buf, "1");
			   break;

		   case EM_key_2:
			   if ( strlen(buf) < uiMaxText )
				   strcat(buf, "2");
			   break;

		   case EM_key_3:
			   if ( strlen(buf) < uiMaxText )
				   strcat(buf, "3");
			   break;

		   case EM_key_4:
			   if ( strlen(buf) < uiMaxText )
				   strcat(buf, "4");
			   break;

		   case EM_key_5:
			   if ( strlen(buf) < uiMaxText )
				   strcat(buf, "5");
			   break;

		   case EM_key_6:
			   if ( strlen(buf) < uiMaxText )
				   strcat(buf, "6");
			   break;

		   case EM_key_7:
			   if ( strlen(buf) < uiMaxText )
				   strcat(buf, "7");
			   break;

		   case EM_key_8:
			   if ( strlen(buf) < uiMaxText )
				   strcat(buf, "8");
			   break;

		   case EM_key_9:
			   if ( strlen(buf) < uiMaxText )
				   strcat(buf, "9");
			   break;

		   case EM_key_INTERPUNCTION:           //��
			   if ( strlen(buf) < uiMaxText )
			   {
				   if ( strstr(buf, ".") == NULL )
					   strcat(buf, ".");
			   }
			   break;

		   case EM_key_CLEAR:
			   if ( strlen(buf) > 0 )
			   {
				   buf[strlen(buf) - 1] = '\0';
				   lcddisp(iStartLine, iStartCol, (void *)clr_buf);
			   }
			   break;

		   case EM_key_CANCEL:
			   return EM_ABOLISH;

		   case EM_key_ENTER:                   //ȷ��
			   if ( strlen(buf) < uiMinText )
			   {
				   break;
			   }
			   else
			   {
				   strcpy(pcString, buf);
				   return EM_SUCCESS;
			   }

		   case EM_key_F1:
			   return EM_key_F1;

		   case EM_key_F2:
			   return EM_key_F2;

		   default:
			   break;
		}

	}


}

/*****************************************************************
����ԭ�ͣ�cal_crc
 ��������������һ�����ݵ�CRCֵ������CCITT�㷨
 ����������
 �������ƣ�	����/�����	����			����
 buf		����		INT8U const *	Ҫ����CRC������������ʼָ��
 len		����		INT16U			Ҫ����CRC���������ĳ���

 ��  ��  ֵ��uint�͵�CRCֵ(2�ֽ�)
 ��      �ߣ�����
 ��      �ڣ�2004-02-04
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
*****************************************************************/
INT16U cal_crc(INT8U const *buf, INT16U len)
{
	return Cal_Crc(buf, len);
}

/******************************************************************************
 �������ƣ�crc_check
 �����������Խ��յ������ݽ���CRCУ��
 ����������
 �������ƣ�	����/�����	����		����
 rcv_buf	����		const INT8U*	Ҫ����BCCУ������ݻ�����
 rcvd_len	����		INT16U		���յ��ĳ���
				
 ��  ��  ֵ��ok(0)-�ɹ�
				 notok(0xFF)-ʧ��(BCCУ�����)
				   
 ��      ��	������
 ��      �ڣ�2004-09-02
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	------------- 
******************************************************************************/
INT8U crc_check(const INT8U *rcv_buf, INT16U rcvd_len)
{
	INT16U crc = 0;
	//  INT16U crc_rcv = 0;
//	INT8U crc_ascii[10];

	crc = Cal_Crc((void *)rcv_buf, rcvd_len - 2);

	if ( (rcv_buf[rcvd_len - 2] == (INT8U)(crc / 256)) && (rcv_buf[rcvd_len - 1] == (INT8U)(crc % 256)) )
		return ok;
	else 
		return notok;

}

/*****************************************************************
 ����ԭ�ͣ�comm_add_end
 �������������2�ֽ�crc���ڰ�β
 ����������
 �������ƣ�		����/�����	����		����
 -----------	-----------	------	   	-------
 snd_buf		����		INT8U  * 	���ͻ�����
 snd_length		����		INT16U constҪ���͵ĳ���

 �� �� ֵ����
 ��    �ߣ�����
 ��    �ڣ�2013-10-22
 �޸���ʷ��
 ����		�޸���		�޸�����
 ------		---------	-------------
*****************************************************************/
void comm_add_end(INT8U *snd_buf, INT16U const snd_length)
{
	INT16U crc = 0;

	crc = cal_crc(snd_buf, snd_length);

	snd_buf[snd_length] = (INT8U)(crc / 256);
	snd_buf[snd_length + 1] = (INT8U)(crc % 256);
}

/******************************************************************************
 �������ƣ�com1_send_hex
 ����������
 ����������
 �������ƣ� ����/����� ����		����
 str_to_send	����	INT8U *		Ҫ���͵��ַ���
 len			����	INT16U		����
				
 ��  ��  ֵ����
				   
 �� 	 �� ������
 �� 	 �ڣ�2005-04-07
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void com1_send_hex(INT8U *str_to_send, INT16U len)
{
	(void)EA_ucWriteDevice(hRS232Handle, len, 0, (void *)str_to_send);
}

/*****************************************************************
����ԭ�ͣ�com1_gets
��������������1����
����������
�������ƣ�		����/�����	����		����
-----------		-----------	------	   	-------
str_got			���		INT8U*		�õ����ַ���
timeout_start	����		INT32U		���ֳ�ʱ����λmS��0����
timeout			����		INT16U		�����ֽڼ����ʱ����λmS

 ��  ��  ֵ��ok(0)-�ɹ�
				 KEY_CANCEL-���˳�����
				 notok(0xFF)-ʧ�ܣ���ʱ
��      ��	������
��      �ڣ�2005-08-20
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
INT8U com1_gets(INT8U *str_got, INT16U *rcv_len, INT32U timeout_start, INT16U timeout)
{
//	DevHandle Timer;
//	ulong clock_value;
//
//	// ����һ����ʱ��
//	EA_ucCreateTimer(&Timer);
//
//	// ������ʱ����ʼ��ʱ
//	EA_ucResetTimer(Timer);
//
//	// ��ȡ��ʱ����ʱʱ�䣬�����������Ҫ��ʱ��
//	EA_ucGetPassTimer(Timer, &clock_value);

	INT8U i = 0;
	INT16U len = 0;
//	INT32U timeout_cntr = 0;
	INT8U ch = 0;

	DevHandle timerHandle;  // ��ʱ�����
	ulong ulPassTime;

	// ����һ����ʱ��
	EA_ucCreateTimer(&timerHandle);

	//��λ��ʱ��ʱ��
	ulPassTime = 0;
	EA_ucResetTimer(timerHandle); 

	//��һ���ֽ�
	for(;;)
	{
		i = com1_query_ch(&ch);
		if (i == ok)
		{
			str_got[len++] = ch;
			break;
		}
		else
		{
			if (timeout_start == 0)
			{
				//do nothing
			}
			else
			{
				EA_ucGetPassTimer(timerHandle, &ulPassTime);
				if (ulPassTime > timeout_start)
				{
					str_got[len] = '\0';
					*rcv_len = len;
					EA_ucCloseTimer(&timerHandle);			// �رռ�ʱ��
					return notok;					//��ʱ����
				}
			}

		}
	}


	//��λ��ʱ��ʱ��
	ulPassTime = 0;
	EA_ucResetTimer(timerHandle); 

	//�����ֽ�
	for(;;)
	{
		i = com1_query_ch(&ch);
		if (i == ok)
		{
			str_got[len++] = ch;
			ulPassTime = 0;
			EA_ucResetTimer(timerHandle); 
		}
		else
		{
			EA_ucGetPassTimer(timerHandle, &ulPassTime);
			if (ulPassTime > timeout)
			{
				str_got[len] = '\0';
				*rcv_len = len;
				EA_ucCloseTimer(&timerHandle);			// �رռ�ʱ��
				return ok;					//��ʱ���������
			}
		}
	}

}

/******************************************************************************
 �������ƣ�uart1_buf_flush
 ������������մ���1����
 ����������
 �������ƣ� ����/����� ����		����

 ��  ��  ֵ����

 �� 	 �� ������
 �� 	 �ڣ�2005-04-11
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void uart1_buf_flush(void)
{
//	(void)EA_ucClearRs232(hRS232Handle);

	INT32U rcv_len = 0;
	(void)EA_ucReadDevice(hRS232Handle, 1, 0, Com1RcvBuf2, &rcv_len);				//��һ�黺��

}

/*****************************************************************
����ԭ�ͣ�com1_getch
��������������1����
����������
�������ƣ�		����/�����	����		����
-----------		-----------	------	   	------- 
ch				���		INT8U *		�õ�����

��  ��  ֵ��ok(0) - �ɹ� 
			notok(0xFF)-ʧ�ܣ���ʱ
��      �ߣ�
��      �ڣ�2005-08-20
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
INT8U com1_query_ch(INT8U *ch)
{
	INT8U i = 0;
	INT32U rcv_len = 0;

	i = EA_ucReadDevice(hRS232Handle, 1, 0, Com1RcvBuf2, &rcv_len);
	if ( i != EM_SUCCESS )
	{
	  	return notok;
	}
	if ( rcv_len >= 1 )
	{
		*ch = Com1RcvBuf2[0];
		return ok;
	}

	return notok;
}

/*****************************************************************
����ԭ�ͣ�com1_gets_keyboard
��������������1���գ�������ȡ��
����������
�������ƣ�		����/�����	����		����
-----------		-----------	------	   	-------
str_got			���		INT8U*		�õ����ַ���
timeout_start	����		INT32U		���ֳ�ʱ����λmS��0����
timeout			����		INT16U		�����ֽڼ����ʱ����λmS

 ��  ��  ֵ��ok(0)-�ɹ�
			0x01-���˳�����
			notok(0xFF)-ʧ�ܣ���ʱ
��      ��	������
��      �ڣ�2005-08-20
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
INT8U com1_gets_keyboard(INT8U *str_got, INT16U *rcv_len, INT32U timeout_start, INT16U timeout)
{
//	DevHandle Timer;
//	ulong clock_value;
//
//	// ����һ����ʱ��
//	EA_ucCreateTimer(&Timer);
//
//	// ������ʱ����ʼ��ʱ
//	EA_ucResetTimer(Timer);
//
//	// ��ȡ��ʱ����ʱʱ�䣬�����������Ҫ��ʱ��
//	EA_ucGetPassTimer(Timer, &clock_value);

	INT8U i = 0;
	INT16U len = 0;
//	INT32U timeout_cntr = 0;
	INT8U ch = 0;
	INT16U key = EM_key_NOHIT;

	DevHandle timerHandle;  // ��ʱ�����
	ulong ulPassTime;

	// ����һ����ʱ��
	EA_ucCreateTimer(&timerHandle);

	//��λ��ʱ��ʱ��
	ulPassTime = 0;
	EA_ucResetTimer(timerHandle); 

	//��һ���ֽ�
	for(;;)
	{
		i = com1_query_ch(&ch);
		if (i == ok)
		{
			str_got[len++] = ch;
			break;
		}
		else
		{
			key = EA_ucKBHit();
			if ( key == EM_key_HIT )    //�а���
			{
				key = EA_uiInkey(1);    //��ȡ������ֵ
				if ( key == EM_key_CANCEL )
				{
					EA_ucCloseTimer(&timerHandle);			// �رռ�ʱ��
					return 0x01;		//��ȡ������
				}
			}

			if (timeout_start == 0)
			{
				//do nothing
			}
			else
			{
				EA_ucGetPassTimer(timerHandle, &ulPassTime);
				if (ulPassTime > timeout_start)
				{
					str_got[len] = '\0';
					*rcv_len = len;
					EA_ucCloseTimer(&timerHandle);			// �رռ�ʱ��
					return notok;					//��ʱ����
				}
			}

		}
	}


	//��λ��ʱ��ʱ��
	ulPassTime = 0;
	EA_ucResetTimer(timerHandle); 

	//�����ֽ�
	for(;;)
	{
		i = com1_query_ch(&ch);
		if (i == ok)
		{
			str_got[len++] = ch;
			ulPassTime = 0;
			EA_ucResetTimer(timerHandle); 
		}
		else
		{
			key = EA_ucKBHit();
			if ( key == EM_key_HIT )    //�а���
			{
				key = EA_uiInkey(1);    //��ȡ������ֵ
				if ( key == EM_key_CANCEL )
				{
					EA_ucCloseTimer(&timerHandle);			// �رռ�ʱ��
					return 0x01;		//��ȡ������
				}
			}

			EA_ucGetPassTimer(timerHandle, &ulPassTime);
			if (ulPassTime > timeout)
			{
				str_got[len] = '\0';
				*rcv_len = len;
				EA_ucCloseTimer(&timerHandle);			// �رռ�ʱ��
				return ok;					//��ʱ���������
			}
		}
	}

}

/*****************************************************************
����ԭ�ͣ�com1_snd_and_rcv
��������������1���������
����������
�������ƣ�	����/�����	����		����

��  ��  ֵ��ok-�ɹ�
			0x01-���˳�����
			notok-ʧ��

��      ��	������
��      �ڣ�2005-05-19
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
INT8U com1_snd_and_rcv(INT8U *str_to_snd, INT16U snd_len, INT8U *str_rcv, INT16U *rcv_len, INT16U timeout_start, INT16U timeout)
{
	INT8U i = 0;
	INT8U j = 0;

	comm_add_end(str_to_snd, snd_len);

	for ( i = 0; i <= COM1_RETRY_TIMES; i++ )
	{
		uart1_buf_flush();
		com1_send_hex(str_to_snd, snd_len + 2);

		j = com1_gets_keyboard(str_rcv, rcv_len, timeout_start, timeout);
		if ( j == ok )
		{
			if ( str_rcv[0] != 0xBB )            //�жϿ�ͷ
				continue;
			if ( crc_check(str_rcv, *rcv_len) != ok )
				continue;

			return ( ok );
		}

		if ( j == 0x01 )
			return 0x01;       //��ȡ������
	}
	return ( notok );
}

/*****************************************************************
����ԭ�ͣ�com1_snd_and_rcv2
��������������1��������գ����������ж�
����������
�������ƣ�	����/�����	����		����

��  ��  ֵ��ok-�ɹ�
			notok-ʧ��

��      ��	������
��      �ڣ�2005-05-19
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
INT8U com1_snd_and_rcv2(INT8U *str_to_snd, INT16U snd_len, INT8U *str_rcv, INT16U *rcv_len, INT16U timeout_start, INT16U timeout)
{
	INT8U i = 0;
	INT8U j = 0;

	comm_add_end(str_to_snd, snd_len);

	for ( i = 0; i <= COM1_RETRY_TIMES; i++ )
	{
		uart1_buf_flush();
		com1_send_hex(str_to_snd, snd_len + 2);

		j = com1_gets(str_rcv, rcv_len, timeout_start, timeout);
		if ( j == ok )
		{
			if ( str_rcv[0] != 0xBB )            //�жϿ�ͷ
				continue;
			if ( crc_check(str_rcv, *rcv_len) != ok )
				continue;

			return ( ok );
		}
	}
	return ( notok );
}

/******************************************************************************
 �������ƣ�beep
 �������������Ʒ���������
 ����������
 �������ƣ�	����/�����	����		����
 _ms	����		INT16U			Ҫ���е�ʱ�䣬��λms

 ��  ��  ֵ����

 ��      ��	������
 ��      �ڣ�2004-09-02
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void beep(INT16U _ms)
{
	EA_vBeepMs(_ms);
}

/*****************************************************************
 ����ԭ�ͣ�beep_success
 ������������������ʾ�ɹ�
 ����������
 �������ƣ�		����/�����	����		����
 -----------	-----------	------	   	-------

 �� �� ֵ����
 ��    �ߣ�����
 ��    �ڣ�2013-10-22
 �޸���ʷ��
 ����		�޸���		�޸�����
 ------		---------	-------------
*****************************************************************/
void beep_success(void)
{
	beep(1000);
}

/*****************************************************************
 ����ԭ�ͣ�beep_failed
 ������������������ʾʧ��
 ����������
 �������ƣ�		����/�����	����		����
 -----------	-----------	------	   	-------

 �� �� ֵ����
 ��    �ߣ�����
 ��    �ڣ�2013-10-22
 �޸���ʷ��
 ����		�޸���		�޸�����
 ------		---------	-------------
*****************************************************************/
void beep_failed(void)
{
	beep(70);
	sleep_ms(70);
	beep(70);
	sleep_ms(70);
	beep(70);
}

/******************************************************************************
 �������ƣ�sleep_ms
 ������������ʱ����
 �������ƣ�	����/�����	����		����
 _ms	����	INT16U				Ҫ��ʱ�ı��� 1ms*?
 
 ��  ��  ֵ����
 
 ��      ��	������
 ��      �ڣ�2004-09-02
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void sleep_ms(INT16U _ms)
{
	EA_vDelayMs(_ms);
}

/******************************************************************************
 �������ƣ�cal_sec
 ���������������2000��1��1�յ�����ʱ�������
 ����������
 �������ƣ�	����/�����	����		����
 time			����			BUS_TIME*	Ҫ���м����ʱ�䣬Ҫ������ʱ�����2000��
 
 ��  ��  ֵ������
   
 ��      ��	������
 ��      �ڣ�2006.1.9
 �޸���ʷ��
		����		�޸���		�޸�����

******************************************************************************/
INT32U cal_sec(BUS_TIME * time)
{
	INT8U i;
	INT16U year;
	INT32U sec;

	const INT32U days[] = 
	{
		0,
		0,
		0 + 31,
		0 + 31 + 28,
		0 + 31 + 28 + 31,
		0 + 31 + 28 + 31 + 30,
		0 + 31 + 28 + 31 + 30 + 31,
		0 + 31 + 28 + 31 + 30 + 31 + 30,
		0 + 31 + 28 + 31 + 30 + 31 + 30 + 31,
		0 + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31,
		0 + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30,
		0 + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31,
		0 + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30,
	};

	const INT32U days_leap[] = 
	{
		0,
		0,
		0 + 31,
		0 + 31 + 29,
		0 + 31 + 29 + 31,
		0 + 31 + 29 + 31 + 30,
		0 + 31 + 29 + 31 + 30 + 31,
		0 + 31 + 29 + 31 + 30 + 31 + 30,
		0 + 31 + 29 + 31 + 30 + 31 + 30 + 31,
		0 + 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31,
		0 + 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30,
		0 + 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31,
		0 + 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30,
	};

	if ( time->century != 0x20 )		  //����ʱ�����Ϊ2000���Ժ�
		return 0;

	//�������ǰ�������
	i = Is_Leap((INT16U)bcdtohex(time->century) * 100 + (INT16U)bcdtohex(time->year));
	if ( i == TRUE )
	{	//������
		sec = ( (days_leap[bcdtohex(time->month)] + (INT32U)bcdtohex(time->day)) * 24 * 60 * 60 
				+ (INT32U)bcdtohex(time->hour) * 60 * 60 + (INT32U)bcdtohex(time->minute) * 60 + (INT32U)bcdtohex(time->second) );
	}
	else
	{
		sec = ( (days[bcdtohex(time->month)] + (INT32U)bcdtohex(time->day)) * 24 * 60 * 60 
				+ (INT32U)bcdtohex(time->hour) * 60 * 60 + (INT32U)bcdtohex(time->minute) * 60 + (INT32U)bcdtohex(time->second) );
	}

	//�������ǰһ�굽2000��֮�������
	for ( year = 2000; year < ((INT16U)bcdtohex(time->century) * 100 + (INT16U)bcdtohex(time->year)); year++ )
	{
		if ( Is_Leap(year) == TRUE )
			sec += 366 * 24 * 60 * 60;
		else
			sec	+= 365 * 24 * 60 * 60;
	}

	return sec;
}

/*****************************************************************
����ԭ�ͣ�Is_Leap
���������� �����Ƿ�Ϊ����
����������
�������ƣ�	����/����� ����		����
			year	����		uint 		��Ҫ�ж��Ƿ�Ϊ��������
			
��	��	 ֵ��	0-��������
				1-������
				
��		 �� ������
��		 �ڣ�2003-11-27
�޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
*****************************************************************/
INT8U Is_Leap(INT16U year)
{
	return(year % 4 == 0) && ( (year % 100 != 0) || (year % 400 == 0) );
}

