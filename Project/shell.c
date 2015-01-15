/******************************************************************************
Copyright 2005 
All rights riserved.

文件名	：shell.c
模块名称：com1通讯任务 人机界面
功能概要：com1通讯任务 人机界面

取代版本：0.0.1
修改人	：
完成日期：2005.04.06
升级说明：create

******************************************************************************/
#include 		"main.h"

extern volatile DEV_STAT DevStat;

INT8U Com1SndBuf[UART1_SEND_QUEUE_LENGTH];              //串口发送缓冲区
INT8U Com1RcvBuf[UART1_RCV_QUEUE_LENGTH];               //串口接收缓冲区
INT8U Com1RcvBuf2[UART1_RCV_QUEUE_LENGTH];               //串口接收缓冲区

extern int UART1RcvBuf[(UART1_RCV_QUEUE_LENGTH + sizeof(int)- 1)/ sizeof(int)];


#define MAX_COMMAND_NUM    10


typedef struct
{
	INT16S num;
	INT8S name[50];
	INT8U(*CommandFunc)(INT8U argc, INT8S **argv);
}command;

command ShellComms[MAX_COMMAND_NUM];

#define MAX_LEN_CMD_BUF 50

#define ENTR 0x0D
#define ERRORCOMMAND    notok

//#define armgetch()			UART0Getch()
//#define armputch(exp)		UART1Putch(exp)

static INT8S *argv[20];
static INT8U argc;

//INT8U fpage_buf[512];

extern DevHandle hRS232Handle;


/******************************************************************************
 函数名称：communication
 功能描述：com1通讯任务
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者：
 日 	 期：2010-06-18
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void communication(void)
{
	INT8U i = 0;
	//  INT8U passwd_failed_times = 0;
	INT8U num;
	//  INT8U login_success = FALSE;
	INT8S ch;
	INT8U(*Func)(INT8U argc,INT8S **argv);
	INT8S cmdbuf[MAX_LEN_CMD_BUF + 1];


	InitCommands();
	cmdbuf[0] = '\0';

	armputs("\r\nWelcome to xuyan's shell.\r\n");

	armputs("\r\n#");                                   //显示提示符
	i = 0;

	EA_ucClearRS232(hRS232Handle);

	for (;;)
	{

		//  	do
		//  	{				  //only accept a-z,0-9,A-Z,.,space,/,-
		//  		ch = armgetch();
		//  	}while ( !((ch>='0'&&ch<='9')||(ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||(ch=='.')||(ch==' ')||(ch=='-')||(ch=='/')||(ch==ENTR)||(ch=='\b')||(ch==',')) );

		ch = armgetch();
		if ( ch == -1 )
			return;             //按取消键了
		if ( (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch == '.') || (ch == ' ') || (ch == '-')
			 || (ch == '/') || (ch == ENTR) || (ch == '\b') || (ch == ',') )
		{
			//do nothing
		}
		else
		{
			continue;
		}


		switch ( ch )
		{
		   case ENTR:              //enter
			   if ( i == 0 )
			   {                              //commandbuf is null,begin a new line
				   armputs("\r\n#");
			   }
			   else
			   {
				   if ( cmdbuf[i - 1] == ' ' )
					   i--;            //get rid of the end space
				   cmdbuf[i] = '\0';
				   num = CommandAnalys(cmdbuf);    //analys the argv in the commandbuf
				   if ( num == ERRORCOMMAND )
				   {              //error or none exist command
					   i = 0;
					   cmdbuf[i] = '\0';
					   armputs("\r\nWrong command.");
					   armputs("\r\n#");
				   }
				   else
				   {
					   Func = ShellComms[num].CommandFunc; //call corresponding CommandFunc
					   Func(argc, argv);
					   i = 0;
					   cmdbuf[i] = '\0';
					   armputs("\r\n#");
				   }
			   }
			   break;

		   case '\b':              //backspace
			   if ( i == 0 )
			   {        //has backed to first one
						//do nothing
			   }
			   else
			   {
				   i--;            //pointer back once
								   //  				armputch('\b');		//cursor back once
								   //  				armputch(' ');		  //earse last char in screen
								   //  				armputch('\b');		//cursor back again
				   armputs("\b \b");
			   }
			   break;

		   case ' ':               //don't allow continuous or begin space(' ')
			   if ( (cmdbuf[i - 1] == ' ') || (i == 0) || (i > MAX_LEN_CMD_BUF - 2) )
			   {
				   //do nothing
			   }
			   else
			   {
				   cmdbuf[i] = ch;
				   i++;
				   armputch(ch);  //display and store ch
			   }
			   break;

		   default:                //normal key
			   if ( i > MAX_LEN_CMD_BUF - 2 )
			   {    //the buf reached MAX
					//do nothing
			   }
			   else
			   {
				   cmdbuf[i] = ch;
				   i++;
				   armputch(ch);  //display and store ch
			   }
			   break;
		}  //switch
	}

}

/******************************************************************************
 函数名称：CommandAnalys
 功能描述：命令解析函数
 参数描述：
 参数名称： 输入/输出？ 类型		描述
 Buf			输入			INT8S *		接收到的字符串（要进行命令解析的字符串）
				
 返  回  值：无
				   
 作 	 者 ：
 日 	 期：2005-04-25
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
INT8U CommandAnalys(INT8S *Buf)
{
	INT8U i;
	INT8U pointer;
	INT8U num;
	INT8S name[30];     //command name length <20

	argc = 0;              //argc is global
	pointer = 0;
	num = 0;
	armputs("\r\n");

	while ( (Buf[pointer] != ' ') && (Buf[pointer] != '\0') && pointer < 20 )
	{
		name[pointer] = Buf[pointer];
		pointer++;
	}
	name[pointer] = '\0';   //now got the command name, and pointer is to the first space in the Buf

	for ( i = 0; i < MAX_COMMAND_NUM; i++ )
	{
		if ( !strcmp((void *)name, (void *)ShellComms[i].name) )
		{
			num = i;
			break;
		}               //find the command number
	}
	//not find it
	if ( i >= MAX_COMMAND_NUM )
	{
		return ERRORCOMMAND;
	}

	while ( Buf[pointer] != '\0' )
	{
		if ( Buf[pointer] == ' ' )
		{
			if ( argc > 0 )
			{
				Buf[pointer] = '\0';            //end of last argv
			}
			pointer++;
			argv[argc] = &Buf[pointer];         //add a parameter for every space
			argc++;
		}
		else
		{
			pointer++;
		}
	} //while

	return num;
}

/******************************************************************************
 函数名称：armputs
 功能描述：串口1输出函数
 参数描述：
 参数名称： 输入/输出？ 类型		描述
 snd_buf	输入			void *		要发送的字符串
				
 返  回  值：无
				   
 作 	 者 ：
 日 	 期：2005-04-07
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void armputs(const void *snd_buf)
{
	//  com1_send_hex((void *)snd_buf, (INT16U)strlen(snd_buf));
	(void)EA_ucWriteDevice(hRS232Handle, (INT32U)strlen(snd_buf), 0, (void *)snd_buf);
}

/******************************************************************************
 函数名称：arm_send_hex
 功能描述：串口1发送函数
 参数描述：
 参数名称： 输入/输出？ 类型		描述
 str_to_send	输入	INT8U *		要发送的字符串
 len			输入	INT32U		长度
				
 返  回  值：无
				   
 作 	 者 ：
 日 	 期：2005-04-07
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void arm_send_hex(INT8U *str_to_send, INT32U len)
{
	(void)EA_ucWriteDevice(hRS232Handle, len, 0, (void *)str_to_send);
}

/******************************************************************************
 函数名称：armputch
 功能描述：串口1输出函数
 参数描述：
 参数名称： 输入/输出？ 类型		描述
 ch			输入		INT8S		要发送的字节
				
 返  回  值：无
				   
 作 	 者 ：
 日 	 期：2005-04-07
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void armputch(INT8S ch)
{
	(void)EA_ucWriteDevice(hRS232Handle, 1, 0, (void *)&ch);
}

/*****************************************************************
函数原型：armgetch
功能描述：串口1接收
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------

返  回  值：串口得到的数
作      者：
日      期：2005-08-20
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U armgetch(void)
{
	INT16U key = EM_key_NOHIT;
	INT8U i = 0;
	INT32U rcv_len = 0;

	for (;;)
	{
		rcv_len = 0;
		i = EA_ucReadDevice(hRS232Handle, 1, 0, Com1RcvBuf2, &rcv_len);
		if ( i != EM_SUCCESS )
		{
			//  	return ucResult;	/* 错误处理 */
		}
		if ( rcv_len >= 1 )
		{
			return Com1RcvBuf2[0];
		}

		key = EA_ucKBHit();
		if ( key == EM_key_HIT )    //有按键
		{
			key = EA_uiInkey(1);    //读取按键键值
			if ( key == EM_key_CANCEL )
				return -1;  //退出
		}
	}

}

/*****************************************************************
函数原型：armgetch2
功能描述：串口1接收
参数描述：
参数名称：		输入/输出？	类型		描述
-----------		-----------	------	   	------- 
 timeout_ms		输入		INT32U		超时，单位毫秒
 ch				输出		INT8U *		得到的数

返  回  值：ok(0) - 成功 
			-1 - 按取消键了 
			-2 - 超时
作      者：
日      期：2005-08-20
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8S armgetch2(INT32U timeout_ms, INT8U *ch)
{
	INT16U key = EM_key_NOHIT;
	INT8U i = 0;
	INT32U rcv_len = 0;

	i = EA_ucReadDevice(hRS232Handle, 1, timeout_ms, Com1RcvBuf2, &rcv_len);
	if ( i != EM_SUCCESS )
	{
		return -2;              //超时
	}

	key = EA_ucKBHit();
	if ( key == EM_key_HIT )    //有按键
	{
		key = EA_uiInkey(1);    //读取按键键值
		if ( key == EM_key_CANCEL )
			return -1;  //退出
	}

	if ( rcv_len >= 1 )
	{
		*ch = Com1RcvBuf2[0];
		return ok;
	}

	return -2;
}

/*****************************************************************
函数原型：armgetch3
功能描述：串口1接收
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------

返  回  值：串口得到的数
作      者：
日      期：2005-08-20
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U armgetch3(void)
{
	//  INT16U key = EM_key_NOHIT;
	INT8U i = 0;
	INT32U rcv_len = 0;

	for (;;)
	{
		rcv_len = 0;
		i = EA_ucReadDevice(hRS232Handle, 1, 0, Com1RcvBuf2, &rcv_len);
		if ( i != EM_SUCCESS )
		{
			//  	return ucResult;	/* 错误处理 */
		}
		if ( rcv_len >= 1 )
		{
			return Com1RcvBuf2[0];
		}
	}

}

/******************************************************************************
 函数名称：InitCommands
 功能描述：命令初始化
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者 ：
 日 	 期：2005-04-07
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
INT8U InitCommands(void)
{
	//  INT8U i = 0;
	//
	//  for ( i = 0; i < MAX_COMMAND_NUM; i++ )
	//  {
	//  	ShellComms[i].name = NULL;
	//  }
	//
	//  ShellComms[0].num = 0;
	//  ShellComms[0].name = (INT8S *)"help";
	//  ShellComms[0].CommandFunc = HelpFunc;
	//
	//  ShellComms[1].num = 1;
	//  ShellComms[1].name = (INT8S *)"hello";
	//  ShellComms[1].CommandFunc = HelloFunc;
	//
	//  ShellComms[2].num = 2;
	//  ShellComms[2].name = (INT8S *)"flpage1";
	//  ShellComms[2].CommandFunc = FLPageFunc1;
	//
	//  ShellComms[3].num = 3;
	//  ShellComms[3].name = (INT8S *)"flpage0";
	//  ShellComms[3].CommandFunc = FLPageFunc0;
	//
	//  ShellComms[4].num = 4;
	//  ShellComms[4].name = (INT8S *)"devstat";
	//  ShellComms[4].CommandFunc = DevStatFunc;
	//
	//  ShellComms[5].num = 5;
	//  ShellComms[5].name = (INT8S *)"format";
	//  ShellComms[5].CommandFunc = FormatFunc;
	//
	//  ShellComms[6].num = 6;
	//  ShellComms[6].name = (INT8S *)"restart";
	//  ShellComms[6].CommandFunc = RestartFunc;
	//
	//  ShellComms[7].num = 7;
	//  ShellComms[7].name = (INT8S *)"flpage";
	//  ShellComms[7].CommandFunc = FLPageFunc;
	//
	//  ShellComms[8].num = 8;
	//  ShellComms[8].name = (INT8S *)"chkstk";
	//  ShellComms[8].CommandFunc = ChkStkFunc;
	//
	//  ShellComms[9].num = 9;
	//  ShellComms[9].name = (INT8S *)"eraseflash";
	//  ShellComms[9].CommandFunc = EraseFlashFunc;

	INT8U i = 0;

	for ( i = 0; i < MAX_COMMAND_NUM; i++ )
	{
		strcpy((void *)ShellComms[i].name, "");
	}

	ShellComms[0].num = 0;
	strcpy((void *)ShellComms[0].name, "help");
	ShellComms[0].CommandFunc = HelpFunc;

	ShellComms[1].num = 1;
	strcpy((void *)ShellComms[1].name, "hello");
	ShellComms[1].CommandFunc = HelloFunc;

	ShellComms[2].num = 2;
	strcpy((void *)ShellComms[2].name, "dir");
	ShellComms[2].CommandFunc = dir_func;

	ShellComms[3].num = 3;
	strcpy((void *)ShellComms[3].name, "fget");
	ShellComms[3].CommandFunc = fget_func;

	ShellComms[4].num = 4;
	strcpy((void *)ShellComms[4].name, "fput");
	ShellComms[4].CommandFunc = fput_func;

	ShellComms[5].num = 5;
	strcpy((void *)ShellComms[5].name, "del");
	ShellComms[5].CommandFunc = del_func;

	ShellComms[6].num = 6;
	strcpy((void *)ShellComms[6].name, "crf");
	ShellComms[6].CommandFunc = crf_func;


	return 0;
}

/******************************************************************************
 函数名称：HelpFunc
 功能描述：显示帮助
 参数名称： 输入/输出？ 类型        描述
 argc1		输入		INT8U 		参数数量
 argv1		输入		INT8S**		指向参数的指针

 返  回  值：ok(0)-成功
				 notok(0xFF)-失败

 作      者 ：
 日      期：2005-05-08
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
INT8U HelpFunc(INT8U argc1, INT8S **argv1)
{
	INT8U i = 0;

	armputs("\r\n");
	armputs("all commands:\r\n");
	for ( i = 0; i < MAX_COMMAND_NUM; i++ )
	{
		//  	if ( ShellComms[i].name != NULL )
		if ( ShellComms[i].name[0] != '\0' )
		{
			armputs(ShellComms[i].name);
			armputs("\r\n");
		}
	}

	return ok;
}

INT8U HelloFunc(INT8U argc1, INT8S **argv1)
{
	INT16S i;

	armputs("\r\nHello! I am xuyan's shell");
	armputs("\r\nyour argv is :");
	if ( argc1 > 0 )
	{
		for ( i = 0; i < argc1; i++ )
		{
			armputs("\r\n");
			armputs(argv1[i]);
		}
	}
	return ok;
}

///******************************************************************************
// 函数名称：FLPageFunc1
// 功能描述：显示FLASH1一页的内容
// 参数名称： 输入/输出？ 类型        描述
// argc       输入        INT8U           参数数量
// argv       输入        INT8S**     指向参数的指针
//
// 返  回  值：ok(0)-成功
//                 notok(0xFF)-失败
//
// 作      者 ：许岩
// 日      期：2005-05-08
// 修改历史：
//        日期        修改人      修改描述
//        ------      ---------   -------------
//******************************************************************************/
//INT8U FLPageFunc1(INT8U argc, INT8S **argv)
//{
////  INT8S buf[20];
////  INT8U page_buf[SECTOR_SIZE];
//    INT8S *end;
//    INT32S value;
////  INT16U i = 0;
//
//    ToggleWD();
//
//    if (argc == 0)
//    {
//        armputs("flpage1 <page number>\r\n");
//        armputs("page number: 0-4095\r\n");
//        armputs("For example: flpage1 0\r\n");
//        armputs("get data from page0.\r\n");
//        return notok;
//    }
//
//    value = strtol((void *)argv[0], (void *)&end, 10);
//
//    if (end == argv[0])
//    {   //输入的参数开头不是数字
//        armputs("flpage1 <page number>\r\n");
//        armputs("page number: 0-4095\r\n");
//        armputs("For example: flpage1 0\r\n");
//        armputs("get data from page0.\r\n");
//        return notok;
//    }
//
//    if ( value > 4095 )
//    {
//        armputs("Page number error!\r\n");
//        armputs("Page number range: 0 ~ 4095\r\n");
//        return notok;
//    }
//
//
//    FlashPageRead512(value, fpage_buf, 1);
//
//    com1_send_hex(fpage_buf, 256);
//    com1_send_hex(&fpage_buf[256], 256);
//    armputs("\r\n");
//
//    return ok;
//}

///******************************************************************************
// 函数名称：HostNameFunc
// 功能描述：显示收费机终端号
// 参数名称： 输入/输出？ 类型        描述
// argc       输入        INT8U           参数数量
// argv       输入        INT8S**     指向参数的指针
//
// 返  回  值：ok(0)-成功
//                 notok(0xFF)-失败
//
// 作      者 ：许岩
// 日      期：2005-05-08
// 修改历史：
//        日期        修改人      修改描述
//        ------      ---------   -------------
//******************************************************************************/
//INT8U HostNameFunc(INT8U argc,INT8S **argv)
//{
//    INT8S buf[100];
//    INT8S * end;
//    INT32S value;
//    INT8U i;
//
//    ToggleWD();
//
//    armputs("\r\n");
//    if (argc == 0)              //仅是显示而不作修改
//    {
//        sprintf((void *)buf, "POS number %u\n", DevStat.bus_number);
//        armputs(buf);
//        return ok;
//    }
//
//    value = strtol((void *)argv[0], (void *)&end, 10);
//
//    if (end == argv[0])
//    {   //输入的参数开头不是数字
//        armputs("Please Input the POS number you want to modify.\n");
//        armputs("For example: hostname 12345\n");
//        return notok;
//    }
//
//    if ( value > 65535 )
//    {
//        armputs("POS number error!\n");
//        armputs("POS number range: 0 ~ 65535\n");
//        return notok;
//    }
//
//    DevStat.bus_number = value;
//    i = SaveTypeBParamtoFlash(PRI, (void *)&DevStat.bus_number, BUS_NUMBER, sizeof(DevStat.bus_number));
//    if (i != FLOK)
//    {
//        armputs("Store POS number failed!\n");
//        return notok;
//    }
//
//    armputs("Success!\n");
//    sprintf((void *)buf, "POS number %u\n", DevStat.bus_number);
//    armputs(buf);
//
//    return ok;
//}
//
///******************************************************************************
// 函数名称：FlStatFunc
// 功能描述：显示flash状态
// 参数名称： 输入/输出？ 类型        描述
// argc       输入        INT8U           参数数量
// argv       输入        INT8S**     指向参数的指针
//
// 返  回  值：ok(0)-成功
//                 notok(0xFF)-失败
//
// 作      者 ：许岩
// 日      期：2005-05-08
// 修改历史：
//        日期        修改人      修改描述
//        ------      ---------   -------------
//******************************************************************************/
//INT8U FlStatFunc(INT8U argc, INT8S **argv)
//{
//    INT8S buf[100];
//    INT16U j = 4096;
//    INT8U i = 0;
//
//    ToggleWD();
//
//    armputs("\r\n");
//    i = Check_SST_29VF040(PRI);             //主FLASH检查
//    if ( i != ok)
//        armputs("Flash Error!\n");
//    else
//        armputs("Flash is OK.\n");
//
//    armputs("Flash type: SST39LV040\n");
//    armputs("Sector size: 128bytes\n");
//    sprintf((void *)buf, "Sector number: %d\n", j);
//    armputs(buf);
//    armputs("Flash size: 512Kbytes\n");
//
//    i = FLVerifyFormatSign(PRI);
//    if ( i != ok )
//        armputs("Flash has not been formatted!\n");
//    else
//        armputs("Flash has been formatted.\n");
//
//    return ok;
//}
//
///******************************************************************************
// 函数名称：DevStatFunc
// 功能描述：显示DevStat变量的所有内容
// 参数名称： 输入/输出？ 类型        描述
// argc       输入        INT8U           参数数量
// argv       输入        INT8S**     指向参数的指针
//
// 返  回  值：ok(0)-成功
//                 notok(0xFF)-失败
//
// 作      者 ：许岩
// 日      期：2005-05-08
// 修改历史：
//        日期        修改人      修改描述
//        ------      ---------   -------------
//******************************************************************************/
//INT8U DevStatFunc(INT8U argc, INT8S **argv)
//{
//    INT8S buf[100];
//
//    ToggleWD();
//
//    armputs("\r\n");
//    sprintf((void *)buf, "Size of variable DevStat: %u\n", sizeof(DevStat));
//    armputs(buf);
//    armputs("variable DevStat's content:\n");
//
//    sprintf((void *)buf, "DevStat.err_occured: %u\n", DevStat.err_occured);
//    armputs(buf);
//
//    sprintf((void *)buf, "DevStat.err_code: 0x%x\n", DevStat.err_code);
//    armputs(buf);
//
////     sprintf((void *)buf, "DevStat.flash_err: %u\n", DevStat.flash_err);
////     armputs(buf);
//
//    sprintf((void *)buf, "DevStat.allow_upload_data: %u\n", DevStat.allow_upload_data);
//    armputs(buf);
//
//    sprintf((void *)buf, "DevStat.databox_cmd_rcvd: %u\n", DevStat.databox_cmd_rcvd);
//    armputs(buf);
//
//    sprintf((void *)buf, "DevStat.purchase_again_req: %u\n", DevStat.purchase_again_req);
//    armputs(buf);
//
//
//    sprintf((void *)buf, "DevStat.record_base_ptr: %u\n", DevStat.record_base_ptr);
//    armputs(buf);
//
//    sprintf((void *)buf, "DevStat.record_number: %u\n", DevStat.record_number);
//    armputs(buf);
//
//    sprintf((void *)buf, "DevStat.last_record_base_ptr: %u\n", DevStat.last_record_base_ptr);
//    armputs(buf);
//
//    sprintf((void *)buf, "DevStat.last_record_number: %u\n", DevStat.last_record_number);
//    armputs(buf);
//
//
//    sprintf((void *)buf, "DevStat.bus_number: %u\n", DevStat.bus_number);
//    armputs(buf);
//
//    sprintf((void *)buf, "DevStat.black_name_number: %u\n", DevStat.black_name_number);
//    armputs(buf);
//
//    sprintf((void *)buf, "DevStat.typea_para_position: %u\n", DevStat.typea_para_position);
//    armputs(buf);
//
//    sprintf((void *)buf, "DevStat.flash_cycled: %u\n", DevStat.flash_cycled);
//    armputs(buf);
//
//    return ok;
//}
//
///******************************************************************************
// 函数名称：FormatFunc
// 功能描述：格式化FLASH
// 参数名称： 输入/输出？ 类型        描述
// argc       输入        INT8U           参数数量
// argv       输入        INT8S**     指向参数的指针
//
// 返  回  值：ok(0)-成功
//                 notok(0xFF)-失败
//
// 作      者 ：许岩
// 日      期：2005-05-08
// 修改历史：
//        日期        修改人      修改描述
//        ------      ---------   -------------
//******************************************************************************/
//INT8U FormatFunc(INT8U argc, INT8S **argv)
//{
//    INT8U i;
//    INT8U step1 = FALSE;
//    INT8U step2 = FALSE;
//    INT8U step3 = FALSE;
//    INT8U step4 = FALSE;
//    INT8S * end;
//    INT32S value;
//
//    ToggleWD();
//
//    armputs("\r\n");
//
//    if (argc == 0)              //如果没有参数，则执行全部流程
//    {
//        step1 = TRUE;
//        step2 = TRUE;
//        step3 = TRUE;
//        step4 = TRUE;
//    }
//    else
//    {
//        value = strtol((void *)argv[0], (void *)&end, 10);
//
//        if (end == argv[0])
//        {   //输入的参数开头不是数字
//            armputs("Please Input the step number you want to execute.\n");
//            armputs("For example: format 1\n");
//            return notok;
//        }
//
//        if ( (value > 4) || (value <= 0) )
//        {
//            armputs("Step number error!\n");
//            armputs("Step number range: 1 ~ 4\n");
//            return notok;
//        }
//
//        if (value == 1)                             //判断执行第几步
//        {
//            step1 = TRUE;
//        }
//        else if (value == 2)
//        {
//            step2 = TRUE;
//        }
//        else if (value == 3)
//        {
//            step3 = TRUE;
//        }
//        else
//        {
//            step4 = TRUE;
//        }
//    }
//
//    if (step1 == TRUE)
//    {
//        armputs("Start formatting...\n");
//
//        i = FlashFormat(PRI);
//        if (i != FLOK)
//        {
//            armputs("Failed!\n");
//            return notok;
//        }
//
//        armputs("Success!\n\n");
//    }
//
//    if (step2 == TRUE)
//    {
//        armputs("Check format sign...\n");
//
//        i = FLVerifyFormatSign(PRI);
//        if ( i != FLOK )
//            armputs("Flash has not been formatted!\n");
//        else
//            armputs("Flash has been formatted.\n");
//
//        armputs("Success!\n\n");
//    }
//
//    if (step3 == TRUE)
//    {
//        armputs("Initializing parameters...\n");
//
//        i = InitImportantParameter();
//        if ( i != FLOK )
//        {
//            armputs("Failed!\n");
//            return notok;
//        }
//        armputs("Success!\n\n");
//    }
//
//    if (step4 == TRUE)
//    {
//        armputs("Initializing RTC...\n");
//
//        i = Rx8025Init();
//        if (i != ok)
//        {
//            armputs("Failed!\n");
//            return notok;
//        }
//        armputs("Success!\n");
//    }
//
//    return ok;
//}
//
///******************************************************************************
// 函数名称：RestartFunc
// 功能描述：复位命令
// 参数名称： 输入/输出？ 类型        描述
// argc       输入        INT8U           参数数量
// argv       输入        INT8S**     指向参数的指针
//
// 返  回  值：ok(0)-成功
//                 notok(0xFF)-失败
//
// 作      者 ：许岩
// 日      期：2005-05-08
// 修改历史：
//        日期        修改人      修改描述
//        ------      ---------   -------------
//******************************************************************************/
//INT8U RestartFunc(INT8U argc, INT8S **argv)
//{
//    resetpos();             //复位
//
//    return ok;
//}
///******************************************************************************
// 函数名称：ChkStkFunc
// 功能描述：堆栈检查命令
// 参数名称： 输入/输出？ 类型        描述
// argc       输入        INT8U           参数数量
// argv       输入        INT8S**     指向参数的指针
//
// 返  回  值：ok(0)-成功
//                 notok(0xFF)-失败
//
// 作      者 ：许岩
// 日      期：2005-05-08
// 修改历史：
//        日期        修改人      修改描述
//        ------      ---------   -------------
//******************************************************************************/
//INT8U ChkStkFunc(INT8U argc, INT8S **argv)
//{
//    OS_STK_DATA stk_data;
//    INT8U err;
//    INT8S buf[20];
//
//    ToggleWD();
//
//    armputs("\n");
//
//    armputs("Task                       Prio  Total Stack  Free Stack  Used Stack \n");
//    armputs("-------------------------  ----  -----------  ----------  ---------- \n");
//    armputs("TaskStart               :  ");
//    err = OSTaskStkChk(TASK_START_PRIO, &stk_data);
//    if (err == OS_NO_ERR)
//    {
//        sprintf((void *)buf, "%4d  ", TASK_START_PRIO);         //print Prio
//        armputs(buf);
//        sprintf((void *)buf, "%11d  ", stk_data.OSFree + stk_data.OSUsed);          //print Total Stack
//        armputs(buf);
//        sprintf((void *)buf, "%10d  ", stk_data.OSFree);            //print Free Stack
//        armputs(buf);
//        sprintf((void *)buf, "%10d \n", stk_data.OSUsed);           //print Used Stack
//        armputs(buf);
//    }
//
//    armputs("Task_Beep               :  ");
//    err = OSTaskStkChk(TASK_BEEP_PRIO, &stk_data);
//    if (err == OS_NO_ERR)
//    {
//        sprintf((void *)buf, "%4d  ", TASK_BEEP_PRIO);          //print Prio
//        armputs(buf);
//        sprintf((void *)buf, "%11d  ", stk_data.OSFree + stk_data.OSUsed);          //print Total Stack
//        armputs(buf);
//        sprintf((void *)buf, "%10d  ", stk_data.OSFree);            //print Free Stack
//        armputs(buf);
//        sprintf((void *)buf, "%10d \n", stk_data.OSUsed);           //print Used Stack
//        armputs(buf);
//    }
//
//    armputs("Task_Display_Time       :  ");
//    err = OSTaskStkChk(TASK_DISPLAY_TIME_PRIO, &stk_data);
//    if (err == OS_NO_ERR)
//    {
//        sprintf((void *)buf, "%4d  ", TASK_DISPLAY_TIME_PRIO);          //print Prio
//        armputs(buf);
//        sprintf((void *)buf, "%11d  ", stk_data.OSFree + stk_data.OSUsed);          //print Total Stack
//        armputs(buf);
//        sprintf((void *)buf, "%10d  ", stk_data.OSFree);            //print Free Stack
//        armputs(buf);
//        sprintf((void *)buf, "%10d \n", stk_data.OSUsed);           //print Used Stack
//        armputs(buf);
//    }
//
//    armputs("Task_Indicated_Led      :  ");
//    err = OSTaskStkChk(TASK_INDICATED_LED_PRIO, &stk_data);
//    if (err == OS_NO_ERR)
//    {
//        sprintf((void *)buf, "%4d  ", TASK_INDICATED_LED_PRIO);         //print Prio
//        armputs(buf);
//        sprintf((void *)buf, "%11d  ", stk_data.OSFree + stk_data.OSUsed);          //print Total Stack
//        armputs(buf);
//        sprintf((void *)buf, "%10d  ", stk_data.OSFree);            //print Free Stack
//        armputs(buf);
//        sprintf((void *)buf, "%10d \n", stk_data.OSUsed);           //print Used Stack
//        armputs(buf);
//    }
//
////     armputs("Task_Key                :  ");
////     err = OSTaskStkChk(TASK_KEY_PRIO, &stk_data);
////     if (err == OS_NO_ERR)
////     {
////         sprintf((void *)buf, "%4d  ", TASK_KEY_PRIO);           //print Prio
////         armputs(buf);
////         sprintf((void *)buf, "%11d  ", stk_data.OSFree + stk_data.OSUsed);          //print Total Stack
////         armputs(buf);
////         sprintf((void *)buf, "%10d  ", stk_data.OSFree);            //print Free Stack
////         armputs(buf);
////         sprintf((void *)buf, "%10d \n", stk_data.OSUsed);           //print Used Stack
////         armputs(buf);
////     }
//
//    armputs("Task_Com0               :  ");
//    err = OSTaskStkChk(TASK_COM0_PRIO, &stk_data);
//    if (err == OS_NO_ERR)
//    {
//        sprintf((void *)buf, "%4d  ", TASK_COM0_PRIO);          //print Prio
//        armputs(buf);
//        sprintf((void *)buf, "%11d  ", stk_data.OSFree + stk_data.OSUsed);          //print Total Stack
//        armputs(buf);
//        sprintf((void *)buf, "%10d  ", stk_data.OSFree);            //print Free Stack
//        armputs(buf);
//        sprintf((void *)buf, "%10d \n", stk_data.OSUsed);           //print Used Stack
//        armputs(buf);
//    }
//
//    armputs("Task_Com1               :  ");
//    err = OSTaskStkChk(TASK_COM1_PRIO, &stk_data);
//    if (err == OS_NO_ERR)
//    {
//        sprintf((void *)buf, "%4d  ", TASK_COM1_PRIO);          //print Prio
//        armputs(buf);
//        sprintf((void *)buf, "%11d  ", stk_data.OSFree + stk_data.OSUsed);          //print Total Stack
//        armputs(buf);
//        sprintf((void *)buf, "%10d  ", stk_data.OSFree);            //print Free Stack
//        armputs(buf);
//        sprintf((void *)buf, "%10d \n", stk_data.OSUsed);           //print Used Stack
//        armputs(buf);
//    }
//
////     armputs("Task_Commu_Disptime_Ctrl:  ");
////     err = OSTaskStkChk(TASK_COMMU_DISPTIME_CTRL_PRIO, &stk_data);
////     if (err == OS_NO_ERR)
////     {
////         sprintf((void *)buf, "%4d  ", TASK_COMMU_DISPTIME_CTRL_PRIO);           //print Prio
////         armputs(buf);
////         sprintf((void *)buf, "%11d  ", stk_data.OSFree + stk_data.OSUsed);          //print Total Stack
////         armputs(buf);
////         sprintf((void *)buf, "%10d  ", stk_data.OSFree);            //print Free Stack
////         armputs(buf);
////         sprintf((void *)buf, "%10d \n", stk_data.OSUsed);           //print Used Stack
////         armputs(buf);
////     }
//
//    armputs("TaskGeneralCtrl         :  ");
//    err = OSTaskStkChk(TASK_GENCTRL_PRIO, &stk_data);
//    if (err == OS_NO_ERR)
//    {
//        sprintf((void *)buf, "%4d  ", TASK_GENCTRL_PRIO);           //print Prio
//        armputs(buf);
//        sprintf((void *)buf, "%11d  ", stk_data.OSFree + stk_data.OSUsed);          //print Total Stack
//        armputs(buf);
//        sprintf((void *)buf, "%10d  ", stk_data.OSFree);            //print Free Stack
//        armputs(buf);
//        sprintf((void *)buf, "%10d \n", stk_data.OSUsed);           //print Used Stack
//        armputs(buf);
//    }
//
//    return ok;
//}
//
///******************************************************************************
// 函数名称：EraseFlashFunc
// 功能描述：整片擦除FLASH命令
// 参数名称： 输入/输出？ 类型        描述
// argc       输入        INT8U           参数数量
// argv       输入        INT8S**     指向参数的指针
//
// 返  回  值：ok(0)-成功
//                 notok(0xFF)-失败
//
// 作      者 ：许岩
// 日      期：2005-05-08
// 修改历史：
//        日期        修改人      修改描述
//        ------      ---------   -------------
//******************************************************************************/
//INT8U EraseFlashFunc(INT8U argc, INT8S **argv)
//{
//    ToggleWD();
//
//    armputs("\r\n");
//    Erase_Entire_Chip(PRI);
//    armputs("Erase OK.\n");
//
//    return ok;
//}

///******************************************************************************
// 函数名称：FLPageFunc0
// 功能描述：显示FLASH0一页的内容
// 参数名称： 输入/输出？ 类型        描述
// argc       输入        INT8U           参数数量
// argv       输入        INT8S**     指向参数的指针
//
// 返  回  值：ok(0)-成功
//                 notok(0xFF)-失败
//
// 作      者 ：许岩
// 日      期：2005-05-08
// 修改历史：
//        日期        修改人      修改描述
//        ------      ---------   -------------
//******************************************************************************/
//INT8U FLPageFunc0(INT8U argc, INT8S **argv)
//{
////  INT8S buf[20];
////  INT8U page_buf[SECTOR_SIZE];
//    INT8S *end;
//    INT32S value;
//    INT16U i = 0;
//
//    ToggleWD();
//
//    if (argc == 0)
//    {
//        armputs("flpage0 <page number>\r\n");
//        armputs("page number: 0-4095\r\n");
//        armputs("For example: flpage0 0\r\n");
//        armputs("get data from page0.\r\n");
//        return notok;
//    }
//
//    value = strtol((void *)argv[0], (void *)&end, 10);
//
//    if (end == argv[0])
//    {   //输入的参数开头不是数字
//        armputs("flpage0 <page number>\r\n");
//        armputs("page number: 0-4095\r\n");
//        armputs("For example: flpage0 0\r\n");
//        armputs("get data from page0.\r\n");
//        return notok;
//    }
//
//    if ( value > 4095 )
//    {
//        armputs("Page number error!\r\n");
//        armputs("Page number range: 0 ~ 4095\r\n");
//        return notok;
//    }
//
//    FlashPageRead512(value, fpage_buf, 0);
//
//
////  for(i=0; i<15; i++)
////  {
////  	htoa(Com1SndBuf, fpage_buf + 34 * i, 34);
////  	armputs(Com1SndBuf);
////  	armputs("\r\n");
////  }
//    for(i=0; i<16; i++)
//    {
//        htoa(Com1SndBuf, fpage_buf + 32 * i, 32);
//        armputs(Com1SndBuf);
//        armputs("\r\n");
//    }
//
//
//    return ok;
//}

/******************************************************************************
 函数名称：dir_func
 功能描述：显示文件信息
 参数名称： 输入/输出？ 类型        描述
 argc1		输入		INT8U 		参数数量
 argv1		输入		INT8S**		指向参数的指针

 返  回  值：ok(0)-成功
				 notok(0xFF)-失败

 作      者 ：许岩
 日      期：2005-05-08
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
INT8U dir_func(INT8U argc1, INT8S **argv1)
{
	uchar ucResult;
	uint  uiFreeSpace;
	uint  uiFileNums;
	E_cfs_FileDetailInfo *ptFileInfo;
	INT8U buf[100];
	INT32U i = 0;

	// 获取文件个数
	uiFileNums = EA_iCfsGetFileNum();

	// 动态分配空间
	ptFileInfo = (E_cfs_FileDetailInfo *)EA_pvMalloc(uiFileNums * sizeof(E_cfs_FileDetailInfo));
	if ( ptFileInfo == NULL )
	{
		armputs("malloc error.\r\n");
		return notok;
	}

	// 获取文件信息
	ucResult = EA_iCfsGetFileSystemInfo(&uiFileNums, ptFileInfo, &uiFreeSpace);


	for ( i = 0; i < uiFileNums; i++ )
	{
		//  	sprintf((void *)buf, "%-20s%-20s%8d\r\n", ptFileInfo[i].acAreaName, ptFileInfo[i].cFileName, ptFileInfo[i].uiFileSize);
		sprintf((void *)buf, "%-20s%8d\r\n", ptFileInfo[i].cFileName, ptFileInfo[i].uiFileSize);
		armputs(buf);
	}

	EA_vFree(ptFileInfo);
	ptFileInfo = NULL;

	return ok;
}

/******************************************************************************
 函数名称：fget_func
 功能描述：取文件命令
 参数名称： 输入/输出？ 类型        描述
 argc1		输入		INT8U 		参数数量
 argv1		输入		INT8S**		指向参数的指针

 返  回  值：ok(0)-成功
				 notok(0xFF)-失败

 作      者 ：许岩
 日      期：2005-05-08
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
INT8U fget_func(INT8U argc1, INT8S **argv1)
{
	INT32U i = 0;
	EV_cfs_fHandle fHandle;
	int_64 file_len = 0;
	INT8U buf[200];
	INT8U file_name[50];
	INT8S ch;
	//  E_cfs_FileDetailInfo ptFileInfo;
	INT32U len = 0;
	INT16U crc = 0;
	INT8U pack_num = 1;
	//  INT32U send_times = 0;
	int_64 read_len = 0;
	INT32U pack_len = 1024;

	if ( argc1 == 0 )
	{
		//  	armputs("please input file name.\r\n");
		//  	return notok;

		strcpy((void *)file_name, "rec.txt");
	}
	else
	{
		//  strcpy((void *)buf, (void *)argv1[0]);
		strcpy((void *)file_name, (void *)argv1[0]);
	}


	//  fHandle = EA_iCfsOpen("rec.txt","w+");
	fHandle = EA_iCfsOpen((char *)file_name, "r");
	if ( fHandle == NULL )
	{
		armputs("open file failed.\r\n");
		//  	lcddisperr("打开rec.txt失败");
		return notok;
	}
	file_len = EA_i64CfsGetFileSize(fHandle);
	if ( file_len < 0 )
	{
		armputs("get file length failed.\r\n");
		lcddisperr("读取rec.txt长度失败");
		(void)EA_iCfsClose(fHandle);
		return notok;
	}

	//    //add for debug
	//    memset(Com1SndBuf, 0x00, sizeof(Com1SndBuf));
	//    i = 0;
	////  Com1SndBuf[i++] = 0x01;
	////  Com1SndBuf[i++] = 0x00;
	////  Com1SndBuf[i++] = 0xFF;
	//    Com1SndBuf[i++] = '1';
	//    Com1SndBuf[i++] = '2';
	//    Com1SndBuf[i++] = '3';
	//    Com1SndBuf[i++] = '.';
	//    Com1SndBuf[i++] = 't';
	//    Com1SndBuf[i++] = 'x';
	//    Com1SndBuf[i++] = 't';
	//    Com1SndBuf[i++] = '\0';
	//    Com1SndBuf[i++] = '1';
	//    Com1SndBuf[i++] = '0';
	//    Com1SndBuf[i++] = '\0';
	//
	////  crc = Cal_Crc(Com1SndBuf, 131);
	//    crc = Cal_CRC16(Com1SndBuf, 128);
	//    sprintf((void *)buf, "%04x\r\n", crc);
	//    armputs(buf);
	//    return ok;
	//    //debug end

	EA_ucClearRS232(hRS232Handle);

	for (;;)
	{
		ch = armgetch();
		if ( ch == -1 )
		{
			(void)EA_iCfsClose(fHandle);
			return notok;             //按取消键了
		}
		if ( ch == 'C' )
			break;
	}

	//发送文件名和长度
	memset(Com1SndBuf, 0x00, sizeof(Com1SndBuf));
	Com1SndBuf[0] = SOH;
	Com1SndBuf[1] = 0x00;
	Com1SndBuf[2] = 0xFF;
	sprintf((void *)&Com1SndBuf[3], "%s", file_name);
	i = 3 + strlen((void *)file_name) + 1;
	sprintf((void *)buf, "%lld", file_len);
	len = strlen((void *)buf);
	memcpy(&Com1SndBuf[i], buf, len + 1);
	i = i + len + 1;
	memset(&Com1SndBuf[i], 0x00, 128);
	crc = Cal_CRC16(&Com1SndBuf[3], 128);
	Com1SndBuf[131] = (INT8U)(crc >> 8);
	Com1SndBuf[132] = (INT8U)crc;


	for (;;)
	{
		arm_send_hex(Com1SndBuf, 133);
		ch = armgetch();
		if ( ch == -1 )
		{
			(void)EA_iCfsClose(fHandle);
			return notok;             //按取消键了
		}
		if ( ch == ACK )
			break;
	}

	for (;;)
	{
		ch = armgetch();
		if ( ch == -1 )
		{
			(void)EA_iCfsClose(fHandle);
			return notok;             //按取消键了
		}
		if ( ch == 'C' )
			break;
	}

	//  //发送文件内容
	//  for(;;)
	//  {
	//  	Com1SndBuf[0] = SOH;
	//  	Com1SndBuf[1] = pack_num;
	//  	Com1SndBuf[2] = ~pack_num;
	//  	read_len = EA_i64CfsRead(fHandle, &Com1SndBuf[3], 128);
	//  	if (read_len < 128)
	//  	{
	//  		//最后一包
	//  		memset(&Com1SndBuf[3 + read_len], 0x1A, 128 - read_len);
	//  		crc = Cal_CRC16(&Com1SndBuf[3], 128);
	//  		Com1SndBuf[131] = (INT8U)(crc >> 8);
	//  		Com1SndBuf[132] = (INT8U)crc;
	//
	//  		for (;;)
	//  		{
	//  			arm_send_hex(Com1SndBuf, 133);
	//  			ch = armgetch();
	//  			if ( ch == -1 )
	//  			{
	//  				armputs("\x18\x18\x18\x18\x18");
	//  				(void)EA_iCfsClose(fHandle);
	//  				return notok;             //按取消键了
	//  			}
	//  			if ( ch == ACK )
	//  				break;
	//  		}
	//
	//  		break;
	//  	}
	//  	crc = Cal_CRC16(&Com1SndBuf[3], 128);
	//  	Com1SndBuf[131] = (INT8U)(crc >> 8);
	//  	Com1SndBuf[132] = (INT8U)crc;
	//
	//  	for (;;)
	//  	{
	//  		arm_send_hex(Com1SndBuf, 133);
	//  		ch = armgetch();
	//  		if ( ch == -1 )
	//  		{
	//  			armputs("\x18\x18\x18\x18\x18");
	//  			(void)EA_iCfsClose(fHandle);
	//  			return notok;             //按取消键了
	//  		}
	//  		if ( ch == ACK )
	//  			break;
	//  	}
	//
	//  	pack_num++;
	//  }

	//发送文件内容
	for (;;)
	{
		Com1SndBuf[0] = STX;
		Com1SndBuf[1] = pack_num;
		Com1SndBuf[2] = ~pack_num;
		read_len = EA_i64CfsRead(fHandle, &Com1SndBuf[3], pack_len);
		if ( read_len < pack_len )
		{
			//最后一包
			memset(&Com1SndBuf[3 + read_len], 0x1A, pack_len - read_len);
			crc = Cal_CRC16(&Com1SndBuf[3], pack_len);
			Com1SndBuf[3 + pack_len] = (INT8U)(crc >> 8);
			Com1SndBuf[3 + pack_len + 1] = (INT8U)crc;

			for (;;)
			{
				arm_send_hex(Com1SndBuf, pack_len + 5);
				ch = armgetch();
				if ( ch == -1 )
				{
					armputs("\x18\x18\x18\x18\x18");
					(void)EA_iCfsClose(fHandle);
					return notok;             //按取消键了
				}
				if ( ch == ACK )
					break;
			}

			break;
		}
		crc = Cal_CRC16(&Com1SndBuf[3], pack_len);
		Com1SndBuf[3 + pack_len] = (INT8U)(crc >> 8);
		Com1SndBuf[3 + pack_len + 1] = (INT8U)crc;

		for (;;)
		{
			arm_send_hex(Com1SndBuf, pack_len + 5);
			ch = armgetch();
			if ( ch == -1 )
			{
				armputs("\x18\x18\x18\x18\x18");
				(void)EA_iCfsClose(fHandle);
				return notok;             //按取消键了
			}
			if ( ch == CA )
			{
				armputs("\x18\x18\x18\x18\x18");
				(void)EA_iCfsClose(fHandle);
				return notok;             //传输被中断
			}
			if ( ch == ACK )
				break;
		}

		pack_num++;
	}


	//发送文件结束标志
	for (;;)
	{
		armputch(EOT);
		ch = armgetch();
		if ( ch == -1 )
		{
			(void)EA_iCfsClose(fHandle);
			return notok;             //按取消键了
		}
		if ( ch == ACK )
			break;
	}


	for (;;)
	{
		ch = armgetch();
		if ( ch == -1 )
		{
			(void)EA_iCfsClose(fHandle);
			return notok;             //按取消键了
		}
		if ( ch == 'C' )
			break;
	}


	//发送文件名和长度
	memset(Com1SndBuf, 0x00, sizeof(Com1SndBuf));
	Com1SndBuf[0] = SOH;
	Com1SndBuf[1] = 0x00;
	Com1SndBuf[2] = 0xFF;
	crc = Cal_CRC16(&Com1SndBuf[3], 128);
	Com1SndBuf[131] = (INT8U)(crc >> 8);
	Com1SndBuf[132] = (INT8U)crc;


	for (;;)
	{
		arm_send_hex(Com1SndBuf, 133);
		ch = armgetch();
		if ( ch == -1 )
		{
			(void)EA_iCfsClose(fHandle);
			return notok;             //按取消键了
		}
		if ( ch == ACK )
			break;
	}





	(void)EA_iCfsClose(fHandle);
	return ok;
}

/*******************************************************************************
  * @函数名称    UpdateCRC16
  * @函数说明   更新输入数据的CRC校验
  * @输入参数   crcIn
				byte
  * @输出参数   无
  * @返回参数   CRC校验值
*******************************************************************************/
uint16 UpdateCRC16(uint16 crcIn, uint8 byte)
{
	uint32 crc = crcIn;
	uint32 in = byte | 0x100;
	do
	{
		crc <<= 1;
		in <<= 1;
		if ( in & 0x100 )
			++crc;
		if ( crc & 0x10000 )
			crc ^= 0x1021;
	}
	while ( !(in & 0x10000) );
	return crc & 0xffffu;
}

/*******************************************************************************
  * @函数名称    UpdateCRC16
  * @函数说明   更新输入数据的CRC校验
  * @输入参数   data ：数据
				size ：长度
  * @输出参数   无
  * @返回参数   CRC校验值
*******************************************************************************/
uint16 Cal_CRC16(const uint8 *data, uint32 size)
{
	uint32 crc = 0;
	const uint8 *dataEnd = data + size;

	while ( data < dataEnd )
		crc = UpdateCRC16(crc, *data++);

	crc = UpdateCRC16(crc, 0);
	crc = UpdateCRC16(crc, 0);
	return crc & 0xffffu;
}

/******************************************************************************
 函数名称：fput_func
 功能描述：放文件命令
 参数名称： 输入/输出？ 类型        描述
 argc1		输入		INT8U 		参数数量
 argv1		输入		INT8S**		指向参数的指针

 返  回  值：ok(0)-成功
				 notok(0xFF)-失败

 作      者 ：许岩
 日      期：2005-05-08
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
INT8U fput_func(INT8U argc1, INT8S **argv1)
{
	INT32U i = 0;
	EV_cfs_fHandle fHandle;
	int_64 file_len = 0;
	INT8U buf[200];
	INT8U file_name[150];
	INT8S ch;
	//  E_cfs_FileDetailInfo ptFileInfo;
	//  INT32U len = 0;
	//  INT16U crc = 0;
	//  INT8U pack_num = 1;
	//  INT32U send_times = 0;
	//  int_64 read_len = 0;
	INT8S j = 0;
	INT8U m = 0;
	INT32U rcv_len = 0;
	int_64 iWriteSize = 0;
	int a = 0;

	//  if ( argc1 == 0 )
	//  {
	//  	armputs("please input file name.\r\n");
	//  	return notok;
	//  }
	//  //  strcpy((void *)buf, (void *)argv1[0]);
	//  strcpy((void *)file_name, (void *)argv1[0]);
	//
	//
	//  //  fHandle = EA_iCfsOpen("rec.txt","w+");
	//  fHandle = EA_iCfsOpen((char *)file_name, "w+");
	//  if ( fHandle == NULL )
	//  {
	//  	armputs("create file failed.\r\n");
	//  	lcddisperr("建立文件失败");
	//  	return notok;
	//  }

	EA_ucClearRS232(hRS232Handle);



	//收包头
	for (;;)
	{
		armputch('C');

		j = armgetch2(1, (INT8U *)&ch);
		if ( j == -1 )
		{
			//  		(void)EA_iCfsClose(fHandle);
			armputs("\x18\x18\x18\x18\x18");
			return notok;             //按取消键了
		}
		else if ( j == -2 )
		{
			continue;
		}

		if ( ch == SOH )
		{
			Com1RcvBuf[0] = ch;
			for ( i = 0; i < 128 + 4; i++ )
			{
				Com1RcvBuf[1 + i] = armgetch();
			}
			m = test_crc16(&Com1RcvBuf[3], 128);
			if ( m != ok )
				continue;
			else
				break;

		}
		else
		{
			continue;
		}


	}

	armputch(ACK);

	if ( strlen((void *)&Com1RcvBuf[3]) > 19 )
		Com1RcvBuf[3 + 19] = '\0';
	strcpy((void *)file_name, (void *)&Com1RcvBuf[3]);
	fHandle = EA_iCfsOpen((char *)file_name, "w+");
	if ( fHandle == NULL )
	{
		armputs("\x18\x18\x18\x18\x18");
		armputs("create file failed.\r\n");
		lcddisperr("建立文件失败");
		return notok;
	}
	strcpy((void *)buf, (void *)&Com1RcvBuf[3 + strlen((void *)file_name) + 1]);
	sscanf((void *)buf, "%lld", &file_len);


	//收数据
	armputch('C');

	for (;;)
	{
		ch = armgetch();
		if ( ch == EOT )
		{
			armputch(ACK);
			break;
		}
		else if ( ch == SOH )
		{
			rcv_len = 128;
		}
		else if ( ch == STX )
		{
			rcv_len = 1024;
		}
		else if ( ch == ABORT1 || ch == ABORT2 )
		{
			(void)EA_iCfsClose(fHandle);
			armputs("\x18\x18\x18\x18\x18");
			armputs("transmission aborted.\r\n");
			lcddisperr("传输中断");
			return notok;
		}


		Com1RcvBuf[0] = ch;
		for ( i = 0; i < rcv_len + 4; i++ )
		{
			Com1RcvBuf[1 + i] = armgetch();
		}
		m = test_crc16(&Com1RcvBuf[3], rcv_len);
		if ( m == ok )
		{
			iWriteSize = EA_i64CfsWrite(fHandle, &Com1RcvBuf[3], rcv_len);      //写入文件
			if ( iWriteSize < 0 )
			{
				(void)EA_iCfsClose(fHandle);
				armputs("\x18\x18\x18\x18\x18");
				armputs("transmission aborted.\r\n");
				lcddisperr("写文件失败");
				return notok;
			}
			armputch(ACK);
			continue;
		}
		else
		{
			armputch(NAK);
			continue;
		}

	}

	//结尾部分
	armputch('C');
	for ( i = 0; i < 128 + 5; i++ )
	{
		Com1RcvBuf[i] = armgetch();
	}
	armputch(ACK);

	a = EA_iCfsFtruncate(fHandle, file_len);
	if ( a < 0 )
	{
		(void)EA_iCfsClose(fHandle);
		lcddisperr("修改文件大小失败");
		return notok;
	}

	a = EA_iCfsFsync(fHandle);
	if ( a < 0 )
	{
		lcddisperr("同步文件失败");
		(void)EA_iCfsClose(fHandle);
		return notok;
	}

	a = EA_iCfsClose(fHandle);
	if ( a < 0 )
	{
		lcddisperr("关闭文件失败");
		return notok;
	}

	return ok;
}

/*****************************************************************
 函数原型：test_crc16
 功能描述：判断一包数据的crc是否正确
 参数描述：
 参数名称：	输入/输出？	类型		描述
 data_to_cal_crc	输入	INT8U const *		要进行CRC计算的数组的起始指针
 len				输入	INT32U				要进行CRC计算的数组的长度

 返  回  值：ok(0) - 成功
			notok(0xff) - 失败
 作      者	：许岩
 日      期：2004-02-04
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
*****************************************************************/
INT8U test_crc16(INT8U const *data_to_cal_crc, INT32U len)
{
	INT16U crc;
	INT8U crc_high = 0;
	INT8U crc_low = 0;

	crc = Cal_CRC16(data_to_cal_crc, len);

	crc_high = (INT8U)(crc >> 8);
	crc_low = (INT8U)(crc);

	//  //add for debug
	//  sprintf((void *)Com1SndBuf, "%04X", crc);
	//  armputs((void *)Com1SndBuf);
	//  //debug end

	if ( crc_high == data_to_cal_crc[len] && crc_low == data_to_cal_crc[len + 1] )
	{
		return ok;
	}
	else
	{
		return notok;
	}

}

/******************************************************************************
 函数名称：del_func
 功能描述：删除文件命令
 参数名称： 输入/输出？ 类型        描述
 argc1		输入		INT8U 		参数数量
 argv1		输入		INT8S**		指向参数的指针

 返  回  值：ok(0)-成功
				 notok(0xFF)-失败

 作      者 ：许岩
 日      期：2005-05-08
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
INT8U del_func(INT8U argc1, INT8S **argv1)
{
	//  INT32U i = 0;
	//  EV_cfs_fHandle fHandle;
	//  int_64 file_len = 0;
	//  INT8U buf[200];
	INT8U file_name[50];
	//  INT8S ch;
	//  E_cfs_FileDetailInfo ptFileInfo;
	//  INT32U len = 0;
	//  INT16U crc = 0;
	//  INT8U pack_num = 1;
	//  INT32U send_times = 0;
	//  int_64 read_len = 0;
	//  INT32U pack_len = 1024;
	int a = 0;

	if ( argc1 == 0 )
	{
		armputs("please input file name.\r\n");
		return notok;
	}
	//  strcpy((void *)buf, (void *)argv1[0]);
	strcpy((void *)file_name, (void *)argv1[0]);

	//  //add for debug
	//  buf[0] = 0x01;
	//  buf[1] = 0x00;
	//  a = EA_iCfsDelete((char *)buf);
	//  if (a < 0)
	//  {
	//  	armputs("delete file failed.\r\n");
	//  	return notok;
	//  }
	//  //debug end

	a = EA_iCfsDelete((char *)file_name);
	if ( a < 0 )
	{
		armputs("delete file failed.\r\n");
		return notok;
	}


	armputs("delete file success.\r\n");

	return ok;
}

/******************************************************************************
 函数名称：crf_func
 功能描述：清除记录文件命令
 参数名称： 输入/输出？ 类型        描述
 argc1		输入		INT8U 		参数数量
 argv1		输入		INT8S**		指向参数的指针

 返  回  值：ok(0)-成功
				 notok(0xFF)-失败

 作      者 ：许岩
 日      期：2005-05-08
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
INT8U crf_func(INT8U argc1, INT8S **argv1)
{
	//  INT32U i = 0;
	//  EV_cfs_fHandle fHandle;
	//  int_64 file_len = 0;
	//  INT8U buf[200];
	//  INT8U file_name[50];
	//  INT8S ch;
	//  E_cfs_FileDetailInfo ptFileInfo;
	//  INT32U len = 0;
	//  INT16U crc = 0;
	//  INT8U pack_num = 1;
	//  INT32U send_times = 0;
	//  int_64 read_len = 0;
	//  INT32U pack_len = 1024;
	//  int a = 0;
	//  const char recFileName[] = "record";
	INT8U ucOpenID0 = 0;
	INT8U ucOpenID1 = 0;
	INT32U total_rec_num = 0;
	INT32U uiResult;
	INT32U uiReadCnt;
	INT32U i = 0;
	TICKET_INFO ticket;
	uchar m = 0;

	//  if ( argc1 == 0 )
	//  {
	//  	armputs("please input file name.\r\n");
	//  	return notok;
	//  }
	//  //  strcpy((void *)buf, (void *)argv1[0]);
	//  strcpy((void *)file_name, (void *)argv1[0]);

	// 打开文件
	if ( EA_ucPFOpen((INT8U *)"record", &ucOpenID0) != EM_ffs_SUCCESS )
	{
		armputs("open file failed.\r\n");
		return notok;
	}

	// 打开文件
	if ( EA_ucPFOpen((INT8U *)"backup", &ucOpenID1) != EM_ffs_SUCCESS )
	{
		armputs("open file failed.\r\n");
		(void)EA_ucPFClose(ucOpenID0);
		return notok;
	}

	//倒数据
	m = EA_ucPFInfo(ucOpenID0, EM_ffs_VALIDREC, &uiResult);
	if ( m != EM_ffs_SUCCESS )
	{
		armputs("read record file failed.\r\n");
		(void)EA_ucPFClose(ucOpenID0);
		(void)EA_ucPFClose(ucOpenID1);
		return notok;
	}
	total_rec_num = uiResult;

	for ( i = 0; i < total_rec_num; i++ )
	{
		if ( EA_ucPFReadRec(ucOpenID0, i + 1, 1, &uiReadCnt, &ticket.pos_number[0]) != EM_ffs_SUCCESS )
		{
			armputs("read record file failed.\r\n");
			(void)EA_ucPFClose(ucOpenID0);
			(void)EA_ucPFClose(ucOpenID1);
			return notok;
		}

		if ( EA_ucPFWriteRec(ucOpenID1, 0, &ticket.pos_number[0]) != EM_ffs_SUCCESS )
		{
			armputs("write backup file failed.\r\n");
			(void)EA_ucPFClose(ucOpenID0);
			(void)EA_ucPFClose(ucOpenID1);
			return notok;
		}
	}

	(void)EA_ucPFClose(ucOpenID0);
	(void)EA_ucPFClose(ucOpenID1);

	//删除重建记录文件
	m = EA_ucPFDel((INT8U *)"record");
	if ( m != EM_ffs_SUCCESS )
	{
		armputs("delete record file failed.\r\n");
		return notok;
	}
	m = EA_ucPFCreate((INT8U *)"record", sizeof(TICKET_INFO));
	if ( m != EM_ffs_SUCCESS )
	{
		armputs("create record file failed.\r\n");
		return notok;
	}


	armputs("clean record file success.\r\n");

	return ok;
}

