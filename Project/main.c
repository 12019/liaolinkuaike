/******************************************************************************
Copyright 2012 �����״�ͨ�������޹�˾
All rights riserved.

�ļ���	��main.c
ģ�����ƣ���;��Ʊ��������
���ܸ�Ҫ����;��Ʊ�������� 
 
 
 
��ǰ�汾��0.1.0
�޸���	��frank 
������ڣ�2015.1.15
����˵����  1.ȡ���ֽ����ѣ�ˢ�����ѣ����ֿ�ݼ�����Ӧ���ֿ����͡�
    		2.���ѽ��ֶ����룬
    		3.����ӡ����Ʊ��
    		4.���Ѽ�¼��ʾ��ʱ���ӳ���
    		5.Ŀǰ�����е��Żݿ������ѣ�û�м�¼��Ҳ����ӡƱ�ݣ����ɺʹ�ӡ����ʼ���йأ�����ȡ����ӡ����
 
 
��ǰ�汾��0.0.9
�޸���	��frank 
������ڣ�2014.11.28
����˵����  1.�޸�һ��ˢ�������ѵ��ظ��ۿ۵�Bug��
    		2.����ͬ������4Ԫ��Ʊ�ۣ����12Ԫ ����ӡ��ʱ��Ʊ������ʾ��ÿ�Ŷ���12Ԫ��Ӧ���޸�Ϊÿ��4Ԫ��
    		3.Pos��ȫ����ܵ���Ŀ��ֱ����ʾ��Ʊ����Żݿ������ȡ��
 
 
��ǰ�汾��0.0.8
�޸���	��frank 
������ڣ�2014.11.28
����˵����  1.add youxiaoqi  ,kaishi he jiezhi riqi shezhi
			2.add yuepiao ka , zhijie shuaka kuaijie fangshi 
 
��ǰ�汾��0.0.7
�޸���	������
������ڣ�2014.10.11
����˵����  1. ����ˢ������
 
��ǰ�汾��0.0.6
�޸���	��������
������ڣ�2013.4.17
����˵����  1. ����ȱֽ�����ǻ�о��ʼ��ʧ�ܵĶ���жϣ��ж�ʮ�Σ�ÿ�μ��100ms��10�ζ��ж�ʧ�ܣ� 
		Ȼ���˳�������κ�һ�γɹ��ˣ��ͼ�����ӡ�� 
 
��ǰ�汾��0.0.5
�޸���	��������
������ڣ�2013.1.22
����˵����  1. ���š�Ʊ�š�Ա���ƣ������������ã�����ȡ��һ���˳�����
    		2. ����ȱֽ���жϹ��ܣ���ֽ�ٴ�����Ʊ�Ź���
    		3. ����Ʊ�ڼ䣬�����жϳ�ʼ����0000����ʼ����12345���жϹ��ܣ����������µ�¼
 
��ǰ�汾��0.0.4
�޸���	��������
������ڣ�2012.12.14
����˵����  1. ������������Ʊ�Ųſ�����Ʊ 
    		2. ���Ӵ�ӡ�ս�Ʊ�ݹ���
    		3. ���Խ���ӡ�������洢��Ʊ�ż�¼Ϊ6����ѯϵͳ���Բ�ѯ��Ʊ
 
��ǰ�汾��0.0.3
�޸���	������
������ڣ�2012.12.01
����˵����  1. ���ûѡ����·��������Ʊ 
    		2. ��һ�θ�ʽ���󣬱������복��
    		3. ��һ�θ�ʽ���󣬱�������POS��
 
��ǰ�汾��0.0.2
�޸���	������
������ڣ�2012.11.24
����˵����1. ��������ƱԱ��¼�����������0000�ţ��ٸ������Ų��ܵ�¼��bug 
    		2. �����˵��������һ�δ����ʾ��̬�ڴ��������bug
    		3. ����ϵͳ���ò˵��ĳ�ʼ����������Ϊ����������ļ���������¼�Ͳ��ɼ�¼��
    		4. �ſ���ѯ�����ܼƹ��ܡ�
    		5. Ʊ�м����С����Ʊ������С��

ȡ���汾��0.0.1
�޸���	������
������ڣ�2012.10.24
����˵����create

******************************************************************************/

#include "main.h"


const char *VERSION = " 0.1.0";         //�汾��
char dbuf[100];
volatile char Flag_Exticket = 0;
const char *BUSS_NUM_FULL_PRE =	"³P-";



INT8U main_menu[7*20+2] = "1.��Ʊ              2.Ʊ������          3.�������          4.���ݴ���          5.����ͳ��          6.ѡ����·          7.ϵͳ����          ";

//INT8U data_menu[] = "1.��Ʊ              2.�ϴ���Ʊ��¼      3.���ز�����        ";
INT8U data_menu[2*20+2] = "1.��Ʊ              2.��¼�Ͳ�������    ";
INT8U setting_menu[5 *20 + 2] = "1.ʱ������          2.�豸�������      3.��ʼ������        4.�ϴ����ݼ�¼      5.�Żݿ��ۿ�����    ";
INT8U confirm_menu[2*20+2] = "1.ȷ��              2.ȡ��              ";


//OPERATOR_INFO OPinfo[MAX_OPERATOR_NUM];
OPERATOR_INFO *OPinfo;
//LINE_INFO Linfo[MAX_LINE_NUM];
LINE_INFO *Linfo;
INT8U line_menu[100 * 20 + 2];
//STATION_INFO STinfo[MAX_LINE_NUM];
STATION_INFO *STinfo;
INT8U station_menu[MAX_STATION_NUM * 20 + 2];

DEV_STAT DevStat;
//Devhandle hRS232Handle;
TICKET_INFO Tinfo;

DevHandle hRS232Handle;
DevHandle hDprinterHandle;

extern INT8U Com1SndBuf[UART1_SEND_QUEUE_LENGTH];              //���ڷ��ͻ�����
extern INT8U Com1RcvBuf[UART1_RCV_QUEUE_LENGTH];               //���ڽ��ջ�����
extern INT8U Com1RcvBuf2[UART1_RCV_QUEUE_LENGTH];               //���ڽ��ջ�����
CARD_INFO CardInfo;

/*****************************************************************
 ����ԭ�ͣ�main()
 ����������������
 ����������

 ����ֵ��   ��

 ��  �ߣ�   ����
 ��  �ڣ�   2012-10-24
 �޸���ʷ��
		����        �޸���      �޸�����
		------      ---------   -------------
*****************************************************************/
int main(int arc, char **argv)
{
	INT8S menus = 0;

	param_init();
	system_check();

	operator_login();
	line_number_verify();
	bus_number_verify();
	set_ticket_number();

	for (;;)
	{
		menus = browse_menu_time(1, (void *)"     ���˵�     ", main_menu, FALSE);
		switch ( menus )
		{
		   case 0:              //1.��Ʊ
			   sell_ticket();
			   break;

		   case 1:              //2.Ʊ������
			   set_ticket_number();
			   break;

		   case 2:              //3.�������
			   set_banci();
			   break;

		   case 3:              //4.���ݴ���
				//  		   mdata();
			   send_record_process();
			   break;

		   case 4:              //5.����ͳ��
			   query_today_summary();
			   break;

		   case 5:              //6.ѡ����·
			   choose_line();
			   break;

		   case 6:              //7.ϵͳ����
			   msystem();
			   break;

		   case 10:              //8.��Ʊ������
			   ic_yuepiaocard_process();
			   break;

		   default:
			   break;

			   //  	   case 0:              //1.��Ʊ
			   //  		   sell_ticket();
			   //  		   break;
			   //
			   //  	   case 1:              //2.��·ѡ��
			   //  		   choose_line();
			   //  		   break;
			   //
			   //  	   case 2:              //3.��ѯ
			   //  		   break;
			   //
			   //  	   case 3:              //4.����ͳ��
			   //  		   query_today_summary();
			   //  		   break;
			   //
			   //  	   case 4:              //5.����
			   //  		   mdata();
			   //  		   break;
			   //
			   //  	   case 5:              //6.ϵͳ
			   //  		   msystem();
			   //  		   break;
			   //
			   //  	   default:
			   //  		   break;
		}

	}

	return 0;
}

/*****************************************************************
 ����ԭ�ͣ�param_init()
 ���������������ȳ�ʼ��
 ����������

 ����ֵ��   ��

 ��  �ߣ�   ����
 ��  �ڣ�   2004-09-17
 �޸���ʷ��
		����        �޸���      �޸�����
		------      ---------   -------------
*****************************************************************/
void param_init(void)
{

	//    // ʵ���ַ���������ʾ
	//
	//    int  x, y, i;
	//    uint uiZoom;
	//    const char acTestString[] = "�����ַ���,Test string";
	//
	//
	//    uiZoom = 2; // ���ַ����Ŵ�2����ʾ
	//    x = strlen(acTestString) * 8 * uiZoom;
	////     y = (128 - 16 * uiZoom)/2;
	//    y = (64 - 16 * uiZoom) / 2;
	//    for (i = 0; i < 160 + x; i++)
	//    {
	//        // ������Ļ����ֹ���������Ļ��˸
	//        EA_vLockScreen();
	//        EA_vCls();
	//        EA_vTextOut(i - x, y, EM_key_FONT8X16, 0, uiZoom, uiZoom, acTestString);
	//        // �����Ļ����
	//        EA_vUnlockScreen();
	//        EA_uiInkeyMs(50); // ������ʱ
	//    }

	//  EA_vCls();
	//  EA_ucSetInverse(EM_lcd_INVOFF);
	//  EA_vDisplay(1, "123456789012345678901234567890");
	//  EA_vDisplay(2, "223456789012345678901234567890");
	//  EA_vDisplay(3, "323456789012345678901234567890");
	//  EA_vDisplay(4, "423456789012345678901234567890");
	//  EA_vDisplay(5, "523456789012345678901234567890");
	//  EA_vDisplay(6, "623456789012345678901234567890");
	//  EA_vDelay(10);
	//  EA_ucSetInverse(EM_lcd_INVON);
	//  EA_vDisp(1, 1, "123456789012345678901234567890");
	//  EA_vDisp(2, 1, "223456789012345678901234567890");
	//  EA_vDisp(3, 1, "323456789012345678901234567890");
	//  EA_vDisp(4, 1, "423456789012345678901234567890");
	//  EA_vDisp(5, 1, "523456789012345678901234567890");
	//  EA_vDisp(6, 1, "623456789012345678901234567890");
	//  EA_vDelay(10);

	//  BUS_TIME ltime;
	INT8U buf[30];
	INT8U i = 0;
	INT32S j = 0;
	INT16U key = EM_key_NOHIT;

	EA_vCls();
	EA_ucSetInverse(EM_lcd_INVOFF);
	//  EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "�����״�ͨ���Ӽ���");
	EA_vDisp(1, 1, "    �����纣�Ƽ�    ");
	EA_vDisp(2, 1, "     ������Ʊ��     ");
	EA_vDisp(3, 1, "     Ver");
	EA_vDisp(3, 9, VERSION);
	//  Get_Time(&ltime);
	//  sprintf((void *)buf, "  %02X%02X-%02X-%02X %02X:%02X",
	//  		ltime.century, ltime.year, ltime.month, ltime.day, ltime.hour, ltime.minute);
	//  EA_vDisp(4, 1, (void *)buf);
	//
	//  (void)EA_uiInkeyMs(2000);
	key = EA_uiInkey(1);
	if ( key == EM_key_F3 )
	{
		format_process();
	}


	memset((void *)&DevStat, 0x00, sizeof(DevStat));

	i = ReadParam();
	if ( i != EM_SUCCESS )
	{
		param_factory_default(0);
	}
	sprintf((void *)&DevStat.bus_number_full[0], "%s%s", BUSS_NUM_FULL_PRE, &DevStat.bus_number[0]);


	//  //��̬����ռ�
	//  OPinfo = (OPERATOR_INFO *)EA_pvMalloc(MAX_OPERATOR_NUM * sizeof(OPERATOR_INFO) + 1);
	//  if ( OPinfo == NULL )
	//  {
	//  	lcddisperr("��̬����ռ����");
	//  }
	//  Linfo = (LINE_INFO *)EA_pvMalloc(MAX_LINE_NUM * sizeof(LINE_INFO) + 1);
	//  if ( Linfo == NULL )
	//  {
	//  	lcddisperr("��̬����ռ����");
	//  }
	//  STinfo = (STATION_INFO *)EA_pvMalloc(MAX_LINE_NUM * sizeof(STATION_INFO) + 1);
	//  if ( STinfo == NULL )
	//  {
	//  	lcddisperr("��̬����ռ����");
	//  }
	//
	//
	//  memset((void *)OPinfo, 0x00, MAX_OPERATOR_NUM * sizeof(OPERATOR_INFO));
	//  memset((void *)Linfo, 0x00, MAX_LINE_NUM * sizeof(LINE_INFO));
	//  memset((void *)STinfo, 0x00, MAX_LINE_NUM * sizeof(STATION_INFO));

	DevStat.operator_amount = 1;
	DevStat.line_amount = 1;
	//��̬�����ڴ棬��ʼ������
	OPinfo = (OPERATOR_INFO *)EA_pvMalloc(DevStat.operator_amount * sizeof(OPERATOR_INFO) + 1);
	if ( OPinfo == NULL )
	{
		lcddisperr("��̬����ռ����");
	}
	Linfo = (LINE_INFO *)EA_pvMalloc(DevStat.line_amount * sizeof(LINE_INFO) + 1);
	if ( Linfo == NULL )
	{
		lcddisperr("��̬����ռ����");
	}
	STinfo = (STATION_INFO *)EA_pvMalloc(DevStat.line_amount * sizeof(STATION_INFO) + 1);
	if ( STinfo == NULL )
	{
		lcddisperr("��̬����ռ����");
	}
	memset((void *)OPinfo, 0x00, DevStat.operator_amount * sizeof(OPERATOR_INFO));
	memset((void *)Linfo, 0x00, DevStat.line_amount * sizeof(LINE_INFO));
	memset((void *)STinfo, 0x00, DevStat.line_amount * sizeof(STATION_INFO));

	read_param_table();

	//��֯��·ѡ��˵�
	strcpy((void *)line_menu, (void *)&Linfo[0].menu_name[0]);
	for ( j = 1; j < DevStat.line_amount; j++ )
	{
		strcat((void *)line_menu, (void *)&Linfo[j].menu_name[0]);
	}

	sprintf((void *)buf, "�����汾%s", DevStat.version);
	lcddisp(4, 1, buf);
	(void)EA_uiInkey(2);

	//  sprintf((void *)buf, "%s,%s,%s",OPinfo[0].number, OPinfo[0].name, OPinfo[0].passwd);
	//  lcddisperr((void *)buf);




	//  memset((void *)OPinfo, 0x00, sizeof(OPinfo));
	//  strcpy((void *)&OPinfo[0].number[0], "0000");
	//  strcpy((void *)&OPinfo[0].name[0], "����");
	//  strcpy((void *)&OPinfo[0].passwd[0], "123456");
	//  //  strcpy((void *)&DevStat.bus_number[0], "12345");
	//
	//
	//
	//  DevStat.line_amount = 3;
	//  strcpy((void *)&Linfo[0].number[0], "@55");
	//  strcpy((void *)&Linfo[0].menu_name[0], "55�Ͳ�(���)        ");
	//  strcpy((void *)&Linfo[0].full_name[0], "�ĳ�--�Ͳ�(���)");
	//
	//  strcpy((void *)&Linfo[1].number[0], "@59");
	//  strcpy((void *)&Linfo[1].menu_name[0], "59�Ͳ�              ");
	//  strcpy((void *)&Linfo[1].full_name[0], "�ĳ�--�Ͳ�");
	//
	//  strcpy((void *)&Linfo[2].number[0], "@63");
	//  strcpy((void *)&Linfo[2].menu_name[0], "63����(����)        ");
	//  strcpy((void *)&Linfo[2].full_name[0], "�ĳ�--����(����)");
	//
	//  strcpy((void *)line_menu, (void *)&Linfo[0].menu_name[0]);
	//  strcat((void *)line_menu, (void *)&Linfo[1].menu_name[0]);
	//  strcat((void *)line_menu, (void *)&Linfo[2].menu_name[0]);
	//
	//
	//  //  DevStat.line_ptr = 0;
	//  //  strcpy((void *)&DevStat.line_number[0], (void *)&Linfo[DevStat.line_ptr].number[0]);
	//  //  strcpy((void *)&DevStat.line_menu_name[0], (void *)&Linfo[DevStat.line_ptr].menu_name[0]);
	//  //  strcpy((void *)&DevStat.line_full_name[0], (void *)&Linfo[DevStat.line_ptr].full_name[0]);
	//
	//
	//
	//  memset((void *)STinfo, 0x00, sizeof(STinfo));
	//
	//  STinfo[0].forward_station_num = 5;
	//  STinfo[0].backward_station_num = 5;
	//
	//  strcpy((void *)&STinfo[0].forward_number[0][0], "0001");
	//  strcpy((void *)&STinfo[0].forward_number[1][0], "0002");
	//  strcpy((void *)&STinfo[0].forward_number[2][0], "0003");
	//  strcpy((void *)&STinfo[0].forward_number[3][0], "0004");
	//  strcpy((void *)&STinfo[0].forward_number[4][0], "0005");
	//
	//  strcpy((void *)&STinfo[0].forward_name[0][0], "վ��һ");
	//  strcpy((void *)&STinfo[0].forward_name[1][0], "վ���");
	//  strcpy((void *)&STinfo[0].forward_name[2][0], "վ����");
	//  strcpy((void *)&STinfo[0].forward_name[3][0], "վ����");
	//  strcpy((void *)&STinfo[0].forward_name[4][0], "վ����");
	//
	//  strcpy((void *)&STinfo[0].backward_number[0][0], "0001");
	//  strcpy((void *)&STinfo[0].backward_number[1][0], "0002");
	//  strcpy((void *)&STinfo[0].backward_number[2][0], "0003");
	//  strcpy((void *)&STinfo[0].backward_number[3][0], "0004");
	//  strcpy((void *)&STinfo[0].backward_number[4][0], "0005");
	//
	//  strcpy((void *)&STinfo[0].backward_name[0][0], "վ����");
	//  strcpy((void *)&STinfo[0].backward_name[1][0], "վ����");
	//  strcpy((void *)&STinfo[0].backward_name[2][0], "վ����");
	//  strcpy((void *)&STinfo[0].backward_name[3][0], "վ���");
	//  strcpy((void *)&STinfo[0].backward_name[4][0], "վ��һ");
	//
	//  strcpy((void *)&STinfo[0].forward_price_table[0][0][0], "0.0");
	//  strcpy((void *)&STinfo[0].forward_price_table[0][1][0], "1.0");
	//  strcpy((void *)&STinfo[0].forward_price_table[0][2][0], "2.0");
	//  strcpy((void *)&STinfo[0].forward_price_table[0][3][0], "3.0");
	//  strcpy((void *)&STinfo[0].forward_price_table[0][4][0], "4.0");
	//  strcpy((void *)&STinfo[0].forward_price_table[0][5][0], "5.0");
	//
	//  strcpy((void *)&STinfo[0].forward_price_table[1][1][0], "0.0");
	//  strcpy((void *)&STinfo[0].forward_price_table[1][2][0], "1.0");
	//  strcpy((void *)&STinfo[0].forward_price_table[1][3][0], "2.0");
	//  strcpy((void *)&STinfo[0].forward_price_table[1][4][0], "3.0");
	//  strcpy((void *)&STinfo[0].forward_price_table[1][5][0], "4.0");
	//
	//  strcpy((void *)&STinfo[0].forward_price_table[2][2][0], "0.0");
	//  strcpy((void *)&STinfo[0].forward_price_table[2][3][0], "1.0");
	//  strcpy((void *)&STinfo[0].forward_price_table[2][4][0], "2.0");
	//  strcpy((void *)&STinfo[0].forward_price_table[2][5][0], "3.0");
	//
	//  strcpy((void *)&STinfo[0].forward_price_table[3][3][0], "0.0");
	//  strcpy((void *)&STinfo[0].forward_price_table[3][4][0], "1.0");
	//  strcpy((void *)&STinfo[0].forward_price_table[3][5][0], "2.0");
	//
	//  strcpy((void *)&STinfo[0].forward_price_table[4][4][0], "0.0");
	//  strcpy((void *)&STinfo[0].forward_price_table[4][5][0], "1.0");
	//
	//  strcpy((void *)&STinfo[0].forward_price_table[5][5][0], "0.0");
	//
	//
	//  strcpy((void *)&STinfo[0].backward_price_table[0][0][0], "0.0");
	//  strcpy((void *)&STinfo[0].backward_price_table[0][1][0], "1.0");
	//  strcpy((void *)&STinfo[0].backward_price_table[0][2][0], "2.0");
	//  strcpy((void *)&STinfo[0].backward_price_table[0][3][0], "3.0");
	//  strcpy((void *)&STinfo[0].backward_price_table[0][4][0], "4.0");
	//  strcpy((void *)&STinfo[0].backward_price_table[0][5][0], "5.0");
	//
	//  strcpy((void *)&STinfo[0].backward_price_table[1][1][0], "0.0");
	//  strcpy((void *)&STinfo[0].backward_price_table[1][2][0], "1.0");
	//  strcpy((void *)&STinfo[0].backward_price_table[1][3][0], "2.0");
	//  strcpy((void *)&STinfo[0].backward_price_table[1][4][0], "3.0");
	//  strcpy((void *)&STinfo[0].backward_price_table[1][5][0], "4.0");
	//
	//  strcpy((void *)&STinfo[0].backward_price_table[2][2][0], "0.0");
	//  strcpy((void *)&STinfo[0].backward_price_table[2][3][0], "1.0");
	//  strcpy((void *)&STinfo[0].backward_price_table[2][4][0], "2.0");
	//  strcpy((void *)&STinfo[0].backward_price_table[2][5][0], "3.0");
	//
	//  strcpy((void *)&STinfo[0].backward_price_table[3][3][0], "0.0");
	//  strcpy((void *)&STinfo[0].backward_price_table[3][4][0], "1.0");
	//  strcpy((void *)&STinfo[0].backward_price_table[3][5][0], "2.0");
	//
	//  strcpy((void *)&STinfo[0].backward_price_table[4][4][0], "0.0");
	//  strcpy((void *)&STinfo[0].backward_price_table[4][5][0], "1.0");
	//
	//  strcpy((void *)&STinfo[0].backward_price_table[5][5][0], "0.0");
	//
	//
	//  STinfo[2].forward_station_num = 5;
	//  STinfo[2].backward_station_num = 5;
	//
	//  strcpy((void *)&STinfo[2].forward_number[0][0], "0001");
	//  strcpy((void *)&STinfo[2].forward_number[1][0], "0002");
	//  strcpy((void *)&STinfo[2].forward_number[2][0], "0003");
	//  strcpy((void *)&STinfo[2].forward_number[3][0], "0004");
	//  strcpy((void *)&STinfo[2].forward_number[4][0], "0005");
	//
	//  strcpy((void *)&STinfo[2].forward_name[0][0], "վ��һ");
	//  strcpy((void *)&STinfo[2].forward_name[1][0], "վ���");
	//  strcpy((void *)&STinfo[2].forward_name[2][0], "վ����");
	//  strcpy((void *)&STinfo[2].forward_name[3][0], "վ����");
	//  strcpy((void *)&STinfo[2].forward_name[4][0], "վ����");
	//
	//  strcpy((void *)&STinfo[2].backward_number[0][0], "0001");
	//  strcpy((void *)&STinfo[2].backward_number[1][0], "0002");
	//  strcpy((void *)&STinfo[2].backward_number[2][0], "0003");
	//  strcpy((void *)&STinfo[2].backward_number[3][0], "0004");
	//  strcpy((void *)&STinfo[2].backward_number[4][0], "0005");
	//
	//  strcpy((void *)&STinfo[2].backward_name[0][0], "վ����");
	//  strcpy((void *)&STinfo[2].backward_name[1][0], "վ����");
	//  strcpy((void *)&STinfo[2].backward_name[2][0], "վ����");
	//  strcpy((void *)&STinfo[2].backward_name[3][0], "վ���");
	//  strcpy((void *)&STinfo[2].backward_name[4][0], "վ��һ");
	//
	//  strcpy((void *)&STinfo[2].forward_price_table[0][0][0], "0.0");
	//  strcpy((void *)&STinfo[2].forward_price_table[0][1][0], "1.0");
	//  strcpy((void *)&STinfo[2].forward_price_table[0][2][0], "2.0");
	//  strcpy((void *)&STinfo[2].forward_price_table[0][3][0], "3.0");
	//  strcpy((void *)&STinfo[2].forward_price_table[0][4][0], "4.0");
	//  strcpy((void *)&STinfo[2].forward_price_table[0][5][0], "5.0");
	//
	//  strcpy((void *)&STinfo[2].forward_price_table[1][1][0], "0.0");
	//  strcpy((void *)&STinfo[2].forward_price_table[1][2][0], "1.0");
	//  strcpy((void *)&STinfo[2].forward_price_table[1][3][0], "2.0");
	//  strcpy((void *)&STinfo[2].forward_price_table[1][4][0], "3.0");
	//  strcpy((void *)&STinfo[2].forward_price_table[1][5][0], "4.0");
	//
	//  strcpy((void *)&STinfo[2].forward_price_table[2][2][0], "0.0");
	//  strcpy((void *)&STinfo[2].forward_price_table[2][3][0], "1.0");
	//  strcpy((void *)&STinfo[2].forward_price_table[2][4][0], "2.0");
	//  strcpy((void *)&STinfo[2].forward_price_table[2][5][0], "3.0");
	//
	//  strcpy((void *)&STinfo[2].forward_price_table[3][3][0], "0.0");
	//  strcpy((void *)&STinfo[2].forward_price_table[3][4][0], "1.0");
	//  strcpy((void *)&STinfo[2].forward_price_table[3][5][0], "2.0");
	//
	//  strcpy((void *)&STinfo[2].forward_price_table[4][4][0], "0.0");
	//  strcpy((void *)&STinfo[2].forward_price_table[4][5][0], "1.0");
	//
	//  strcpy((void *)&STinfo[2].forward_price_table[5][5][0], "0.0");
	//
	//
	//  strcpy((void *)&STinfo[2].backward_price_table[0][0][0], "0.0");
	//  strcpy((void *)&STinfo[2].backward_price_table[0][1][0], "1.0");
	//  strcpy((void *)&STinfo[2].backward_price_table[0][2][0], "2.0");
	//  strcpy((void *)&STinfo[2].backward_price_table[0][3][0], "3.0");
	//  strcpy((void *)&STinfo[2].backward_price_table[0][4][0], "4.0");
	//  strcpy((void *)&STinfo[2].backward_price_table[0][5][0], "5.0");
	//
	//  strcpy((void *)&STinfo[2].backward_price_table[1][1][0], "0.0");
	//  strcpy((void *)&STinfo[2].backward_price_table[1][2][0], "1.0");
	//  strcpy((void *)&STinfo[2].backward_price_table[1][3][0], "2.0");
	//  strcpy((void *)&STinfo[2].backward_price_table[1][4][0], "3.0");
	//  strcpy((void *)&STinfo[2].backward_price_table[1][5][0], "4.0");
	//
	//  strcpy((void *)&STinfo[2].backward_price_table[2][2][0], "0.0");
	//  strcpy((void *)&STinfo[2].backward_price_table[2][3][0], "1.0");
	//  strcpy((void *)&STinfo[2].backward_price_table[2][4][0], "2.0");
	//  strcpy((void *)&STinfo[2].backward_price_table[2][5][0], "3.0");
	//
	//  strcpy((void *)&STinfo[2].backward_price_table[3][3][0], "0.0");
	//  strcpy((void *)&STinfo[2].backward_price_table[3][4][0], "1.0");
	//  strcpy((void *)&STinfo[2].backward_price_table[3][5][0], "2.0");
	//
	//  strcpy((void *)&STinfo[2].backward_price_table[4][4][0], "0.0");
	//  strcpy((void *)&STinfo[2].backward_price_table[4][5][0], "1.0");
	//
	//  strcpy((void *)&STinfo[2].backward_price_table[5][5][0], "0.0");


}

/*****************************************************************
 ����ԭ�ͣ�system_check()
 ����������ϵͳ�Լ��Լ�gprsģ���������
 ����������	
				 
 ����ֵ��	��
 
 ��  �ߣ�	����
 ��  �ڣ�	2004-09-17
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
*****************************************************************/
void system_check(void)
{
	INT8U i = 0;
	//  INT8U input[50];

	i = TimeFormatChk();
	if ( i != ok )
	{
		TimeModify();
	}

	//��ʽ�����һ���ϵ磬���������豸��
	if ( strcmp((void *)&DevStat.pos_number[0], "000000") == 0 )
	{
		set_pos_number();
	}

}

/******************************************************************************
 �������ƣ�TimeFormatChk
 ������������ʱ�ӣ���ʱ�Ӹ�ʽ�Ƿ���ȷ
 ����������
 �������ƣ�	����/�����	����		����
				
 ��  ��  ֵ��ok(0)-�Լ�ɹ�
				 notok(0xFF)-�Լ�ʧ��
				   
 ��      ��	������
 ��      �ڣ�2004-09-02
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
INT8U TimeFormatChk(void)
{
	INT8U i = 0;
	BUS_TIME ltime;

	i = Get_Time(&ltime);
	if ( i != ok )
	{
		return notok;
	}

	i = CheckTimeFormat(&ltime);
	return i;
}

/******************************************************************************
 �������ƣ�TimeModify
 ����������Ҫ������ʱ�䣬�����޸�
 ����������
 �������ƣ�	����/�����	����		����
				
 ��  ��  ֵ����
				   
 ��      ��	������
 ��      �ڣ�2004-09-02
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void TimeModify(void)
{
	INT8U i = 0;
	INT8U input[50];
	INT8U buf[50];
	BUS_TIME ltime;

	for (;;)
	{
		EA_vCls();
		Get_Time(&ltime);
		sprintf((void *)buf, "��ǰ:%02X%02X%02X%02X%02X%02X%02X", ltime.century, ltime.year, ltime.month,
				ltime.day, ltime.hour, ltime.minute, ltime.second);
		EA_vDisp(1, 1, (void *)buf);
		EA_vDisp(2, 1, "�������µ�ʱ��:");
		input[0] = '\0';
		i = EA_ucGetInputStr(3, 1, 20, EM_BIG_FONT | EM_MODE_NUMBER | EM_ALIGN_LEFT | EM_SHOW_ORIGINALLY
							 , 14, 14, 0, (void *)input);

		//  	sprintf(dbuf, "%s", input);
		//  	EA_vDisp(3, 1, dbuf);
		//  	sprintf(dbuf, "result:%02X", i);
		//  	EA_vDisp(4, 1, dbuf);
		if ( i == EM_ABOLISH )
			return;
		if ( i != EM_SUCCESS )
			continue;
		ltime.century = (ascii_to_hex(input[0]) << 4) | ascii_to_hex(input[1]);
		ltime.year = (ascii_to_hex(input[2]) << 4) | ascii_to_hex(input[3]);
		ltime.month = (ascii_to_hex(input[4]) << 4) | ascii_to_hex(input[5]);
		ltime.day = (ascii_to_hex(input[6]) << 4) | ascii_to_hex(input[7]);
		ltime.hour = (ascii_to_hex(input[8]) << 4) | ascii_to_hex(input[9]);
		ltime.minute = (ascii_to_hex(input[10]) << 4) | ascii_to_hex(input[11]);
		ltime.second = (ascii_to_hex(input[12]) << 4) | ascii_to_hex(input[13]);

		i = CheckTimeFormat(&ltime);
		if ( i != ok )
		{
			EA_vDisp(4, 1, "ʱ���ʽ����");
			SleepMs(1500);
			//  		EA_vCls();
			continue;
		}

		Modify_Time(&ltime);
		EA_vDisp(4, 1, "ʱ���޸ĳɹ�");
		SleepMs(1500);
		break;
	}
}

void printer_test(void)
{
	INT8U ucRet = 0;
	//  DevHandle hDprinterHandle;

	// ��ӡ�豸����I/Oģ���һ�֣�Ӧ�Ȼ�ò����������ʱ������ӡ���õ�ģʽ
	ucRet = EA_ucOpenDevice("PRINT", EM_io_REALMODE, &hDprinterHandle);
	if ( ucRet != EM_SUCCESS )
	{
		EA_vDisplay(2, "��ô�ӡ�������ʧ��!");
		return;
	}

	// POS��ʱ,���ó�ʼ��������ʼ����ӡ��о״̬
	ucRet =  EA_ucInitPrinter(hDprinterHandle);
	if ( ucRet != EM_SUCCESS )
	{
		EA_vDisplay(2, "��ʼ����ӡ��оʧ��!");
		return;
	}

	// �ɽ��д�ӡ��һЩ��������
	ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_XSPACE, 0);
	if ( ucRet != EM_SUCCESS )
	{
		EA_vDisplay(2, "����ʧ��!");
		return;
	}
	ucRet =  EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_YSPACE, 8);
	if ( ucRet != EM_SUCCESS )
	{
		EA_vDisplay(2, "����ʧ��!");
		return;
	}

	ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_PAGESPACE, 792);
	if ( ucRet != EM_SUCCESS )
	{
		EA_vDisplay(2, "����ʧ��!");
		return;
	}

	ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_ASCII_DOTTYPE, EM_prn_ASCIIDOT7X7);
	if ( ucRet != EM_SUCCESS )
	{
		EA_vDisplay(2, "����ʧ��!");
		return;
	}

	// ��ӡ����
	ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "��ӡ����!\n");
	if ( ucRet != EM_SUCCESS )
	{
		EA_vDisplay(2, "���д�ӡʧ��!");
		return;
	}
	ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "�����״�ͨ���Ӽ������޹�˾1\n");
	ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "�����״�ͨ���Ӽ������޹�˾2\n");
	ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "�����״�ͨ���Ӽ������޹�˾3\n");
	ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "�����״�ͨ���Ӽ������޹�˾4\n");
	ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "�����״�ͨ���Ӽ������޹�˾5\n");
	ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "�����״�ͨ���Ӽ������޹�˾6\n");

	ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_ASCII_DOTTYPE, EM_prn_ASCIIDOT16X8);
	ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "�����״�ͨ���Ӽ������޹�˾1\n");
	ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "�����״�ͨ���Ӽ������޹�˾2\n");

	// ��ӡ�����û�ҳ����
	ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "\f");
	if ( ucRet == EM_SUCCESS )
	{
		EA_vDisplay(2, "���δ�ӡ����!");
	}

	//  ucRet = EA_ucCutter(hDprinterHandle);
	//  if ( ucRet != EM_SUCCESS )
	//  {
	//  	EA_vDisplay(2,"��ֽ����ʧ��!");
	//  }

}

/******************************************************************************
 �������ƣ�operator_login
 ����������Ա����¼
 ����������
 �������ƣ�	����/�����	����		����
				
 ��  ��  ֵ����
				   
 ��      ��	������
 ��      �ڣ�2012-10-30
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void operator_login(void)
{
	INT8U i = 0;
	INT8U input1[50];
	INT8U input2[50];
	INT8U buf[50];
	INT16U j = 0;
	INT8U admin_num[5];
	INT8U admin_name[11];
	INT8U admin_passwd[11];
	INT8U admin_login = FALSE;

	strcpy((void *)admin_num, "0000");
	strcpy((void *)admin_name, "�纣");
	strcpy((void *)admin_passwd, "228846");

start_login:
	admin_login = FALSE;
	EA_vCls();
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "    Ա����¼    ");
	lcddisp(1, 1, (void *)"����:");
	strcpy((void *)input1, "");
	strcpy((void *)input2, "");
	//  input[0] = '\0';
	for (;;)
	{
		i = EA_ucGetInputStr(1, 6, 20, EM_BIG_FONT | EM_MODE_NUMBER | EM_ALIGN_LEFT | EM_SHOW_ORIGINALLY
							 , 4, 4, 0, (void *)input1);
		if ( i == EM_SUCCESS )
			break;
	}

	lcddisp(1, 1, (void *)"����:");
	for (;;)
	{
		i = EA_ucGetInputStr(1, 6, 20, EM_BIG_FONT | EM_MODE_NUMBER | EM_ALIGN_LEFT | EM_SHOW_ORIGINALLY
							 , 4, 4, 0, (void *)input2);
		if ( i == EM_SUCCESS )
			break;
	}
	if(strcmp((void*)input1, (void*)input2) != 0)     //������������Ա���ţ����뱣֤���ζ�һ�²ſ��Լ������в�����BY ������13.1.22
	{
		goto start_login;
	}
	//  for ( j = 0; j < MAX_OPERATOR_NUM; j++ )
	for ( j = 0; j < DevStat.operator_amount; j++ )
	{
		if ( strcmp((void *)input2, (void *)&OPinfo[j].number[0]) == 0 )
		{
			break;
		}
	}

	if ( j >= DevStat.operator_amount )
	{
		if ( strcmp((void *)input2, (void *)admin_num) == 0 )
		{
			admin_login = TRUE;
			sprintf((void *)buf, "����:%s %s", admin_num, admin_name);
			lcddisp(1, 1, (void *)buf);
			goto input_passwd;
		}
		else
		{

			lcddisp(4, 1, (void *)"Ա������Ч!         ");
			SleepMs(1500);
			goto start_login;
		}
	}

	sprintf((void *)buf, "����:%s %s", &OPinfo[j].number[0], &OPinfo[j].name[0]);
	lcddisp(1, 1, (void *)buf);

input_passwd:
	//��������
	lcddisp(2, 1, (void *)"����:");
	strcpy((void *)input1, "");
	for (;;)
	{
		i = EA_ucGetInputStr(2, 6, 20, EM_BIG_FONT | EM_MODE_NUMBER | EM_ALIGN_LEFT | EM_SHOW_PASSWORD
							 , 6, 6, 0, (void *)input1);
		if ( i == EM_SUCCESS )
			break;
		else if ( i == EM_ABOLISH )
			goto start_login;
	}

	if ( admin_login == TRUE )
	{
		if ( strcmp((void *)input1, (void *)admin_passwd) == 0 )
		{
			DevStat.op_ptr = 0;
			strcpy((void *)&DevStat.op_number[0], (void *)admin_num);
			strcpy((void *)&DevStat.op_name[0], (void *)admin_name);
			return;
		}
		else
		{
			lcddisp(4, 1, (void *)"�������!           ");
			SleepMs(1500);
			lcddisp(4, 1, (void *)"                    ");
			goto input_passwd;
		}
	}

	if ( strcmp((void *)input1, (void *)&OPinfo[j].passwd[0]) == 0 )
	{}
	else
	{
		lcddisp(4, 1, (void *)"�������!           ");
		SleepMs(1500);
		lcddisp(4, 1, (void *)"                    ");
		goto input_passwd;

	}

	DevStat.op_ptr = j;
	strcpy((void *)&DevStat.op_number[0], (void *)&OPinfo[j].number[0]);
	strcpy((void *)&DevStat.op_name[0], (void *)&OPinfo[j].name[0]);


}

/******************************************************************************
 �������ƣ�line_number_verify
 �����������˶���·��
 ����������
 �������ƣ�	����/�����	����		����
				
 ��  ��  ֵ����
				   
 ��      ��	������/������
 ��      �ڣ�2012-12-02
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void line_number_verify(void)
{
	//  INT8U i = 0;
	//  INT8U input[50];
	INT8U buf[50];
	//  INT16U j = 0;
	INT16U key = EM_key_NOHIT;

	EA_vCls();
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "    ��·ȷ��    ");
	sprintf((void *)buf, "��ǰ��·:%s", (char *)&DevStat.line_number[0]);
	lcddisp(1, 1, (void *)buf);
	sprintf((void *)buf, "%s", (char *)&DevStat.line_full_name[0]);
	lcddisp(2, 1, (void *)buf);
	lcddisp(4, 1, (void *)"F1-�޸�   ����-����");
	if(strcmp((void *)buf,"") == 0)    //���������·ѡ�� �������޷�����һ�²�
	{
		for( ;; )
		{
			key = EA_uiInkey(0);
			if ( key == EM_key_F1 )
			{
				choose_line();
				return;
			}
		}
	}
	key = EA_uiInkey(0);
	if ( key == EM_key_F1 )
	{
		choose_line();
	}
}

/******************************************************************************
 �������ƣ�bus_number_verify
 �����������˶Գ���
 ����������
 �������ƣ�	����/�����	����		����
				
 ��  ��  ֵ����
				   
 ��      ��	������/������
 ��      �ڣ�2012-12-2     
 �޸���ʷ�����ӽ��복�����ã�������޸ĵĻ����޷�������һ������
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void bus_number_verify(void)
{
	INT8U i = 0;
	INT8U input1[50];
	INT8U input2[50];
	INT8U buf[50];
	//  INT16U j = 0;
	INT16U key = EM_key_NOHIT;

	//��ʽ�����һ���ϵ磬�������복��
	if ( strcmp((void *)&DevStat.bus_number[0], "12345") == 0 )
	{
		goto set_bus_number;
	}

	EA_vCls();
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "    ����ȷ��    ");
	//  sprintf((void *)buf, "����:³P-%s", &DevStat.bus_number[0]);
	sprintf((void *)buf, "����:%s", &DevStat.bus_number_full[0]);
	lcddisp(1, 1, (void *)buf);
	//  lcddisp(3, 1, (void *)"�޸��밴F1��");
	//  lcddisp(4, 1, (void *)"������������..");
	lcddisp(4, 1, (void *)"F1-�޸�   ����-����");

	key = EA_uiInkey(0);
	if ( key == EM_key_F1 )
	{
set_bus_number:
		lcddisp(3, 1, (void *)"                    ");
		lcddisp(4, 1, (void *)"                    ");
		lcddisp(2, 1, (void *)"���복��:           ");
		strcpy((void *)input1, "");
		strcpy((void *)input2, "");
		for (;;)
		{
			i = EA_ucGetInputStr(2, 10, 20, EM_BIG_FONT | EM_MODE_CHAR | EM_ALIGN_LEFT | EM_SHOW_ORIGINALLY
								 , 5, 5, 0, (void *)input1); //���������޸�ֻ���������ֺ���ĸ
			if ( i == EM_SUCCESS )
				break;
			//if ( i == EM_ABOLISH )
				//return;      do nothing
		}

		lcddisp(2, 1, (void *)"���복��:           ");

		for (;;)
		{
			i = EA_ucGetInputStr(2, 10, 20, EM_BIG_FONT | EM_MODE_CHAR | EM_ALIGN_LEFT | EM_SHOW_ORIGINALLY
								 , 5, 5, 0, (void *)input2); //���������޸�ֻ���������ֺ���ĸ
			if ( i == EM_SUCCESS )
				break;
			//if ( i == EM_ABOLISH )
				//return;      do nothing
		}
		if(strcmp((void*)input1, (void*)input2) != 0)
		{
			goto set_bus_number;
		}
		strcpy((void *)&DevStat.bus_number[0], (void *)input2);
		sprintf((void *)&DevStat.bus_number_full[0], "%s%s", BUSS_NUM_FULL_PRE, &DevStat.bus_number[0]);

		WriteParam();

		EA_ucSetInverse(EM_lcd_INVON);
		lcddisp(4, 1, (void *)"�޸ĳɹ�            ");
		EA_ucSetInverse(EM_lcd_INVOFF);
		SleepMs(1500);
	}
}

/******************************************************************************
 �������ƣ�choose_line
 ����������ѡ����·
 ����������
 �������ƣ�	����/�����	����		����
				
 ��  ��  ֵ����
				   
 ��      ��	������
 ��      �ڣ�2012-10-30
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void choose_line(void)
{
	INT8S menus = 0;
	//  INT8U i = 0;
	//  INT8U input[20];
	//  INT8U buf[30];
	//  INT16U j = 0;
	//  INT16U key = EM_key_NOHIT;


	for (;;)
	{
		menus = browse_menu(1, (void *)"    ѡ����·    ", line_menu, TRUE);
		if ( menus == -1 )
			return;           //ȡ��������
		else
			break;
	}

	DevStat.line_ptr = menus;
	strcpy((void *)&DevStat.line_number[0], (void *)&Linfo[DevStat.line_ptr].number[0]);
	strcpy((void *)&DevStat.line_menu_name[0], (void *)&Linfo[DevStat.line_ptr].menu_name[0]);
	strcpy((void *)&DevStat.line_full_name[0], (void *)&Linfo[DevStat.line_ptr].full_name[0]);

	WriteParam();

}

/******************************************************************************
 �������ƣ�sell_ticket
 ������������Ʊ����
 ����������
 �������ƣ�	����/�����	����		����
				
 ��  ��  ֵ����
				   
 ��      ��	������
 ��      �ڣ�2012-10-30
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
//void sell_ticket(void)
//{
//    //  INT8S menus = 0;
//    INT8U i = 0;
//    INT8U input[50];
//    INT8U buf[50];
//    INT16U j = 0;
//    INT16U key = EM_key_NOHIT;
//    int m;
//    BUS_TIME ltime;
//    INT8U ucRet = 0;
//    //  DevHandle hDprinterHandle;
//    INT8U disp_mem[4][20];              //��ʾ����
//
//    //������·ȷ���ж�
//    if ( strcmp((void *)&DevStat.line_number[0], "") == 0 )
//    {
//        lcddisperr("��·δѡ��,���ص�¼!");
//        return;
//    }
//    //����Ա����ȷ���ж�
//    if ( strcmp((void *)&DevStat.op_number[0], "0000") == 0 )
//    {
//        lcddisperr("����δ����,���ص�¼!");
//        return;
//    }
//    //���ӳ���ȷ���ж�
//    if ( strcmp((void *)&DevStat.bus_number[0], "12345") == 0 )
//    {
//        lcddisperr("����δ����,���ص�¼!");
//        return;
//    }
//    if( Flag_Exticket == 1)
//    {
//        set_ticket_number();
//        Flag_Exticket = 0;
//    }
//
//sell_ticket_step1:
//    //ѡ����ʻ����
//    EA_vCls();
//    EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "      ��Ʊ      ");
//    sprintf((void *)buf, "��·:%s     ���:%2d", (char *)&DevStat.line_number[0], DevStat.banci);
//    lcddisp(1, 1, (void *)buf);
//    sprintf((void *)buf, "%s", (char *)&DevStat.line_full_name[0]);
//    lcddisp(2, 1, (void *)buf);
//    sprintf((void *)buf, "Ʊ��:%07d", DevStat.cur_ticket_number);
//    lcddisp(3, 1, (void *)buf);
//
//    lcddisp(4, 1, (void *)"F1����    F2����");
//    //  lcddisp(4, 1, (void *)"F1����  F2����  F3���");
//
//    for (;;)
//    {
//        key = EA_uiInkey(0);
//        if ( key == EM_key_F1 )
//        {
//            DevStat.last_direction = DevStat.direction;
//            DevStat.direction = FORWARD;
//            if ( DevStat.direction != DevStat.last_direction )
//            {
//                DevStat.last_start_station = 0;
//                DevStat.last_end_station = 0;
//            }
//            break;
//        }
//        else if ( key == EM_key_F2 )
//        {
//            DevStat.last_direction = DevStat.direction;
//            DevStat.direction = BACKWARD;
//            if ( DevStat.direction != DevStat.last_direction )
//            {
//                DevStat.last_start_station = 0;
//                DevStat.last_end_station = 0;
//            }
//            break;
//        }
//        //      else if ( key == EM_key_F3 )
//        //      {
//        //          DevStat.banci++;
//        //          if ( DevStat.banci > 99 )
//        //              DevStat.banci = 99;
//        //          sprintf((void *)buf, "��·:%s     ���:%2d", (char *)&DevStat.line_number[0], DevStat.banci);
//        //          lcddisp(1, 1, (void *)buf);
//        //          WriteParam();
//        //          //          break;
//        //      }
//        else if ( key == EM_key_CANCEL )
//        {
//            return;
//        }
//        else
//        {
//            ;           //do nothing
//        }
//    }
//
//sell_ticket_step2:
//    i = choose_start_station();
//    if ( i != ok )
//    {
//        goto sell_ticket_step1;         //��ȡ������
//    }
//
//sell_ticket_step3:
//    i = choose_end_station();
//    if ( i != ok )
//    {
//        goto sell_ticket_step2;         //��ȡ������
//    }
//
//
//sell_ticket_step4:
//    //ѡ��ȫ�۰��
//    EA_vCls();
//    EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "      ��Ʊ      ");
//    sprintf((void *)buf, "%s-%s", (char *)&DevStat.start_station_name[0], (char *)&DevStat.end_station_name[0]);
//    lcddisp(1, 1, (void *)buf);
//    strcpy((char *)&disp_mem[0][0], (char *)buf);           //������ʾ����
//    //  sprintf((void *)buf, "ȫ��: %.1fԪ", DevStat.origin_pricef);
//    //  lcddisp(2, 1, (void *)buf);
//    //  DevStat.half_discount = FALSE;
//    DevStat.ticket_type = '1';
//    EA_ucSetInverse(EM_lcd_INVON);
//    strcpy((void *)buf, "                    ");
//    lcddisp(2, 1, (void *)buf);
//    sprintf((void *)buf, "ȫ��: %.1fԪ", (FP32)DevStat.origin_price / 100.0);
//    lcddisp(2, 1, (void *)buf);
//    EA_ucSetInverse(EM_lcd_INVOFF);
//
//    //  lcddisp(4, 1, (void *)"F1ȫ��    F2���");
//    lcddisp(4, 1, (void *)"F1ȫ��  F2���  F3��Ʊ");
//
//    for (;;)
//    {
//        key = EA_uiInkey(0);
//    key_judge:
//        if ( key == EM_key_F1 )
//        {
//            //          DevStat.half_discount = FALSE;
//            DevStat.ticket_type = '1';
//            EA_ucSetInverse(EM_lcd_INVON);
//            strcpy((void *)buf, "                    ");
//            lcddisp(2, 1, (void *)buf);
//            sprintf((void *)buf, "ȫ��: %.1fԪ", (FP32)DevStat.origin_price / 100.0);
//            lcddisp(2, 1, (void *)buf);
//            EA_ucSetInverse(EM_lcd_INVOFF);
//            //          break;
//        }
//        else if ( key == EM_key_F2 )
//        {
//            //          DevStat.half_discount = TRUE;
//            DevStat.ticket_type = '2';
//            EA_ucSetInverse(EM_lcd_INVON);
//            strcpy((void *)buf, "                    ");
//            lcddisp(2, 1, (void *)buf);
//            sprintf((void *)buf, "���: %.1fԪ", (FP32)DevStat.half_price / 100.0);
//            lcddisp(2, 1, (void *)buf);
//            EA_ucSetInverse(EM_lcd_INVOFF);
//            //          break;
//        }
//        else if ( key == EM_key_F3 )
//        {
//            //          DevStat.half_discount = TRUE;
//            DevStat.ticket_type = '3';
//
//            EA_ucSetInverse(EM_lcd_INVON);
//            //          strcpy((void *)buf, "                    ");
//            //          lcddisp(2, 1, (void *)buf);
//            lcddisp(2, 1, (void *)"��Ʊ:               ");
//            strcpy((void *)input, "");
//            for (;;)
//            {
//                // i = EA_ucGetInputStr(2, 7, 20, EM_BIG_FONT | EM_MODE_NUMBER | EM_ALIGN_LEFT | EM_SHOW_ORIGINALLY
//                //   , 1, 7, 0, (void *)input);
//                i = get_input_str(2, 7, 1, 7, (void *)input);
//                if ( i == EM_SUCCESS )
//                    break;
//                if ( i == EM_ABOLISH )
//                    return;
//                if ( i == EM_key_F1 )
//                {
//                    key = EM_key_F1;
//                    goto key_judge;
//                }
//                if ( i == EM_key_F2 )
//                {
//                    key = EM_key_F2;
//                    goto key_judge;
//                }
//            }
//            sscanf((void *)input, "%f", &DevStat.package_pricef);
//
//            sprintf((void *)buf, "��Ʊ: %.2fԪ", DevStat.package_pricef);
//            lcddisp(2, 1, (void *)buf);
//            EA_ucSetInverse(EM_lcd_INVOFF);
//        }
//        else if ( key == EM_key_ENTER )
//        {
//            //          if ( DevStat.half_discount == TRUE )
//            if ( DevStat.ticket_type == '1' )
//            {
//                strcpy((void *)buf, "                    ");
//                lcddisp(2, 1, (void *)buf);
//                sprintf((void *)buf, "ȫ��: %.1fԪ", (FP32)DevStat.origin_price / 100.0);
//                lcddisp(2, 1, (void *)buf);
//                strcpy((char *)&disp_mem[1][0], (char *)buf);           //������ʾ����
////				DevStat.iccard_price = DevStat.origin_price;			//����ˢ��Ʊ��
//                sprintf((void *)&DevStat.prices[0], "%.2f", (FP32)DevStat.origin_price / 100.0);
//            }
//            else if ( DevStat.ticket_type == '2' )
//            {
//                strcpy((void *)buf, "                    ");
//                lcddisp(2, 1, (void *)buf);
//                sprintf((void *)buf, "���: %.1fԪ", (FP32)DevStat.half_price / 100.0);
//                lcddisp(2, 1, (void *)buf);
//                strcpy((char *)&disp_mem[1][0], (char *)buf);           //������ʾ����
////				DevStat.iccard_price = DevStat.half_price;				//����ˢ��Ʊ��
//                sprintf((void *)&DevStat.prices[0], "%.2f", (FP32)DevStat.half_price / 100.0);
//            }
//            else            //��Ʊ
//            {
//                strcpy((void *)buf, "                    ");
//                lcddisp(2, 1, (void *)buf);
//                sprintf((void *)buf, "��Ʊ: %.2fԪ", DevStat.package_pricef);
//                lcddisp(2, 1, (void *)buf);
//                strcpy((char *)&disp_mem[1][0], (char *)buf);           //������ʾ����
//                sprintf((void *)&DevStat.prices[0], "%.2f", DevStat.package_pricef);
//            }
//            break;
//        }
//        else if ( key == EM_key_CANCEL )
//        {
//            goto sell_ticket_step3;         //��ȡ������
//        }
//        else
//        {
//            ;           //do nothing
//        }
//    }
//
//
//sell_ticket_step5:
//    lcddisp(4, 1, (void *)"                      ");
//    lcddisp(3, 1, (void *)"����:");
//    strcpy((void *)input, "");
//    for ( ;; )
//    {
//        i = EA_ucGetInputStr(3, 6, 20, EM_BIG_FONT | EM_MODE_NUMBER | EM_ALIGN_LEFT | EM_SHOW_ORIGINALLY
//                             , 1, 1, 0, (void *)input);
//
//        if ( i == EM_ABOLISH )
//            goto sell_ticket_step4;         //��ȡ������
//        if ( i == EM_SUCCESS )
//            break;
//    }
//
//    sscanf((void *)input, "%d", &m);
//    DevStat.ticket_amount = (INT16U)m;
//
//    sprintf((char *)buf, "����:%d", m);
//    strcpy((char *)&disp_mem[2][0], (char *)buf);           //������ʾ����
//
//
//sell_ticket_step6:
//    //add by ver 0.0.7 ˢ��ѡ��
//    lcddisp(4, 1, (void *)"F1�ֽ�    F2ˢ��    ");
//    memset(&CardInfo, 0x00, sizeof(CardInfo));
//
//    for ( ;; )
//    {
//        key = EA_uiInkey(0);
//        if ( key == EM_key_F1 )   //|| key == EM_key_ENTER
//        {
//            break;      //�ֽ� do nothing
//        }
//        else if ( key == EM_key_F2 )
//        {
//            i = ic_card_process();   //ˢ������ϵͳ
//            if (i != ok)
//            {
//                strcpy((void *)buf, "                    ");
//                lcddisp(1, 1, (void *)buf);
//                lcddisp(2, 1, (void *)buf);
//                lcddisp(3, 1, (void *)buf);
//                lcddisp(1, 1, (void *)&disp_mem[0][0]);
//                lcddisp(2, 1, (void *)&disp_mem[1][0]);
//                lcddisp(3, 1, (void *)&disp_mem[2][0]);
//                goto sell_ticket_step6;         //��ȡ������
//            }
//            break;
//        }
//        else if ( key == EM_key_CANCEL )
//        {
//            goto sell_ticket_step5;         //��ȡ������
//        }
//        else
//        {
//            ;           //do nothing
//        }
//    }
//
//
//    (void)Get_Time(&ltime);
//    memcpy(&DevStat.sell_ticket_time.century, &ltime.century, sizeof(BUS_TIME));
//    sprintf((void *)&DevStat.sell_ticket_times[0], "%02X%02X%02X%02X%02X%02X%02X",
//            ltime.century, ltime.year, ltime.month, ltime.day, ltime.hour, ltime.minute, ltime.second);
//
//
//    //���ж��Ƿ���ֽ
//    // ��ӡ�豸����I/Oģ���һ�֣�Ӧ�Ȼ�ò����������ʱ������ӡ���õ�ģʽ
//    ucRet = EA_ucOpenDevice("PRINT", EM_io_REALMODE, &hDprinterHandle);
//    if ( ucRet != EM_SUCCESS )
//    {
//        lcddisperr("��ô�ӡ�������ʧ��!");
//        return;
//    }
//    uint k = 0;
//    for ( j = 0; j < DevStat.ticket_amount; j++ )
//    {
//        sprintf((void *)buf, "���ڴ�ӡ...   ��%2d��", j + 1);
//        lcddisp(4, 1, (void *)buf);
//
//        //����������ʱ��ʹ��
//        // POS��ʱ,���ó�ʼ��������ʼ����ӡ��о״̬
//        for(k=0;k<20;k++)
//        {
//            ucRet = EA_ucInitPrinter(hDprinterHandle);
//            (void)EA_uiInkeyMs(100);
//            if(ucRet == EM_SUCCESS)
//            {
//                break;
//            }
//        }
//        if((k >= 20) && (ucRet != EM_SUCCESS))
//        {
//            if ( ucRet == EM_prn_PAPERENDED )
//            {
//                lcddisperr("��ֽ!��ӡʧ��!");
//                Flag_Exticket = 1;
//                (void)EA_ucCloseDevice(&hDprinterHandle);
//                return;
//            }
//            else if ( ucRet != EM_SUCCESS )
//            {
//                lcddisperr("��ʼ����ӡ��оʧ��!");
//                (void)EA_ucCloseDevice(&hDprinterHandle);
//                return;
//            }
//        }
//        generate_ticket_info();                 //���ɼ�¼
//        store_record(&Tinfo.pos_number[0]);     //�����¼
//        if( strcmp((void *)&Tinfo.price_type[0], "8") == 0 )
//        {
//            goto endbuy;
//        }
//
//        print_ticket(&Tinfo);                   //���ݼ�¼��ӡ��Ʊ
//
//        DevStat.cur_ticket_number++;
//        if ( DevStat.cur_ticket_number > 9999999 )
//            DevStat.cur_ticket_number = 0;
//        sprintf((void *)&Tinfo.ticket_number[0], "%07d", DevStat.cur_ticket_number);            //Ʊ�� 7
//
//    }
//endbuy:
//
//    (void)EA_ucCloseDevice(&hDprinterHandle);
//}

void sell_ticket(void)
{
	
	INT8U i = 0;
	INT8U input[50];
	INT8U buf[50];
	INT16U j = 0;
	INT16U key = EM_key_NOHIT;
	int m;
	BUS_TIME ltime;
//	INT8U ucRet = 0;
	INT8U disp_mem[4][20];				//��ʾ����

	//������·ȷ���ж�
	if ( strcmp((void *)&DevStat.line_number[0], "") == 0 )
	{
		lcddisperr("��·δѡ��,���ص�¼!");
		return;
	}
	//����Ա����ȷ���ж�
	if ( strcmp((void *)&DevStat.op_number[0], "0000") == 0 )
	{
		lcddisperr("����δ����,���ص�¼!");
		return;
	}
	//���ӳ���ȷ���ж�
	if ( strcmp((void *)&DevStat.bus_number[0], "12345") == 0 )
	{
		lcddisperr("����δ����,���ص�¼!");
		return;
	}
	if( Flag_Exticket == 1)
	{
		set_ticket_number();
		Flag_Exticket = 0; 
	}

sell_ticket_step1:
	//ѡ����ʻ����
	EA_vCls();
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "      ��Ʊ      ");
	sprintf((void *)buf, "%s    ���:%2d", (char *)&DevStat.line_full_name[0], DevStat.banci);
	lcddisp(1, 1, (void *)buf);
	sprintf((void *)buf, "0��Ʊ��            ");
	lcddisp(4, 1, (void *)buf);

	EA_ucSetInverse(EM_lcd_INVON);
	lcddisp(2, 1, (void *)"Ʊ��:               ");
	strcpy((void *)input, "");

	for ( ;; )
	{
		i = get_input_str(2, 7, 1, 7, (void *)input);
		if ( i == EM_SUCCESS )
			break;
		if ( i == EM_ABOLISH )
			return;
		if ( i == EM_key_F1 )
		{
			key = EM_key_F1;
			goto sell_ticket_step1;
		}
		if ( i == EM_key_0 )
		{
			key = EM_key_0;
			return;
		}
	}
	sscanf((void *)input, "%f", &DevStat.origin_pricef);

	if(DevStat.origin_pricef > 20)
	{
		lcddisperr("����������Ʊ��");
		goto sell_ticket_step1;
	}

	sprintf((void *)buf, "Ʊ��: %.2fԪ", DevStat.origin_pricef);
	lcddisp(2, 1, (void *)buf);
	EA_ucSetInverse(EM_lcd_INVOFF);

	strcpy((char *)&disp_mem[1][0], (char *)buf);			//������ʾ����
	sprintf((void *)&DevStat.prices[0], "%.2f", DevStat.origin_pricef);


//sell_ticket_step5:
	lcddisp(4, 1, (void *)"                      ");
	lcddisp(3, 1, (void *)"����:");
	strcpy((void *)input, "");
	for ( ;; )
	{
		i = EA_ucGetInputStr(3, 6, 20, EM_BIG_FONT | EM_MODE_NUMBER | EM_ALIGN_LEFT | EM_SHOW_ORIGINALLY
							 , 1, 1, 0, (void *)input);

		if ( i == EM_ABOLISH )
			goto sell_ticket_step1;         //��ȡ������
		if ( i == EM_SUCCESS )
			break;
	}

	sscanf((void *)input, "%d", &m);
	DevStat.ticket_amount = (INT16U)m;

	sprintf((char *)buf, "����:%d", m);
	strcpy((char *)&disp_mem[2][0], (char *)buf);			//������ʾ����


sell_ticket_step6:

	memset(&CardInfo, 0x00, sizeof(CardInfo));

	i = ic_card_process();   //ˢ������ϵͳ
	if (i != ok)
	{
		strcpy((void *)buf, "                    ");
		lcddisp(1, 1, (void *)buf);
		lcddisp(2, 1, (void *)buf);
		lcddisp(3, 1, (void *)buf);
		lcddisp(1, 1, (void *)&disp_mem[0][0]);
		lcddisp(2, 1, (void *)&disp_mem[1][0]);
		lcddisp(3, 1, (void *)&disp_mem[2][0]);
		goto sell_ticket_step1;         //��ȡ������
	}


	(void)Get_Time(&ltime);
	memcpy(&DevStat.sell_ticket_time.century, &ltime.century, sizeof(BUS_TIME));
	sprintf((void *)&DevStat.sell_ticket_times[0], "%02X%02X%02X%02X%02X%02X%02X",
			ltime.century, ltime.year, ltime.month, ltime.day, ltime.hour, ltime.minute, ltime.second);

	for ( j = 0; j < DevStat.ticket_amount; j++ )
	{

		generate_ticket_info();                 //���ɼ�¼
		store_record(&Tinfo.pos_number[0]);     //�����¼

//  	DevStat.cur_ticket_number++;
//  	if ( DevStat.cur_ticket_number > 9999999 )
//  		DevStat.cur_ticket_number = 0;
//  	sprintf((void *)&Tinfo.ticket_number[0], "%07d", DevStat.cur_ticket_number);            //Ʊ�� 7

	}

	return;

}
/******************************************************************************
 �������ƣ�choose_start_station
 ����������ѡ���ϳ�վ��
 ����������
 �������ƣ�	����/�����	����		����
				
 ��  ��  ֵ��ok(0)-�ɹ�
				 notok(0xFF)-��ȡ������
				   
 ��      ��	������
 ��      �ڣ�2012-10-30
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
INT8U choose_start_station(void)
{
	INT16S menus = 0;
	//  INT8U i = 0;
	//  INT8U input[50];
	INT8U buf[50];
	INT16U j = 0;
	//  INT16U key = EM_key_NOHIT;

	strcpy((void *)station_menu, "");
	if ( DevStat.direction == FORWARD )
	{
		for ( j = 0; j < STinfo[DevStat.line_ptr].forward_station_num; j++ )
		{
			sprintf((void *)buf, "%-20s", (char *)&STinfo[DevStat.line_ptr].forward_name[j][0]);
			strcat((void *)station_menu, (void *)buf);
		}
	}
	else
	{
		for ( j = 0; j < STinfo[DevStat.line_ptr].backward_station_num; j++ )
		{
			sprintf((void *)buf, "%-20s", (char *)&STinfo[DevStat.line_ptr].backward_name[j][0]);
			strcat((void *)station_menu, (void *)buf);
		}
	}

	menus = browse_menu2(1, (void *)"  �ϳ�վ��ѡ��  ", station_menu, TRUE, DevStat.last_start_station);
	if ( menus == -1 )
		return notok;

	DevStat.start_station = (INT16U)menus;
	DevStat.last_start_station = DevStat.start_station;

	if ( DevStat.direction == FORWARD )
	{
		strcpy((void *)&DevStat.start_station_number[0], (char *)&STinfo[DevStat.line_ptr].forward_number[DevStat.start_station][0]);
		strcpy((void *)&DevStat.start_station_name[0], (char *)&STinfo[DevStat.line_ptr].forward_name[DevStat.start_station][0]);
	}
	else
	{
		strcpy((void *)&DevStat.start_station_number[0], (char *)&STinfo[DevStat.line_ptr].backward_number[DevStat.start_station][0]);
		strcpy((void *)&DevStat.start_station_name[0], (char *)&STinfo[DevStat.line_ptr].backward_name[DevStat.start_station][0]);
	}

	return ok;
}

/******************************************************************************
 �������ƣ�choose_end_station
 ����������ѡ���³�վ��
 ����������
 �������ƣ�	����/�����	����		����
				
 ��  ��  ֵ��ok(0)-�ɹ�
				 notok(0xFF)-��ȡ������
				   
 ��      ��	������
 ��      �ڣ�2012-10-30
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
INT8U choose_end_station(void)
{
	INT16S menus = 0;
	//  INT8U i = 0;
	//  INT8U input[50];
	INT8U buf[50];
	INT16U j = 0;
	//  INT16U key = EM_key_NOHIT;
	INT32U fill = 0;

	strcpy((void *)station_menu, "");
	if ( DevStat.direction == FORWARD )
	{
		for ( j = DevStat.start_station; j < STinfo[DevStat.line_ptr].forward_station_num; j++ )
		{
			sprintf((void *)buf, "%-14s%6s", (char *)&STinfo[DevStat.line_ptr].forward_name[j][0], (char *)&STinfo[DevStat.line_ptr].forward_price_table[DevStat.start_station][j][0]);
			strcat((void *)station_menu, (void *)buf);
		}
	}
	else
	{
		for ( j = DevStat.start_station; j < STinfo[DevStat.line_ptr].backward_station_num; j++ )
		{
			sprintf((void *)buf, "%-14s%6s", (char *)&STinfo[DevStat.line_ptr].backward_name[j][0], (char *)&STinfo[DevStat.line_ptr].backward_price_table[DevStat.start_station][j][0]);
			strcat((void *)station_menu, (void *)buf);
		}
	}

	menus = browse_menu2(1, (void *)"  �³�վ��ѡ��  ", station_menu, TRUE, 51);
	if ( menus == -1 )
		return notok;

	DevStat.end_station = (INT16U)menus + DevStat.start_station;
	DevStat.last_end_station = DevStat.end_station;

	if ( DevStat.direction == FORWARD )
	{
		strcpy((void *)&DevStat.end_station_number[0], (char *)&STinfo[DevStat.line_ptr].forward_number[DevStat.end_station][0]);
		strcpy((void *)&DevStat.end_station_name[0], (char *)&STinfo[DevStat.line_ptr].forward_name[DevStat.end_station][0]);
		strcpy((void *)&DevStat.origin_prices[0], (char *)&STinfo[DevStat.line_ptr].forward_price_table[DevStat.start_station][DevStat.end_station][0]);
	}
	else
	{
		strcpy((void *)&DevStat.end_station_number[0], (char *)&STinfo[DevStat.line_ptr].backward_number[DevStat.end_station][0]);
		strcpy((void *)&DevStat.end_station_name[0], (char *)&STinfo[DevStat.line_ptr].backward_name[DevStat.end_station][0]);
		strcpy((void *)&DevStat.origin_prices[0], (char *)&STinfo[DevStat.line_ptr].backward_price_table[DevStat.start_station][DevStat.end_station][0]);
	}

	DevStat.origin_pricef = atof((void *)&DevStat.origin_prices[0]);
	DevStat.origin_price = (INT32U)(DevStat.origin_pricef * 100.0);
	DevStat.half_price = DevStat.origin_price / 2;
	if ( (DevStat.half_price % 50) != 0 )
	{
		//���ϲ���5ë������
		fill = 50 - (DevStat.half_price % 50);
		DevStat.half_price += fill;
	}
	return ok;
}

/******************************************************************************
 �������ƣ�msystem
 �����������˵�->ϵͳ����
 ����������
 �������ƣ�	����/�����	����		����
				
 ��  ��  ֵ����
				   
 ��      ��	������
 ��      �ڣ�2012-10-30
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void msystem(void)
{
	INT8S menus = 0;

	INT8U i = 0;
	INT8U input[50];
	//  INT8U buf[50];
	//  INT16U j = 0;
	//  INT16U key = EM_key_NOHIT;
	//  int m = 0;
	INT8U passwd[10] = "555555";

	EA_vCls();
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "    ϵͳ����    ");
	//  sprintf((void *)buf, "���:%s", DevStat.pos_number);
	//  lcddisp(1, 1, (void *)buf);

	lcddisp(2, 1, (void *)"��������:");
	strcpy((void *)input, "");
	for (;;)
	{
		i = EA_ucGetInputStr(2, 10, 20, EM_BIG_FONT | EM_MODE_NUMBER | EM_ALIGN_LEFT | EM_SHOW_PASSWORD
							 , 6, 6, 0, (void *)input);
		if ( i == EM_SUCCESS )
			break;
		if ( i == EM_ABOLISH )
			return;
	}

	if ( strcmp((void *)input, (void *)passwd) == 0 )
	{}
	else
	{
		EA_ucSetInverse(EM_lcd_INVON);
		lcddisp(4, 1, (void *)"�������            ");
		EA_ucSetInverse(EM_lcd_INVOFF);
		(void)EA_uiInkey(1);
		return;
	}



	for (;;)
	{
		menus = browse_menu(1, (void *)"    ϵͳ����    ", setting_menu, TRUE);
		switch ( menus )
		{
		   case -1:             //ȡ����
			   return;

			   //  	   case 0:              //1.�������
			   //  		   set_banci();
			   //  		   break;
			   //
			   //  	   case 1:              //2.Ʊ������
			   //  		   set_ticket_number();
			   //  		   break;
			   //
			   //  	   case 0:              //1.��·ѡ��
			   //  		   choose_line();
			   //  		   break;
			   //
			   //  	   case 1:              //2.ʱ������
			   //  		   TimeModify();
			   //  		   break;
			   //
		   case 0:              //1.ʱ������
			   TimeModify();
			   break;

		   case 1:              //2.�豸�������
			   set_pos_number();
			   break;

		   case 2:              //3.��ʼ������
			   set_pos_init();
			   break;

		   case 3:              //4.�ϴ����ݼ�¼
			   send_backup_record_process();
			   break;

		   case 4:              //5.�����ۿ�
			   set_youhui_zhekou();
			   break;

		   default:
			   break;
		}

	}

}

/******************************************************************************
 �������ƣ�set_banci
 �������������ð��
 ����������
 �������ƣ�	����/�����	����		����
				
 ��  ��  ֵ����
				   
 ��      ��	������
 ��      �ڣ�2012-10-30
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void set_banci(void)
{
	INT8U i = 0;
	INT8U input[50];
	INT8U buf[50];
	//  INT16U j = 0;
	//  INT16U key = EM_key_NOHIT;
	int m = 0;

set_banci_start:
	EA_vCls();
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "    �������    ");
	sprintf((void *)buf, "���:%d", DevStat.banci);
	lcddisp(1, 1, (void *)buf);

	lcddisp(2, 1, (void *)"�����°��:");
	strcpy((void *)input, "");
	for (;;)
	{
		i = EA_ucGetInputStr(2, 12, 20, EM_BIG_FONT | EM_MODE_NUMBER | EM_ALIGN_LEFT | EM_SHOW_ORIGINALLY
							 , 1, 2, 0, (void *)input);
		if ( i == EM_SUCCESS )
			break;
		if ( i == EM_ABOLISH )
			return;
	}

	sscanf((void *)input, "%d", &m);
	if ( m == 0 )
	{
		EA_ucSetInverse(EM_lcd_INVON);
		lcddisp(4, 1, (void *)"��β���Ϊ0         ");
		EA_ucSetInverse(EM_lcd_INVOFF);
		(void)EA_uiInkey(1);
		goto set_banci_start;
	}

	DevStat.banci = (INT16U)m;

	WriteParam();

	EA_ucSetInverse(EM_lcd_INVON);
	lcddisp(4, 1, (void *)"�޸ĳɹ�            ");
	EA_ucSetInverse(EM_lcd_INVOFF);
	(void)EA_uiInkey(1);

}

/******************************************************************************
 �������ƣ�set_youhui_zhekou
 ���������������Żݿ��ۿ�
 ����������
 �������ƣ�	����/�����	����		����
				
 ��  ��  ֵ����
				   
 ��      ��	��������
 ��      �ڣ�2014-10-19
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void set_youhui_zhekou(void)
{
	INT8U i = 0;
	int tmp = 0;
	INT8U input[22];
	INT8U buf[22];

	EA_vCls();
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "  �Ż��ۿ�����  ");
	sprintf((void *)buf, "ԭʼ�ۿ�:%d         ", DevStat.zhekou);
	lcddisp(1, 1, (void *)buf);
	lcddisp(2, 1, (void *)"��ǰ�ۿ�:           ");
	strcpy((void *)input, "");

	for ( ;; )
	{
		i = get_input_str(2, 10, 2, 3, (void *)input);
		if ( i == EM_SUCCESS )
			break;
		if ( i == EM_ABOLISH )
			return;
	}
	sscanf((void *)input, "%d", (int *)&tmp);
	if(( 70 > tmp ) || ( tmp > 100 ))
	{
		lcddisperr("��ǰ�ۿ����ô���");
		//beeperr();
		return;
	}
	DevStat.zhekou = tmp;

	sprintf((void *)buf, "��ǰ�ۿ�: %d         ", DevStat.zhekou);
	lcddisp(2, 1, (void *)buf);

	WriteParam();

	EA_ucSetInverse(EM_lcd_INVON);
	lcddisp(4, 1, (void *)"      �޸ĳɹ�      ");
	EA_ucSetInverse(EM_lcd_INVOFF);
	(void)EA_uiInkey(1);
}
/******************************************************************************
 �������ƣ�set_pos_number
 ��������������POS��
 ����������
 �������ƣ�	����/�����	����		����
				
 ��  ��  ֵ����
				   
 ��      ��	������
 ��      �ڣ�2012-10-30
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void set_pos_number(void)
{
	INT8U i = 0;
	INT8U input[50];
	INT8U buf[50];
	//  INT16U j = 0;
	//  INT16U key = EM_key_NOHIT;
	//  int m = 0;

	EA_vCls();
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "  �豸�������  ");
	sprintf((void *)buf, "���:%s", DevStat.pos_number);
	lcddisp(1, 1, (void *)buf);

	lcddisp(2, 1, (void *)"�����±��:");
	strcpy((void *)input, "");

	for ( ;; )
	{
		i = EA_ucGetInputStr(2, 12, 20, EM_BIG_FONT | EM_MODE_NUMBER | EM_ALIGN_LEFT | EM_SHOW_ORIGINALLY
							 , 6, 6, 0, (void *)input);
		if ( i == EM_SUCCESS )
			break;
//  	if ( i == EM_ABOLISH )
//  		return;
	}

	strcpy((void *)&DevStat.pos_number[0], (void *)input);

	WriteParam();

	EA_ucSetInverse(EM_lcd_INVON);
	lcddisp(4, 1, (void *)"�޸ĳɹ�            ");
	EA_ucSetInverse(EM_lcd_INVOFF);
	(void)EA_uiInkey(1);

}

/******************************************************************************
 �������ƣ�set_pos_init
 ������������ʼ��POS��
 ����������
 �������ƣ�	����/�����	����		����
				
 ��  ��  ֵ����
				   
 ��      ��	������
 ��      �ڣ�2012-10-30
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void set_pos_init(void)
{
	INT8U i = 0;
	INT8U input[50];
	//  INT8U buf[50];
	//  INT16U j = 0;
	//  INT16U key = EM_key_NOHIT;
	//  int m = 0;

	EA_vCls();
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "   ��ʼ������   ");
	//  sprintf((void *)buf, "���:%s", DevStat.pos_number);
	//  lcddisp(1, 1, (void *)buf);
	lcddisp(1, 1, (void *)"�������ݽ�ȫ�����!!");

	lcddisp(2, 1, (void *)"����1��ȷ��:");
	strcpy((void *)input, "");

	i = EA_ucGetInputStr(2, 13, 20, EM_BIG_FONT | EM_MODE_NUMBER | EM_ALIGN_LEFT | EM_SHOW_ORIGINALLY
						 , 1, 1, 0, (void *)input);
	if ( i != EM_SUCCESS )
		return;

	if ( strcmp((void *)input, "1") == 0 )
	{
		EA_ucSetInverse(EM_lcd_INVON);
		lcddisp(4, 1, (void *)"��ʼ����..          ");
		EA_ucSetInverse(EM_lcd_INVOFF);
		param_factory_default(2);

		EA_ucSetInverse(EM_lcd_INVON);
		lcddisp(4, 1, (void *)"��ʼ�����          ");
		EA_ucSetInverse(EM_lcd_INVOFF);
		lcddisp(2, 1, (void *)"    �������ϵ�!!    ");
		(void)EA_uiInkey(1);
		for (;;)
		{
			;
		}
		//  	resetpos();
	}
	else
	{
		return;
	}


}

/******************************************************************************
 �������ƣ�generate_ticket_info
 ��������������Ʊ�ۼ�¼
 ����������
 �������ƣ�	����/�����	����		����
				
 ��  ��  ֵ����
				   
 ��      ��	������
 ��      �ڣ�2012-10-30
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void generate_ticket_info(void)
{
	strcpy((void *)&Tinfo.pos_number[0], (void *)&DevStat.pos_number[0]);                   //������ 6
	sprintf((void *)&Tinfo.ticket_number[0], "%07d", DevStat.cur_ticket_number);            //Ʊ�� 7
	strcpy((void *)&Tinfo.ticket_time[0], (void *)&DevStat.sell_ticket_times[0]);           //�շ�����ʱ��  14
	strcpy((void *)&Tinfo.op_number[0], (void *)&DevStat.op_number[0]);                     //�շ�Ա��  6
	strcpy((void *)&Tinfo.start_station_num[0], (void *)&DevStat.start_station_number[0]);  //��ʼվ��� 4
	strcpy((void *)&Tinfo.start_station_name[0], (void *)&DevStat.start_station_name[0]);   //��ʼվ   14
	strcpy((void *)&Tinfo.end_station_num[0], (void *)&DevStat.end_station_number[0]);      //����վ��� 4
	strcpy((void *)&Tinfo.end_station_name[0], (void *)&DevStat.end_station_name[0]);       //����վ  14
	//  if ( DevStat.half_discount == TRUE )
	//  	strcpy((void *)&Tinfo.price_type[0], "2");                                      //�۸�����(ȫƱ����Ʊ��3/4Ʊ) 1  ȫƱ="1" ��Ʊ="2"
	//  else
	//  	strcpy((void *)&Tinfo.price_type[0], "1");                                      //�۸�����(ȫƱ����Ʊ��3/4Ʊ) 1  ȫƱ="1" ��Ʊ="2"
	Tinfo.price_type[0] = DevStat.ticket_type;                                              //�۸�����(ȫƱ����Ʊ��3/4Ʊ) 1  ȫƱ="1" ��Ʊ="2"  �Ż�=��7��  ��Ʊ =�� 8��
	Tinfo.price_type[1] = '\0';
	strcpy((void *)&Tinfo.price[0], (void *)&DevStat.prices[0]);                            //�շѽ�� 7
	strcpy((void *)&Tinfo.bus_number[0], (void *)&DevStat.bus_number_full[0]);              //���� 9 ³P-12345
	sprintf((void *)&Tinfo.banci[0], "%d", DevStat.banci);                                  //��� 2
	if ( DevStat.direction == FORWARD )
		strcpy((void *)&Tinfo.direction[0], "1");                                           //���� 1 ����=1 ����=2
	else
		strcpy((void *)&Tinfo.direction[0], "2");                                           //���� 1 ����=1 ����=2
	strcpy((void *)&Tinfo.line_number[0], (void *)&DevStat.line_number[0]);                 //��·��� 5

	//������ˢ�����3���ֶ�
	sprintf((void *)&Tinfo.trade_type[0], "%02X", CardInfo.card_purchase_type);				//��������

	switch ( CardInfo.card_purchase_type )
	{
		case 0x06:
		case 0x07:
		case 0x11:
			sprintf((void *)&Tinfo.card_number[0], "%02X%02X%02X%02X", 
					CardInfo._CARD_NUMBER.detail_card_number.serial_number[0], 
					CardInfo._CARD_NUMBER.detail_card_number.serial_number[1], 
					CardInfo._CARD_NUMBER.detail_card_number.serial_number[2], 
					CardInfo._CARD_NUMBER.detail_card_number.serial_number[3] );			//����
			if ( CardInfo.balance > 99999999 )
				CardInfo.balance = 99999999;
			sprintf((void *)&Tinfo.card_balance[0], "%d", CardInfo.balance);				//�����
			break;

		default:
			memset((void *)&Tinfo.card_number[0], 0x00, sizeof(Tinfo.card_number));			//�ֽ��޴��ֶ�
			memset((void *)&Tinfo.card_balance[0], 0x00, sizeof(Tinfo.card_balance));		//�ֽ��޴��ֶ�
			break;
	}

}

/******************************************************************************
 �������ƣ�generate_summary_info
 ��������������Ʊ�ۼ�¼
 ����������
 �������ƣ�	����/�����	����		����
				
 ��  ��  ֵ����
				   
 ��      ��	������
 ��      �ڣ�2012-10-30
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void generate_summary_info(TODAY_SUM *pbill)
{
	//BUS_TIME ltime;
//  (void)Get_Time(&ltime);
//  memcpy(&DevStat.sell_ticket_time.century, &ltime.century, sizeof(BUS_TIME));
//  sprintf((void *)&DevStat.sell_ticket_times[0], "%02X%02X%02X%02X%02X%02X%02X",
//  		ltime.century, ltime.year, ltime.month, ltime.day, ltime.hour, ltime.minute, ltime.second);

	strcpy((void *)&Tinfo.pos_number[0], (void *)&DevStat.pos_number[0]);                   //������ 6
	sprintf((void *)&Tinfo.ticket_number[0], "%07d", DevStat.cur_ticket_number);            //Ʊ�� 7
	strcpy((void *)&Tinfo.ticket_time[0], (void *)&pbill->query_times[0]);           //�շ�����ʱ��  14
	strcpy((void *)&Tinfo.op_number[0], (void *)&DevStat.op_number[0]);                     //�շ�Ա��  6
	strcpy((void *)&Tinfo.start_station_num[0], "0000");  //��ʼվ��� 4  �̶�����0000�ĸ�0
	strcpy((void *)&Tinfo.start_station_name[0], "ʼ��վ");   //��ʼվ   14
	strcpy((void *)&Tinfo.end_station_num[0],"0000");      //����վ��� 4  �̶�����0000�ĸ�0
	strcpy((void *)&Tinfo.end_station_name[0], "�յ�վ");       //����վ  14
//  if ( DevStat.half_discount == TRUE )
//  	strcpy((void *)&Tinfo.price_type[0], "2");                                          //�۸�����(ȫƱ����Ʊ��3/4Ʊ) 1  ȫƱ="1" ��Ʊ="2"
//  else
//  	strcpy((void *)&Tinfo.price_type[0], "1");                                          //�۸�����(ȫƱ����Ʊ��3/4Ʊ) 1  ȫƱ="1" ��Ʊ="2"
	Tinfo.price_type[0] = '6';                     //�۸�����(ȫƱ����Ʊ��3/4Ʊ) 1  ȫƱ="1" ��Ʊ="2",�ɷѵ� = "6", youhui = "7" ,  yuepiao = "8"
	Tinfo.price_type[1] = '\0';
	strcpy((void *)&Tinfo.price[0], "0.0");                            //�շѽ�� 7
	strcpy((void *)&Tinfo.bus_number[0], (void *)&DevStat.bus_number_full[0]);              //���� 9 ³P-12345
	if(pbill->day_or_banci == 0)
	{
		sprintf((void *)&Tinfo.banci[0], "%d", 7);                                  //��� 2
	}
	else
	{
		sprintf((void *)&Tinfo.banci[0], "%d", pbill->banci);	
	}

	if ( DevStat.direction == FORWARD )
		strcpy((void *)&Tinfo.direction[0], "1");                                           //���� 1 ����=1 ����=2
	else
		strcpy((void *)&Tinfo.direction[0], "2");                                           //���� 1 ����=1 ����=2
	strcpy((void *)&Tinfo.line_number[0], (void *)&DevStat.line_number[0]);                 //��·��� 5

	memset((void *)&Tinfo.trade_type[0], 0x00, sizeof(Tinfo.trade_type));			//���������޴��ֶ�
	memset((void *)&Tinfo.card_number[0], 0x00, sizeof(Tinfo.card_number));			//���㵥�޴��ֶ�
	memset((void *)&Tinfo.card_balance[0], 0x00, sizeof(Tinfo.card_balance));		//���㵥�޴��ֶ�
}
/******************************************************************************
 �������ƣ�set_ticket_number
 ��������������Ʊ��
 ����������
 �������ƣ�	����/�����	����		����
				
 ��  ��  ֵ����
				   
 ��      ��	������
 ��      �ڣ�2012-10-30
 �޸���ʷ��
    	���ڣ�2012-12-1		�޸���: ������		�޸�������������������Ʊ�ţ�
    	�����ж����ε�Ʊ�ű���һ��
    	���ɽ�����һ������
		------		---------	-------------
******************************************************************************/
void set_ticket_number(void)
{
	INT8U i = 0;
	INT8U input1[50];
	INT8U input2[50];
	//  INT8U buf[50];
	//  INT16U j = 0;
	//  INT16U key = EM_key_NOHIT;
	long m = 0;

	EA_vCls();
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "    Ʊ������    ");
	//  sprintf((void *)buf, "��ǰƱ��:%07d", DevStat.cur_ticket_number);
	//  lcddisp(1, 1, (void *)buf);
	//
	//  lcddisp(2, 1, (void *)"�����º�:");
	//  strcpy((void *)input, "");
	//  for (;;)
	//  {
	//  	i = EA_ucGetInputStr(2, 10, 20, EM_BIG_FONT | EM_MODE_NUMBER | EM_ALIGN_LEFT | EM_SHOW_ORIGINALLY
	//  						 , 7, 7, 0, (void *)input);
	//  	if ( i == EM_SUCCESS )
	//  		break;
	//  	if ( i == EM_ABOLISH )
	//  		return;
	//  }
input_again:
	lcddisp(2, 1, (void *)"����Ʊ��:           ");
	strcpy((void *)input1, "");
	strcpy((void *)input2, "");
	for (;;)
	{
		i = EA_ucGetInputStr(2, 10, 20, EM_BIG_FONT | EM_MODE_NUMBER | EM_ALIGN_LEFT | EM_SHOW_ORIGINALLY
							 , 7, 7, 0, (void *)input1);
		if ( i == EM_SUCCESS )
			break;
//  	if ( i == EM_ABOLISH )
//  		return;
	}
	lcddisp(2, 1, (void *)"����Ʊ��:           ");
	for (;;)
	{
		i = EA_ucGetInputStr(2, 10, 20, EM_BIG_FONT | EM_MODE_NUMBER | EM_ALIGN_LEFT | EM_SHOW_ORIGINALLY
							 , 7, 7, 0, (void *)input2);
		if ( i == EM_SUCCESS )
			break;
//  	if ( i == EM_ABOLISH )
//  		return;
	}
	if ( strcmp((void *)input1, (void *)input2) == 0 )
	{
		//sprintf((void *)buf, "����:%s %s", admin_num, admin_name);
		lcddisp(4, 1, (void *)"Ʊ�����óɹ�!       ");	
	}
	else
	{
		goto input_again;
	}
	sscanf((void *)input1, "%ld", &m);
	DevStat.cur_ticket_number = (INT32U)m;

	WriteParam();

	EA_ucSetInverse(EM_lcd_INVON);
	lcddisp(4, 1, (void *)"�޸ĳɹ�            ");
	EA_ucSetInverse(EM_lcd_INVOFF);
	(void)EA_uiInkey(1);

}

/******************************************************************************
 �������ƣ�print_ticket
 �������������ݼ�¼��Ʊ 
 �������ƣ�	����/�����	����		����
 ticket		����		TICKET_INFO *	��Ʊ�ļ�¼
 
 ��  ��  ֵ����
 
 ��      �ߣ�����
 ��      �ڣ�2012-10-29
 �޸���ʷ��
	����		�޸���		�޸�����
	------		---------	-------------
******************************************************************************/
void print_ticket(TICKET_INFO *ticket)
{
	INT8U ucRet = 0;
	FP64 balance=0.0;
	//  DevHandle hDprinterHandle;
	char printbuf[300];

	//  // ��ӡ�豸����I/Oģ���һ�֣�Ӧ�Ȼ�ò����������ʱ������ӡ���õ�ģʽ
	//  ucRet = EA_ucOpenDevice("PRINT", EM_io_REALMODE, &hDprinterHandle);
	//  if ( ucRet != EM_SUCCESS )
	//  {
	//  	lcddisperr("��ô�ӡ�������ʧ��!");
	//  	return;
	//  }

	//    // POS��ʱ,���ó�ʼ��������ʼ����ӡ��о״̬
	//    ucRet =  EA_ucInitPrinter(hDprinterHandle);
	//    if ( ucRet == EM_prn_PAPERENDED )
	//    {
	////  	(void)EA_ucCloseDevice(&hDprinterHandle);
	//        lcddisperr("��ֽ!��ӡʧ��!");
	//        return;
	//    }
	//    else if ( ucRet != EM_SUCCESS )
	//    {
	////  	(void)EA_ucCloseDevice(&hDprinterHandle);
	//        lcddisperr("��ʼ����ӡ��оʧ��!");
	//        return;
	//    }

	// �ɽ��д�ӡ��һЩ��������
	ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_XSPACE, 0);
	if ( ucRet != EM_SUCCESS )
	{
		//  	(void)EA_ucCloseDevice(&hDprinterHandle);
		lcddisperr("��ӡ������ʧ��!");
		return;
	}
//  ucRet =  EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_YSPACE, 4);
	ucRet =  EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_YSPACE, 2);
	if ( ucRet != EM_SUCCESS )
	{
		//  	(void)EA_ucCloseDevice(&hDprinterHandle);
		lcddisperr("��ӡ������ʧ��!");
		return;
	}

	//  ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_PAGESPACE, 578);
//  ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_PAGESPACE, 577);
	ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_PAGESPACE, 576);
	if ( ucRet != EM_SUCCESS )
	{
		//  	(void)EA_ucCloseDevice(&hDprinterHandle);
		lcddisperr("��ӡ������ʧ��!");
		return;
	}

	ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_ASCII_DOTTYPE, EM_prn_ASCIIDOT16X8);
	if ( ucRet != EM_SUCCESS )
	{
		//  	(void)EA_ucCloseDevice(&hDprinterHandle);
		lcddisperr("��ӡ������ʧ��!");
		return;
	}

	ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_HZ_DOTTYPE, EM_prn_HZDOT16X16);
	if ( ucRet != EM_SUCCESS )
	{
		//  	(void)EA_ucCloseDevice(&hDprinterHandle);
		lcddisperr("��ӡ������ʧ��!");
		return;
	}

	ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_HZ_PRNTYPE, EM_prn_HZPRN2X2);
	if ( ucRet != EM_SUCCESS )
	{
		//  	(void)EA_ucCloseDevice(&hDprinterHandle);
		lcddisperr("��ӡ������ʧ��!");
		return;
	}

	ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_PRNGRAY, 8);
	if ( ucRet != EM_SUCCESS )
	{
		//  	(void)EA_ucCloseDevice(&hDprinterHandle);
		lcddisperr("��ӡ������ʧ��!");
		return;
	}

	//  ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_ASCII_PRNTYPE, EM_prn_ASCIIPRN2X2);
	//  if ( ucRet != EM_SUCCESS )
	//  {
	//  	lcddisperr("��ӡ������ʧ��!");
	//  	return;
	//  }

	// ��ӡ����
	//  ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "��ӡ����!\n");
	//  if ( ucRet != EM_SUCCESS )
	//  {
	//  	EA_vDisplay(2, "���д�ӡʧ��!");
	//  	return;
	//  }
	//  ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "�����״�ͨ���Ӽ������޹�˾1\n");
	//  ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "�����״�ͨ���Ӽ�����23\n");
	//  ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "abcd1234ABCD!@#$\n");
	//  ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "�����״�ͨ���Ӽ������޹�˾4\n");


	//  �ϳ�վ���³�վ
	//  ȫƱ��1234.0Ԫ
	//	2012-11-04 17:06:38
	//  ��Σ� 2		����
	//  ���ţ�³P-12345
	//  ��ƱԱ�ţ� 1234
	//  Ʊ�ţ�  1234567
	//	POS���ţ�123456

	//  (void)EA_ucLinePrinter(hDprinterHandle, 0, "\n");
	sprintf(printbuf, "%s��%s\n", ticket->start_station_name, ticket->end_station_name);
	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);               //�ϳ�վ���³�վ

//  if ( strcmp((void *)ticket->price_type, "1") == 0 )
//  	sprintf(printbuf, "    ȫƱ��%7sԪ\n", ticket->price);
//  else
//  	sprintf(printbuf, "    ��Ʊ��%7sԪ\n", ticket->price);
	if ( strcmp((void *)ticket->price_type, "1") == 0 )
	{
		sprintf(printbuf, "    ȫƱ��%7sԪ\n", ticket->price);
	}
	else if ( strcmp((void *)ticket->price_type, "2") == 0 )
	{
		sprintf(printbuf, "    ��Ʊ��%7sԪ\n", ticket->price);
	}
	else if ( strcmp((void *)ticket->price_type, "7") == 0 )   //�Żݿ�
	{
		sprintf(printbuf, "  �Ż�Ʊ��%7sԪ\n", ticket->price);
	}
	else if ( strcmp((void *)ticket->price_type, "8") == 0 )   //��Ʊ��
	{
		sprintf(printbuf, "    ��Ʊ��%7sԪ\n", ticket->price);
	}
	else
	{
		sprintf(printbuf, "    ��Ʊ��%7sԪ\n", ticket->price);
	}

	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);               //ȫƱ��1234.00Ԫ

	sprintf(printbuf, "%c%c%c%c-%c%c-%c%c %c%c:%c%c:%c%c\n",
			ticket->ticket_time[0], ticket->ticket_time[1], ticket->ticket_time[2], ticket->ticket_time[3],
			ticket->ticket_time[4], ticket->ticket_time[5], ticket->ticket_time[6], ticket->ticket_time[7],
			ticket->ticket_time[8], ticket->ticket_time[9], ticket->ticket_time[10], ticket->ticket_time[11],
			ticket->ticket_time[12], ticket->ticket_time[13]);
	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);               //2012-11-04 17:06:38

	//  if ( strcmp((void *)ticket->direction, "1") == 0 )
	//      sprintf(printbuf, "��Σ�%2s       ����\n", ticket->banci);
	//  else
	//      sprintf(printbuf, "��Σ�%2s       ����\n", ticket->banci);
	if ( strcmp((void *)ticket->price_type, "7") == 0 )
	{
		balance = atof((void *)&ticket->card_balance[0]);
		sprintf(printbuf, "������%5.2fԪ\n", balance / 100.00);
		(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);
		sprintf(printbuf, "��    �ţ�%7s\n", ticket->card_number);
		(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);
	}
//  else if ( strcmp((void *)ticket->price_type, "8" == 0 )
//  {
//  	sprintf(printbuf, "��ֹ���ڣ�%2s %5s \n", ticket->banci, ticket->line_number);
//  	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);
//  	sprintf(printbuf, "��    �ţ�%7s\n", ticket->card_number);
//  	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);
//  }

	if ( strcmp((void *)ticket->direction, "1") == 0 )
		sprintf(printbuf, "��Σ�%2s %5s ����\n", ticket->banci, ticket->line_number);
	else
		sprintf(printbuf, "��Σ�%2s %5s ����\n", ticket->banci, ticket->line_number);
	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);               //��Σ� 2  @406 ����

	sprintf(printbuf, "���ţ�%s\n", ticket->bus_number);
	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);               //���ţ�³P-12345

	sprintf(printbuf, "��ƱԱ�ţ� %s\n", ticket->op_number);
	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);               //��ƱԱ�ţ�1234

	sprintf(printbuf, "Ʊ�ţ�  %s\n", ticket->ticket_number);
	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);               //Ʊ�ţ�1234567

	sprintf(printbuf, "POS���ţ�%s\n", ticket->pos_number);
	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);               //POS���ţ�123456


	// ��ӡ�����û�ҳ����
	ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "\f");
	if ( ucRet == EM_SUCCESS )
	{
		EA_vDisplay(2, "���δ�ӡ����!");
	}

	//  (void)EA_ucCloseDevice(&hDprinterHandle);

}

/******************************************************************************
 �������ƣ�query_capacity
 ������������ѯ��¼�ռ�
 ����������
 �������ƣ�	����/�����	����		����
				
 ��  ��  ֵ����
				   
 ��      ��	������
 ��      �ڣ�2012-10-30
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void query_capacity(void)
{
	//  INT8U i = 0;
	//  INT8U input[50];
	INT8U buf[50];
	//  INT16U j = 0;
	//  INT16U key = EM_key_NOHIT;
	//  int m = 0;
	const char recFileName[] = "record";
	//  INT32U uiRecordLen = sizeof(TICKET_INFO);
	//  INT8U i = 0;
	INT8U ucOpenID = 0;
	INT32U uiResult;


	// ���ļ�
	if ( EA_ucPFOpen((uchar *)recFileName, &ucOpenID) != EM_ffs_SUCCESS )
	{
		lcddisperr("�򿪼�¼�ļ�ʧ��");
		return;
	}


	EA_vCls();
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "  ��¼������ѯ  ");

	(void)EA_ucPFInfo(ucOpenID, EM_ffs_VALIDREC, &uiResult);
	sprintf((void *)buf, "��ǰ��¼��:%d", uiResult);
	lcddisp(1, 1, (void *)buf);

	(void)EA_ucPFInfo(ucOpenID, EM_ffs_FREEREC, &uiResult);
	sprintf((void *)buf, "ʣ���¼��:%d", uiResult);
	lcddisp(2, 1, (void *)buf);

	if ( EA_ucPFClose(ucOpenID) != EM_ffs_SUCCESS )
	{
		lcddisperr("�رռ�¼�ļ�ʧ��");
		return;
	}

	(void)EA_uiInkey(0);

}

///******************************************************************************
// �������ƣ�query_today_summary
// ������������ѯ���ջ���
// ����������
// �������ƣ� ����/����� ����        ����
//
// ��  ��  ֵ����
//
// ��      �� ������
// ��      �ڣ�2012-10-30
// �޸���ʷ��
//        ����        �޸���      �޸�����
//        ------      ---------   -------------
//******************************************************************************/
//void query_today_summary(void)
//{
//    INT8S menus = 0;
//    //  INT8U i = 0;
//    //  INT8U input[20];
//    INT8U buf[30];
//    INT32U j = 0;
//    //  INT16U key = EM_key_NOHIT;
//    const char recFileName[] = "record";
//    INT8U ucOpenID = 0;
//    INT32U uiResult;
//    INT32U uiReadCnt;
//    INT32U total_rec_num = 0;
//    TICKET_INFO ticket;
//    BUS_TIME ltime;
//    INT8U day[30];
//    //  INT32U full_num = 0;        //ȫ��Ʊ����
//    //  INT32U full_sum = 0;        //ȫ��Ʊ�ܶ� ��λ��
//    //  INT32U half_num = 0;        //���Ʊ����
//    //  INT32U half_sum = 0;        //���Ʊ�ܶ� ��λ��
//    FP32 fprice = 0.0;
//    INT32U price = 0;
//    struct _TODAY_SUM
//    {
//        INT32U full_num;        //ȫ��Ʊ����
//        INT32U full_sum;        //ȫ��Ʊ�ܶ� ��λ��
//        INT32U half_num;        //���Ʊ����
//        INT32U half_sum;        //���Ʊ�ܶ� ��λ��
//        INT32U total_num;       //������
//        INT32U total_sum;       //�ܶ�
//    }banci[101], today;
//    INT32U cur_banci = 0;
//    int m = 0;
//    INT32U max_banci = 0;
//    INT8U sum_menu[101 * 20 + 2];
//    INT32S n = 0;
//
//
//
//    //  for (;;)
//    //  {
//    //      menus = browse_menu(1, (void *)"    ѡ����·    ", line_menu, FALSE);
//    //      if ( menus == -1 )
//    //          continue;           //ȡ��������
//    //      else
//    //          break;
//    //  }
//    //
//    //  DevStat.line_ptr = menus;
//    //  strcpy((void *)&DevStat.line_number[0], (void *)&Linfo[DevStat.line_ptr].number[0]);
//    //  strcpy((void *)&DevStat.line_menu_name[0], (void *)&Linfo[DevStat.line_ptr].menu_name[0]);
//    //  strcpy((void *)&DevStat.line_full_name[0], (void *)&Linfo[DevStat.line_ptr].full_name[0]);
//
//
//    EA_vCls();
//    EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "    ����ͳ��    ");
//    lcddisp(2, 1, (void *)"      ͳ����...");
//
//
//    // ���ļ�
//    if ( EA_ucPFOpen((uchar *)recFileName, &ucOpenID) != EM_ffs_SUCCESS )
//    {
//        lcddisperr("�򿪼�¼�ļ�ʧ��");
//        return;
//    }
//
//    (void)EA_ucPFInfo(ucOpenID, EM_ffs_VALIDREC, &uiResult);
//    total_rec_num = uiResult;
//    (void)Get_Time(&ltime);
//    sprintf((void *)day, "%02X", ltime.day);
//
//    today.full_num = 0;
//    today.full_sum = 0;
//    today.half_num = 0;
//    today.half_sum = 0;
//    today.total_num = 0;
//    today.total_sum = 0;
//
//    for ( j = 0; j < 100; j++ )
//    {
//        banci[j].full_num = 0;
//        banci[j].full_sum = 0;
//        banci[j].half_num = 0;
//        banci[j].half_sum = 0;
//        banci[j].total_num = 0;
//        banci[j].total_sum = 0;
//    }
//
//
//    //  for ( j = 0; j < total_rec_num; j++ )
//    for ( n = total_rec_num; n >= 1; n-- )
//    {
//        (void)EA_ucPFReadRec(ucOpenID, n, 1, &uiReadCnt, &ticket.pos_number[0]);
//        if ( !(day[0] == ticket.ticket_time[6] && day[1] == ticket.ticket_time[7]) )
//        {
//            //debug
//            sprintf((void *)buf, "%c%c %c%c", day[0], day[1], ticket.ticket_time[6], ticket.ticket_time[7]);
//            lcddisperr((void *)buf);
//            //debug end
//            break;          //����ͬһ�죬�˳�
//        }
//
//        sscanf((void *)&ticket.banci[0], "%d", &m);
//        cur_banci = (INT32U)m;
//        if ( cur_banci > max_banci )
//            max_banci = cur_banci;
//
//        if ( strcmp((void *)&ticket.price_type[0], "1") == 0 )
//        {
//            //ȫ��Ʊ
//            banci[cur_banci].full_num++;
//            today.full_num++;
//            sscanf((void *)&ticket.price[0], "%f", &fprice);
//            price = (INT32U)(fprice * 100.0);
//            banci[cur_banci].full_sum += price;
//            today.full_sum += price;
//
//            banci[cur_banci].total_num++;
//            banci[cur_banci].total_sum += price;
//            today.total_num++;
//            today.total_sum += price;
//        }
//        else if ( strcmp((void *)&ticket.price_type[0], "2") == 0 )
//        {
//            //���Ʊ
//            banci[cur_banci].half_num++;
//            today.half_num++;
//            sscanf((void *)&ticket.price[0], "%f", &fprice);
//            price = (INT32U)(fprice * 100.0);
//            banci[cur_banci].half_sum += price;
//            today.half_sum += price;
//
//            banci[cur_banci].total_num++;
//            banci[cur_banci].total_sum += price;
//            today.total_num++;
//            today.total_sum += price;
//        }
//        else
//        {
//            //��Ʊ������
//        }
//
//    }
//
//    if ( EA_ucPFClose(ucOpenID) != EM_ffs_SUCCESS )
//    {
//        lcddisperr("�رռ�¼�ļ�ʧ��");
//        return;
//    }
//
//    //���ɲ˵�
//    strcpy((void *)sum_menu, "���ջ���            ");
//    for ( j = 1; j <= max_banci; j++ )
//    {
//        sprintf((void *)buf, "���%2d              ", j);
//        strcat((void *)sum_menu, (void *)buf);
//    }
//
//
//    for (;;)
//    {
//        menus = browse_menu(1, (void *)"    ����ͳ��    ", sum_menu, TRUE);
//        if ( menus == -1 )                //�˳�
//        {
//            return;
//        }
//        else if ( menus == 0 )             //���ջ���
//        {
//            EA_vCls();
//            EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "    ���ջ���    ");
//            sprintf((void *)buf, "ȫƱ%4d��%8.1fԪ", today.full_num, (FP32)today.full_sum / 100.0);
//            lcddisp(1, 1, (void *)buf);
//            sprintf((void *)buf, "��Ʊ%4d��%8.1fԪ", today.half_num, (FP32)today.half_sum / 100.0);
//            lcddisp(2, 1, (void *)buf);
//            sprintf((void *)buf, "�ܼ�%4d��%8.1fԪ", today.total_num, (FP32)today.total_sum / 100.0);
//            lcddisp(4, 1, (void *)buf);
//
//            (void)EA_uiInkeyMs(0);
//        }
//        else
//        {
//            if ( menus > 99 )
//                menus = 99;
//
//            EA_vCls();
//            sprintf((void *)buf, "   ���%2d   ", menus);
//            EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, (void *)buf);
//            sprintf((void *)buf, "ȫƱ%4d��%8.1fԪ", banci[menus].full_num, (FP32)banci[menus].full_sum / 100.0);
//            lcddisp(1, 1, (void *)buf);
//            sprintf((void *)buf, "��Ʊ%4d��%8.1fԪ", banci[menus].half_num, (FP32)banci[menus].half_sum / 100.0);
//            lcddisp(2, 1, (void *)buf);
//            sprintf((void *)buf, "�ܼ�%4d��%8.1fԪ", banci[menus].total_num, (FP32)banci[menus].total_sum / 100.0);
//            lcddisp(4, 1, (void *)buf);
//
//            (void)EA_uiInkeyMs(0);
//        }
//
//
//    }
//}


/******************************************************************************
 �������ƣ�query_today_summary
 ������������ѯ���ջ���
 ����������
 �������ƣ�	����/�����	����		����
				
 ��  ��  ֵ����
				   
 ��      ��	������
 ��      �ڣ�2012-10-30
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void query_today_summary(void)
{
	INT8S menus = 0;
	//  INT8U i = 0;
	//  INT8U input[20];
	INT8U buf[30];
	INT32U j = 0;


	INT16U key = EM_key_NOHIT;
	const char recFileName[] = "record";
	INT8U ucOpenID = 0;
	INT32U uiResult;
	INT32U uiReadCnt;
	INT32S total_rec_num = 0;
	TICKET_INFO ticket;
	TODAY_SUM banci[10];
	TODAY_SUM today;
	BUS_TIME ltime;
	INT8U day[30];
	FP32 fprice = 0.0;
	INT32U price = 0;
	INT32U cur_banci = 0;
	int m = 0;
	INT32U max_banci = 0;
	INT8U sum_menu[101 * 20 + 2];
	INT32S n = 0;


	EA_vCls();
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "    ����ͳ��    ");
	lcddisp(2, 1, (void *)"      ͳ����...");


	// ���ļ�
	if ( EA_ucPFOpen((uchar *)recFileName, &ucOpenID) != EM_ffs_SUCCESS )
	{
		lcddisperr("�򿪼�¼�ļ�ʧ��");
		return;
	}

	(void)EA_ucPFInfo(ucOpenID, EM_ffs_VALIDREC, &uiResult);
	total_rec_num = uiResult;
	(void)Get_Time(&ltime);
	sprintf((void *)day, "%02X", ltime.day);

	today.full_num = 0;
	today.full_sum = 0;
	today.half_num = 0;
	today.half_sum = 0;
	today.package_num = 0;
	today.package_sum = 0;
	today.valid_num = 0;
	today.valid_sum = 0;
	today.youhui_num = 0;      //�Żݿ�����
	today.youhui_sum = 0;      //�Żݿ��ܶ� ��λ��
	today.yuepiao_num = 0;     //��Ʊ������
	today.yuepiao_sum = 0;     //��Ʊ���ܶ� ��λ��
	today.total_num = 0;
	today.total_sum = 0;
	today.day_or_banci = 0;

	for ( j = 0; j < 10; j++ )
	{
		banci[j].full_num = 0;
		banci[j].full_sum = 0;
		banci[j].half_num = 0;
		banci[j].half_sum = 0;
		banci[j].package_num = 0;
		banci[j].package_sum = 0;
		banci[j].valid_num = 0;
		banci[j].valid_sum = 0;
		banci[j].youhui_num = 0;
		banci[j].youhui_sum = 0;
		banci[j].yuepiao_num = 0;
		banci[j].yuepiao_sum = 0;
		banci[j].total_num = 0;
		banci[j].total_sum = 0;
		banci[j].day_or_banci = 1;
	}


	//  for ( j = 0; j < total_rec_num; j++ )
	for ( n = total_rec_num; n >= 1; n-- )
	{
		(void)EA_ucPFReadRec(ucOpenID, n, 1, &uiReadCnt, &ticket.pos_number[0]);
		if ( !(day[0] == ticket.ticket_time[6] && day[1] == ticket.ticket_time[7]) )
		{
			//debug
//  		sprintf((void *)buf, "%c%c %c%c", day[0], day[1], ticket.ticket_time[6], ticket.ticket_time[7]);
//  		lcddisperr((void *)buf);
			//debug end
			break;          //����ͬһ�죬�˳�
		}

		sscanf((void *)&ticket.banci[0], "%d", &m);
		cur_banci = (INT32U)m;
		if ( cur_banci > max_banci )
			max_banci = cur_banci;

		if ( strcmp((void *)&ticket.price_type[0], "1") == 0 )
		{
			//ȫ��Ʊ
			banci[cur_banci].full_num++;
			today.full_num++;
			sscanf((void *)&ticket.price[0], "%f", &fprice);
			price = (INT32U)(fprice * 100.0);
			banci[cur_banci].full_sum += price;
			today.full_sum += price;

			banci[cur_banci].total_num++;
			banci[cur_banci].total_sum += price;
			today.total_num++;
			today.total_sum += price;
		}
		else if ( strcmp((void *)&ticket.price_type[0], "2") == 0 )
		{
			//���Ʊ
			banci[cur_banci].half_num++;
			today.half_num++;
			sscanf((void *)&ticket.price[0], "%f", &fprice);
			price = (INT32U)(fprice * 100.0);
			banci[cur_banci].half_sum += price;
			today.half_sum += price;

			banci[cur_banci].total_num++;
			banci[cur_banci].total_sum += price;
			today.total_num++;
			today.total_sum += price;
		}
		else if ( strcmp((void *)&ticket.price_type[0], "3") == 0 )
		{
			//��Ʊ
			banci[cur_banci].package_num++;
			today.package_num++;
			sscanf((void *)&ticket.price[0], "%f", &fprice);
			price = (INT32U)(fprice * 100.0);
			banci[cur_banci].package_sum += price;
			today.package_sum += price;

			banci[cur_banci].total_num++;
			banci[cur_banci].total_sum += price;
			today.total_num++;
			today.total_sum += price;
		}
		else if ( strcmp((void *)&ticket.price_type[0], "4") == 0 )
		{
//  		//��Ʊ
//  		banci[cur_banci].valid_num++;
//  		today.valid_num++;
//  		sscanf((void *)&ticket.price[0], "%f", &fprice);
//  		price = (INT32U)(fprice * 100.0);
//  		banci[cur_banci].valid_sum += price;
//  		today.valid_sum += price;
//
//  		banci[cur_banci].total_num++;
//  		banci[cur_banci].total_sum -= price;
//  		today.total_num++;
//  		today.total_sum -= price;
		}
 
		else if ( strcmp((void *)&ticket.price_type[0], "7" ) == 0 )
		{
			//�Ż�Ʊ
			banci[cur_banci].youhui_num++;
			today.youhui_num++;
			sscanf((void *)&ticket.price[0], "%f", &fprice);
			price = (INT32U)(fprice * 100.0);
			banci[cur_banci].youhui_sum += price;
			today.youhui_sum += price;

			banci[cur_banci].total_num++;
			banci[cur_banci].total_sum += price;
			today.total_num++;
			today.total_sum += price;
		}
		else if ( strcmp((void *)&ticket.price_type[0], "8") == 0 )
		{
			//��Ʊ
			banci[cur_banci].yuepiao_num++;
			today.yuepiao_num++;
    	}

		else
		{
			//��Ʊ������
		}

	}

	if ( EA_ucPFClose(ucOpenID) != EM_ffs_SUCCESS )
	{
		lcddisperr("�رռ�¼�ļ�ʧ��");
		return;
	}

	//���ɲ˵�
	strcpy((void *)sum_menu, "���ջ���            ");
	for ( j = 1; j <= max_banci; j++ )
	{
		sprintf((void *)buf, "���%2d              ", j);
		strcat((void *)sum_menu, (void *)buf);
	}


	for (;;)
	{
		menus = browse_menu(1, (void *)"    ����ͳ��    ", sum_menu, TRUE);
		if ( menus == -1 )                //�˳�
		{
			return;
		}
		else if ( menus == 0 )            //���ջ���
		{
			EA_vCls();
//  		EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "    ���ջ���    ");
//  		sprintf((void *)buf, "ȫƱ%4d��%8.1fԪ", today.full_num, (FP32)today.full_sum / 100.0);
//  		lcddisp(1, 1, (void *)buf);
//  		sprintf((void *)buf, "��Ʊ%4d��%8.1fԪ", today.half_num, (FP32)today.half_sum / 100.0);
//  		lcddisp(2, 1, (void *)buf);
			sprintf((void *)buf, "�Ż�%4d��%8.1fԪ", today.youhui_num, (FP32)today.youhui_sum / 100.0);
			lcddisp(1, 1, (void *)buf);
//  		sprintf((void *)buf, "��Ʊ%4d��%8.1fԪ", today.package_num, (FP32)today.package_sum / 100.0);
//  		lcddisp(2, 1, (void *)buf);
			sprintf((void *)buf, "��Ʊ%4d��%8.1fԪ", today.yuepiao_num, (FP32)today.yuepiao_sum / 100.0);
			lcddisp(2, 1, (void *)buf);
			sprintf((void *)buf, "�ܼ�%3d��%5.1fԪ", today.total_num, (FP32)today.total_sum / 100.0);
			EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, (void *)buf);


			//(void)EA_uiInkey(0);
			key = EA_uiInkey(0);
			if ( key == EM_key_ENTER )
			{
				EA_vCls();
				EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "�Ƿ�ȷ����ӡ��  ");
				EA_vDisp(4, 1, "ȷ��-��ӡ  ����-ȡ��");
				key = EA_uiInkey(0);
				if ( key == EM_key_ENTER )
				{
					(void)print_summary(&today);
				}
			}
		}
		else
		{
			if ( menus > 10 )
				menus = 10;
			banci[menus].banci = menus;
			EA_vCls();
//  		sprintf((void *)buf, "   ���%2d   ", menus);
//  		EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, (void *)buf);
			sprintf((void *)buf, "ȫƱ%4d��%8.1fԪ", banci[menus].full_num, (FP32)banci[menus].full_sum / 100.0);
			lcddisp(1, 1, (void *)buf);
			sprintf((void *)buf, "��Ʊ%4d��%8.1fԪ", banci[menus].half_num, (FP32)banci[menus].half_sum / 100.0);
			lcddisp(2, 1, (void *)buf);
			sprintf((void *)buf, "�Ż�%4d��%8.1fԪ", today.youhui_num, (FP32)today.youhui_sum / 100.0);
			lcddisp(3, 1, (void *)buf);
//  		sprintf((void *)buf, "��Ʊ%4d��%8.1fԪ", banci[menus].valid_num, (FP32)banci[menus].valid_sum / 100.0);
//  		lcddisp(3, 1, (void *)buf);
			sprintf((void *)buf, "��Ʊ%4d��%8.1fԪ", banci[menus].package_num, (FP32)banci[menus].package_sum / 100.0);
			lcddisp(4, 1, (void *)buf);
			sprintf((void *)buf, "�ܼ�%3d��%5.1fԪ", banci[menus].total_num, (FP32)banci[menus].total_sum / 100.0);
//  		sprintf((void *)buf, "�ܼ�%3d��%5.1fԪ", today.total_num, (FP32)today.total_sum / 100.0);
			EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, (void *)buf);
//  		lcddisp(4, 1, (void *)buf);

			//(void)EA_uiInkey(0);
			key = EA_uiInkey(0);
			if ( key == EM_key_ENTER )
			{
				EA_vCls();
				EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "�Ƿ�ȷ����ӡ��  ");
				EA_vDisp(4, 1, "ȷ��-��ӡ  ����-ȡ��");
				key = EA_uiInkey(0);
				if ( key == EM_key_ENTER )
				{
					(void)print_summary(&banci[menus]);
				}
			}
		}


	}
}
/******************************************************************************
 �������ƣ�print_sum
 ������������ӡͳ������
 �������ƣ�	����/�����	����		����
 pSummary		����		TODAY_SUM *	ͳ�Ƶ����ݣ���������ȫ�죬��ÿ����ε�
 
 ��  ��  ֵ����
 
 ��      �ߣ�����/������
 ��      �ڣ�2012-12-02
 �޸���ʷ��
	����		�޸���		�޸�����
	------		---------	-------------
******************************************************************************/
void print_summary(TODAY_SUM *pSummary)
{
	INT8U ucRet = 0;
	//  DevHandle hDprinterHandle;
	char printbuf[300];
	char buf[300];
	BUS_TIME ltime;

	// ��ӡ�豸����I/Oģ���һ�֣�Ӧ�Ȼ�ò����������ʱ������ӡ���õ�ģʽ
	ucRet = EA_ucOpenDevice("PRINT", EM_io_REALMODE, &hDprinterHandle);
	if ( ucRet != EM_SUCCESS )
	{
		lcddisperr("��ô�ӡ�������ʧ��!");
		return;
	}

	  // POS��ʱ,���ó�ʼ��������ʼ����ӡ��о״̬
	  ucRet =  EA_ucInitPrinter(hDprinterHandle);
	  if ( ucRet == EM_prn_PAPERENDED )
	  {
          (void)EA_ucCloseDevice(&hDprinterHandle);
		  lcddisperr("��ֽ!��ӡʧ��!");
		  return;
	  }
	  else if ( ucRet != EM_SUCCESS )
	  {
    	  (void)EA_ucCloseDevice(&hDprinterHandle);
		  lcddisperr("��ʼ����ӡ��оʧ��!");
		  return;
	  }

	// �ɽ��д�ӡ��һЩ��������
	ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_XSPACE, 0);
	if ( ucRet != EM_SUCCESS )
	{
		//  	(void)EA_ucCloseDevice(&hDprinterHandle);
		lcddisperr("��ӡ������ʧ��!");
		return;
	}
//  ucRet =  EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_YSPACE, 4);
	ucRet =  EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_YSPACE, 2);
	if ( ucRet != EM_SUCCESS )
	{
		//  	(void)EA_ucCloseDevice(&hDprinterHandle);
		lcddisperr("��ӡ������ʧ��!");
		return;
	}

	//  ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_PAGESPACE, 578);
//  ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_PAGESPACE, 577);
	ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_PAGESPACE, 576);
	if ( ucRet != EM_SUCCESS )
	{
		//  	(void)EA_ucCloseDevice(&hDprinterHandle);
		lcddisperr("��ӡ������ʧ��!");
		return;
	}

	ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_ASCII_DOTTYPE, EM_prn_ASCIIDOT16X8);
	if ( ucRet != EM_SUCCESS )
	{
		//  	(void)EA_ucCloseDevice(&hDprinterHandle);
		lcddisperr("��ӡ������ʧ��!");
		return;
	}

	ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_HZ_DOTTYPE, EM_prn_HZDOT16X16);
	if ( ucRet != EM_SUCCESS )
	{
		//  	(void)EA_ucCloseDevice(&hDprinterHandle);
		lcddisperr("��ӡ������ʧ��!");
		return;
	}

	ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_HZ_PRNTYPE, EM_prn_HZPRN2X2);
	if ( ucRet != EM_SUCCESS )
	{
		//  	(void)EA_ucCloseDevice(&hDprinterHandle);
		lcddisperr("��ӡ������ʧ��!");
		return;
	}


	//  ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_ASCII_PRNTYPE, EM_prn_ASCIIPRN2X2);
	//  if ( ucRet != EM_SUCCESS )
	//  {
	//  	lcddisperr("��ӡ������ʧ��!");
	//  	return;
	//  }

	// ��ӡ����
	//  ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "��ӡ����!\n");
	//  if ( ucRet != EM_SUCCESS )
	//  {
	//  	EA_vDisplay(2, "���д�ӡʧ��!");
	//  	return;
	//  }
	//  ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "�����״�ͨ���Ӽ������޹�˾1\n");
	//  ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "�����״�ͨ���Ӽ�����23\n");
	//  ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "abcd1234ABCD!@#$\n");
	//  ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "�����״�ͨ���Ӽ������޹�˾4\n");

	//ȫ���Ʊ��
	//  ȫƱ  5��  45.0Ԫ
	//  ��Ʊ  4��  10.0Ԫ
	// 	��Ʊ  9��   9.0Ԫ
	//  �ܼ�  18��  65.00Ԫ
	//	2012-11-04 17:06:38
	//  ȫ��  ���ţ�³P-12345		
	//  ��ƱԱ�ţ� 1234
	//	POS���ţ�123456

	//���˵�Ʊ��
	//  ȫƱ  5��  45.0Ԫ
	//  ��Ʊ  4��  10.0Ԫ
	// 	��Ʊ  9��   9.0Ԫ
	//  �ܼ�  18��  65.00Ԫ
	//	2012-11-04 17:06:38
	//  ��� 2 ���ţ�³P-12345		
	//  ��ƱԱ�ţ� 1234
	//	POS���ţ�123456
	EA_vCls();
	EA_vDisp(4, 1, "���ڴ�ӡ......      ");
	sprintf(printbuf, "ȫƱ%4d��%6.1fԪ\n", pSummary->full_num, (FP32)pSummary->full_sum / 100.0);
	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf); 
	sprintf(printbuf, "��Ʊ%4d��%6.1fԪ\n", pSummary->half_num, (FP32)pSummary->half_sum / 100.0);
	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf); 
	sprintf(printbuf, "��Ʊ%4d��%6.1fԪ\n", pSummary->package_num, (FP32)pSummary->package_sum / 100.0);
	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf); 
	sprintf(printbuf, "�Ż�%4d��%6.1fԪ\n", pSummary->youhui_num, (FP32)pSummary->youhui_sum / 100.0);
	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf); 
	sprintf(printbuf, "��Ʊ%4d��\n", pSummary->yuepiao_num);
	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf); 
	sprintf(printbuf, "�ܼ�%4d��%6.1fԪ\n", pSummary->total_num, (FP32)pSummary->total_sum / 100.0);
	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);    
	           //Ʊ��ͳ��
	(void)Get_Time(&ltime);
	sprintf((void *)&pSummary->query_times[0], "%02X%02X%02X%02X%02X%02X%02X",
			ltime.century, ltime.year, ltime.month, ltime.day, ltime.hour, ltime.minute, ltime.second);
	sprintf(printbuf, "%c%c%c%c-%c%c-%c%c %c%c:%c%c:%c%c\n",
			pSummary->query_times[0], pSummary->query_times[1], pSummary->query_times[2], pSummary->query_times[3],
			pSummary->query_times[4], pSummary->query_times[5], pSummary->query_times[6], pSummary->query_times[7],
			pSummary->query_times[8], pSummary->query_times[9], pSummary->query_times[10], pSummary->query_times[11],
			pSummary->query_times[12], pSummary->query_times[13]);
	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);               //2012-11-04 17:06:38

	//  if ( strcmp((void *)ticket->direction, "1") == 0 )
	//      sprintf(printbuf, "��Σ�%2s       ����\n", ticket->banci);
	//  else
	//      sprintf(printbuf, "��Σ�%2s       ����\n", ticket->banci);
	if ( pSummary->day_or_banci == 0 )
		sprintf(printbuf, "ȫ��    ");
	else
		sprintf(printbuf, "���:%2d   ", pSummary->banci);
	//(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);               //��Σ� 2  @406 ����
	sprintf(buf, "%s\n", DevStat.bus_number_full);
	strcat((void *)printbuf, (void *)buf);
	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);               //���ţ�³P-12345

	sprintf(printbuf, "��ƱԱ�ţ� %s\n", DevStat.op_number);
	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);               //��ƱԱ�ţ�1234

//  sprintf(printbuf, "Ʊ�ţ�  %s\n", pTicket->ticket_number);
//  (void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);               //Ʊ�ţ�1234567

	sprintf(printbuf, "POS���ţ�%s\n", DevStat.pos_number);
	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);               //POS���ţ�123456


	// ��ӡ�����û�ҳ����
	ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "\f");
	if ( ucRet == EM_SUCCESS )
	{
		EA_vDisplay(2, "���δ�ӡ����!");
		EA_vDelay(1);		
	}

	generate_summary_info((void *)pSummary);                 //���ɼ�¼
	store_record(&Tinfo.pos_number[0]);     //�����¼

	DevStat.cur_ticket_number++;
	if ( DevStat.cur_ticket_number > 9999999 )
		DevStat.cur_ticket_number = 0;
	sprintf((void *)&Tinfo.ticket_number[0], "%07d", DevStat.cur_ticket_number);            //Ʊ�� 7

	(void)EA_ucCloseDevice(&hDprinterHandle);

}

/******************************************************************************
 �������ƣ�mdata
 �����������˵�->���ݴ���
 ����������
 �������ƣ�	����/�����	����		����
				
 ��  ��  ֵ����
				   
 ��      ��	������
 ��      �ڣ�2012-10-30
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void mdata(void)
{
	INT8S menus = 0;


	for (;;)
	{
		menus = browse_menu(1, (void *)"    ���ݴ���    ", data_menu, TRUE);
		switch ( menus )
		{
		   case -1:             //ȡ����
			   return;

		   case 0:              //1.��Ʊ
			   invalid_ticket_process();
			   break;

		   case 1:              //2.��¼�Ͳ�������

			   send_record_process();
			   break;

			   //  	   case 2:              //3.���ز���
			   //  		   set_banci();
			   //  		   break;

		   default:
			   break;
		}

	}





}

///******************************************************************************
// �������ƣ�invalid_ticket_process
// ������������Ʊ����
// ����������
// �������ƣ� ����/����� ����        ����
//
// ��  ��  ֵ����
//
// ��      �� ������
// ��      �ڣ�2012-10-30
// �޸���ʷ��
//        ����        �޸���      �޸�����
//        ------      ---------   -------------
//******************************************************************************/
//void invalid_ticket_process(void)
//{
//    const char recFileName[] = "record";
//    INT8U ucOpenID = 0;
//    INT32U uiResult;
//    INT32U uiReadCnt;
//    INT32U total_rec_num = 0;
//    INT8U buf[30];
//    //  INT32U j = 0;
//    TICKET_INFO ticket;
//    INT8S menus = 0;
//
//
//    // ���ļ�
//    if ( EA_ucPFOpen((uchar *)recFileName, &ucOpenID) != EM_ffs_SUCCESS )
//    {
//        lcddisperr("�򿪼�¼�ļ�ʧ��");
//        return;
//    }
//
//    (void)EA_ucPFInfo(ucOpenID, EM_ffs_VALIDREC, &uiResult);
//    total_rec_num = uiResult;
//
//    if ( total_rec_num == 0 )
//    {
//        lcddisperr("û�м�¼!");
//        (void)EA_ucPFClose(ucOpenID);
//        return;
//    }
//
//    (void)EA_ucPFReadRec(ucOpenID, total_rec_num, 1, &uiReadCnt, &ticket.pos_number[0]);
//
//    if ( EA_ucPFClose(ucOpenID) != EM_ffs_SUCCESS )
//    {
//        lcddisperr("�رռ�¼�ļ�ʧ��");
//        return;
//    }
//
//    //��ʾ���һ����Ʊ��¼
//    EA_vCls();
//    EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "      ��Ʊ      ");
//    sprintf((void *)buf, "Ʊ��:%s", ticket.ticket_number);
//    lcddisp(1, 1, buf);
//    sprintf((void *)buf, "%c%c%c%c-%c%c-%c%c %c%c:%c%c:%c%c",
//            ticket.ticket_time[0], ticket.ticket_time[1], ticket.ticket_time[2], ticket.ticket_time[3],
//            ticket.ticket_time[4], ticket.ticket_time[5], ticket.ticket_time[6], ticket.ticket_time[7],
//            ticket.ticket_time[8], ticket.ticket_time[9], ticket.ticket_time[10], ticket.ticket_time[11],
//            ticket.ticket_time[12], ticket.ticket_time[13]);
//    lcddisp(2, 1, buf);
//    sprintf((void *)buf, "%s��%s\n", ticket.start_station_name, ticket.end_station_name);
//    lcddisp(3, 1, buf);
//    if ( strcmp((void *)ticket.price_type, "1") == 0 )
//    {
//        sprintf((void *)buf, "ȫƱ��%sԪ\n", ticket.price);
//    }
//    else if ( strcmp((void *)ticket.price_type, "2") == 0 )
//    {
//        sprintf((void *)buf, "��Ʊ��%sԪ\n", ticket.price);
//    }
//    else if ( strcmp((void *)ticket.price_type, "3") == 0 )
//    {
//        sprintf((void *)buf, "��Ʊ��%sԪ\n", ticket.price);
//    }
//    else
//    {
//        sprintf((void *)buf, "��Ʊ��%sԪ\n", ticket.price);
//    }
//    lcddisp(4, 1, buf);
//
//    (void)EA_uiInkeyMs(0);
//
//    menus = browse_menu(1, (void *)"ȷ��Ҫ��Ϊ��Ʊ? ", confirm_menu, TRUE);
//    if ( menus == 0 )
//    {
//        strcpy((void *)&ticket.price_type[0], "4");
//        store_record(&ticket.pos_number[0]);     //�����¼
//        EA_ucSetInverse(EM_lcd_INVON);
//        lcddisp(4, 1, (void *)"�������            ");
//        EA_ucSetInverse(EM_lcd_INVOFF);
//        (void)EA_uiInkeyMs(2000);
//        return;
//    }
//    else
//    {
//        return;
//    }
//
//}

/******************************************************************************
 �������ƣ�invalid_ticket_process
 ������������Ʊ����
 ����������
 �������ƣ�	����/�����	����		����
				
 ��  ��  ֵ����
				   
 ��      ��	������
 ��      �ڣ�2012-10-30
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void invalid_ticket_process(void)
{
	const char recFileName[] = "record";
	INT8U ucOpenID = 0;
	INT32U uiResult;
	INT32U uiReadCnt;
	INT32U total_rec_num = 0;
	INT8U buf[30];
	//  INT32U j = 0;
	TICKET_INFO ticket;
	INT8S menus = 0;


	// ���ļ�
	if ( EA_ucPFOpen((uchar *)recFileName, &ucOpenID) != EM_ffs_SUCCESS )
	{
		lcddisperr("�򿪼�¼�ļ�ʧ��");
		return;
	}

	(void)EA_ucPFInfo(ucOpenID, EM_ffs_VALIDREC, &uiResult);
	total_rec_num = uiResult;

	if ( total_rec_num == 0 )
	{
		lcddisperr("û�м�¼!");
		(void)EA_ucPFClose(ucOpenID);
		return;
	}

	(void)EA_ucPFReadRec(ucOpenID, total_rec_num, 1, &uiReadCnt, &ticket.pos_number[0]);

	if ( EA_ucPFClose(ucOpenID) != EM_ffs_SUCCESS )
	{
		lcddisperr("�رռ�¼�ļ�ʧ��");
		return;
	}

	//��ʾ���һ����Ʊ��¼
	EA_vCls();
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "      ��Ʊ      ");
	sprintf((void *)buf, "Ʊ��:%s", ticket.ticket_number);
	lcddisp(1, 1, buf);
	sprintf((void *)buf, "%c%c%c%c-%c%c-%c%c %c%c:%c%c:%c%c",
			ticket.ticket_time[0], ticket.ticket_time[1], ticket.ticket_time[2], ticket.ticket_time[3],
			ticket.ticket_time[4], ticket.ticket_time[5], ticket.ticket_time[6], ticket.ticket_time[7],
			ticket.ticket_time[8], ticket.ticket_time[9], ticket.ticket_time[10], ticket.ticket_time[11],
			ticket.ticket_time[12], ticket.ticket_time[13]);
	lcddisp(2, 1, buf);
	sprintf((void *)buf, "%s��%s\n", ticket.start_station_name, ticket.end_station_name);
	lcddisp(3, 1, buf);
	if ( strcmp((void *)ticket.price_type, "1") == 0 )
	{
		sprintf((void *)buf, "ȫƱ��%sԪ\n", ticket.price);
	}
	else if ( strcmp((void *)ticket.price_type, "2") == 0 )
	{
		sprintf((void *)buf, "��Ʊ��%sԪ\n", ticket.price);
	}
	else if ( strcmp((void *)ticket.price_type, "3") == 0 )
	{
		sprintf((void *)buf, "��Ʊ��%sԪ\n", ticket.price);
	}
	else
	{
		sprintf((void *)buf, "��Ʊ��%sԪ\n", ticket.price);
	}
	lcddisp(4, 1, buf);

	(void)EA_uiInkey(0);

	menus = browse_menu(1, (void *)"ȷ��Ҫ��Ϊ��Ʊ? ", confirm_menu, TRUE);
	if ( menus == 0 )
	{
		strcpy((void *)&ticket.price_type[0], "4");
		store_record(&ticket.pos_number[0]);     //�����¼
		EA_ucSetInverse(EM_lcd_INVON);
		lcddisp(4, 1, (void *)"�������            ");
		EA_ucSetInverse(EM_lcd_INVOFF);
		(void)EA_uiInkey(2);
		return;
	}
	else
	{
		return;
	}

}

/******************************************************************************
 �������ƣ�send_record_process
 �����������ϴ���Ʊ��¼����
 ����������
 �������ƣ�	����/�����	����		����
				
 ��  ��  ֵ����
				   
 ��      ��	������
 ��      �ڣ�2012-10-30
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void send_record_process(void)
{
	INT8U ucResult = 0;
	//  DevHandle hRS232Handle;
	//  INT8U buf[30];

	EA_vCls();
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "  �ϴ���Ʊ��¼  ");
	//  lcddisp(2, 1, (void *)"      ͨѶ��..      ");
	lcddisp(2, 1, (void *)"    ���ݴ�����..    ");
	generate_record_file();
	lcddisp(2, 1, (void *)"      ͨѶ��..      ");

	//  ��RS232�豸
	ucResult = EA_ucOpenDevice("COM2", EM_io_RDWR, &hRS232Handle);
	if ( ucResult != EM_SUCCESS )
	{
		lcddisperr("�򿪴���ʧ��");
		return;
	}

	ucResult = EA_ucInitRS232(hRS232Handle, EM_232_115200BPS, EM_232_NOPAR, EM_232_8DBS);
	if ( ucResult != EM_SUCCESS )
	{
		lcddisperr("���ڳ�ʼ��ʧ��");
		(void)EA_ucCloseDevice(&hRS232Handle);
		return;
	}


	//  strcpy((void *)buf, "0123456789ABCDEF");
	//  (void)EA_ucWriteDevice(hRS232Handle, 16, 10, buf);


	communication();



	ucResult = EA_ucCloseDevice(&hRS232Handle);
	if ( ucResult != EM_SUCCESS )
	{
		lcddisperr("�򿪴���ʧ��");
	}

}

/******************************************************************************
 �������ƣ�generate_record_file
 �������������ɼ�¼�ļ�
 ����������
 �������ƣ�	����/�����	����		����
				
 ��  ��  ֵ����
				   
 ��      ��	������
 ��      �ڣ�2012-10-30
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void generate_record_file(void)
{
	const char recFileName[] = "record";
	//  const char file_name[] = "rec.txt";
	INT8U ucOpenID = 0;
	int i = 0;
	EV_cfs_fHandle fHandle;
	int_64 iWriteSize = 0;
	INT8U buf[200];
	TICKET_INFO ticket;
	INT32U total_rec_num = 0;
	INT32U uiResult;
	INT32U uiReadCnt;
	INT32U j = 0;


	//  //��ɾ���ļ�
	//  (void)EA_iCfsDelete("rec.txt");

	fHandle = EA_iCfsOpen("rec.txt", "w+");
	if ( fHandle == NULL )
	{
		lcddisperr("����rec.txtʧ��");
		return;
	}

	// ���ļ�
	if ( EA_ucPFOpen((uchar *)recFileName, &ucOpenID) != EM_ffs_SUCCESS )
	{
		lcddisperr("�򿪼�¼�ļ�ʧ��");
		(void)EA_iCfsClose(fHandle);
		return;
	}

	(void)EA_ucPFInfo(ucOpenID, EM_ffs_VALIDREC, &uiResult);
	total_rec_num = uiResult;


	for ( j = 0; j < total_rec_num; j++ )
	{
		(void)EA_ucPFReadRec(ucOpenID, j + 1, 1, &uiReadCnt, &ticket.pos_number[0]);
//		sprintf((void *)buf, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\r\n",
//				ticket.pos_number,
//				ticket.ticket_number,
//				ticket.ticket_time,
//				ticket.op_number,
//				ticket.start_station_num,
//				ticket.start_station_name,
//				ticket.end_station_num,
//				ticket.end_station_name,
//				ticket.price_type,
//				ticket.price,
//				ticket.bus_number,
//				ticket.banci,
//				ticket.direction,
//				ticket.line_number);
		sprintf((void *)buf, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\r\n",
				ticket.pos_number,
				ticket.ticket_number,
				ticket.ticket_time,
				ticket.op_number,
				ticket.start_station_num,
				ticket.start_station_name,
				ticket.end_station_num,
				ticket.end_station_name,
				ticket.price_type,
				ticket.price,
				ticket.bus_number,
				ticket.banci,
				ticket.direction,
				ticket.line_number,
				ticket.trade_type,
				ticket.card_number,
				ticket.card_balance);

		iWriteSize = EA_i64CfsWrite(fHandle, buf, strlen((void *)buf));         //д���ļ�
		if ( iWriteSize < 0 )
		{
			lcddisperr("дrec.txtʧ��");
			(void)EA_ucPFClose(ucOpenID);
			(void)EA_iCfsClose(fHandle);
			return;
		}

	}

	i = EA_iCfsFsync(fHandle);
	if ( i < 0 )
	{
		lcddisperr("ͬ��rec.txtʧ��");
		(void)EA_ucPFClose(ucOpenID);
		(void)EA_iCfsClose(fHandle);
		return;
	}

	if ( EA_ucPFClose(ucOpenID) != EM_ffs_SUCCESS )
	{
		lcddisperr("�رռ�¼�ļ�ʧ��");
		(void)EA_iCfsClose(fHandle);
		return;
	}

	i = EA_iCfsClose(fHandle);
	if ( i < 0 )
	{
		lcddisperr("�ر�rec.txtʧ��");
		return;
	}


}

/*****************************************************************
 ����ԭ�ͣ�read_param_table()
 ������������ȡ������
 ����������	
				 
 ����ֵ��	��
 
 ��  �ߣ�	����
 ��  �ڣ�	2004-09-17
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
*****************************************************************/
void read_param_table(void)
{
	EV_cfs_fHandle fHandle;
	int_64 iReadSize = 0;
	//  INT32U uiResult;
	//  INT32U uiReadCnt;
	INT32S j = 0;
	int_64 file_len = 0;
	char *pfile;
	char *p;
	char line_buf[1000];
	char search_buf[200];
	char buf[200];
	INT32S i = 0;
	char *pl;
	INT32S k = 0;


	fHandle = EA_iCfsOpen("parm.txt", "r");
	if ( fHandle == NULL )
	{
		lcddisperr("��ȡparm.txt�ļ�ʧ��");
		return;
	}
	file_len = EA_i64CfsGetFileSize(fHandle);
	if ( file_len < 0 )
	{
		lcddisperr("��ȡ�����ļ�����ʧ��");
		(void)EA_iCfsClose(fHandle);
		return;
	}

	//��̬����ռ�
	pfile = (char *)EA_pvMalloc(file_len * sizeof(char) + 1);
	if ( pfile == NULL )
	{
		lcddisperr("��̬����ռ����");
		(void)EA_iCfsClose(fHandle);
		return;
	}

	//��ȡȫ���ļ�
	iReadSize = EA_i64CfsRead(fHandle, pfile, file_len);
	if ( iReadSize < file_len )
	{
		lcddisperr("��ȡ�ļ�����ʧ��");
		(void)EA_iCfsClose(fHandle);
		return;
	}
	(void)EA_iCfsClose(fHandle);

	//  p = pfile;

	//ȡ��[param]��
	p = strstr(pfile, "[param]");
	sscanf(p, "%[^\x00d^\x00a]", line_buf); //�õ��� 0 ��//����0D0A����
	p = p + strlen(line_buf) + 2;

	sscanf(p, "%[^\x00d^\x00a]", line_buf); //�õ��� 1 ��//����0D0A����
	sscanf(line_buf, "version=%s", DevStat.version);
	p = p + strlen(line_buf) + 2;

	sscanf(p, "%[^\x00d^\x00a]", line_buf); //�õ��� 2 ��//����0D0A����
	sscanf(line_buf, "operator_amount=%d", &DevStat.operator_amount);
	p = p + strlen(line_buf) + 2;

	sscanf(p, "%[^\x00d^\x00a]", line_buf); //�õ��� 3 ��//����0D0A����
	sscanf(line_buf, "line_amout=%d", &DevStat.line_amount);

	//��̬�����ڴ棬��ʼ������
	EA_vFree(OPinfo);
	OPinfo = (OPERATOR_INFO *)EA_pvMalloc(DevStat.operator_amount * sizeof(OPERATOR_INFO) + 1);
	if ( OPinfo == NULL )
	{
		lcddisperr("��̬����ռ����");
	}
	EA_vFree(Linfo);
	Linfo = (LINE_INFO *)EA_pvMalloc(DevStat.line_amount * sizeof(LINE_INFO) + 1);
	if ( Linfo == NULL )
	{
		lcddisperr("��̬����ռ����");
	}
	EA_vFree(STinfo);
	STinfo = (STATION_INFO *)EA_pvMalloc(DevStat.line_amount * sizeof(STATION_INFO) + 1);
	if ( STinfo == NULL )
	{
		lcddisperr("��̬����ռ����");
	}
	memset((void *)OPinfo, 0x00, DevStat.operator_amount * sizeof(OPERATOR_INFO));
	memset((void *)Linfo, 0x00, DevStat.line_amount * sizeof(LINE_INFO));
	memset((void *)STinfo, 0x00, DevStat.line_amount * sizeof(STATION_INFO));


	//ȡ��[operator_info]��
	p = strstr(pfile, "[operator_info]");
	sscanf(p, "%[^\x00d^\x00a]", line_buf);     //ȡ����β
	p = p + strlen(line_buf) + 2;
	for ( j = 0; j < DevStat.operator_amount; j++ )
	{
		sscanf(p, "%[^\x00d^\x00a]", line_buf);     //ȡ����β
													//  	sscanf(line_buf, "%s,%s,%s", OPinfo[j].number, OPinfo[j].name, OPinfo[j].passwd);
		sscanf(line_buf, "%[^,],%[^,],%s", OPinfo[j].number, OPinfo[j].name, OPinfo[j].passwd);
		p = p + strlen(line_buf) + 2;

		//        //add for debug
		////  	sprintf((void *)buf, "%s,%s,%s",OPinfo[0].number, OPinfo[0].name, OPinfo[0].passwd);
		////  	lcddisperr((void *)buf);
		//        sprintf((void *)buf, "%s",OPinfo[0].number);
		//        lcddisperr((void *)buf);
		//        sprintf((void *)buf, "%s",OPinfo[0].name);
		//        lcddisperr((void *)buf);
		//        sprintf((void *)buf, "%s",OPinfo[0].passwd);
		//        lcddisperr((void *)buf);
		//        //debug end
	}


	//ȡ��[line_info]��
	p = strstr(pfile, "[line_info]");
	sscanf(p, "%[^\x00d^\x00a]", line_buf);     //ȡ����β
	p = p + strlen(line_buf) + 2;
	for ( j = 0; j < DevStat.line_amount; j++ )
	{
		sscanf(p, "%[^\x00d^\x00a]", line_buf);     //ȡ����β
													//  	sscanf(line_buf, "%s,%s,%s", Linfo[j].number, Linfo[j].menu_name, Linfo[j].full_name);
		sscanf(line_buf, "%[^,],%[^,],%s", Linfo[j].number, Linfo[j].menu_name, Linfo[j].full_name);
		p = p + strlen(line_buf) + 2;

		//  	//add for debug
		//  	sprintf((void *)buf, "%s",Linfo[j].number);
		//  	lcddisperr((void *)buf);
		//  	sprintf((void *)buf, "%s",Linfo[j].menu_name);
		//  	lcddisperr((void *)buf);
		//  	sprintf((void *)buf, "%s",Linfo[j].full_name);
		//  	lcddisperr((void *)buf);
		//  	//debug end
	}


	//ȡ��Ʊ�۱�
	for ( j = 0; j < DevStat.line_amount; j++ )
	{
		sprintf(search_buf, "[stations_of_line_%d]", j);
		p = strstr(pfile, search_buf);
		if ( p == NULL )
			continue;
		sscanf(p, "%[^\x00d^\x00a]", line_buf);     //ȡ����β
		p = p + strlen(line_buf) + 2;

		sscanf(p, "%[^\x00d^\x00a]", line_buf);     //ȡ����β
		sscanf(line_buf, "%d", &STinfo[j].forward_station_num);
		p = p + strlen(line_buf) + 2;

		sscanf(p, "%[^\x00d^\x00a]", line_buf);     //ȡ����β
		sscanf(line_buf, "%d", &STinfo[j].backward_station_num);
		p = p + strlen(line_buf) + 2;

		//  	//add for debug
		//  	sprintf((void *)buf, "%d",STinfo[j].forward_station_num);
		//  	lcddisperr((void *)buf);
		//  	sprintf((void *)buf, "%d",STinfo[j].backward_station_num);
		//  	lcddisperr((void *)buf);
		//  	//debug end

		//ȡ����վ����
		sscanf(p, "%[^\x00d^\x00a]", line_buf);     //ȡ����β
		pl = line_buf;
		for ( i = 0; i < STinfo[j].forward_station_num; i++ )
		{
			sscanf(pl, "%[^,\x00d]", buf);          //ȡ��,
			strcpy((void *)&STinfo[j].forward_number[i][0], buf);
			pl = pl + strlen(buf) + 1;
		}
		p = p + strlen(line_buf) + 2;

		//ȡ����վ����
		sscanf(p, "%[^\x00d^\x00a]", line_buf);     //ȡ����β
		pl = line_buf;
		for ( i = 0; i < STinfo[j].backward_station_num; i++ )
		{
			sscanf(pl, "%[^,\x00d]", buf);          //ȡ��,
			strcpy((void *)&STinfo[j].backward_number[i][0], buf);
			pl = pl + strlen(buf) + 1;
		}
		p = p + strlen(line_buf) + 2;

		//ȡ����վ������
		sscanf(p, "%[^\x00d^\x00a]", line_buf);     //ȡ����β
		pl = line_buf;
		for ( i = 0; i < STinfo[j].forward_station_num; i++ )
		{
			sscanf(pl, "%[^,\x00d]", buf);          //ȡ��,
			strcpy((void *)&STinfo[j].forward_name[i][0], buf);
			pl = pl + strlen(buf) + 1;
		}
		p = p + strlen(line_buf) + 2;

		//ȡ����վ������
		sscanf(p, "%[^\x00d^\x00a]", line_buf);     //ȡ����β
		pl = line_buf;
		for ( i = 0; i < STinfo[j].backward_station_num; i++ )
		{
			sscanf(pl, "%[^,\x00d]", buf);          //ȡ��,
			strcpy((void *)&STinfo[j].backward_name[i][0], buf);
			pl = pl + strlen(buf) + 1;
		}
		p = p + strlen(line_buf) + 2;

		//ȡ����Ʊ�۱�
		for ( k = 0; k < STinfo[j].forward_station_num; k++ )
		{
			sscanf(p, "%[^\x00d^\x00a]", line_buf);     //ȡ����β
			pl = line_buf;
			for ( i = 0; i < STinfo[j].forward_station_num; i++ )
			{
				sscanf(pl, "%[^,\x00d]", buf);          //ȡ��,
				strcpy((void *)&STinfo[j].forward_price_table[k][i][0], buf);
				pl = pl + strlen(buf) + 1;
			}
			p = p + strlen(line_buf) + 2;

		}

		//ȡ����Ʊ�۱�
		for ( k = 0; k < STinfo[j].backward_station_num; k++ )
		{
			sscanf(p, "%[^\x00d^\x00a]", line_buf);     //ȡ����β
			pl = line_buf;
			for ( i = 0; i < STinfo[j].backward_station_num; i++ )
			{
				sscanf(pl, "%[^,\x00d]", buf);          //ȡ��,
				strcpy((void *)&STinfo[j].backward_price_table[k][i][0], buf);
				pl = pl + strlen(buf) + 1;
			}
			p = p + strlen(line_buf) + 2;

		}

	}


}

/******************************************************************************
 �������ƣ�send_backup_record_process
 �����������ϴ�������Ʊ��¼����
 ����������
 �������ƣ�	����/�����	����		����
				
 ��  ��  ֵ����
				   
 ��      ��	������
 ��      �ڣ�2012-10-30
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void send_backup_record_process(void)
{
	INT8U ucResult = 0;
	//  DevHandle hRS232Handle;
	//  INT8U buf[30];

	EA_vCls();
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "  �ϴ����ݼ�¼  ");
	lcddisp(2, 1, (void *)"    ���ݴ�����..    ");
	generate_record_file();
	generate_backup_record_file();
	lcddisp(2, 1, (void *)"      ͨѶ��..      ");

	//  ��RS232�豸
	ucResult = EA_ucOpenDevice("COM2", EM_io_RDWR, &hRS232Handle);
	if ( ucResult != EM_SUCCESS )
	{
		lcddisperr("�򿪴���ʧ��");
		return;
	}

	ucResult = EA_ucInitRS232(hRS232Handle, EM_232_115200BPS, EM_232_NOPAR, EM_232_8DBS);
	if ( ucResult != EM_SUCCESS )
	{
		lcddisperr("���ڳ�ʼ��ʧ��");
		(void)EA_ucCloseDevice(&hRS232Handle);
		return;
	}



	communication();


	ucResult = EA_ucCloseDevice(&hRS232Handle);
	if ( ucResult != EM_SUCCESS )
	{
		lcddisperr("�򿪴���ʧ��");
	}

}

/******************************************************************************
 �������ƣ�generate_backup_record_file
 �������������ɱ��ݼ�¼�ļ�
 ����������
 �������ƣ�	����/�����	����		����
				
 ��  ��  ֵ����
				   
 ��      ��	������
 ��      �ڣ�2012-10-30
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void generate_backup_record_file(void)
{
	//  const char recFileName[] = "record";
	//  const char file_name[] = "rec.txt";
	INT8U ucOpenID = 0;
	int i = 0;
	EV_cfs_fHandle fHandle;
	int_64 iWriteSize = 0;
	INT8U buf[200];
	TICKET_INFO ticket;
	INT32U total_rec_num = 0;
	INT32U uiResult;
	INT32U uiReadCnt;
	INT32U j = 0;


	fHandle = EA_iCfsOpen("backup.txt", "w+");
	if ( fHandle == NULL )
	{
		lcddisperr("����backup.txtʧ��");
		return;
	}

	// ���ļ�
	if ( EA_ucPFOpen((INT8U *)"backup", &ucOpenID) != EM_ffs_SUCCESS )
	{
		lcddisperr("�򿪱��ݼ�¼�ļ�ʧ��");
		(void)EA_iCfsClose(fHandle);
		return;
	}

	(void)EA_ucPFInfo(ucOpenID, EM_ffs_VALIDREC, &uiResult);
	total_rec_num = uiResult;


	for ( j = 0; j < total_rec_num; j++ )
	{
		(void)EA_ucPFReadRec(ucOpenID, j + 1, 1, &uiReadCnt, &ticket.pos_number[0]);
//		sprintf((void *)buf, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\r\n",
//				ticket.pos_number,
//				ticket.ticket_number,
//				ticket.ticket_time,
//				ticket.op_number,
//				ticket.start_station_num,
//				ticket.start_station_name,
//				ticket.end_station_num,
//				ticket.end_station_name,
//				ticket.price_type,
//				ticket.price,
//				ticket.bus_number,
//				ticket.banci,
//				ticket.direction,
//				ticket.line_number);
		sprintf((void *)buf, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\r\n",
				ticket.pos_number,
				ticket.ticket_number,
				ticket.ticket_time,
				ticket.op_number,
				ticket.start_station_num,
				ticket.start_station_name,
				ticket.end_station_num,
				ticket.end_station_name,
				ticket.price_type,
				ticket.price,
				ticket.bus_number,
				ticket.banci,
				ticket.direction,
				ticket.line_number,
				ticket.trade_type,
				ticket.card_number,
				ticket.card_balance);

		iWriteSize = EA_i64CfsWrite(fHandle, buf, strlen((void *)buf));         //д���ļ�
		if ( iWriteSize < 0 )
		{
			lcddisperr("дbackup.txtʧ��");
			(void)EA_ucPFClose(ucOpenID);
			(void)EA_iCfsClose(fHandle);
			return;
		}

	}

	i = EA_iCfsFsync(fHandle);
	if ( i < 0 )
	{
		lcddisperr("ͬ��backup.txtʧ��");
		(void)EA_ucPFClose(ucOpenID);
		(void)EA_iCfsClose(fHandle);
		return;
	}

	if ( EA_ucPFClose(ucOpenID) != EM_ffs_SUCCESS )
	{
		lcddisperr("�رձ��ݼ�¼�ļ�ʧ��");
		(void)EA_iCfsClose(fHandle);
		return;
	}

	i = EA_iCfsClose(fHandle);
	if ( i < 0 )
	{
		lcddisperr("�ر�backup.txtʧ��");
		return;
	}


}

/******************************************************************************
 �������ƣ�format_process
 ������������ʽ������
 ����������
 �������ƣ�	����/�����	����		����
				
 ��  ��  ֵ����
				   
 ��      ��	������
 ��      �ڣ�2012-10-30
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void format_process(void)
{
	INT8U i = 0;
	INT8U input[50];
	//  INT8U buf[50];
	//  INT16U j = 0;
	//  INT16U key = EM_key_NOHIT;
	//  int m = 0;
	INT8U passwd[10] = "22884646";
	uchar ucResult;
	uint  uiFreeSpace;
	uint  uiFileNums;
	E_cfs_FileDetailInfo *ptFileInfo;
	INT8U buf[100];
	INT32U j = 0;
	int a = 0;


	EA_vCls();
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "   ϵͳ��ʼ��   ");
	//  sprintf((void *)buf, "���:%s", DevStat.pos_number);
	//  lcddisp(1, 1, (void *)buf);

	lcddisp(2, 1, (void *)"��������:");
	strcpy((void *)input, "");
	for (;;)
	{
		i = EA_ucGetInputStr(2, 10, 20, EM_BIG_FONT | EM_MODE_NUMBER | EM_ALIGN_LEFT | EM_SHOW_PASSWORD
							 , 8, 8, 0, (void *)input);
		if ( i == EM_SUCCESS )
			break;
		if ( i == EM_ABOLISH )
			return;
	}

	if ( strcmp((void *)input, (void *)passwd) == 0 )
	{}
	else
	{
		EA_ucSetInverse(EM_lcd_INVON);
		lcddisp(4, 1, (void *)"�������            ");
		EA_ucSetInverse(EM_lcd_INVOFF);
		(void)EA_uiInkey(1);
		return;
	}


	//��ʼ������
	param_factory_default(2);

	//ɾ�������ļ�
	// ��ȡ�ļ�����
	uiFileNums = EA_iCfsGetFileNum();
	if ( uiFileNums == 0 )
	{
		return;				//���ļ�������ɾ����
	}

	// ��̬����ռ�
	ptFileInfo = (E_cfs_FileDetailInfo *)EA_pvMalloc(uiFileNums * sizeof(E_cfs_FileDetailInfo));
	if ( ptFileInfo == NULL )
	{
		lcddisperr("��̬����ռ����");
		return;
	}

	// ��ȡ�ļ���Ϣ
	ucResult = EA_iCfsGetFileSystemInfo(&uiFileNums, ptFileInfo, &uiFreeSpace);


	for ( j = 0; j < uiFileNums; j++ )
	{

		a = EA_iCfsDelete((char *)ptFileInfo[j].cFileName);
		if ( a < 0 )
		{
			sprintf((void *)buf, "ɾ��%sʧ��", ptFileInfo[j].cFileName);
			lcddisperr((void *)buf);
			return;
		}
	}

	EA_vFree(ptFileInfo);
	ptFileInfo = NULL;
}

/******************************************************************************
 �������ƣ�ic_card_process
 ����������ˢ������
 ����������
 �������ƣ�	����/�����	����		����
				
 ��  ��  ֵ��ok(0)-�ɹ�
			 notok(0xFF)-��ȡ������
				   
 ��      ��	������
 ��      �ڣ�2012-10-30
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
INT8U ic_yuepiaocard_process(void)
{
	INT8U ucResult = 0;
	INT8U i = 0;
	INT16U rcv_len = 0;
	INT16U result = 0;
	BUS_TIME ltime;

	EA_vCls();
	lcddisp(1, 1, (void *)"   ��ˢ��Ʊ��..   ");

	//��RS232�豸
	ucResult = EA_ucOpenDevice("COM1", EM_io_RDWR, &hRS232Handle);
	if ( ucResult != EM_SUCCESS )
	{
		lcddisperr("�򿪴���ʧ��");
		return notok;
	}

	ucResult = EA_ucInitRS232(hRS232Handle, EM_232_115200BPS, EM_232_NOPAR, EM_232_8DBS);
	if ( ucResult != EM_SUCCESS )
	{
		lcddisperr("���ڳ�ʼ��ʧ��");
		(void)EA_ucCloseDevice(&hRS232Handle);
		return notok;
	}


	for( ;; )
	{
//  	key = EA_ucKBHit();
//  	if ( key == EM_key_HIT )    //�а���
//  	{
//  		key = EA_uiInkey(1);    //��ȡ������ֵ
//  		if ( key == EM_key_CANCEL )
//  		{
//  			(void)EA_ucCloseDevice(&hRS232Handle);
//  			return notok;       //�˳�
//  		}
//  	}
		//�ҿ�
		memcpy(Com1SndBuf, "\xAA\x05\x00", 3);
		i = com1_snd_and_rcv(Com1SndBuf, 3, (void *)Com1RcvBuf, &rcv_len, 1000, 10);
		if ( i == 0x01 )
		{
			(void)EA_ucCloseDevice(&hRS232Handle);
			return notok;			//��ȡ�����˳�
		}
		if ( i != ok )
		{
			continue;				//ͨѶ����
		}
		if ( rcv_len == 9 )
		{
			//����ʧ�ܴ���
			result = (INT16U)Com1RcvBuf[5] * 256 + (INT16U)Com1RcvBuf[6];
			if ( result == 0x6A01 )			//�˿�δ����
			{
				lcddisp(2, 1, (void *)"    �˿�δ����!!    ");
				lcddisp(3, 1, (void *)"                    ");
				lcddisp(4, 1, (void *)"                    ");
				beep_failed();
				sleep_ms(1000);
				lcddisp(2, 1, (void *)"                    ");
			}
			else if ( result == 0x6A02 )	//��������
			{
				lcddisp(2, 1, (void *)"     ��������!!     ");
				lcddisp(3, 1, (void *)"                    ");
				lcddisp(4, 1, (void *)"                    ");
				beep_failed();
				sleep_ms(1000);
				lcddisp(2, 1, (void *)"                    ");
			}
			else if ( result == 0x6A03 )	//���Ǳ�ϵͳ��
			{
				lcddisp(2, 1, (void *)"    ���Ǳ�ϵͳ��    ");
				lcddisp(3, 1, (void *)"                    ");
				lcddisp(4, 1, (void *)"                    ");
				beep_failed();
				sleep_ms(1000);
				lcddisp(2, 1, (void *)"                    ");
			}
			else							//û�ҵ���
			{
			}

			continue;
		}
		else if ( rcv_len == 26 )
		{
//			//�ҿ��ɹ�����
//			lcddisp(2, 1, (void *)"      �ҿ��ɹ�      ");
//			continue;
		}
		else
		{
			continue;				//���ղ���ȷ
		}


		//�ҿ��ɹ�����
		memcpy(&CardInfo._CARD_NUMBER.detail_card_number.serial_number[0], &Com1RcvBuf[3], 4);
		CardInfo.card_type = Com1RcvBuf[7];
		memcpy(&CardInfo.init_balance, &Com1RcvBuf[8], 4);
		memcpy(&CardInfo.csn[0], &Com1RcvBuf[12], 4);
		CardInfo.valid_time.century = Com1RcvBuf[16];
		CardInfo.valid_time.year = Com1RcvBuf[17];
		CardInfo.valid_time.month = Com1RcvBuf[18];
		CardInfo.valid_time.day = Com1RcvBuf[19];
		CardInfo.valid_time.hour = 0x00;
		CardInfo.valid_time.minute = 0x00;
		CardInfo.valid_time.second = 0x00;
		CardInfo.qishi = Com1RcvBuf[20];
		CardInfo.daoda = Com1RcvBuf[21];

		(void)Get_Time(&CardInfo.card_in_time);


		////////////////////////////////////////
		//���ֿ����Ӵ�������
		////////////////////////////////////////
		switch ( CardInfo.card_type )
		{
//  	   case CARDT_YOUHUI:		//�Żݿ�
//  		   i = notok;
//  		   lcddisperr("������Ʊ����");
//  		   break;

		   case CARDT_YUEPIAO:		//��Ʊ��
			   i = card_yuepiao_process();
			   break;

		   default:                //�����ʹ���
			   lcddisp(2, 1, (void *)"    �����ʹ���!!    ");
			   lcddisp(3, 1, (void *)"                    ");
			   lcddisp(4, 1, (void *)"                    ");
			   beep_failed();
			   sleep_ms(1000);
			   lcddisp(2, 1, (void *)"                    ");
			   continue;
		}

		if ( i != ok )
			continue;

		break;
		
	}

	(void)Get_Time(&ltime);
	memcpy(&DevStat.sell_ticket_time.century, &ltime.century, sizeof(BUS_TIME));
	sprintf((void *)&DevStat.sell_ticket_times[0], "%02X%02X%02X%02X%02X%02X%02X",
			ltime.century, ltime.year, ltime.month, ltime.day, ltime.hour, ltime.minute, ltime.second);

	generate_ticket_info();                 //���ɼ�¼
	store_record(&Tinfo.pos_number[0]);     //�����¼
	

	(void)EA_ucCloseDevice(&hRS232Handle);

	return ok;

}


/******************************************************************************
 �������ƣ�ic_card_process
 ����������ˢ������
 ����������
 �������ƣ�	����/�����	����		����
				
 ��  ��  ֵ��ok(0)-�ɹ�
			 notok(0xFF)-��ȡ������
				   
 ��      ��	������
 ��      �ڣ�2012-10-30
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
INT8U ic_card_process(void)
{
	INT8U ucResult = 0;
	INT8U i = 0;
	INT16U rcv_len = 0;
	INT16U result = 0;
	INT8U key = 0;

	EA_vCls();
	lcddisp(1, 1, (void *)"    ��ˢ�Żݿ�..    ");

	//��RS232�豸
	ucResult = EA_ucOpenDevice("COM1", EM_io_RDWR, &hRS232Handle);
	if ( ucResult != EM_SUCCESS )
	{
		lcddisperr("�򿪴���ʧ��");
		return notok;
	}

	ucResult = EA_ucInitRS232(hRS232Handle, EM_232_115200BPS, EM_232_NOPAR, EM_232_8DBS);
	if ( ucResult != EM_SUCCESS )
	{
		lcddisperr("���ڳ�ʼ��ʧ��");
		(void)EA_ucCloseDevice(&hRS232Handle);
		return notok;
	}


	for( ;; )
	{
//  	key = EA_ucKBHit();
//  	if ( key == EM_key_HIT )    //�а���
//  	{
//  		key = EA_uiInkey(1);    //��ȡ������ֵ
//  		if ( key == EM_key_CANCEL )
//  		{
//  			(void)EA_ucCloseDevice(&hRS232Handle);
//  			return notok;       //�˳�
//  		}
//  	}

		//�ҿ�
		memcpy(Com1SndBuf, "\xAA\x05\x00", 3);
		i = com1_snd_and_rcv(Com1SndBuf, 3, (void *)Com1RcvBuf, &rcv_len, 1000, 10);
		if ( i == 0x01 )
		{
			(void)EA_ucCloseDevice(&hRS232Handle);
			return notok;			//��ȡ�����˳�
		}
		if ( i != ok )
		{
			continue;				//ͨѶ����
		}
		if ( rcv_len == 9 )
		{
			//����ʧ�ܴ���
			result = (INT16U)Com1RcvBuf[5] * 256 + (INT16U)Com1RcvBuf[6];
			if ( result == 0x6A01 )			//�˿�δ����
			{
				lcddisp(2, 1, (void *)"    �˿�δ����!!    ");
				lcddisp(3, 1, (void *)"                    ");
				lcddisp(4, 1, (void *)"                    ");
				beep_failed();
				sleep_ms(1000);
				lcddisp(2, 1, (void *)"                    ");
			}
			else if ( result == 0x6A02 )	//��������
			{
				lcddisp(2, 1, (void *)"     ��������!!     ");
				lcddisp(3, 1, (void *)"                    ");
				lcddisp(4, 1, (void *)"                    ");
				beep_failed();
				sleep_ms(1000);
				lcddisp(2, 1, (void *)"                    ");
			}
			else if ( result == 0x6A03 )	//���Ǳ�ϵͳ��
			{
				lcddisp(2, 1, (void *)"    ���Ǳ�ϵͳ��    ");
				lcddisp(3, 1, (void *)"                    ");
				lcddisp(4, 1, (void *)"                    ");
				beep_failed();
				sleep_ms(1000);
				lcddisp(2, 1, (void *)"                    ");
			}
			else							//û�ҵ���
			{
			}

			continue;
		}
		else if ( rcv_len == 26 )
		{
//			//�ҿ��ɹ�����
//			lcddisp(2, 1, (void *)"      �ҿ��ɹ�      ");
//			continue;
		}
		else
		{
			continue;				//���ղ���ȷ
		}


		//�ҿ��ɹ�����
		memcpy(&CardInfo._CARD_NUMBER.detail_card_number.serial_number[0], &Com1RcvBuf[3], 4);
		CardInfo.card_type = Com1RcvBuf[7];
		memcpy(&CardInfo.init_balance, &Com1RcvBuf[8], 4);
		memcpy(&CardInfo.csn[0], &Com1RcvBuf[12], 4);
		CardInfo.valid_time.century = Com1RcvBuf[16];
		CardInfo.valid_time.year = Com1RcvBuf[17];
		CardInfo.valid_time.month = Com1RcvBuf[18];
		CardInfo.valid_time.day = Com1RcvBuf[19];
		CardInfo.valid_time.hour = 0x00;
		CardInfo.valid_time.minute = 0x00;
		CardInfo.valid_time.second = 0x00;

		strcpy((void *)&DevStat.start_station_number[0], (char *)"");
		strcpy((void *)&DevStat.start_station_name[0], (char *)"");

		strcpy((void *)&DevStat.end_station_number[0], (char *)"");
		strcpy((void *)&DevStat.end_station_name[0], (char *)"");

		(void)Get_Time(&CardInfo.card_in_time);
//		memcpy((void *)&CardInfo.card_in_time, (void *)&time, sizeof(BUS_TIME));


		////////////////////////////////////////
		//���ֿ����Ӵ�������
		////////////////////////////////////////
		switch ( CardInfo.card_type )
		{
		   case CARDT_YOUHUI:		//�Żݿ�
			   i = card_youhui_process();
			   break;

//           case CARDT_YUEPIAO:      //��Ʊ��
////  		   i = card_yuepiao_process();
//               i = notok;
//               break;

		   default:                //�����ʹ���
			   lcddisp(2, 1, (void *)"    �����ʹ���!!    ");
			   lcddisp(3, 1, (void *)"                    ");
			   lcddisp(4, 1, (void *)"                    ");
			   beep_failed();
			   sleep_ms(1000);
			   lcddisp(2, 1, (void *)"                    ");
			   continue;
		}
		if(i != ok)
			continue;

		//ˢ���ɹ� ����ѭ��
		break;

	}

	(void)EA_ucCloseDevice(&hRS232Handle);

	return ok;

}

/******************************************************************************
 �������ƣ�card_youhui_process
 �����������Żݿ���������
 ����������
 �������ƣ�	����/�����	����		����
				
 ��  ��  ֵ��ok(0)-�ɹ�
			 notok(0xFF)-ʧ��
				   
 ��      ��	������
 ��      �ڣ�2012-10-30
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
INT8U card_youhui_process(void)
{
	FP64 pricef = 0.0;
	INT8U buf[30];
	INT8U prices_tmp[8];
	INT8U i = 0;
	INT16U rcv_len = 0;

	//ȡƱ��
	pricef = atof((void *)&DevStat.prices[0]);
//	DevStat.iccard_price = (INT32U)(pricef * 100.0);
	CardInfo.fare = (INT32U)(pricef * 100.0);

	CardInfo.fare = CardInfo.fare * DevStat.ticket_amount * ( DevStat.zhekou / 100.0 );			//����*����
	sprintf((void *)prices_tmp, "%.2f", ((FP32)pricef * (DevStat.zhekou / 100.00)));
//  htoa( (void *)&DevStat.prices[0], &CardInfo.fare, 4);
//  strcpy((void *)&DevStat.prices[0], (void *)prices_tmp);

	if ( CardInfo.init_balance < CardInfo.fare )
	{
		//���㴦��
		lcddisp(2, 1, (void *)"     ����!!     ");
		sprintf((void *)buf, "�����: %.2fԪ", (FP32)CardInfo.init_balance / 100.0);
		lcddisp(3, 1, (void *)buf);
		lcddisp(4, 1, (void *)"                    ");
		beep_failed();
		sleep_ms(3000);

		return notok;
	}

	//�ۿ�
	memcpy(Com1SndBuf, "\xAA\x07\x14\x00\x00\x00\x00", 7);
	memcpy(&Com1SndBuf[7], &CardInfo._CARD_NUMBER.detail_card_number.serial_number[0], 4);
	memcpy(&Com1SndBuf[11], &CardInfo.card_in_time, 7);
	memcpy(&Com1SndBuf[18], &CardInfo.fare, 4);
	Com1SndBuf[22] = 0x06;								//��������
	i = com1_snd_and_rcv2(Com1SndBuf, 23, (void *)Com1RcvBuf, &rcv_len, 1000, 10);
	if ( i != ok )
	{
		lcddisp(2, 1, (void *)"      �ۿ�ʧ��      ");
		lcddisp(3, 1, (void *)"�������: 0001      ");
		lcddisp(4, 1, (void *)"                    ");
		beep_failed();
		sleep_ms(3000);
		return notok;				//ͨѶ����
	}
	if ( rcv_len == 9 )
	{
		//����ʧ�ܴ���
		lcddisp(2, 1, (void *)"      �ۿ�ʧ��      ");
		sprintf((void *)buf, "�������: %02X%02X", Com1RcvBuf[5], Com1RcvBuf[6]);
		lcddisp(3, 1, (void *)buf);
		lcddisp(4, 1, (void *)"                    ");
		beep_failed();
		sleep_ms(3000);
		return notok;
	}
	else if ( rcv_len == 16 )
	{
		//�ۿ�ɹ�����
//		memcpy(&CardInfo._CARD_NUMBER.detail_card_number.serial_number[0], &Com1RcvBuf[3], 4);
//		CardInfo.card_purchase_type = Com1RcvBuf[3];

		DevStat.ticket_type = '9';
		CardInfo.card_purchase_type = 0x06;
		memcpy(&CardInfo.balance, &Com1RcvBuf[8], 4);
		strcpy((void *)&DevStat.prices[0], (void *)prices_tmp);

		lcddisp(2, 1, (void *)"�Żݿ�  �ۿ�ɹ�    ");
		sprintf((void *)buf, "�����: %.2fԪ", (FP32)CardInfo.balance / 100.0);
		lcddisp(3, 1, (void *)buf);
		sprintf((void *)buf, "�������: %.2fԪ", (FP32)CardInfo.fare / 100.0);
		lcddisp(4, 1, (void *)buf);
//  	lcddisp(4, 1, (void *)"                    ");

		if ( CardInfo.balance <= 500 )
		{
			lcddisp(4, 1, (void *)"������  ���ֵ!!");
		}

		beep_success();
		sleep_ms(5500);
		return ok;
	}
	else
	{
		lcddisp(2, 1, (void *)"      �ۿ�ʧ��      ");
		lcddisp(3, 1, (void *)"�������: 0002      ");
		lcddisp(4, 1, (void *)"                    ");
		beep_failed();
		sleep_ms(3000);
		return notok;				//���ղ���ȷ
	}

}

/******************************************************************************
 �������ƣ�card_yuepiao_process
 ������������Ʊ����������
 ����������
 �������ƣ�	����/�����	����		����
				
 ��  ��  ֵ��ok(0)-�ɹ�
			 notok(0xFF)-ʧ��
				   
 ��      ��	������
 ��      �ڣ�2012-10-30
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
INT8U card_yuepiao_process(void)
{
	//FP64 pricef = 0.0;
	INT8U buf[50];
	INT8U i = 0;
	INT16U rcv_len = 0;
//	INT32S time_rec;
	INT32S time_cur;
	INT32S time_incard;

	//�ж���Ч��
	time_cur = cal_sec((void *)&CardInfo.card_in_time);
	time_incard = cal_sec((void *)&CardInfo.valid_time);

	if (time_cur > time_incard)
	{
		//�����ˣ��ۿ�
		lcddisp(2, 1, (void *)"  �ÿ��ѳ�����Ч��  ");
		beep_failed();
		sleep_ms(3000);
		return notok;				//ֱ���˳�
	}
	else 
	{
		//����Ч���ڣ�����Ǯ
		//ֻ�ܰ��ն�Ӧ�Ľ���������ˢ�� ,, frank 14��10��18��
		//ȡƱ��
		CardInfo.fare = 0;
		CardInfo.balance = 0;			//���Ϳ�������0
//  	memset((void *)&DevStat.prices[0], 0x00, 8);                    //Ʊ��Ϊ0.00Ԫ
		sprintf((void *)&DevStat.prices[0], "%.2f", 0.00);
		DevStat.ticket_amount = 1;										//����Ϊ1

		DevStat.start_station = CardInfo.qishi;
		strcpy((void *)&DevStat.start_station_number[0], (char *)&STinfo[DevStat.line_ptr].forward_number[DevStat.start_station][0]);
		strcpy((void *)&DevStat.start_station_name[0], (char *)&STinfo[DevStat.line_ptr].forward_name[DevStat.start_station][0]);

		DevStat.end_station = CardInfo.daoda;
		strcpy((void *)&DevStat.end_station_number[0], (char *)&STinfo[DevStat.line_ptr].forward_number[DevStat.end_station][0]);
		strcpy((void *)&DevStat.end_station_name[0], (char *)&STinfo[DevStat.line_ptr].forward_name[DevStat.end_station][0]);
//  	if ( CardInfo.init_balance < CardInfo.fare )
//  	{
//  		//���㴦��
//  		lcddisp(3, 1, (void *)"���Ǹ��������Ʊ��!!");
//  		lcddisp(4, 1, (void *)"     ���ܳ���!!     ");
//  		beep_failed();
//  		sleep_ms(1000);
//  		return notok;
//  	}

		//5�����ڲ���������ˢ��
		i = Cal_Interval_Sec(300);
		if (i != ok)
		{
			lcddisp(2, 1, (void *)"      �ۿ�ʧ��      ");
			lcddisp(3, 1, (void *)"��Ʊ�����ܴ���      ");
			lcddisp(4, 1, (void *)"����������5����   ");
			beep_failed();
			sleep_ms(3000);
			return notok;				//ͨѶ����
		}

		//����ֻ��Ϊ1��
		if ( DevStat.ticket_amount > 1 )
		{
			lcddisp(2, 1, (void *)"      �ۿ�ʧ��      ");
			lcddisp(3, 1, (void *)"����Ч���ڵ���Ʊ��  ");
			lcddisp(4, 1, (void *)"ֻ����һ��Ʊ!!      ");
			beep_failed();
			sleep_ms(3000);
			return notok;				//ͨѶ����
		}

		//�ۿ�
		memcpy(Com1SndBuf, "\xAA\x07\x14\x00\x00\x00\x00", 7);
		memcpy(&Com1SndBuf[7], &CardInfo._CARD_NUMBER.detail_card_number.serial_number[0], 4);
		memcpy(&Com1SndBuf[11], &CardInfo.card_in_time, 7);
		memset(&Com1SndBuf[18], 0x00, 4);
		Com1SndBuf[22] = 0x07;			//��������
		i = com1_snd_and_rcv2(Com1SndBuf, 23, (void *)Com1RcvBuf, &rcv_len, 1000, 10);
		if ( i != ok )
		{
			lcddisp(2, 1, (void *)"      �ۿ�ʧ��      ");
			lcddisp(3, 1, (void *)"�������: 0001      ");
			lcddisp(4, 1, (void *)"                    ");
			beep_failed();
			sleep_ms(3000);
			return notok;				//ͨѶ����
		}
		if ( rcv_len == 9 )
		{
			//����ʧ�ܴ���
			lcddisp(2, 1, (void *)"      �ۿ�ʧ��      ");
			sprintf((void *)buf, "�������: %02X%02X", Com1RcvBuf[5], Com1RcvBuf[6]);
			lcddisp(3, 1, (void *)buf);
			lcddisp(4, 1, (void *)"                    ");
			beep_failed();
			sleep_ms(3000);
			return notok;
		}
		else if ( rcv_len == 16 )
		{
			//�ۿ�ɹ�����
//			CardInfo.card_purchase_type = Com1RcvBuf[3];
			DevStat.ticket_type = '8';
			CardInfo.card_purchase_type = 0x07;
			memcpy(&CardInfo.balance, &Com1RcvBuf[8], 4);

			lcddisp(2, 1, (void *)"��Ʊ��  �ۿ�ɹ�    ");
			sprintf((void *)buf, "��Ч����:%02X%02X.%02X.%02X ", CardInfo.valid_time.century, 
					CardInfo.valid_time.year, CardInfo.valid_time.month, CardInfo.valid_time.day);
			lcddisp(3, 1, (void *)buf);

			sprintf((void *)buf, "%s-%s", (char *)&DevStat.start_station_name[0], (char *)&DevStat.end_station_name[0]);  // ��ʾ��ʼ�͵���վ����Ϣ 
			lcddisp(4, 1, (void *)buf);

			if ( time_incard - time_cur <= 15UL * 24UL * 60UL * 60UL )			//��Ч����ǰ15������
			{
				lcddisp(4, 1, (void *)"��Ч�ڽ���  ���ֵ!!");
			}

			beep_success();

			sleep_ms(5500);
			return ok;
		}
		else
		{
			lcddisp(2, 1, (void *)"      �ۿ�ʧ��      ");
			lcddisp(3, 1, (void *)"�������: 0002      ");
			lcddisp(4, 1, (void *)"                    ");
			beep_failed();
			sleep_ms(3000);
			return notok;				//���ղ���ȷ
		}
	}


}

/******************************************************************************
 �������ƣ�Cal_Interval_Sec
 ��������������ˢ�����ʱ��
 ����������
 �������ƣ�	����/�����	����		����
 interval	����		INT32U		��
 
 ��  ��  ֵ��ok - �Ѿ�����ʱ������,�������ˢ��
				notok - ���������ˢ��
   
 ��      ��	������
 ��      �ڣ�2006.1.9
 �޸���ʷ��
		����		�޸���		�޸�����

******************************************************************************/
INT8U Cal_Interval_Sec(INT32U interval)
{
	INT32U k = 0;
//	INT8U rec[REC_LEN + 2];
//	RECORD *prec = (RECORD *)rec;
	INT8S h = 0;
	BUS_TIME rec_time;
	INT32S time_rec;
	INT32S time_cur;
	INT8U card_num_cur[10];				//��ǰ����

	const char recFileName[] = "record";
	INT8U ucOpenID = 0;
	INT32U total_rec_num = 0;

	TICKET_INFO ticket;
	INT32U uiResult;
	INT32U uiReadCnt;
//	INT32U j = 0;

	// ���ļ�
	if ( EA_ucPFOpen((uchar *)recFileName, &ucOpenID) != EM_ffs_SUCCESS )
	{
		lcddisperr("�򿪼�¼�ļ�ʧ��");
		return ok;
	}


	(void)EA_ucPFInfo(ucOpenID, EM_ffs_VALIDREC, &uiResult);
	total_rec_num = uiResult;
	DevStat.record_number = total_rec_num;

//	if ( EA_ucPFWriteRec(ucOpenID, 0, rec) != EM_ffs_SUCCESS )
//	{
//		lcddisperr("��Ӽ�¼ʧ��");
//		return ok;
//	}

//	if ( EA_ucPFClose(ucOpenID) != EM_ffs_SUCCESS )
//	{
//		lcddisperr("�رռ�¼�ļ�ʧ��");
//		return ok;
//	}

	if (DevStat.record_number == 0)
	{
		(void)EA_ucPFClose(ucOpenID);
		return ok;
	}

	time_cur = cal_sec((void *)&CardInfo.card_in_time);
	sprintf((void *)card_num_cur, "%02X%02X%02X%02X", 
			CardInfo._CARD_NUMBER.detail_card_number.serial_number[0], 
			CardInfo._CARD_NUMBER.detail_card_number.serial_number[1], 
			CardInfo._CARD_NUMBER.detail_card_number.serial_number[2], 
			CardInfo._CARD_NUMBER.detail_card_number.serial_number[3] );			//����
	
	////////////////////////////////////////
	//����ˢ�����ʱ��
	////////////////////////////////////////
	for ( k = 0; k < DevStat.record_number; k++ )
	{
//		get_record((INT8U *)rec, DevStat.record_number - k - 1);    //32�ֽڼ�¼
		(void)EA_ucPFReadRec(ucOpenID, DevStat.record_number - k, 1, &uiReadCnt, &ticket.pos_number[0]);

//		h = memcmp((void *)&CardInfo.card_in_time, (void *)&prec->card_in_time, 2); //�Ƚ�����,��
//		if ( h != ok )
//		{
//			//�겻���,��������
//			return ok;
//		}

		rec_time.century = (ascii_to_hex(ticket.ticket_time[0]) << 4) | ascii_to_hex(ticket.ticket_time[1]);
		rec_time.year = (ascii_to_hex(ticket.ticket_time[2]) << 4) | ascii_to_hex(ticket.ticket_time[3]);
		rec_time.month = (ascii_to_hex(ticket.ticket_time[4]) << 4) | ascii_to_hex(ticket.ticket_time[5]);
		rec_time.day = (ascii_to_hex(ticket.ticket_time[6]) << 4) | ascii_to_hex(ticket.ticket_time[7]);
		rec_time.hour = (ascii_to_hex(ticket.ticket_time[8]) << 4) | ascii_to_hex(ticket.ticket_time[9]);
		rec_time.minute = (ascii_to_hex(ticket.ticket_time[10]) << 4) | ascii_to_hex(ticket.ticket_time[11]);
		rec_time.second = (ascii_to_hex(ticket.ticket_time[12]) << 4) | ascii_to_hex(ticket.ticket_time[13]);

		time_rec = cal_sec(&rec_time);
		if ( (time_cur - time_rec >= (INT32S)interval) || (time_rec - time_cur >= (INT32S)interval) )   //����ʱ����Ҳû���ҵ��˿�����
		{
			(void)EA_ucPFClose(ucOpenID);
			return ok;
		}
		else   //��ʱ������,�������Ƿ���ͬ
		{
//			h = memcmp((void *)&CardInfo._CARD_NUMBER.detail_card_number.serial_number[0], (void *)&prec->card_number[4], 4);
			h = strcmp((char *)card_num_cur, (char *)ticket.card_number);
			if ( h == ok ) //�������
			{
				(void)EA_ucPFClose(ucOpenID);
				return notok;
			}
			else //������������ѯ
			{
				;
			}
		}
	}

	(void)EA_ucPFClose(ucOpenID);
	return ok;
}






