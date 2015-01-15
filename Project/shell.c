/******************************************************************************
Copyright 2005 
All rights riserved.

�ļ���	��shell.c
ģ�����ƣ�com1ͨѶ���� �˻�����
���ܸ�Ҫ��com1ͨѶ���� �˻�����

ȡ���汾��0.0.1
�޸���	��
������ڣ�2005.04.06
����˵����create

******************************************************************************/
#include 		"main.h"

extern volatile DEV_STAT DevStat;

INT8U Com1SndBuf[UART1_SEND_QUEUE_LENGTH];              //���ڷ��ͻ�����
INT8U Com1RcvBuf[UART1_RCV_QUEUE_LENGTH];               //���ڽ��ջ�����
INT8U Com1RcvBuf2[UART1_RCV_QUEUE_LENGTH];               //���ڽ��ջ�����

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
 �������ƣ�communication
 ����������com1ͨѶ����
 ����������
 �������ƣ� ����/����� ����		����
				
 ��  ��  ֵ����
				   
 �� 	 �ߣ�
 �� 	 �ڣ�2010-06-18
 �޸���ʷ��
		����		�޸���		�޸�����
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

	armputs("\r\n#");                                   //��ʾ��ʾ��
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
			return;             //��ȡ������
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
 �������ƣ�CommandAnalys
 ���������������������
 ����������
 �������ƣ� ����/����� ����		����
 Buf			����			INT8S *		���յ����ַ�����Ҫ��������������ַ�����
				
 ��  ��  ֵ����
				   
 �� 	 �� ��
 �� 	 �ڣ�2005-04-25
 �޸���ʷ��
		����		�޸���		�޸�����
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
 �������ƣ�armputs
 ��������������1�������
 ����������
 �������ƣ� ����/����� ����		����
 snd_buf	����			void *		Ҫ���͵��ַ���
				
 ��  ��  ֵ����
				   
 �� 	 �� ��
 �� 	 �ڣ�2005-04-07
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void armputs(const void *snd_buf)
{
	//  com1_send_hex((void *)snd_buf, (INT16U)strlen(snd_buf));
	(void)EA_ucWriteDevice(hRS232Handle, (INT32U)strlen(snd_buf), 0, (void *)snd_buf);
}

/******************************************************************************
 �������ƣ�arm_send_hex
 ��������������1���ͺ���
 ����������
 �������ƣ� ����/����� ����		����
 str_to_send	����	INT8U *		Ҫ���͵��ַ���
 len			����	INT32U		����
				
 ��  ��  ֵ����
				   
 �� 	 �� ��
 �� 	 �ڣ�2005-04-07
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void arm_send_hex(INT8U *str_to_send, INT32U len)
{
	(void)EA_ucWriteDevice(hRS232Handle, len, 0, (void *)str_to_send);
}

/******************************************************************************
 �������ƣ�armputch
 ��������������1�������
 ����������
 �������ƣ� ����/����� ����		����
 ch			����		INT8S		Ҫ���͵��ֽ�
				
 ��  ��  ֵ����
				   
 �� 	 �� ��
 �� 	 �ڣ�2005-04-07
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void armputch(INT8S ch)
{
	(void)EA_ucWriteDevice(hRS232Handle, 1, 0, (void *)&ch);
}

/*****************************************************************
����ԭ�ͣ�armgetch
��������������1����
����������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------

��  ��  ֵ�����ڵõ�����
��      �ߣ�
��      �ڣ�2005-08-20
�޸���ʷ��
����		�޸���		�޸�����
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
			//  	return ucResult;	/* ������ */
		}
		if ( rcv_len >= 1 )
		{
			return Com1RcvBuf2[0];
		}

		key = EA_ucKBHit();
		if ( key == EM_key_HIT )    //�а���
		{
			key = EA_uiInkey(1);    //��ȡ������ֵ
			if ( key == EM_key_CANCEL )
				return -1;  //�˳�
		}
	}

}

/*****************************************************************
����ԭ�ͣ�armgetch2
��������������1����
����������
�������ƣ�		����/�����	����		����
-----------		-----------	------	   	------- 
 timeout_ms		����		INT32U		��ʱ����λ����
 ch				���		INT8U *		�õ�����

��  ��  ֵ��ok(0) - �ɹ� 
			-1 - ��ȡ������ 
			-2 - ��ʱ
��      �ߣ�
��      �ڣ�2005-08-20
�޸���ʷ��
����		�޸���		�޸�����
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
		return -2;              //��ʱ
	}

	key = EA_ucKBHit();
	if ( key == EM_key_HIT )    //�а���
	{
		key = EA_uiInkey(1);    //��ȡ������ֵ
		if ( key == EM_key_CANCEL )
			return -1;  //�˳�
	}

	if ( rcv_len >= 1 )
	{
		*ch = Com1RcvBuf2[0];
		return ok;
	}

	return -2;
}

/*****************************************************************
����ԭ�ͣ�armgetch3
��������������1����
����������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------

��  ��  ֵ�����ڵõ�����
��      �ߣ�
��      �ڣ�2005-08-20
�޸���ʷ��
����		�޸���		�޸�����
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
			//  	return ucResult;	/* ������ */
		}
		if ( rcv_len >= 1 )
		{
			return Com1RcvBuf2[0];
		}
	}

}

/******************************************************************************
 �������ƣ�InitCommands
 ���������������ʼ��
 ����������
 �������ƣ� ����/����� ����		����
				
 ��  ��  ֵ����
				   
 �� 	 �� ��
 �� 	 �ڣ�2005-04-07
 �޸���ʷ��
		����		�޸���		�޸�����
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
 �������ƣ�HelpFunc
 ������������ʾ����
 �������ƣ� ����/����� ����        ����
 argc1		����		INT8U 		��������
 argv1		����		INT8S**		ָ�������ָ��

 ��  ��  ֵ��ok(0)-�ɹ�
				 notok(0xFF)-ʧ��

 ��      �� ��
 ��      �ڣ�2005-05-08
 �޸���ʷ��
		����        �޸���      �޸�����
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
// �������ƣ�FLPageFunc1
// ������������ʾFLASH1һҳ������
// �������ƣ� ����/����� ����        ����
// argc       ����        INT8U           ��������
// argv       ����        INT8S**     ָ�������ָ��
//
// ��  ��  ֵ��ok(0)-�ɹ�
//                 notok(0xFF)-ʧ��
//
// ��      �� ������
// ��      �ڣ�2005-05-08
// �޸���ʷ��
//        ����        �޸���      �޸�����
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
//    {   //����Ĳ�����ͷ��������
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
// �������ƣ�HostNameFunc
// ������������ʾ�շѻ��ն˺�
// �������ƣ� ����/����� ����        ����
// argc       ����        INT8U           ��������
// argv       ����        INT8S**     ָ�������ָ��
//
// ��  ��  ֵ��ok(0)-�ɹ�
//                 notok(0xFF)-ʧ��
//
// ��      �� ������
// ��      �ڣ�2005-05-08
// �޸���ʷ��
//        ����        �޸���      �޸�����
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
//    if (argc == 0)              //������ʾ�������޸�
//    {
//        sprintf((void *)buf, "POS number %u\n", DevStat.bus_number);
//        armputs(buf);
//        return ok;
//    }
//
//    value = strtol((void *)argv[0], (void *)&end, 10);
//
//    if (end == argv[0])
//    {   //����Ĳ�����ͷ��������
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
// �������ƣ�FlStatFunc
// ������������ʾflash״̬
// �������ƣ� ����/����� ����        ����
// argc       ����        INT8U           ��������
// argv       ����        INT8S**     ָ�������ָ��
//
// ��  ��  ֵ��ok(0)-�ɹ�
//                 notok(0xFF)-ʧ��
//
// ��      �� ������
// ��      �ڣ�2005-05-08
// �޸���ʷ��
//        ����        �޸���      �޸�����
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
//    i = Check_SST_29VF040(PRI);             //��FLASH���
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
// �������ƣ�DevStatFunc
// ������������ʾDevStat��������������
// �������ƣ� ����/����� ����        ����
// argc       ����        INT8U           ��������
// argv       ����        INT8S**     ָ�������ָ��
//
// ��  ��  ֵ��ok(0)-�ɹ�
//                 notok(0xFF)-ʧ��
//
// ��      �� ������
// ��      �ڣ�2005-05-08
// �޸���ʷ��
//        ����        �޸���      �޸�����
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
// �������ƣ�FormatFunc
// ������������ʽ��FLASH
// �������ƣ� ����/����� ����        ����
// argc       ����        INT8U           ��������
// argv       ����        INT8S**     ָ�������ָ��
//
// ��  ��  ֵ��ok(0)-�ɹ�
//                 notok(0xFF)-ʧ��
//
// ��      �� ������
// ��      �ڣ�2005-05-08
// �޸���ʷ��
//        ����        �޸���      �޸�����
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
//    if (argc == 0)              //���û�в�������ִ��ȫ������
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
//        {   //����Ĳ�����ͷ��������
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
//        if (value == 1)                             //�ж�ִ�еڼ���
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
// �������ƣ�RestartFunc
// ������������λ����
// �������ƣ� ����/����� ����        ����
// argc       ����        INT8U           ��������
// argv       ����        INT8S**     ָ�������ָ��
//
// ��  ��  ֵ��ok(0)-�ɹ�
//                 notok(0xFF)-ʧ��
//
// ��      �� ������
// ��      �ڣ�2005-05-08
// �޸���ʷ��
//        ����        �޸���      �޸�����
//        ------      ---------   -------------
//******************************************************************************/
//INT8U RestartFunc(INT8U argc, INT8S **argv)
//{
//    resetpos();             //��λ
//
//    return ok;
//}
///******************************************************************************
// �������ƣ�ChkStkFunc
// ������������ջ�������
// �������ƣ� ����/����� ����        ����
// argc       ����        INT8U           ��������
// argv       ����        INT8S**     ָ�������ָ��
//
// ��  ��  ֵ��ok(0)-�ɹ�
//                 notok(0xFF)-ʧ��
//
// ��      �� ������
// ��      �ڣ�2005-05-08
// �޸���ʷ��
//        ����        �޸���      �޸�����
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
// �������ƣ�EraseFlashFunc
// ������������Ƭ����FLASH����
// �������ƣ� ����/����� ����        ����
// argc       ����        INT8U           ��������
// argv       ����        INT8S**     ָ�������ָ��
//
// ��  ��  ֵ��ok(0)-�ɹ�
//                 notok(0xFF)-ʧ��
//
// ��      �� ������
// ��      �ڣ�2005-05-08
// �޸���ʷ��
//        ����        �޸���      �޸�����
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
// �������ƣ�FLPageFunc0
// ������������ʾFLASH0һҳ������
// �������ƣ� ����/����� ����        ����
// argc       ����        INT8U           ��������
// argv       ����        INT8S**     ָ�������ָ��
//
// ��  ��  ֵ��ok(0)-�ɹ�
//                 notok(0xFF)-ʧ��
//
// ��      �� ������
// ��      �ڣ�2005-05-08
// �޸���ʷ��
//        ����        �޸���      �޸�����
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
//    {   //����Ĳ�����ͷ��������
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
 �������ƣ�dir_func
 ������������ʾ�ļ���Ϣ
 �������ƣ� ����/����� ����        ����
 argc1		����		INT8U 		��������
 argv1		����		INT8S**		ָ�������ָ��

 ��  ��  ֵ��ok(0)-�ɹ�
				 notok(0xFF)-ʧ��

 ��      �� ������
 ��      �ڣ�2005-05-08
 �޸���ʷ��
		����        �޸���      �޸�����
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

	// ��ȡ�ļ�����
	uiFileNums = EA_iCfsGetFileNum();

	// ��̬����ռ�
	ptFileInfo = (E_cfs_FileDetailInfo *)EA_pvMalloc(uiFileNums * sizeof(E_cfs_FileDetailInfo));
	if ( ptFileInfo == NULL )
	{
		armputs("malloc error.\r\n");
		return notok;
	}

	// ��ȡ�ļ���Ϣ
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
 �������ƣ�fget_func
 ����������ȡ�ļ�����
 �������ƣ� ����/����� ����        ����
 argc1		����		INT8U 		��������
 argv1		����		INT8S**		ָ�������ָ��

 ��  ��  ֵ��ok(0)-�ɹ�
				 notok(0xFF)-ʧ��

 ��      �� ������
 ��      �ڣ�2005-05-08
 �޸���ʷ��
		����        �޸���      �޸�����
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
		//  	lcddisperr("��rec.txtʧ��");
		return notok;
	}
	file_len = EA_i64CfsGetFileSize(fHandle);
	if ( file_len < 0 )
	{
		armputs("get file length failed.\r\n");
		lcddisperr("��ȡrec.txt����ʧ��");
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
			return notok;             //��ȡ������
		}
		if ( ch == 'C' )
			break;
	}

	//�����ļ����ͳ���
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
			return notok;             //��ȡ������
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
			return notok;             //��ȡ������
		}
		if ( ch == 'C' )
			break;
	}

	//  //�����ļ�����
	//  for(;;)
	//  {
	//  	Com1SndBuf[0] = SOH;
	//  	Com1SndBuf[1] = pack_num;
	//  	Com1SndBuf[2] = ~pack_num;
	//  	read_len = EA_i64CfsRead(fHandle, &Com1SndBuf[3], 128);
	//  	if (read_len < 128)
	//  	{
	//  		//���һ��
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
	//  				return notok;             //��ȡ������
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
	//  			return notok;             //��ȡ������
	//  		}
	//  		if ( ch == ACK )
	//  			break;
	//  	}
	//
	//  	pack_num++;
	//  }

	//�����ļ�����
	for (;;)
	{
		Com1SndBuf[0] = STX;
		Com1SndBuf[1] = pack_num;
		Com1SndBuf[2] = ~pack_num;
		read_len = EA_i64CfsRead(fHandle, &Com1SndBuf[3], pack_len);
		if ( read_len < pack_len )
		{
			//���һ��
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
					return notok;             //��ȡ������
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
				return notok;             //��ȡ������
			}
			if ( ch == CA )
			{
				armputs("\x18\x18\x18\x18\x18");
				(void)EA_iCfsClose(fHandle);
				return notok;             //���䱻�ж�
			}
			if ( ch == ACK )
				break;
		}

		pack_num++;
	}


	//�����ļ�������־
	for (;;)
	{
		armputch(EOT);
		ch = armgetch();
		if ( ch == -1 )
		{
			(void)EA_iCfsClose(fHandle);
			return notok;             //��ȡ������
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
			return notok;             //��ȡ������
		}
		if ( ch == 'C' )
			break;
	}


	//�����ļ����ͳ���
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
			return notok;             //��ȡ������
		}
		if ( ch == ACK )
			break;
	}





	(void)EA_iCfsClose(fHandle);
	return ok;
}

/*******************************************************************************
  * @��������    UpdateCRC16
  * @����˵��   �����������ݵ�CRCУ��
  * @�������   crcIn
				byte
  * @�������   ��
  * @���ز���   CRCУ��ֵ
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
  * @��������    UpdateCRC16
  * @����˵��   �����������ݵ�CRCУ��
  * @�������   data ������
				size ������
  * @�������   ��
  * @���ز���   CRCУ��ֵ
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
 �������ƣ�fput_func
 �������������ļ�����
 �������ƣ� ����/����� ����        ����
 argc1		����		INT8U 		��������
 argv1		����		INT8S**		ָ�������ָ��

 ��  ��  ֵ��ok(0)-�ɹ�
				 notok(0xFF)-ʧ��

 ��      �� ������
 ��      �ڣ�2005-05-08
 �޸���ʷ��
		����        �޸���      �޸�����
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
	//  	lcddisperr("�����ļ�ʧ��");
	//  	return notok;
	//  }

	EA_ucClearRS232(hRS232Handle);



	//�հ�ͷ
	for (;;)
	{
		armputch('C');

		j = armgetch2(1, (INT8U *)&ch);
		if ( j == -1 )
		{
			//  		(void)EA_iCfsClose(fHandle);
			armputs("\x18\x18\x18\x18\x18");
			return notok;             //��ȡ������
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
		lcddisperr("�����ļ�ʧ��");
		return notok;
	}
	strcpy((void *)buf, (void *)&Com1RcvBuf[3 + strlen((void *)file_name) + 1]);
	sscanf((void *)buf, "%lld", &file_len);


	//������
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
			lcddisperr("�����ж�");
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
			iWriteSize = EA_i64CfsWrite(fHandle, &Com1RcvBuf[3], rcv_len);      //д���ļ�
			if ( iWriteSize < 0 )
			{
				(void)EA_iCfsClose(fHandle);
				armputs("\x18\x18\x18\x18\x18");
				armputs("transmission aborted.\r\n");
				lcddisperr("д�ļ�ʧ��");
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

	//��β����
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
		lcddisperr("�޸��ļ���Сʧ��");
		return notok;
	}

	a = EA_iCfsFsync(fHandle);
	if ( a < 0 )
	{
		lcddisperr("ͬ���ļ�ʧ��");
		(void)EA_iCfsClose(fHandle);
		return notok;
	}

	a = EA_iCfsClose(fHandle);
	if ( a < 0 )
	{
		lcddisperr("�ر��ļ�ʧ��");
		return notok;
	}

	return ok;
}

/*****************************************************************
 ����ԭ�ͣ�test_crc16
 �����������ж�һ�����ݵ�crc�Ƿ���ȷ
 ����������
 �������ƣ�	����/�����	����		����
 data_to_cal_crc	����	INT8U const *		Ҫ����CRC������������ʼָ��
 len				����	INT32U				Ҫ����CRC���������ĳ���

 ��  ��  ֵ��ok(0) - �ɹ�
			notok(0xff) - ʧ��
 ��      ��	������
 ��      �ڣ�2004-02-04
 �޸���ʷ��
		����		�޸���		�޸�����
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
 �������ƣ�del_func
 ����������ɾ���ļ�����
 �������ƣ� ����/����� ����        ����
 argc1		����		INT8U 		��������
 argv1		����		INT8S**		ָ�������ָ��

 ��  ��  ֵ��ok(0)-�ɹ�
				 notok(0xFF)-ʧ��

 ��      �� ������
 ��      �ڣ�2005-05-08
 �޸���ʷ��
		����        �޸���      �޸�����
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
 �������ƣ�crf_func
 ���������������¼�ļ�����
 �������ƣ� ����/����� ����        ����
 argc1		����		INT8U 		��������
 argv1		����		INT8S**		ָ�������ָ��

 ��  ��  ֵ��ok(0)-�ɹ�
				 notok(0xFF)-ʧ��

 ��      �� ������
 ��      �ڣ�2005-05-08
 �޸���ʷ��
		����        �޸���      �޸�����
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

	// ���ļ�
	if ( EA_ucPFOpen((INT8U *)"record", &ucOpenID0) != EM_ffs_SUCCESS )
	{
		armputs("open file failed.\r\n");
		return notok;
	}

	// ���ļ�
	if ( EA_ucPFOpen((INT8U *)"backup", &ucOpenID1) != EM_ffs_SUCCESS )
	{
		armputs("open file failed.\r\n");
		(void)EA_ucPFClose(ucOpenID0);
		return notok;
	}

	//������
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

	//ɾ���ؽ���¼�ļ�
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

