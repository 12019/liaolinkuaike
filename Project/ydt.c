/******************************************************************************
Copyright 2005 大连易达通电子有限公司
All rights riserved.

文件名	：ydt.c
模块名称：
功能概要：

取代版本：0.0.1
修改人	：许岩
完成日期：2005.04.06
升级说明：create

******************************************************************************/
#include "main.h"


#include "Common_exp.h"

extern DEV_STAT DevStat;

extern char dbuf[];

extern INT8U Com1SndBuf[UART1_SEND_QUEUE_LENGTH];              //串口发送缓冲区
extern INT8U Com1RcvBuf[UART1_RCV_QUEUE_LENGTH];               //串口接收缓冲区
extern INT8U Com1RcvBuf2[UART1_RCV_QUEUE_LENGTH];               //串口接收缓冲区

extern DevHandle hRS232Handle;

/*****************************************************************
函数原型：Get_Time
功能描述：读取时间，读取两次，为了防止偶尔一次不成功
参数描述：
参数名称：	输入/输出？	类型		描述
*time_data	输出   _TIME	7个字节长，数据从头到尾顺序是世纪 年 月 日 时 分 秒
返  回  值：ok(0)-成功 
		notok(0xFF)－失败
作      者：许岩
日      期：2005-04-12
修改历史：
日期		修改人		修改描述
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
函数原型：Modify_Time
功能描述：修改时间，修改两次，为了防止偶尔一次不成功
参数描述：
参数名称：	输入/输出？	类型		描述
*time_data	输入	_TIME	7个字节长，数据从头到尾顺序是世纪 年 月 日 时 分 秒
返  回  值：ok(0)-成功 
		notok(0xFF)－失败
作      者	：许岩
日      期：2005-04-12
修改历史：
日期		修改人		修改描述
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
函数原型：CheckTimeFormat
功能描述：检查时间格式是否合法
参数描述：
参数名称：	输入/输出？	类型		描述
time		输入  			const _TIME *	7个字节长，数据从头到尾顺序是世纪 年 月 日 时 分 秒

返  回  值：ok(0)-成功 
		notok(0xFF)－失败
作      者	：许岩
日      期：2003-05-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U CheckTimeFormat(const BUS_TIME *time)
{
	INT8U temp;

	temp = time->month;
	if ( temp >= 0x0a )       //月份应在1－12之间
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
	if ( temp > 0x31 )                   //天应在1-31之间
		return notok;
	temp = temp & 0x0F;
	if ( temp > 0x09 )
		return notok;

	temp = time->hour;
	if ( temp > 0x23 )                                   //小时应在0-23之间
		return notok;
	temp = temp & 0x0F;
	if ( temp > 0x09 )
		return notok;

	temp = time->minute;
	if ( temp > 0x59 )                                   //分钟应在0-59之间
		return notok;
	temp = temp & 0x0F;
	if ( temp > 0x09 )
		return notok;

	temp = time->second;
	if ( temp > 0x59 )                                    //秒应在0-59之间
		return notok;
	temp = temp & 0x0F;
	if ( temp > 0x09 )
		return notok;

	return ok;
}

/******************************************************************************
 函数名称：ascii_to_hex
 功能描述：把一个0-9的ascii代码，转化为16进制数
 参数名称： 输入/输出？ 类型        描述
 num        输入        INT8U           要转化为ascii代码的数，要求范围0-9 a-f A-F

 返  回  值：转化后的16进制数

 作      者 ：许岩
 日      期：2004-09-02
 修改历史：
	日期        修改人      修改描述
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
函数原型：bcdtohex
功能描述：bcd码转换为hex码
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------
bcd				输入			INT8U		输入的bcd码

返  回  值：转换后的hex码
作      者	：许岩
日      期：2006-01-09
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U bcdtohex(INT8U bcd)
{
	//  ToggleWD();

	return ( (bcd >> 4) * 10 + (bcd & 0x0F) );
}

/******************************************************************************
 函数名称：hextobcd
 功能描述：把一个16进制数，转化为bcd码
 参数名称： 输入/输出？ 类型        描述
 num        输入        INT8U           要转化为bcd码的数

 返  回  值：转化后的bcd码

 作      者 ：许岩
 日      期：2004-09-02
 修改历史：
	日期        修改人      修改描述
	------      ---------   -------------
******************************************************************************/
INT8U hextobcd(INT8U num)
{
	return ( ((num / 10) << 4) | (num % 10) );
}

/******************************************************************************
 函数名称：ascii
 功能描述：把一个0-9的数转化为ascii代码，此函数也可扩展用于通讯中，通讯中涉及到一些A-F的非法输入，认为合法
 参数名称：	输入/输出？	类型		描述
 num		输入		INT8U			要转化为ascii代码的数，要求范围0-9
 
 返  回  值：转化后的ascii代码
 
 作      者	：许岩
 日      期：2004-09-02
 修改历史：
	日期		修改人		修改描述
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
// 功能：在屏幕上输出字符
// 入口参数：	lb->显示输出字符
// 出口参数：	Null
// 作者：
// 函数出处：EH0218---COMMON.C例子程序
// *****************************************************************
char HextoBcd(char sour)
{
	//  ToggleWD();

	return ( ((sour / 10) << 4) | (sour % 10) );
}

/******************************************************************************
 函数名称：htoa
 功能描述：hex转换为ascii码
 参数描述：
 参数名称： 输入/输出？ 类型		描述

 返  回  值：无

 作 	 者 ：许岩
 日 	 期：2005-04-07
 修改历史：
	 日期		修改人		修改描述
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
 函数名称：atoh
 功能描述：ascii转换为hex码
 参数描述：
 参数名称： 输入/输出？ 类型		描述
		
 返  回  值：无
		   
 作 	 者 ：许岩
 日 	 期：2005-04-07
 修改历史：
	日期		修改人		修改描述
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
 函数名称：ascii_comm
 功能描述：把一个0-9的数转化为ascii代码，此函数也可扩展用于通讯中，通讯中涉及到一些A-F的非法输入，认为合法
 参数名称：	输入/输出？	类型		描述
 num		输入		INT8U			要转化为ascii代码的数，要求范围0-9
 
 返  回  值：转化后的ascii代码
 
 作      者	：许岩
 日      期：2004-09-02
 修改历史：
	日期		修改人		修改描述
	------		---------	-------------
******************************************************************************/
INT8U ascii_comm(INT8U num)
{
	return ( num + 0x30 );
}

/******************************************************************************
 函数名称：SleepMs
 功能描述：延时函数，11.0592M*4晶振专用，开mem加速
 参数名称：	输入/输出？	类型		描述
 _ms	输入	INT16U				要延时的倍数 1ms*?
 
 返  回  值：无
 
 作      者：许岩
 日      期：2004-09-02
 修改历史：
	日期		修改人		修改描述
	------		---------	-------------
******************************************************************************/
void SleepMs(INT16U _ms)
{
	EA_vDelayMs(_ms);
}

/******************************************************************************
 函数名称：lcddisp
 功能描述：显示函数
 参数名称：	输入/输出？	类型		描述
 line		输入		INT16U		行 1~4
 col		输入		INT16U		列 1~20
 str		输入		INT8U *		要显示的内容
 
 返  回  值：无
 
 作      者：许岩
 日      期：2012-10-29
 修改历史：
	日期		修改人		修改描述
	------		---------	-------------
******************************************************************************/
void lcddisp(INT16U line, INT16U col, const INT8U *str)
{

	EA_vDisp((int)line, (int)col, (char *)str);
}

/******************************************************************************
 函数名称：browse_menu
 功能描述：菜单函数
 参数名称：	输入/输出？	类型		描述
 startline	输入		INT8U		菜单起始显示行 1~4
 title		输入		INT8U *		菜单提示, 不超过8个汉字, 在第一行显示
 p_menu		输入		INT8U *		菜单，每行10个汉字，不够补空格  
 allow_exit	输入		INT8U		是否允许按取消键退出TRUE=允许 FALSE=不允许 
 
 返  回  值：选择的菜单号  0为第一条
		-1 = 取消键 
 
 作      者：许岩
 日      期：2012-10-29
 修改历史：
	日期		修改人		修改描述
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

	//显示title
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, (char *)title);

	//菜单文字初始化
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

	EA_vCls();   //清除屏幕
	EA_ucSetInverse(EM_lcd_INVON);
	lcddisp(lmenu.disp_pos + 1, 1, &menu_text[lmenu.menu_pos][0]);       //显示当前菜单，反显
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
		   case EM_key_INTERPUNCTION:           //上
			   lmenu.disp_pos--;
			   if ( lmenu.disp_pos < 0 )
				   lmenu.disp_pos = 0;
			   lmenu.menu_pos--;
			   if ( lmenu.menu_pos < 0 )
				   lmenu.menu_pos = 0;


			   EA_vCls();   //清除屏幕
			   EA_ucSetInverse(EM_lcd_INVON);
			   lcddisp(lmenu.disp_pos + 1, 1, &menu_text[lmenu.menu_pos][0]);       //显示当前菜单，反显
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

		   case EM_key_LETTER:                  //下
			   lmenu.disp_pos++;
			   if ( lmenu.disp_pos > disp_pos_max )
				   lmenu.disp_pos = disp_pos_max;
			   lmenu.menu_pos++;
			   if ( lmenu.menu_pos > menu_lines - 1 )
				   lmenu.menu_pos = menu_lines - 1;


			   EA_vCls();   //清除屏幕
			   EA_ucSetInverse(EM_lcd_INVON);
			   lcddisp(lmenu.disp_pos + 1, 1, &menu_text[lmenu.menu_pos][0]);       //显示当前菜单，反显
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

		   case EM_key_ENTER:                   //确认
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
 函数名称：browse_menu2
 功能描述：菜单函数,带起始默认行的
 参数名称：	输入/输出？	类型		描述
 startline	输入		INT16U		菜单起始显示行 1~4
 title		输入		INT8U *		菜单提示, 不超过8个汉字, 在第一行显示
 p_menu		输入		INT8U *		菜单，每行10个汉字，不够补空格  
 allow_exit	输入		INT8U		是否允许按取消键退出TRUE=允许 FALSE=不允许 
 default_line输入		INT16U		起始默认行
 
 返  回  值：选择的菜单号  0为第一条
		-1 = 取消键 
 
 作      者：许岩
 日      期：2012-10-29
 修改历史：
	日期		修改人		修改描述
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

	//显示title
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, (char *)title);

	//菜单文字初始化
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

	//  EA_vCls();   //清除屏幕
	//  EA_ucSetInverse(EM_lcd_INVON);
	//  lcddisp(lmenu.disp_pos + 1, 1, &menu_text[lmenu.menu_pos][0]);       //显示当前菜单，反显
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


	//拨到初始位置
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

	EA_vCls();   //清除屏幕
	EA_ucSetInverse(EM_lcd_INVON);
	lcddisp(lmenu.disp_pos + 1, 1, &menu_text[lmenu.menu_pos][0]);       //显示当前菜单，反显
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
		   case EM_key_INTERPUNCTION:           //上
			   lmenu.disp_pos--;
			   if ( lmenu.disp_pos < 0 )
				   lmenu.disp_pos = 0;
			   lmenu.menu_pos--;
			   if ( lmenu.menu_pos < 0 )
				   lmenu.menu_pos = 0;


			   EA_vCls();   //清除屏幕
			   EA_ucSetInverse(EM_lcd_INVON);
			   lcddisp(lmenu.disp_pos + 1, 1, &menu_text[lmenu.menu_pos][0]);       //显示当前菜单，反显
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

		   case EM_key_LETTER:                  //下
			   lmenu.disp_pos++;
			   if ( lmenu.disp_pos > disp_pos_max )
				   lmenu.disp_pos = disp_pos_max;
			   lmenu.menu_pos++;
			   if ( lmenu.menu_pos > menu_lines - 1 )
				   lmenu.menu_pos = menu_lines - 1;


			   EA_vCls();   //清除屏幕
			   EA_ucSetInverse(EM_lcd_INVON);
			   lcddisp(lmenu.disp_pos + 1, 1, &menu_text[lmenu.menu_pos][0]);       //显示当前菜单，反显
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

		   case EM_key_ENTER:                   //确认
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
 函数名称：lcddisperr
 功能描述：液晶屏提示错误
 参数名称：	输入/输出？	类型		描述
 str		输入		char *		要显示的内容
 
 返  回  值：无
 
 作      者：许岩
 日      期：2012-10-29
 修改历史：
	日期		修改人		修改描述
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
 函数原型：param_factory_default()
 功能描述：参数恢复出厂设置
 参数描述：	
 参数名称：	输入/输出？	类型		描述
 level		输入		INT8U		初始化的等级0-不初始化记录和备份记录文件 1-只初始化记录文件 2-记录和备份记录文件都初始化
				 
 返回值：	无
 
 作  者：	许岩
 日  期：	2004-09-17
 修改历史：
		日期		修改人		修改描述
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


	//  // 删除创建的文件
	//  ucRet = EA_ucDeleteParamFile(acModule, acFileName, EM_param_USING_FILE);
	//  if ( ucRet != EM_SUCCESS )
	//  {
	//  	lcddisperr("删除参数文件失败!");		// 操作失败的处理代码
	//  }

	ucRet = EA_ucCreateParamFile(acModule, acFileName, acVersion, EM_param_USING_FILE);
	if ( ucRet != EM_SUCCESS )
	{
		lcddisperr("创建参数文件失败!");       // 创建文件失败的处理代码
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
	DevStat.zhekou = 80;	//初始化折扣数据80，表示8折
	DevStat.cur_ticket_number = 0;

	WriteParam();



	if ( level == 0 )
	{
		return;
	}
	else if ( level == 1 )
	{
		//重新创建记录文件
		i = EA_ucPFCreate((INT8U *)recFileName, uiRecordLen);
		if ( i == EM_ffs_FILEEXIST )          //文件已经存在
		{
			if ( EA_ucPFDel((uchar *)recFileName) != EM_ffs_SUCCESS )
			{
				lcddisperr("删除记录文件失败");
			}
			if ( EA_ucPFCreate((uchar *)recFileName, uiRecordLen) != EM_ffs_SUCCESS )
			{
				lcddisperr("重新创建记录文件失败");
			}
		}
		else if ( i == EM_ffs_FILEFULL )
		{
			lcddisperr("创建记录文件数满");
		}
		else if ( i == EM_ffs_SUCCESS )
		{
			;           //do nothing
		}
		else
		{
			lcddisperr("创建记录文件失败");
		}

		return;
	}
	else if ( level == 2 )
	{
		//重新创建记录文件
		i = EA_ucPFCreate((INT8U *)recFileName, uiRecordLen);
		if ( i == EM_ffs_FILEEXIST )          //文件已经存在
		{
			if ( EA_ucPFDel((uchar *)recFileName) != EM_ffs_SUCCESS )
			{
				lcddisperr("删除记录文件失败");
			}
			if ( EA_ucPFCreate((uchar *)recFileName, uiRecordLen) != EM_ffs_SUCCESS )
			{
				lcddisperr("重新创建记录文件失败");
			}
		}
		else if ( i == EM_ffs_FILEFULL )
		{
			lcddisperr("创建记录文件数满");
		}
		else if ( i == EM_ffs_SUCCESS )
		{
			;           //do nothing
		}
		else
		{
			lcddisperr("创建记录文件失败");
		}


		//重新创建补采记录文件
		i = EA_ucPFCreateLoop((INT8U *)"backup", uiRecordLen, RECNUM_OF_BACKUP_FILE);
		if ( i == EM_ffs_FILEEXIST )          //文件已经存在
		{
			if ( EA_ucPFDel((INT8U *)"backup") != EM_ffs_SUCCESS )
			{
				lcddisperr("删除补采记录文件失败");
			}
			if ( EA_ucPFCreateLoop((INT8U *)"backup", uiRecordLen, RECNUM_OF_BACKUP_FILE) != EM_ffs_SUCCESS )
			{
				lcddisperr("重新创建补采文件失败");
			}
		}
		else if ( i == EM_ffs_FILEFULL )
		{
			lcddisperr("创建记录文件数满");
		}
		else if ( i == EM_ffs_SUCCESS )
		{
			;           //do nothing
		}
		else
		{
			lcddisperr("创建补采记录文件失败");
		}

	}
}

/******************************************************************************
 函数名称：WriteParam
 功能描述：保存参数
 参数描述：
 参数名称： 输入/输出？ 类型        描述

 返  回  值：无

 作      者 ：许岩
 日      期：2006-08-02
 修改历史：
		日期        修改人      修改描述
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


	// 向这个文件写入数据
	ucRet = EA_ucWriteParamFile(acModule, acFileName, &DevStat.end - &DevStat.start, (void *)&DevStat.start);
	if ( ucRet != EM_SUCCESS )
	{
		lcddisperr("写参数文件失败!"); // 写数据失败的处理代码
	}


}

/******************************************************************************
 函数名称：ReadParam
 功能描述：读取参数
 参数描述：
 参数名称： 输入/输出？ 类型        描述

 返  回  值：EA_ucReadParamFile 的返回 EM_SUCCESS表示成功 其他说明失败

 作      者 ：许岩
 日      期：2006-08-02
 修改历史：
		日期        修改人      修改描述
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


	// 读取参数文件数据
	ucRet = EA_ucReadParamFile(acModule, acFileName, &uiFileSize, (void *)&DevStat.start);
	if ( ucRet != EM_SUCCESS )
	{
		sprintf(dbuf, "读参数文件失败%02X", ucRet);
		lcddisperr(dbuf);
		//  	lcddisperr("读参数文件失败!");   // 读取文件失败的处理代码
	}
	return ucRet;
}

/*****************************************************************
 函数原型：Cal_Crc
 功能描述：计算一串数据的CRC值，采用CCITT算法
 参数描述：
 参数名称：	输入/输出？	类型		描述
 data_to_cal_crc	输入	INT8U const *		要进行CRC计算的数组的起始指针
 len				 		输入	INT16U				要进行CRC计算的数组的长度

 返  回  值：uint型的CRC值(2字节)
 作      者	：许岩
 日      期：2004-02-04
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
*****************************************************************/
INT16U Cal_Crc(INT8U const *data_to_cal_crc, INT16U len)
{
	INT16U crc;
	INT8U i;

	crc = 0xFFFF;           //CRC初始值

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
 函数名称：store_record
 功能描述：储存记录
 参数名称：	输入/输出？	类型		描述
 rec		输入		INT8U *		记录
 
 返  回  值：无
 
 作      者：许岩
 日      期：2012-10-29
 修改历史：
	日期		修改人		修改描述
	------		---------	-------------
******************************************************************************/
void store_record(INT8U *rec)
{
	const char recFileName[] = "record";
	//  INT32U uiRecordLen = sizeof(TICKET_INFO);
	//  INT8U i = 0;
	INT8U ucOpenID = 0;

	// 打开文件
	if ( EA_ucPFOpen((uchar *)recFileName, &ucOpenID) != EM_ffs_SUCCESS )
	{
		lcddisperr("打开记录文件失败");
		return;
	}


	if ( EA_ucPFWriteRec(ucOpenID, 0, rec) != EM_ffs_SUCCESS )
	{
		lcddisperr("添加记录失败");
	}

	if ( EA_ucPFClose(ucOpenID) != EM_ffs_SUCCESS )
	{
		lcddisperr("关闭记录文件失败");
		return;
	}


}

///******************************************************************************
// 函数名称：store_record
// 功能描述：储存记录
// 参数名称： 输入/输出？ 类型        描述
// rec        输入        INT8U *     记录
//
// 返  回  值：无
//
// 作      者：许岩
// 日      期：2012-10-29
// 修改历史：
//    日期        修改人      修改描述
//    ------      ---------   -------------
//******************************************************************************/
//void get_record_amount(void)
//{
//    const char recFileName[] = "record";
////  INT32U uiRecordLen = sizeof(TICKET_INFO);
////  INT8U i = 0;
//    INT8U ucOpenID = 0;
//
//    // 打开文件
//    if ( EA_ucPFOpen((uchar *)recFileName, &ucOpenID) != EM_ffs_SUCCESS )
//    {
//        lcddisperr("打开记录文件失败");
//        return;
//    }
//
//
//    if ( EA_ucPFWriteRec(ucOpenID, 0, rec) != EM_ffs_SUCCESS )
//    {
//        lcddisperr("添加记录失败");
//    }
//
//    if ( EA_ucPFClose(ucOpenID) != EM_ffs_SUCCESS )
//    {
//        lcddisperr("关闭记录文件失败");
//        return;
//    }
//
//
//}

/******************************************************************************
 函数名称：browse_menu_time
 功能描述：菜单函数，带显示时间的，主菜单专用
 参数名称：	输入/输出？	类型		描述
 startline	输入		INT8U		菜单起始显示行 1~4
 title		输入		INT8U *		菜单提示, 不超过8个汉字, 在第一行显示
 p_menu		输入		INT8U *		菜单，每行10个汉字，不够补空格  
 allow_exit	输入		INT8U		是否允许按取消键退出TRUE=允许 FALSE=不允许 
 
 返  回  值：选择的菜单号  0为第一条
		-1 = 取消键 
 
 作      者：许岩
 日      期：2012-10-29
 修改历史：
	日期		修改人		修改描述
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

	//  //显示title
	//  EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, (char *)title);
	//2012-11-16 14:25
	Get_Time(&ltime);
	sprintf((void *)buf, "%02X%02X-%02X-%02X %02X:%02X",
			ltime.century, ltime.year, ltime.month, ltime.day, ltime.hour, ltime.minute);
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, (char *)buf);

	//菜单文字初始化
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

	EA_vCls();   //清除屏幕
	EA_ucSetInverse(EM_lcd_INVON);
	lcddisp(lmenu.disp_pos + 1, 1, &menu_text[lmenu.menu_pos][0]);       //显示当前菜单，反显
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
		   case EM_key_INTERPUNCTION:           //上
			   lmenu.disp_pos--;
			   if ( lmenu.disp_pos < 0 )
				   lmenu.disp_pos = 0;
			   lmenu.menu_pos--;
			   if ( lmenu.menu_pos < 0 )
				   lmenu.menu_pos = 0;


			   EA_vCls();   //清除屏幕
			   EA_ucSetInverse(EM_lcd_INVON);
			   lcddisp(lmenu.disp_pos + 1, 1, &menu_text[lmenu.menu_pos][0]);       //显示当前菜单，反显
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

		   case EM_key_LETTER:                  //下
			   lmenu.disp_pos++;
			   if ( lmenu.disp_pos > disp_pos_max )
				   lmenu.disp_pos = disp_pos_max;
			   lmenu.menu_pos++;
			   if ( lmenu.menu_pos > menu_lines - 1 )
				   lmenu.menu_pos = menu_lines - 1;


			   EA_vCls();   //清除屏幕
			   EA_ucSetInverse(EM_lcd_INVON);
			   lcddisp(lmenu.disp_pos + 1, 1, &menu_text[lmenu.menu_pos][0]);       //显示当前菜单，反显
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

		   case EM_key_ENTER:                   //确认
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

		   case EM_key_0:      //新增快捷方式，，0-按键，对应-月票卡
			   return 10;

		   default:
			   break;
		}

	}




	return 0;
}

/******************************************************************************
 函数名称：get_input_str
 功能描述：获取字符串
 参数名称：	输入/输出？	类型		描述
 iStartLine	输入		int			输入起始行
 iStartCol	输入		int			输入起始列
 uiMinText	输入		uint		菜单，每行10个汉字，不够补空格  
 uiMaxText	输入		uint		是否允许按取消键退出TRUE=允许 FALSE=不允许 
 
 返  回  值：选择的菜单号  0为第一条
		-1 = 取消键 
 
 作      者：许岩
 日      期：2012-10-29
 修改历史：
	日期		修改人		修改描述
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

		   case EM_key_INTERPUNCTION:           //上
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

		   case EM_key_ENTER:                   //确认
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
函数原型：cal_crc
 功能描述：计算一串数据的CRC值，采用CCITT算法
 参数描述：
 参数名称：	输入/输出？	类型			描述
 buf		输入		INT8U const *	要进行CRC计算的数组的起始指针
 len		输入		INT16U			要进行CRC计算的数组的长度

 返  回  值：uint型的CRC值(2字节)
 作      者：许岩
 日      期：2004-02-04
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
*****************************************************************/
INT16U cal_crc(INT8U const *buf, INT16U len)
{
	return Cal_Crc(buf, len);
}

/******************************************************************************
 函数名称：crc_check
 功能描述：对接收到的数据进行CRC校验
 参数描述：
 参数名称：	输入/输出？	类型		描述
 rcv_buf	输入		const INT8U*	要进行BCC校验的数据缓冲区
 rcvd_len	输入		INT16U		接收到的长度
				
 返  回  值：ok(0)-成功
				 notok(0xFF)-失败(BCC校验错误)
				   
 作      者	：许岩
 日      期：2004-09-02
 修改历史：
		日期		修改人		修改描述
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
 函数原型：comm_add_end
 功能描述：算出2字节crc附在包尾
 参数描述：
 参数名称：		输入/输出？	类型		描述
 -----------	-----------	------	   	-------
 snd_buf		输入		INT8U  * 	发送缓冲区
 snd_length		输入		INT16U const要发送的长度

 返 回 值：无
 作    者：许岩
 日    期：2013-10-22
 修改历史：
 日期		修改人		修改描述
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
 函数名称：com1_send_hex
 功能描述：
 参数描述：
 参数名称： 输入/输出？ 类型		描述
 str_to_send	输入	INT8U *		要发送的字符串
 len			输入	INT16U		长度
				
 返  回  值：无
				   
 作 	 者 ：许岩
 日 	 期：2005-04-07
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void com1_send_hex(INT8U *str_to_send, INT16U len)
{
	(void)EA_ucWriteDevice(hRS232Handle, len, 0, (void *)str_to_send);
}

/*****************************************************************
函数原型：com1_gets
功能描述：串口1接收
参数描述：
参数名称：		输入/输出？	类型		描述
-----------		-----------	------	   	-------
str_got			输出		INT8U*		得到的字符串
timeout_start	输入		INT32U		首字超时，单位mS，0死等
timeout			输入		INT16U		接收字节间隔超时，单位mS

 返  回  值：ok(0)-成功
				 KEY_CANCEL-按退出键了
				 notok(0xFF)-失败，超时
作      者	：许岩
日      期：2005-08-20
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U com1_gets(INT8U *str_got, INT16U *rcv_len, INT32U timeout_start, INT16U timeout)
{
//	DevHandle Timer;
//	ulong clock_value;
//
//	// 创建一个计时器
//	EA_ucCreateTimer(&Timer);
//
//	// 启动计时器开始计时
//	EA_ucResetTimer(Timer);
//
//	// 获取计时器计时时间，即上面操作需要的时间
//	EA_ucGetPassTimer(Timer, &clock_value);

	INT8U i = 0;
	INT16U len = 0;
//	INT32U timeout_cntr = 0;
	INT8U ch = 0;

	DevHandle timerHandle;  // 定时器句柄
	ulong ulPassTime;

	// 创建一个计时器
	EA_ucCreateTimer(&timerHandle);

	//复位超时定时器
	ulPassTime = 0;
	EA_ucResetTimer(timerHandle); 

	//第一个字节
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
					EA_ucCloseTimer(&timerHandle);			// 关闭计时器
					return notok;					//超时返回
				}
			}

		}
	}


	//复位超时定时器
	ulPassTime = 0;
	EA_ucResetTimer(timerHandle); 

	//其余字节
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
				EA_ucCloseTimer(&timerHandle);			// 关闭计时器
				return ok;					//超时，接收完毕
			}
		}
	}

}

/******************************************************************************
 函数名称：uart1_buf_flush
 功能描述：清空串口1缓存
 参数描述：
 参数名称： 输入/输出？ 类型		描述

 返  回  值：无

 作 	 者 ：许岩
 日 	 期：2005-04-11
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void uart1_buf_flush(void)
{
//	(void)EA_ucClearRs232(hRS232Handle);

	INT32U rcv_len = 0;
	(void)EA_ucReadDevice(hRS232Handle, 1, 0, Com1RcvBuf2, &rcv_len);				//读一遍缓存

}

/*****************************************************************
函数原型：com1_getch
功能描述：串口1接收
参数描述：
参数名称：		输入/输出？	类型		描述
-----------		-----------	------	   	------- 
ch				输出		INT8U *		得到的数

返  回  值：ok(0) - 成功 
			notok(0xFF)-失败，超时
作      者：
日      期：2005-08-20
修改历史：
日期		修改人		修改描述
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
函数原型：com1_gets_keyboard
功能描述：串口1接收，允许按键取消
参数描述：
参数名称：		输入/输出？	类型		描述
-----------		-----------	------	   	-------
str_got			输出		INT8U*		得到的字符串
timeout_start	输入		INT32U		首字超时，单位mS，0死等
timeout			输入		INT16U		接收字节间隔超时，单位mS

 返  回  值：ok(0)-成功
			0x01-按退出键了
			notok(0xFF)-失败，超时
作      者	：许岩
日      期：2005-08-20
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U com1_gets_keyboard(INT8U *str_got, INT16U *rcv_len, INT32U timeout_start, INT16U timeout)
{
//	DevHandle Timer;
//	ulong clock_value;
//
//	// 创建一个计时器
//	EA_ucCreateTimer(&Timer);
//
//	// 启动计时器开始计时
//	EA_ucResetTimer(Timer);
//
//	// 获取计时器计时时间，即上面操作需要的时间
//	EA_ucGetPassTimer(Timer, &clock_value);

	INT8U i = 0;
	INT16U len = 0;
//	INT32U timeout_cntr = 0;
	INT8U ch = 0;
	INT16U key = EM_key_NOHIT;

	DevHandle timerHandle;  // 定时器句柄
	ulong ulPassTime;

	// 创建一个计时器
	EA_ucCreateTimer(&timerHandle);

	//复位超时定时器
	ulPassTime = 0;
	EA_ucResetTimer(timerHandle); 

	//第一个字节
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
			if ( key == EM_key_HIT )    //有按键
			{
				key = EA_uiInkey(1);    //读取按键键值
				if ( key == EM_key_CANCEL )
				{
					EA_ucCloseTimer(&timerHandle);			// 关闭计时器
					return 0x01;		//按取消键了
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
					EA_ucCloseTimer(&timerHandle);			// 关闭计时器
					return notok;					//超时返回
				}
			}

		}
	}


	//复位超时定时器
	ulPassTime = 0;
	EA_ucResetTimer(timerHandle); 

	//其余字节
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
			if ( key == EM_key_HIT )    //有按键
			{
				key = EA_uiInkey(1);    //读取按键键值
				if ( key == EM_key_CANCEL )
				{
					EA_ucCloseTimer(&timerHandle);			// 关闭计时器
					return 0x01;		//按取消键了
				}
			}

			EA_ucGetPassTimer(timerHandle, &ulPassTime);
			if (ulPassTime > timeout)
			{
				str_got[len] = '\0';
				*rcv_len = len;
				EA_ucCloseTimer(&timerHandle);			// 关闭计时器
				return ok;					//超时，接收完毕
			}
		}
	}

}

/*****************************************************************
函数原型：com1_snd_and_rcv
功能描述：串口1发送与接收
参数描述：
参数名称：	输入/输出？	类型		描述

返  回  值：ok-成功
			0x01-按退出键了
			notok-失败

作      者	：许岩
日      期：2005-05-19
修改历史：
日期		修改人		修改描述
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
			if ( str_rcv[0] != 0xBB )            //判断开头
				continue;
			if ( crc_check(str_rcv, *rcv_len) != ok )
				continue;

			return ( ok );
		}

		if ( j == 0x01 )
			return 0x01;       //按取消键了
	}
	return ( notok );
}

/*****************************************************************
函数原型：com1_snd_and_rcv2
功能描述：串口1发送与接收，不带按键判断
参数描述：
参数名称：	输入/输出？	类型		描述

返  回  值：ok-成功
			notok-失败

作      者	：许岩
日      期：2005-05-19
修改历史：
日期		修改人		修改描述
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
			if ( str_rcv[0] != 0xBB )            //判断开头
				continue;
			if ( crc_check(str_rcv, *rcv_len) != ok )
				continue;

			return ( ok );
		}
	}
	return ( notok );
}

/******************************************************************************
 函数名称：beep
 功能描述：控制蜂鸣器鸣叫
 参数描述：
 参数名称：	输入/输出？	类型		描述
 _ms	输入		INT16U			要鸣叫的时间，单位ms

 返  回  值：无

 作      者	：许岩
 日      期：2004-09-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void beep(INT16U _ms)
{
	EA_vBeepMs(_ms);
}

/*****************************************************************
 函数原型：beep_success
 功能描述：蜂鸣器提示成功
 参数描述：
 参数名称：		输入/输出？	类型		描述
 -----------	-----------	------	   	-------

 返 回 值：无
 作    者：许岩
 日    期：2013-10-22
 修改历史：
 日期		修改人		修改描述
 ------		---------	-------------
*****************************************************************/
void beep_success(void)
{
	beep(1000);
}

/*****************************************************************
 函数原型：beep_failed
 功能描述：蜂鸣器提示失败
 参数描述：
 参数名称：		输入/输出？	类型		描述
 -----------	-----------	------	   	-------

 返 回 值：无
 作    者：许岩
 日    期：2013-10-22
 修改历史：
 日期		修改人		修改描述
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
 函数名称：sleep_ms
 功能描述：延时函数
 参数名称：	输入/输出？	类型		描述
 _ms	输入	INT16U				要延时的倍数 1ms*?
 
 返  回  值：无
 
 作      者	：许岩
 日      期：2004-09-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void sleep_ms(INT16U _ms)
{
	EA_vDelayMs(_ms);
}

/******************************************************************************
 函数名称：cal_sec
 功能描述：计算从2000年1月1日到输入时间的秒数
 参数描述：
 参数名称：	输入/输出？	类型		描述
 time			输入			BUS_TIME*	要进行计算的时间，要求输入时间大于2000年
 
 返  回  值：秒数
   
 作      者	：许岩
 日      期：2006.1.9
 修改历史：
		日期		修改人		修改描述

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

	if ( time->century != 0x20 )		  //输入时间必须为2000年以后
		return 0;

	//计算出当前年的秒数
	i = Is_Leap((INT16U)bcdtohex(time->century) * 100 + (INT16U)bcdtohex(time->year));
	if ( i == TRUE )
	{	//是闰年
		sec = ( (days_leap[bcdtohex(time->month)] + (INT32U)bcdtohex(time->day)) * 24 * 60 * 60 
				+ (INT32U)bcdtohex(time->hour) * 60 * 60 + (INT32U)bcdtohex(time->minute) * 60 + (INT32U)bcdtohex(time->second) );
	}
	else
	{
		sec = ( (days[bcdtohex(time->month)] + (INT32U)bcdtohex(time->day)) * 24 * 60 * 60 
				+ (INT32U)bcdtohex(time->hour) * 60 * 60 + (INT32U)bcdtohex(time->minute) * 60 + (INT32U)bcdtohex(time->second) );
	}

	//计算出从前一年到2000年之间的秒数
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
函数原型：Is_Leap
功能描述： 计算是否为闰年
参数描述：
参数名称：	输入/输出？ 类型		描述
			year	输入		uint 		需要判断是否为闰年的年份
			
返	回	 值：	0-不是闰年
				1-是闰年
				
作		 者 ：许岩
日		 期：2003-11-27
修改历史：
		日期		修改人		修改描述
		------		---------	-------------
*****************************************************************/
INT8U Is_Leap(INT16U year)
{
	return(year % 4 == 0) && ( (year % 100 != 0) || (year % 400 == 0) );
}

