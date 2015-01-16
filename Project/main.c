/******************************************************************************
Copyright 2012 大连易达通电子有限公司
All rights riserved.

文件名	：main.c
模块名称：长途售票机主程序
功能概要：长途售票机主程序 
 
 
 
当前版本：0.1.0
修改人	：frank 
完成日期：2015.1.15
升级说明：  1.取消现金消费，刷卡消费，两种快捷键，对应两种卡类型。
    		2.消费金额，手动输入，
    		3.不打印消费票据
    		4.消费记录显示，时间延长，
    		5.目前发现有的优惠卡的消费，没有记录，也不打印票据，怀疑和打印机初始化有关，现在取消打印任务！
 
 
当前版本：0.0.9
修改人	：frank 
完成日期：2014.11.28
升级说明：  1.修改一个刷卡，消费的重复折扣的Bug，
    		2.三人同乘坐，4元，票价，变成12元 ，打印的时候，票面金额显示的每张都是12元，应该修改为每张4元。
    		3.Pos机全天汇总的项目，直接显示月票卡项，优惠卡项，其他取消
 
 
当前版本：0.0.8
修改人	：frank 
完成日期：2014.11.28
升级说明：  1.add youxiaoqi  ,kaishi he jiezhi riqi shezhi
			2.add yuepiao ka , zhijie shuaka kuaijie fangshi 
 
当前版本：0.0.7
修改人	：许岩
完成日期：2014.10.11
升级说明：  1. 增加刷卡功能
 
当前版本：0.0.6
修改人	：刘及华
完成日期：2013.4.17
升级说明：  1. 增加缺纸或者是机芯初始化失败的多次判断，判断十次，每次间隔100ms，10次都判断失败， 
		然后退出，如果任何一次成功了，就继续打印！ 
 
当前版本：0.0.5
修改人	：刘及华
完成日期：2013.1.22
升级说明：  1. 车号、票号、员工浩，都是两次设置，并且取消一键退出功能
    		2. 增加缺纸张判断功能，无纸再次输入票号功能
    		3. 在售票期间，增加判断初始工号0000，初始车号12345，判断功能，若有则重新登录
 
当前版本：0.0.4
修改人	：刘及华
完成日期：2012.12.14
升级说明：  1. 连续输入两次票号才可以售票 
    		2. 增加打印日结票据功能
    		3. 可以将打印功能做存储，票号记录为6，查询系统可以查询该票
 
当前版本：0.0.3
修改人	：许岩
完成日期：2012.12.01
升级说明：  1. 如果没选择线路，不让售票 
    		2. 第一次格式化后，必须输入车号
    		3. 第一次格式化后，必须设置POS号
 
当前版本：0.0.2
修改人	：许岩
完成日期：2012.11.24
升级说明：1. 修正了售票员登录，如果先输入0000号，再改其他号不能登录的bug 
    		2. 修正了当出厂后第一次大格，提示动态内存分配错误的bug
    		3. 进入系统设置菜单的初始化本机，改为会清除参数文件和正常记录和补采记录。
    		4. 放开查询当天总计功能。
    		5. 票行间距缩小，单票长度缩小。

取代版本：0.0.1
修改人	：许岩
完成日期：2012.10.24
升级说明：create

******************************************************************************/

#include "main.h"


const char *VERSION = " 0.1.0";         //版本号
char dbuf[100];
volatile char Flag_Exticket = 0;
const char *BUSS_NUM_FULL_PRE =	"鲁P-";



INT8U main_menu[7*20+2] = "1.售票              2.票号设置          3.班次设置          4.数据处理          5.今日统计          6.选择线路          7.系统设置          ";

//INT8U data_menu[] = "1.废票              2.上传售票记录      3.下载参数表        ";
INT8U data_menu[2*20+2] = "1.废票              2.记录和参数传输    ";
INT8U setting_menu[5 *20 + 2] = "1.时间设置          2.设备编号设置      3.初始化本机        4.上传备份记录      5.优惠卡折扣设置    ";
INT8U confirm_menu[2*20+2] = "1.确认              2.取消              ";


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

extern INT8U Com1SndBuf[UART1_SEND_QUEUE_LENGTH];              //串口发送缓冲区
extern INT8U Com1RcvBuf[UART1_RCV_QUEUE_LENGTH];               //串口接收缓冲区
extern INT8U Com1RcvBuf2[UART1_RCV_QUEUE_LENGTH];               //串口接收缓冲区
CARD_INFO CardInfo;

/*****************************************************************
 函数原型：main()
 功能描述：主函数
 参数描述：

 返回值：   无

 作  者：   许岩
 日  期：   2012-10-24
 修改历史：
		日期        修改人      修改描述
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
		menus = browse_menu_time(1, (void *)"     主菜单     ", main_menu, FALSE);
		switch ( menus )
		{
		   case 0:              //1.售票
			   sell_ticket();
			   break;

		   case 1:              //2.票号设置
			   set_ticket_number();
			   break;

		   case 2:              //3.班次设置
			   set_banci();
			   break;

		   case 3:              //4.数据处理
				//  		   mdata();
			   send_record_process();
			   break;

		   case 4:              //5.今日统计
			   query_today_summary();
			   break;

		   case 5:              //6.选择线路
			   choose_line();
			   break;

		   case 6:              //7.系统设置
			   msystem();
			   break;

		   case 10:              //8.月票卡消费
			   ic_yuepiaocard_process();
			   break;

		   default:
			   break;

			   //  	   case 0:              //1.售票
			   //  		   sell_ticket();
			   //  		   break;
			   //
			   //  	   case 1:              //2.线路选择
			   //  		   choose_line();
			   //  		   break;
			   //
			   //  	   case 2:              //3.查询
			   //  		   break;
			   //
			   //  	   case 3:              //4.今日统计
			   //  		   query_today_summary();
			   //  		   break;
			   //
			   //  	   case 4:              //5.数据
			   //  		   mdata();
			   //  		   break;
			   //
			   //  	   case 5:              //6.系统
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
 函数原型：param_init()
 功能描述：参数等初始化
 参数描述：

 返回值：   无

 作  者：   许岩
 日  期：   2004-09-17
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
*****************************************************************/
void param_init(void)
{

	//    // 实现字符串滚动显示
	//
	//    int  x, y, i;
	//    uint uiZoom;
	//    const char acTestString[] = "测试字符串,Test string";
	//
	//
	//    uiZoom = 2; // 将字符串放大2倍显示
	//    x = strlen(acTestString) * 8 * uiZoom;
	////     y = (128 - 16 * uiZoom)/2;
	//    y = (64 - 16 * uiZoom) / 2;
	//    for (i = 0; i < 160 + x; i++)
	//    {
	//        // 锁定屏幕，防止清屏造成屏幕闪烁
	//        EA_vLockScreen();
	//        EA_vCls();
	//        EA_vTextOut(i - x, y, EM_key_FONT8X16, 0, uiZoom, uiZoom, acTestString);
	//        // 解除屏幕锁定
	//        EA_vUnlockScreen();
	//        EA_uiInkeyMs(50); // 增加延时
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
	//  EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "大连易达通电子技术");
	EA_vDisp(1, 1, "    大连虹海科技    ");
	EA_vDisp(2, 1, "     车载售票机     ");
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


	//  //动态分配空间
	//  OPinfo = (OPERATOR_INFO *)EA_pvMalloc(MAX_OPERATOR_NUM * sizeof(OPERATOR_INFO) + 1);
	//  if ( OPinfo == NULL )
	//  {
	//  	lcddisperr("动态分配空间错误");
	//  }
	//  Linfo = (LINE_INFO *)EA_pvMalloc(MAX_LINE_NUM * sizeof(LINE_INFO) + 1);
	//  if ( Linfo == NULL )
	//  {
	//  	lcddisperr("动态分配空间错误");
	//  }
	//  STinfo = (STATION_INFO *)EA_pvMalloc(MAX_LINE_NUM * sizeof(STATION_INFO) + 1);
	//  if ( STinfo == NULL )
	//  {
	//  	lcddisperr("动态分配空间错误");
	//  }
	//
	//
	//  memset((void *)OPinfo, 0x00, MAX_OPERATOR_NUM * sizeof(OPERATOR_INFO));
	//  memset((void *)Linfo, 0x00, MAX_LINE_NUM * sizeof(LINE_INFO));
	//  memset((void *)STinfo, 0x00, MAX_LINE_NUM * sizeof(STATION_INFO));

	DevStat.operator_amount = 1;
	DevStat.line_amount = 1;
	//动态分配内存，初始化变量
	OPinfo = (OPERATOR_INFO *)EA_pvMalloc(DevStat.operator_amount * sizeof(OPERATOR_INFO) + 1);
	if ( OPinfo == NULL )
	{
		lcddisperr("动态分配空间错误");
	}
	Linfo = (LINE_INFO *)EA_pvMalloc(DevStat.line_amount * sizeof(LINE_INFO) + 1);
	if ( Linfo == NULL )
	{
		lcddisperr("动态分配空间错误");
	}
	STinfo = (STATION_INFO *)EA_pvMalloc(DevStat.line_amount * sizeof(STATION_INFO) + 1);
	if ( STinfo == NULL )
	{
		lcddisperr("动态分配空间错误");
	}
	memset((void *)OPinfo, 0x00, DevStat.operator_amount * sizeof(OPERATOR_INFO));
	memset((void *)Linfo, 0x00, DevStat.line_amount * sizeof(LINE_INFO));
	memset((void *)STinfo, 0x00, DevStat.line_amount * sizeof(STATION_INFO));

	read_param_table();

	//组织线路选择菜单
	strcpy((void *)line_menu, (void *)&Linfo[0].menu_name[0]);
	for ( j = 1; j < DevStat.line_amount; j++ )
	{
		strcat((void *)line_menu, (void *)&Linfo[j].menu_name[0]);
	}

	sprintf((void *)buf, "参数版本%s", DevStat.version);
	lcddisp(4, 1, buf);
	(void)EA_uiInkey(2);

	//  sprintf((void *)buf, "%s,%s,%s",OPinfo[0].number, OPinfo[0].name, OPinfo[0].passwd);
	//  lcddisperr((void *)buf);




	//  memset((void *)OPinfo, 0x00, sizeof(OPinfo));
	//  strcpy((void *)&OPinfo[0].number[0], "0000");
	//  strcpy((void *)&OPinfo[0].name[0], "许岩");
	//  strcpy((void *)&OPinfo[0].passwd[0], "123456");
	//  //  strcpy((void *)&DevStat.bus_number[0], "12345");
	//
	//
	//
	//  DevStat.line_amount = 3;
	//  strcpy((void *)&Linfo[0].number[0], "@55");
	//  strcpy((void *)&Linfo[0].menu_name[0], "55淄博(快客)        ");
	//  strcpy((void *)&Linfo[0].full_name[0], "聊城--淄博(快客)");
	//
	//  strcpy((void *)&Linfo[1].number[0], "@59");
	//  strcpy((void *)&Linfo[1].menu_name[0], "59淄博              ");
	//  strcpy((void *)&Linfo[1].full_name[0], "聊城--淄博");
	//
	//  strcpy((void *)&Linfo[2].number[0], "@63");
	//  strcpy((void *)&Linfo[2].menu_name[0], "63济南(交快)        ");
	//  strcpy((void *)&Linfo[2].full_name[0], "聊城--济南(交快)");
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
	//  strcpy((void *)&STinfo[0].forward_name[0][0], "站点一");
	//  strcpy((void *)&STinfo[0].forward_name[1][0], "站点二");
	//  strcpy((void *)&STinfo[0].forward_name[2][0], "站点三");
	//  strcpy((void *)&STinfo[0].forward_name[3][0], "站点四");
	//  strcpy((void *)&STinfo[0].forward_name[4][0], "站点五");
	//
	//  strcpy((void *)&STinfo[0].backward_number[0][0], "0001");
	//  strcpy((void *)&STinfo[0].backward_number[1][0], "0002");
	//  strcpy((void *)&STinfo[0].backward_number[2][0], "0003");
	//  strcpy((void *)&STinfo[0].backward_number[3][0], "0004");
	//  strcpy((void *)&STinfo[0].backward_number[4][0], "0005");
	//
	//  strcpy((void *)&STinfo[0].backward_name[0][0], "站点五");
	//  strcpy((void *)&STinfo[0].backward_name[1][0], "站点四");
	//  strcpy((void *)&STinfo[0].backward_name[2][0], "站点三");
	//  strcpy((void *)&STinfo[0].backward_name[3][0], "站点二");
	//  strcpy((void *)&STinfo[0].backward_name[4][0], "站点一");
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
	//  strcpy((void *)&STinfo[2].forward_name[0][0], "站点一");
	//  strcpy((void *)&STinfo[2].forward_name[1][0], "站点二");
	//  strcpy((void *)&STinfo[2].forward_name[2][0], "站点三");
	//  strcpy((void *)&STinfo[2].forward_name[3][0], "站点四");
	//  strcpy((void *)&STinfo[2].forward_name[4][0], "站点五");
	//
	//  strcpy((void *)&STinfo[2].backward_number[0][0], "0001");
	//  strcpy((void *)&STinfo[2].backward_number[1][0], "0002");
	//  strcpy((void *)&STinfo[2].backward_number[2][0], "0003");
	//  strcpy((void *)&STinfo[2].backward_number[3][0], "0004");
	//  strcpy((void *)&STinfo[2].backward_number[4][0], "0005");
	//
	//  strcpy((void *)&STinfo[2].backward_name[0][0], "站点五");
	//  strcpy((void *)&STinfo[2].backward_name[1][0], "站点四");
	//  strcpy((void *)&STinfo[2].backward_name[2][0], "站点三");
	//  strcpy((void *)&STinfo[2].backward_name[3][0], "站点二");
	//  strcpy((void *)&STinfo[2].backward_name[4][0], "站点一");
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
 函数原型：system_check()
 功能描述：系统自检以及gprs模块参数配置
 参数描述：	
				 
 返回值：	无
 
 作  者：	许岩
 日  期：	2004-09-17
 修改历史：
		日期		修改人		修改描述
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

	//格式化后第一次上电，必须输入设备号
	if ( strcmp((void *)&DevStat.pos_number[0], "000000") == 0 )
	{
		set_pos_number();
	}

}

/******************************************************************************
 函数名称：TimeFormatChk
 功能描述：读时钟，看时钟格式是否正确
 参数描述：
 参数名称：	输入/输出？	类型		描述
				
 返  回  值：ok(0)-自检成功
				 notok(0xFF)-自检失败
				   
 作      者	：许岩
 日      期：2004-09-02
 修改历史：
		日期		修改人		修改描述
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
 函数名称：TimeModify
 功能描述：要求输入时间，进行修改
 参数描述：
 参数名称：	输入/输出？	类型		描述
				
 返  回  值：无
				   
 作      者	：许岩
 日      期：2004-09-02
 修改历史：
		日期		修改人		修改描述
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
		sprintf((void *)buf, "当前:%02X%02X%02X%02X%02X%02X%02X", ltime.century, ltime.year, ltime.month,
				ltime.day, ltime.hour, ltime.minute, ltime.second);
		EA_vDisp(1, 1, (void *)buf);
		EA_vDisp(2, 1, "请输入新的时间:");
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
			EA_vDisp(4, 1, "时间格式错误");
			SleepMs(1500);
			//  		EA_vCls();
			continue;
		}

		Modify_Time(&ltime);
		EA_vDisp(4, 1, "时间修改成功");
		SleepMs(1500);
		break;
	}
}

void printer_test(void)
{
	INT8U ucRet = 0;
	//  DevHandle hDprinterHandle;

	// 打印设备属于I/O模块的一种，应先获得操作句柄，此时决定打印采用的模式
	ucRet = EA_ucOpenDevice("PRINT", EM_io_REALMODE, &hDprinterHandle);
	if ( ucRet != EM_SUCCESS )
	{
		EA_vDisplay(2, "获得打印操作句柄失败!");
		return;
	}

	// POS起动时,调用初始化函数初始化打印机芯状态
	ucRet =  EA_ucInitPrinter(hDprinterHandle);
	if ( ucRet != EM_SUCCESS )
	{
		EA_vDisplay(2, "初始化打印机芯失败!");
		return;
	}

	// 可进行打印的一些基本设置
	ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_XSPACE, 0);
	if ( ucRet != EM_SUCCESS )
	{
		EA_vDisplay(2, "设置失败!");
		return;
	}
	ucRet =  EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_YSPACE, 8);
	if ( ucRet != EM_SUCCESS )
	{
		EA_vDisplay(2, "设置失败!");
		return;
	}

	ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_PAGESPACE, 792);
	if ( ucRet != EM_SUCCESS )
	{
		EA_vDisplay(2, "设置失败!");
		return;
	}

	ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_ASCII_DOTTYPE, EM_prn_ASCIIDOT7X7);
	if ( ucRet != EM_SUCCESS )
	{
		EA_vDisplay(2, "设置失败!");
		return;
	}

	// 打印测试
	ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "打印测试!\n");
	if ( ucRet != EM_SUCCESS )
	{
		EA_vDisplay(2, "本行打印失败!");
		return;
	}
	ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "大连易达通电子技术有限公司1\n");
	ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "大连易达通电子技术有限公司2\n");
	ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "大连易达通电子技术有限公司3\n");
	ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "大连易达通电子技术有限公司4\n");
	ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "大连易达通电子技术有限公司5\n");
	ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "大连易达通电子技术有限公司6\n");

	ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_ASCII_DOTTYPE, EM_prn_ASCIIDOT16X8);
	ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "大连易达通电子技术有限公司1\n");
	ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "大连易达通电子技术有限公司2\n");

	// 打印最后调用换页命令
	ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "\f");
	if ( ucRet == EM_SUCCESS )
	{
		EA_vDisplay(2, "本次打印结束!");
	}

	//  ucRet = EA_ucCutter(hDprinterHandle);
	//  if ( ucRet != EM_SUCCESS )
	//  {
	//  	EA_vDisplay(2,"切纸操作失败!");
	//  }

}

/******************************************************************************
 函数名称：operator_login
 功能描述：员工登录
 参数描述：
 参数名称：	输入/输出？	类型		描述
				
 返  回  值：无
				   
 作      者	：许岩
 日      期：2012-10-30
 修改历史：
		日期		修改人		修改描述
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
	strcpy((void *)admin_name, "虹海");
	strcpy((void *)admin_passwd, "228846");

start_login:
	admin_login = FALSE;
	EA_vCls();
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "    员工登录    ");
	lcddisp(1, 1, (void *)"工号:");
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

	lcddisp(1, 1, (void *)"工号:");
	for (;;)
	{
		i = EA_ucGetInputStr(1, 6, 20, EM_BIG_FONT | EM_MODE_NUMBER | EM_ALIGN_LEFT | EM_SHOW_ORIGINALLY
							 , 4, 4, 0, (void *)input2);
		if ( i == EM_SUCCESS )
			break;
	}
	if(strcmp((void*)input1, (void*)input2) != 0)     //连续输入两次员工号，必须保证两次都一致才可以继续进行操作，BY 刘及华13.1.22
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
			sprintf((void *)buf, "工号:%s %s", admin_num, admin_name);
			lcddisp(1, 1, (void *)buf);
			goto input_passwd;
		}
		else
		{

			lcddisp(4, 1, (void *)"员工号无效!         ");
			SleepMs(1500);
			goto start_login;
		}
	}

	sprintf((void *)buf, "工号:%s %s", &OPinfo[j].number[0], &OPinfo[j].name[0]);
	lcddisp(1, 1, (void *)buf);

input_passwd:
	//输入密码
	lcddisp(2, 1, (void *)"密码:");
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
			lcddisp(4, 1, (void *)"密码错误!           ");
			SleepMs(1500);
			lcddisp(4, 1, (void *)"                    ");
			goto input_passwd;
		}
	}

	if ( strcmp((void *)input1, (void *)&OPinfo[j].passwd[0]) == 0 )
	{}
	else
	{
		lcddisp(4, 1, (void *)"密码错误!           ");
		SleepMs(1500);
		lcddisp(4, 1, (void *)"                    ");
		goto input_passwd;

	}

	DevStat.op_ptr = j;
	strcpy((void *)&DevStat.op_number[0], (void *)&OPinfo[j].number[0]);
	strcpy((void *)&DevStat.op_name[0], (void *)&OPinfo[j].name[0]);


}

/******************************************************************************
 函数名称：line_number_verify
 功能描述：核对线路号
 参数描述：
 参数名称：	输入/输出？	类型		描述
				
 返  回  值：无
				   
 作      者	：许岩/刘及华
 日      期：2012-12-02
 修改历史：
		日期		修改人		修改描述
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
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "    线路确认    ");
	sprintf((void *)buf, "当前线路:%s", (char *)&DevStat.line_number[0]);
	lcddisp(1, 1, (void *)buf);
	sprintf((void *)buf, "%s", (char *)&DevStat.line_full_name[0]);
	lcddisp(2, 1, (void *)buf);
	lcddisp(4, 1, (void *)"F1-修改   其它-继续");
	if(strcmp((void *)buf,"") == 0)    //必须进行线路选择 ，否则无法进行一下步
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
 函数名称：bus_number_verify
 功能描述：核对车号
 参数描述：
 参数名称：	输入/输出？	类型		描述
				
 返  回  值：无
				   
 作      者	：许岩/刘及华
 日      期：2012-12-2     
 修改历史：增加进入车号设置，如果不修改的话，无法进行下一步操作
		日期		修改人		修改描述
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

	//格式化后第一次上电，必须输入车号
	if ( strcmp((void *)&DevStat.bus_number[0], "12345") == 0 )
	{
		goto set_bus_number;
	}

	EA_vCls();
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "    车号确认    ");
	//  sprintf((void *)buf, "车号:鲁P-%s", &DevStat.bus_number[0]);
	sprintf((void *)buf, "车号:%s", &DevStat.bus_number_full[0]);
	lcddisp(1, 1, (void *)buf);
	//  lcddisp(3, 1, (void *)"修改请按F1键");
	//  lcddisp(4, 1, (void *)"按其它键继续..");
	lcddisp(4, 1, (void *)"F1-修改   其它-继续");

	key = EA_uiInkey(0);
	if ( key == EM_key_F1 )
	{
set_bus_number:
		lcddisp(3, 1, (void *)"                    ");
		lcddisp(4, 1, (void *)"                    ");
		lcddisp(2, 1, (void *)"输入车号:           ");
		strcpy((void *)input1, "");
		strcpy((void *)input2, "");
		for (;;)
		{
			i = EA_ucGetInputStr(2, 10, 20, EM_BIG_FONT | EM_MODE_CHAR | EM_ALIGN_LEFT | EM_SHOW_ORIGINALLY
								 , 5, 5, 0, (void *)input1); //刘及华，修改只能输入数字和字母
			if ( i == EM_SUCCESS )
				break;
			//if ( i == EM_ABOLISH )
				//return;      do nothing
		}

		lcddisp(2, 1, (void *)"输入车号:           ");

		for (;;)
		{
			i = EA_ucGetInputStr(2, 10, 20, EM_BIG_FONT | EM_MODE_CHAR | EM_ALIGN_LEFT | EM_SHOW_ORIGINALLY
								 , 5, 5, 0, (void *)input2); //刘及华，修改只能输入数字和字母
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
		lcddisp(4, 1, (void *)"修改成功            ");
		EA_ucSetInverse(EM_lcd_INVOFF);
		SleepMs(1500);
	}
}

/******************************************************************************
 函数名称：choose_line
 功能描述：选择线路
 参数描述：
 参数名称：	输入/输出？	类型		描述
				
 返  回  值：无
				   
 作      者	：许岩
 日      期：2012-10-30
 修改历史：
		日期		修改人		修改描述
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
		menus = browse_menu(1, (void *)"    选择线路    ", line_menu, TRUE);
		if ( menus == -1 )
			return;           //取消键返回
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
 函数名称：sell_ticket
 功能描述：售票流程
 参数描述：
 参数名称：	输入/输出？	类型		描述
				
 返  回  值：无
				   
 作      者	：许岩
 日      期：2012-10-30
 修改历史：
		日期		修改人		修改描述
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
//    INT8U disp_mem[4][20];              //显示缓存
//
//    //增加下路确认判断
//    if ( strcmp((void *)&DevStat.line_number[0], "") == 0 )
//    {
//        lcddisperr("线路未选择,请重登录!");
//        return;
//    }
//    //增加员工号确认判断
//    if ( strcmp((void *)&DevStat.op_number[0], "0000") == 0 )
//    {
//        lcddisperr("工号未输入,请重登录!");
//        return;
//    }
//    //增加车号确认判断
//    if ( strcmp((void *)&DevStat.bus_number[0], "12345") == 0 )
//    {
//        lcddisperr("车号未设置,请重登录!");
//        return;
//    }
//    if( Flag_Exticket == 1)
//    {
//        set_ticket_number();
//        Flag_Exticket = 0;
//    }
//
//sell_ticket_step1:
//    //选择行驶方向
//    EA_vCls();
//    EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "      售票      ");
//    sprintf((void *)buf, "线路:%s     班次:%2d", (char *)&DevStat.line_number[0], DevStat.banci);
//    lcddisp(1, 1, (void *)buf);
//    sprintf((void *)buf, "%s", (char *)&DevStat.line_full_name[0]);
//    lcddisp(2, 1, (void *)buf);
//    sprintf((void *)buf, "票号:%07d", DevStat.cur_ticket_number);
//    lcddisp(3, 1, (void *)buf);
//
//    lcddisp(4, 1, (void *)"F1正向    F2反向");
//    //  lcddisp(4, 1, (void *)"F1正向  F2反向  F3班次");
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
//        //          sprintf((void *)buf, "线路:%s     班次:%2d", (char *)&DevStat.line_number[0], DevStat.banci);
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
//        goto sell_ticket_step1;         //按取消键了
//    }
//
//sell_ticket_step3:
//    i = choose_end_station();
//    if ( i != ok )
//    {
//        goto sell_ticket_step2;         //按取消键了
//    }
//
//
//sell_ticket_step4:
//    //选择全价半价
//    EA_vCls();
//    EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "      售票      ");
//    sprintf((void *)buf, "%s-%s", (char *)&DevStat.start_station_name[0], (char *)&DevStat.end_station_name[0]);
//    lcddisp(1, 1, (void *)buf);
//    strcpy((char *)&disp_mem[0][0], (char *)buf);           //保存显示内容
//    //  sprintf((void *)buf, "全价: %.1f元", DevStat.origin_pricef);
//    //  lcddisp(2, 1, (void *)buf);
//    //  DevStat.half_discount = FALSE;
//    DevStat.ticket_type = '1';
//    EA_ucSetInverse(EM_lcd_INVON);
//    strcpy((void *)buf, "                    ");
//    lcddisp(2, 1, (void *)buf);
//    sprintf((void *)buf, "全价: %.1f元", (FP32)DevStat.origin_price / 100.0);
//    lcddisp(2, 1, (void *)buf);
//    EA_ucSetInverse(EM_lcd_INVOFF);
//
//    //  lcddisp(4, 1, (void *)"F1全价    F2半价");
//    lcddisp(4, 1, (void *)"F1全价  F2半价  F3货票");
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
//            sprintf((void *)buf, "全价: %.1f元", (FP32)DevStat.origin_price / 100.0);
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
//            sprintf((void *)buf, "半价: %.1f元", (FP32)DevStat.half_price / 100.0);
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
//            lcddisp(2, 1, (void *)"货票:               ");
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
//            sprintf((void *)buf, "货票: %.2f元", DevStat.package_pricef);
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
//                sprintf((void *)buf, "全价: %.1f元", (FP32)DevStat.origin_price / 100.0);
//                lcddisp(2, 1, (void *)buf);
//                strcpy((char *)&disp_mem[1][0], (char *)buf);           //保存显示内容
////				DevStat.iccard_price = DevStat.origin_price;			//生成刷卡票价
//                sprintf((void *)&DevStat.prices[0], "%.2f", (FP32)DevStat.origin_price / 100.0);
//            }
//            else if ( DevStat.ticket_type == '2' )
//            {
//                strcpy((void *)buf, "                    ");
//                lcddisp(2, 1, (void *)buf);
//                sprintf((void *)buf, "半价: %.1f元", (FP32)DevStat.half_price / 100.0);
//                lcddisp(2, 1, (void *)buf);
//                strcpy((char *)&disp_mem[1][0], (char *)buf);           //保存显示内容
////				DevStat.iccard_price = DevStat.half_price;				//生成刷卡票价
//                sprintf((void *)&DevStat.prices[0], "%.2f", (FP32)DevStat.half_price / 100.0);
//            }
//            else            //货票
//            {
//                strcpy((void *)buf, "                    ");
//                lcddisp(2, 1, (void *)buf);
//                sprintf((void *)buf, "货票: %.2f元", DevStat.package_pricef);
//                lcddisp(2, 1, (void *)buf);
//                strcpy((char *)&disp_mem[1][0], (char *)buf);           //保存显示内容
//                sprintf((void *)&DevStat.prices[0], "%.2f", DevStat.package_pricef);
//            }
//            break;
//        }
//        else if ( key == EM_key_CANCEL )
//        {
//            goto sell_ticket_step3;         //按取消键了
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
//    lcddisp(3, 1, (void *)"张数:");
//    strcpy((void *)input, "");
//    for ( ;; )
//    {
//        i = EA_ucGetInputStr(3, 6, 20, EM_BIG_FONT | EM_MODE_NUMBER | EM_ALIGN_LEFT | EM_SHOW_ORIGINALLY
//                             , 1, 1, 0, (void *)input);
//
//        if ( i == EM_ABOLISH )
//            goto sell_ticket_step4;         //按取消键了
//        if ( i == EM_SUCCESS )
//            break;
//    }
//
//    sscanf((void *)input, "%d", &m);
//    DevStat.ticket_amount = (INT16U)m;
//
//    sprintf((char *)buf, "张数:%d", m);
//    strcpy((char *)&disp_mem[2][0], (char *)buf);           //保存显示内容
//
//
//sell_ticket_step6:
//    //add by ver 0.0.7 刷卡选择
//    lcddisp(4, 1, (void *)"F1现金    F2刷卡    ");
//    memset(&CardInfo, 0x00, sizeof(CardInfo));
//
//    for ( ;; )
//    {
//        key = EA_uiInkey(0);
//        if ( key == EM_key_F1 )   //|| key == EM_key_ENTER
//        {
//            break;      //现金 do nothing
//        }
//        else if ( key == EM_key_F2 )
//        {
//            i = ic_card_process();   //刷卡处理系统
//            if (i != ok)
//            {
//                strcpy((void *)buf, "                    ");
//                lcddisp(1, 1, (void *)buf);
//                lcddisp(2, 1, (void *)buf);
//                lcddisp(3, 1, (void *)buf);
//                lcddisp(1, 1, (void *)&disp_mem[0][0]);
//                lcddisp(2, 1, (void *)&disp_mem[1][0]);
//                lcddisp(3, 1, (void *)&disp_mem[2][0]);
//                goto sell_ticket_step6;         //按取消键了
//            }
//            break;
//        }
//        else if ( key == EM_key_CANCEL )
//        {
//            goto sell_ticket_step5;         //按取消键了
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
//    //先判断是否有纸
//    // 打印设备属于I/O模块的一种，应先获得操作句柄，此时决定打印采用的模式
//    ucRet = EA_ucOpenDevice("PRINT", EM_io_REALMODE, &hDprinterHandle);
//    if ( ucRet != EM_SUCCESS )
//    {
//        lcddisperr("获得打印操作句柄失败!");
//        return;
//    }
//    uint k = 0;
//    for ( j = 0; j < DevStat.ticket_amount; j++ )
//    {
//        sprintf((void *)buf, "正在打印...   第%2d张", j + 1);
//        lcddisp(4, 1, (void *)buf);
//
//        //刘及华调试时候，使用
//        // POS起动时,调用初始化函数初始化打印机芯状态
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
//                lcddisperr("无纸!打印失败!");
//                Flag_Exticket = 1;
//                (void)EA_ucCloseDevice(&hDprinterHandle);
//                return;
//            }
//            else if ( ucRet != EM_SUCCESS )
//            {
//                lcddisperr("初始化打印机芯失败!");
//                (void)EA_ucCloseDevice(&hDprinterHandle);
//                return;
//            }
//        }
//        generate_ticket_info();                 //生成记录
//        store_record(&Tinfo.pos_number[0]);     //储存记录
//        if( strcmp((void *)&Tinfo.price_type[0], "8") == 0 )
//        {
//            goto endbuy;
//        }
//
//        print_ticket(&Tinfo);                   //根据记录打印车票
//
//        DevStat.cur_ticket_number++;
//        if ( DevStat.cur_ticket_number > 9999999 )
//            DevStat.cur_ticket_number = 0;
//        sprintf((void *)&Tinfo.ticket_number[0], "%07d", DevStat.cur_ticket_number);            //票号 7
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
	INT8U disp_mem[4][20];				//显示缓存

	//增加下路确认判断
	if ( strcmp((void *)&DevStat.line_number[0], "") == 0 )
	{
		lcddisperr("线路未选择,请重登录!");
		return;
	}
	//增加员工号确认判断
	if ( strcmp((void *)&DevStat.op_number[0], "0000") == 0 )
	{
		lcddisperr("工号未输入,请重登录!");
		return;
	}
	//增加车号确认判断
	if ( strcmp((void *)&DevStat.bus_number[0], "12345") == 0 )
	{
		lcddisperr("车号未设置,请重登录!");
		return;
	}
	if( Flag_Exticket == 1)
	{
		set_ticket_number();
		Flag_Exticket = 0; 
	}

sell_ticket_step1:
	//选择行驶方向
	EA_vCls();
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "      售票      ");
	sprintf((void *)buf, "%s    班次:%2d", (char *)&DevStat.line_full_name[0], DevStat.banci);
	lcddisp(1, 1, (void *)buf);
	sprintf((void *)buf, "0月票卡            ");
	lcddisp(4, 1, (void *)buf);

	EA_ucSetInverse(EM_lcd_INVON);
	lcddisp(2, 1, (void *)"票价:               ");
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
		lcddisperr("输入金额超过最大票额");
		goto sell_ticket_step1;
	}

	sprintf((void *)buf, "票价: %.2f元", DevStat.origin_pricef);
	lcddisp(2, 1, (void *)buf);
	EA_ucSetInverse(EM_lcd_INVOFF);

	strcpy((char *)&disp_mem[1][0], (char *)buf);			//保存显示内容
	sprintf((void *)&DevStat.prices[0], "%.2f", DevStat.origin_pricef);


//sell_ticket_step5:
	lcddisp(4, 1, (void *)"                      ");
	lcddisp(3, 1, (void *)"张数:");
	strcpy((void *)input, "");
	for ( ;; )
	{
		i = EA_ucGetInputStr(3, 6, 20, EM_BIG_FONT | EM_MODE_NUMBER | EM_ALIGN_LEFT | EM_SHOW_ORIGINALLY
							 , 1, 1, 0, (void *)input);

		if ( i == EM_ABOLISH )
			goto sell_ticket_step1;         //按取消键了
		if ( i == EM_SUCCESS )
			break;
	}

	sscanf((void *)input, "%d", &m);
	DevStat.ticket_amount = (INT16U)m;

	sprintf((char *)buf, "张数:%d", m);
	strcpy((char *)&disp_mem[2][0], (char *)buf);			//保存显示内容


sell_ticket_step6:

	memset(&CardInfo, 0x00, sizeof(CardInfo));

	i = ic_card_process();   //刷卡处理系统
	if (i != ok)
	{
		strcpy((void *)buf, "                    ");
		lcddisp(1, 1, (void *)buf);
		lcddisp(2, 1, (void *)buf);
		lcddisp(3, 1, (void *)buf);
		lcddisp(1, 1, (void *)&disp_mem[0][0]);
		lcddisp(2, 1, (void *)&disp_mem[1][0]);
		lcddisp(3, 1, (void *)&disp_mem[2][0]);
		goto sell_ticket_step1;         //按取消键了
	}


	(void)Get_Time(&ltime);
	memcpy(&DevStat.sell_ticket_time.century, &ltime.century, sizeof(BUS_TIME));
	sprintf((void *)&DevStat.sell_ticket_times[0], "%02X%02X%02X%02X%02X%02X%02X",
			ltime.century, ltime.year, ltime.month, ltime.day, ltime.hour, ltime.minute, ltime.second);

	for ( j = 0; j < DevStat.ticket_amount; j++ )
	{

		generate_ticket_info();                 //生成记录
		store_record(&Tinfo.pos_number[0]);     //储存记录

//  	DevStat.cur_ticket_number++;
//  	if ( DevStat.cur_ticket_number > 9999999 )
//  		DevStat.cur_ticket_number = 0;
//  	sprintf((void *)&Tinfo.ticket_number[0], "%07d", DevStat.cur_ticket_number);            //票号 7

	}

	return;

}
/******************************************************************************
 函数名称：choose_start_station
 功能描述：选择上车站点
 参数描述：
 参数名称：	输入/输出？	类型		描述
				
 返  回  值：ok(0)-成功
				 notok(0xFF)-按取消键了
				   
 作      者	：许岩
 日      期：2012-10-30
 修改历史：
		日期		修改人		修改描述
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

	menus = browse_menu2(1, (void *)"  上车站点选择  ", station_menu, TRUE, DevStat.last_start_station);
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
 函数名称：choose_end_station
 功能描述：选择下车站点
 参数描述：
 参数名称：	输入/输出？	类型		描述
				
 返  回  值：ok(0)-成功
				 notok(0xFF)-按取消键了
				   
 作      者	：许岩
 日      期：2012-10-30
 修改历史：
		日期		修改人		修改描述
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

	menus = browse_menu2(1, (void *)"  下车站点选择  ", station_menu, TRUE, 51);
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
		//往上补成5毛整数倍
		fill = 50 - (DevStat.half_price % 50);
		DevStat.half_price += fill;
	}
	return ok;
}

/******************************************************************************
 函数名称：msystem
 功能描述：菜单->系统设置
 参数描述：
 参数名称：	输入/输出？	类型		描述
				
 返  回  值：无
				   
 作      者	：许岩
 日      期：2012-10-30
 修改历史：
		日期		修改人		修改描述
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
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "    系统设置    ");
	//  sprintf((void *)buf, "编号:%s", DevStat.pos_number);
	//  lcddisp(1, 1, (void *)buf);

	lcddisp(2, 1, (void *)"输入密码:");
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
		lcddisp(4, 1, (void *)"密码错误            ");
		EA_ucSetInverse(EM_lcd_INVOFF);
		(void)EA_uiInkey(1);
		return;
	}



	for (;;)
	{
		menus = browse_menu(1, (void *)"    系统设置    ", setting_menu, TRUE);
		switch ( menus )
		{
		   case -1:             //取消键
			   return;

			   //  	   case 0:              //1.班次设置
			   //  		   set_banci();
			   //  		   break;
			   //
			   //  	   case 1:              //2.票号设置
			   //  		   set_ticket_number();
			   //  		   break;
			   //
			   //  	   case 0:              //1.线路选择
			   //  		   choose_line();
			   //  		   break;
			   //
			   //  	   case 1:              //2.时间设置
			   //  		   TimeModify();
			   //  		   break;
			   //
		   case 0:              //1.时间设置
			   TimeModify();
			   break;

		   case 1:              //2.设备编号设置
			   set_pos_number();
			   break;

		   case 2:              //3.初始化本机
			   set_pos_init();
			   break;

		   case 3:              //4.上传备份记录
			   send_backup_record_process();
			   break;

		   case 4:              //5.设置折扣
			   set_youhui_zhekou();
			   break;

		   default:
			   break;
		}

	}

}

/******************************************************************************
 函数名称：set_banci
 功能描述：设置班次
 参数描述：
 参数名称：	输入/输出？	类型		描述
				
 返  回  值：无
				   
 作      者	：许岩
 日      期：2012-10-30
 修改历史：
		日期		修改人		修改描述
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
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "    班次设置    ");
	sprintf((void *)buf, "班次:%d", DevStat.banci);
	lcddisp(1, 1, (void *)buf);

	lcddisp(2, 1, (void *)"输入新班次:");
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
		lcddisp(4, 1, (void *)"班次不能为0         ");
		EA_ucSetInverse(EM_lcd_INVOFF);
		(void)EA_uiInkey(1);
		goto set_banci_start;
	}

	DevStat.banci = (INT16U)m;

	WriteParam();

	EA_ucSetInverse(EM_lcd_INVON);
	lcddisp(4, 1, (void *)"修改成功            ");
	EA_ucSetInverse(EM_lcd_INVOFF);
	(void)EA_uiInkey(1);

}

/******************************************************************************
 函数名称：set_youhui_zhekou
 功能描述：设置优惠卡折扣
 参数描述：
 参数名称：	输入/输出？	类型		描述
				
 返  回  值：无
				   
 作      者	：刘及华
 日      期：2014-10-19
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void set_youhui_zhekou(void)
{
	INT8U i = 0;
	int tmp = 0;
	INT8U input[22];
	INT8U buf[22];

	EA_vCls();
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "  优惠折扣设置  ");
	sprintf((void *)buf, "原始折扣:%d         ", DevStat.zhekou);
	lcddisp(1, 1, (void *)buf);
	lcddisp(2, 1, (void *)"当前折扣:           ");
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
		lcddisperr("当前折扣设置错误");
		//beeperr();
		return;
	}
	DevStat.zhekou = tmp;

	sprintf((void *)buf, "当前折扣: %d         ", DevStat.zhekou);
	lcddisp(2, 1, (void *)buf);

	WriteParam();

	EA_ucSetInverse(EM_lcd_INVON);
	lcddisp(4, 1, (void *)"      修改成功      ");
	EA_ucSetInverse(EM_lcd_INVOFF);
	(void)EA_uiInkey(1);
}
/******************************************************************************
 函数名称：set_pos_number
 功能描述：设置POS号
 参数描述：
 参数名称：	输入/输出？	类型		描述
				
 返  回  值：无
				   
 作      者	：许岩
 日      期：2012-10-30
 修改历史：
		日期		修改人		修改描述
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
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "  设备编号设置  ");
	sprintf((void *)buf, "编号:%s", DevStat.pos_number);
	lcddisp(1, 1, (void *)buf);

	lcddisp(2, 1, (void *)"输入新编号:");
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
	lcddisp(4, 1, (void *)"修改成功            ");
	EA_ucSetInverse(EM_lcd_INVOFF);
	(void)EA_uiInkey(1);

}

/******************************************************************************
 函数名称：set_pos_init
 功能描述：初始化POS机
 参数描述：
 参数名称：	输入/输出？	类型		描述
				
 返  回  值：无
				   
 作      者	：许岩
 日      期：2012-10-30
 修改历史：
		日期		修改人		修改描述
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
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "   初始化本机   ");
	//  sprintf((void *)buf, "编号:%s", DevStat.pos_number);
	//  lcddisp(1, 1, (void *)buf);
	lcddisp(1, 1, (void *)"所有数据将全被清除!!");

	lcddisp(2, 1, (void *)"输入1来确认:");
	strcpy((void *)input, "");

	i = EA_ucGetInputStr(2, 13, 20, EM_BIG_FONT | EM_MODE_NUMBER | EM_ALIGN_LEFT | EM_SHOW_ORIGINALLY
						 , 1, 1, 0, (void *)input);
	if ( i != EM_SUCCESS )
		return;

	if ( strcmp((void *)input, "1") == 0 )
	{
		EA_ucSetInverse(EM_lcd_INVON);
		lcddisp(4, 1, (void *)"初始化中..          ");
		EA_ucSetInverse(EM_lcd_INVOFF);
		param_factory_default(2);

		EA_ucSetInverse(EM_lcd_INVON);
		lcddisp(4, 1, (void *)"初始化完毕          ");
		EA_ucSetInverse(EM_lcd_INVOFF);
		lcddisp(2, 1, (void *)"    请重新上电!!    ");
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
 函数名称：generate_ticket_info
 功能描述：生成票价记录
 参数描述：
 参数名称：	输入/输出？	类型		描述
				
 返  回  值：无
				   
 作      者	：许岩
 日      期：2012-10-30
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void generate_ticket_info(void)
{
	strcpy((void *)&Tinfo.pos_number[0], (void *)&DevStat.pos_number[0]);                   //机器号 6
	sprintf((void *)&Tinfo.ticket_number[0], "%07d", DevStat.cur_ticket_number);            //票号 7
	strcpy((void *)&Tinfo.ticket_time[0], (void *)&DevStat.sell_ticket_times[0]);           //收费日期时间  14
	strcpy((void *)&Tinfo.op_number[0], (void *)&DevStat.op_number[0]);                     //收费员号  6
	strcpy((void *)&Tinfo.start_station_num[0], (void *)&DevStat.start_station_number[0]);  //起始站编号 4
	strcpy((void *)&Tinfo.start_station_name[0], (void *)&DevStat.start_station_name[0]);   //起始站   14
	strcpy((void *)&Tinfo.end_station_num[0], (void *)&DevStat.end_station_number[0]);      //到达站编号 4
	strcpy((void *)&Tinfo.end_station_name[0], (void *)&DevStat.end_station_name[0]);       //到达站  14
	//  if ( DevStat.half_discount == TRUE )
	//  	strcpy((void *)&Tinfo.price_type[0], "2");                                      //价格类型(全票、半票、3/4票) 1  全票="1" 半票="2"
	//  else
	//  	strcpy((void *)&Tinfo.price_type[0], "1");                                      //价格类型(全票、半票、3/4票) 1  全票="1" 半票="2"
	Tinfo.price_type[0] = DevStat.ticket_type;                                              //价格类型(全票、半票、3/4票) 1  全票="1" 半票="2"  优惠=“7”  月票 =“ 8”
	Tinfo.price_type[1] = '\0';
	strcpy((void *)&Tinfo.price[0], (void *)&DevStat.prices[0]);                            //收费金额 7
	strcpy((void *)&Tinfo.bus_number[0], (void *)&DevStat.bus_number_full[0]);              //车号 9 鲁P-12345
	sprintf((void *)&Tinfo.banci[0], "%d", DevStat.banci);                                  //班次 2
	if ( DevStat.direction == FORWARD )
		strcpy((void *)&Tinfo.direction[0], "1");                                           //方向 1 上行=1 下行=2
	else
		strcpy((void *)&Tinfo.direction[0], "2");                                           //方向 1 上行=1 下行=2
	strcpy((void *)&Tinfo.line_number[0], (void *)&DevStat.line_number[0]);                 //线路编号 5

	//新增的刷卡结果3个字段
	sprintf((void *)&Tinfo.trade_type[0], "%02X", CardInfo.card_purchase_type);				//交易类型

	switch ( CardInfo.card_purchase_type )
	{
		case 0x06:
		case 0x07:
		case 0x11:
			sprintf((void *)&Tinfo.card_number[0], "%02X%02X%02X%02X", 
					CardInfo._CARD_NUMBER.detail_card_number.serial_number[0], 
					CardInfo._CARD_NUMBER.detail_card_number.serial_number[1], 
					CardInfo._CARD_NUMBER.detail_card_number.serial_number[2], 
					CardInfo._CARD_NUMBER.detail_card_number.serial_number[3] );			//卡号
			if ( CardInfo.balance > 99999999 )
				CardInfo.balance = 99999999;
			sprintf((void *)&Tinfo.card_balance[0], "%d", CardInfo.balance);				//卡余额
			break;

		default:
			memset((void *)&Tinfo.card_number[0], 0x00, sizeof(Tinfo.card_number));			//现金无此字段
			memset((void *)&Tinfo.card_balance[0], 0x00, sizeof(Tinfo.card_balance));		//现金无此字段
			break;
	}

}

/******************************************************************************
 函数名称：generate_summary_info
 功能描述：生成票价记录
 参数描述：
 参数名称：	输入/输出？	类型		描述
				
 返  回  值：无
				   
 作      者	：许岩
 日      期：2012-10-30
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void generate_summary_info(TODAY_SUM *pbill)
{
	//BUS_TIME ltime;
//  (void)Get_Time(&ltime);
//  memcpy(&DevStat.sell_ticket_time.century, &ltime.century, sizeof(BUS_TIME));
//  sprintf((void *)&DevStat.sell_ticket_times[0], "%02X%02X%02X%02X%02X%02X%02X",
//  		ltime.century, ltime.year, ltime.month, ltime.day, ltime.hour, ltime.minute, ltime.second);

	strcpy((void *)&Tinfo.pos_number[0], (void *)&DevStat.pos_number[0]);                   //机器号 6
	sprintf((void *)&Tinfo.ticket_number[0], "%07d", DevStat.cur_ticket_number);            //票号 7
	strcpy((void *)&Tinfo.ticket_time[0], (void *)&pbill->query_times[0]);           //收费日期时间  14
	strcpy((void *)&Tinfo.op_number[0], (void *)&DevStat.op_number[0]);                     //收费员号  6
	strcpy((void *)&Tinfo.start_station_num[0], "0000");  //起始站编号 4  固定好是0000四个0
	strcpy((void *)&Tinfo.start_station_name[0], "始发站");   //起始站   14
	strcpy((void *)&Tinfo.end_station_num[0],"0000");      //到达站编号 4  固定好是0000四个0
	strcpy((void *)&Tinfo.end_station_name[0], "终点站");       //到达站  14
//  if ( DevStat.half_discount == TRUE )
//  	strcpy((void *)&Tinfo.price_type[0], "2");                                          //价格类型(全票、半票、3/4票) 1  全票="1" 半票="2"
//  else
//  	strcpy((void *)&Tinfo.price_type[0], "1");                                          //价格类型(全票、半票、3/4票) 1  全票="1" 半票="2"
	Tinfo.price_type[0] = '6';                     //价格类型(全票、半票、3/4票) 1  全票="1" 半票="2",缴费单 = "6", youhui = "7" ,  yuepiao = "8"
	Tinfo.price_type[1] = '\0';
	strcpy((void *)&Tinfo.price[0], "0.0");                            //收费金额 7
	strcpy((void *)&Tinfo.bus_number[0], (void *)&DevStat.bus_number_full[0]);              //车号 9 鲁P-12345
	if(pbill->day_or_banci == 0)
	{
		sprintf((void *)&Tinfo.banci[0], "%d", 7);                                  //班次 2
	}
	else
	{
		sprintf((void *)&Tinfo.banci[0], "%d", pbill->banci);	
	}

	if ( DevStat.direction == FORWARD )
		strcpy((void *)&Tinfo.direction[0], "1");                                           //方向 1 上行=1 下行=2
	else
		strcpy((void *)&Tinfo.direction[0], "2");                                           //方向 1 上行=1 下行=2
	strcpy((void *)&Tinfo.line_number[0], (void *)&DevStat.line_number[0]);                 //线路编号 5

	memset((void *)&Tinfo.trade_type[0], 0x00, sizeof(Tinfo.trade_type));			//交易类型无此字段
	memset((void *)&Tinfo.card_number[0], 0x00, sizeof(Tinfo.card_number));			//结算单无此字段
	memset((void *)&Tinfo.card_balance[0], 0x00, sizeof(Tinfo.card_balance));		//结算单无此字段
}
/******************************************************************************
 函数名称：set_ticket_number
 功能描述：设置票号
 参数描述：
 参数名称：	输入/输出？	类型		描述
				
 返  回  值：无
				   
 作      者	：许岩
 日      期：2012-10-30
 修改历史：
    	日期：2012-12-1		修改人: 刘及华		修改描述：增加两次输入票号，
    	并且判断两次的票号必须一致
    	方可进行下一步操作
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
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "    票号设置    ");
	//  sprintf((void *)buf, "当前票号:%07d", DevStat.cur_ticket_number);
	//  lcddisp(1, 1, (void *)buf);
	//
	//  lcddisp(2, 1, (void *)"输入新号:");
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
	lcddisp(2, 1, (void *)"输入票号:           ");
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
	lcddisp(2, 1, (void *)"输入票号:           ");
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
		//sprintf((void *)buf, "工号:%s %s", admin_num, admin_name);
		lcddisp(4, 1, (void *)"票号设置成功!       ");	
	}
	else
	{
		goto input_again;
	}
	sscanf((void *)input1, "%ld", &m);
	DevStat.cur_ticket_number = (INT32U)m;

	WriteParam();

	EA_ucSetInverse(EM_lcd_INVON);
	lcddisp(4, 1, (void *)"修改成功            ");
	EA_ucSetInverse(EM_lcd_INVOFF);
	(void)EA_uiInkey(1);

}

/******************************************************************************
 函数名称：print_ticket
 功能描述：根据记录打票 
 参数名称：	输入/输出？	类型		描述
 ticket		输入		TICKET_INFO *	车票的记录
 
 返  回  值：无
 
 作      者：许岩
 日      期：2012-10-29
 修改历史：
	日期		修改人		修改描述
	------		---------	-------------
******************************************************************************/
void print_ticket(TICKET_INFO *ticket)
{
	INT8U ucRet = 0;
	FP64 balance=0.0;
	//  DevHandle hDprinterHandle;
	char printbuf[300];

	//  // 打印设备属于I/O模块的一种，应先获得操作句柄，此时决定打印采用的模式
	//  ucRet = EA_ucOpenDevice("PRINT", EM_io_REALMODE, &hDprinterHandle);
	//  if ( ucRet != EM_SUCCESS )
	//  {
	//  	lcddisperr("获得打印操作句柄失败!");
	//  	return;
	//  }

	//    // POS起动时,调用初始化函数初始化打印机芯状态
	//    ucRet =  EA_ucInitPrinter(hDprinterHandle);
	//    if ( ucRet == EM_prn_PAPERENDED )
	//    {
	////  	(void)EA_ucCloseDevice(&hDprinterHandle);
	//        lcddisperr("无纸!打印失败!");
	//        return;
	//    }
	//    else if ( ucRet != EM_SUCCESS )
	//    {
	////  	(void)EA_ucCloseDevice(&hDprinterHandle);
	//        lcddisperr("初始化打印机芯失败!");
	//        return;
	//    }

	// 可进行打印的一些基本设置
	ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_XSPACE, 0);
	if ( ucRet != EM_SUCCESS )
	{
		//  	(void)EA_ucCloseDevice(&hDprinterHandle);
		lcddisperr("打印机设置失败!");
		return;
	}
//  ucRet =  EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_YSPACE, 4);
	ucRet =  EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_YSPACE, 2);
	if ( ucRet != EM_SUCCESS )
	{
		//  	(void)EA_ucCloseDevice(&hDprinterHandle);
		lcddisperr("打印机设置失败!");
		return;
	}

	//  ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_PAGESPACE, 578);
//  ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_PAGESPACE, 577);
	ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_PAGESPACE, 576);
	if ( ucRet != EM_SUCCESS )
	{
		//  	(void)EA_ucCloseDevice(&hDprinterHandle);
		lcddisperr("打印机设置失败!");
		return;
	}

	ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_ASCII_DOTTYPE, EM_prn_ASCIIDOT16X8);
	if ( ucRet != EM_SUCCESS )
	{
		//  	(void)EA_ucCloseDevice(&hDprinterHandle);
		lcddisperr("打印机设置失败!");
		return;
	}

	ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_HZ_DOTTYPE, EM_prn_HZDOT16X16);
	if ( ucRet != EM_SUCCESS )
	{
		//  	(void)EA_ucCloseDevice(&hDprinterHandle);
		lcddisperr("打印机设置失败!");
		return;
	}

	ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_HZ_PRNTYPE, EM_prn_HZPRN2X2);
	if ( ucRet != EM_SUCCESS )
	{
		//  	(void)EA_ucCloseDevice(&hDprinterHandle);
		lcddisperr("打印机设置失败!");
		return;
	}

	ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_PRNGRAY, 8);
	if ( ucRet != EM_SUCCESS )
	{
		//  	(void)EA_ucCloseDevice(&hDprinterHandle);
		lcddisperr("打印机设置失败!");
		return;
	}

	//  ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_ASCII_PRNTYPE, EM_prn_ASCIIPRN2X2);
	//  if ( ucRet != EM_SUCCESS )
	//  {
	//  	lcddisperr("打印机设置失败!");
	//  	return;
	//  }

	// 打印测试
	//  ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "打印测试!\n");
	//  if ( ucRet != EM_SUCCESS )
	//  {
	//  	EA_vDisplay(2, "本行打印失败!");
	//  	return;
	//  }
	//  ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "大连易达通电子技术有限公司1\n");
	//  ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "大连易达通电子技术有23\n");
	//  ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "abcd1234ABCD!@#$\n");
	//  ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "大连易达通电子技术有限公司4\n");


	//  上车站→下车站
	//  全票：1234.0元
	//	2012-11-04 17:06:38
	//  班次： 2		上行
	//  车号：鲁P-12345
	//  售票员号： 1234
	//  票号：  1234567
	//	POS机号：123456

	//  (void)EA_ucLinePrinter(hDprinterHandle, 0, "\n");
	sprintf(printbuf, "%s→%s\n", ticket->start_station_name, ticket->end_station_name);
	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);               //上车站→下车站

//  if ( strcmp((void *)ticket->price_type, "1") == 0 )
//  	sprintf(printbuf, "    全票：%7s元\n", ticket->price);
//  else
//  	sprintf(printbuf, "    半票：%7s元\n", ticket->price);
	if ( strcmp((void *)ticket->price_type, "1") == 0 )
	{
		sprintf(printbuf, "    全票：%7s元\n", ticket->price);
	}
	else if ( strcmp((void *)ticket->price_type, "2") == 0 )
	{
		sprintf(printbuf, "    半票：%7s元\n", ticket->price);
	}
	else if ( strcmp((void *)ticket->price_type, "7") == 0 )   //优惠卡
	{
		sprintf(printbuf, "  优惠票：%7s元\n", ticket->price);
	}
	else if ( strcmp((void *)ticket->price_type, "8") == 0 )   //月票卡
	{
		sprintf(printbuf, "    月票：%7s元\n", ticket->price);
	}
	else
	{
		sprintf(printbuf, "    货票：%7s元\n", ticket->price);
	}

	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);               //全票：1234.00元

	sprintf(printbuf, "%c%c%c%c-%c%c-%c%c %c%c:%c%c:%c%c\n",
			ticket->ticket_time[0], ticket->ticket_time[1], ticket->ticket_time[2], ticket->ticket_time[3],
			ticket->ticket_time[4], ticket->ticket_time[5], ticket->ticket_time[6], ticket->ticket_time[7],
			ticket->ticket_time[8], ticket->ticket_time[9], ticket->ticket_time[10], ticket->ticket_time[11],
			ticket->ticket_time[12], ticket->ticket_time[13]);
	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);               //2012-11-04 17:06:38

	//  if ( strcmp((void *)ticket->direction, "1") == 0 )
	//      sprintf(printbuf, "班次：%2s       上行\n", ticket->banci);
	//  else
	//      sprintf(printbuf, "班次：%2s       下行\n", ticket->banci);
	if ( strcmp((void *)ticket->price_type, "7") == 0 )
	{
		balance = atof((void *)&ticket->card_balance[0]);
		sprintf(printbuf, "卡内余额：%5.2f元\n", balance / 100.00);
		(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);
		sprintf(printbuf, "卡    号：%7s\n", ticket->card_number);
		(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);
	}
//  else if ( strcmp((void *)ticket->price_type, "8" == 0 )
//  {
//  	sprintf(printbuf, "截止日期：%2s %5s \n", ticket->banci, ticket->line_number);
//  	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);
//  	sprintf(printbuf, "卡    号：%7s\n", ticket->card_number);
//  	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);
//  }

	if ( strcmp((void *)ticket->direction, "1") == 0 )
		sprintf(printbuf, "班次：%2s %5s 上行\n", ticket->banci, ticket->line_number);
	else
		sprintf(printbuf, "班次：%2s %5s 下行\n", ticket->banci, ticket->line_number);
	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);               //班次： 2  @406 上行

	sprintf(printbuf, "车号：%s\n", ticket->bus_number);
	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);               //车号：鲁P-12345

	sprintf(printbuf, "售票员号： %s\n", ticket->op_number);
	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);               //售票员号：1234

	sprintf(printbuf, "票号：  %s\n", ticket->ticket_number);
	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);               //票号：1234567

	sprintf(printbuf, "POS机号：%s\n", ticket->pos_number);
	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);               //POS机号：123456


	// 打印最后调用换页命令
	ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "\f");
	if ( ucRet == EM_SUCCESS )
	{
		EA_vDisplay(2, "本次打印结束!");
	}

	//  (void)EA_ucCloseDevice(&hDprinterHandle);

}

/******************************************************************************
 函数名称：query_capacity
 功能描述：查询记录空间
 参数描述：
 参数名称：	输入/输出？	类型		描述
				
 返  回  值：无
				   
 作      者	：许岩
 日      期：2012-10-30
 修改历史：
		日期		修改人		修改描述
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


	// 打开文件
	if ( EA_ucPFOpen((uchar *)recFileName, &ucOpenID) != EM_ffs_SUCCESS )
	{
		lcddisperr("打开记录文件失败");
		return;
	}


	EA_vCls();
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "  记录容量查询  ");

	(void)EA_ucPFInfo(ucOpenID, EM_ffs_VALIDREC, &uiResult);
	sprintf((void *)buf, "当前记录数:%d", uiResult);
	lcddisp(1, 1, (void *)buf);

	(void)EA_ucPFInfo(ucOpenID, EM_ffs_FREEREC, &uiResult);
	sprintf((void *)buf, "剩余记录数:%d", uiResult);
	lcddisp(2, 1, (void *)buf);

	if ( EA_ucPFClose(ucOpenID) != EM_ffs_SUCCESS )
	{
		lcddisperr("关闭记录文件失败");
		return;
	}

	(void)EA_uiInkey(0);

}

///******************************************************************************
// 函数名称：query_today_summary
// 功能描述：查询今日汇总
// 参数描述：
// 参数名称： 输入/输出？ 类型        描述
//
// 返  回  值：无
//
// 作      者 ：许岩
// 日      期：2012-10-30
// 修改历史：
//        日期        修改人      修改描述
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
//    //  INT32U full_num = 0;        //全价票张数
//    //  INT32U full_sum = 0;        //全价票总额 单位分
//    //  INT32U half_num = 0;        //半价票张数
//    //  INT32U half_sum = 0;        //半价票总额 单位分
//    FP32 fprice = 0.0;
//    INT32U price = 0;
//    struct _TODAY_SUM
//    {
//        INT32U full_num;        //全价票张数
//        INT32U full_sum;        //全价票总额 单位分
//        INT32U half_num;        //半价票张数
//        INT32U half_sum;        //半价票总额 单位分
//        INT32U total_num;       //总张数
//        INT32U total_sum;       //总额
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
//    //      menus = browse_menu(1, (void *)"    选择线路    ", line_menu, FALSE);
//    //      if ( menus == -1 )
//    //          continue;           //取消键继续
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
//    EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "    今日统计    ");
//    lcddisp(2, 1, (void *)"      统计中...");
//
//
//    // 打开文件
//    if ( EA_ucPFOpen((uchar *)recFileName, &ucOpenID) != EM_ffs_SUCCESS )
//    {
//        lcddisperr("打开记录文件失败");
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
//            break;          //不是同一天，退出
//        }
//
//        sscanf((void *)&ticket.banci[0], "%d", &m);
//        cur_banci = (INT32U)m;
//        if ( cur_banci > max_banci )
//            max_banci = cur_banci;
//
//        if ( strcmp((void *)&ticket.price_type[0], "1") == 0 )
//        {
//            //全价票
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
//            //半价票
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
//            //废票不计算
//        }
//
//    }
//
//    if ( EA_ucPFClose(ucOpenID) != EM_ffs_SUCCESS )
//    {
//        lcddisperr("关闭记录文件失败");
//        return;
//    }
//
//    //生成菜单
//    strcpy((void *)sum_menu, "今日汇总            ");
//    for ( j = 1; j <= max_banci; j++ )
//    {
//        sprintf((void *)buf, "班次%2d              ", j);
//        strcat((void *)sum_menu, (void *)buf);
//    }
//
//
//    for (;;)
//    {
//        menus = browse_menu(1, (void *)"    今日统计    ", sum_menu, TRUE);
//        if ( menus == -1 )                //退出
//        {
//            return;
//        }
//        else if ( menus == 0 )             //今日汇总
//        {
//            EA_vCls();
//            EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "    今日汇总    ");
//            sprintf((void *)buf, "全票%4d张%8.1f元", today.full_num, (FP32)today.full_sum / 100.0);
//            lcddisp(1, 1, (void *)buf);
//            sprintf((void *)buf, "半票%4d张%8.1f元", today.half_num, (FP32)today.half_sum / 100.0);
//            lcddisp(2, 1, (void *)buf);
//            sprintf((void *)buf, "总计%4d张%8.1f元", today.total_num, (FP32)today.total_sum / 100.0);
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
//            sprintf((void *)buf, "   班次%2d   ", menus);
//            EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, (void *)buf);
//            sprintf((void *)buf, "全票%4d张%8.1f元", banci[menus].full_num, (FP32)banci[menus].full_sum / 100.0);
//            lcddisp(1, 1, (void *)buf);
//            sprintf((void *)buf, "半票%4d张%8.1f元", banci[menus].half_num, (FP32)banci[menus].half_sum / 100.0);
//            lcddisp(2, 1, (void *)buf);
//            sprintf((void *)buf, "总计%4d张%8.1f元", banci[menus].total_num, (FP32)banci[menus].total_sum / 100.0);
//            lcddisp(4, 1, (void *)buf);
//
//            (void)EA_uiInkeyMs(0);
//        }
//
//
//    }
//}


/******************************************************************************
 函数名称：query_today_summary
 功能描述：查询今日汇总
 参数描述：
 参数名称：	输入/输出？	类型		描述
				
 返  回  值：无
				   
 作      者	：许岩
 日      期：2012-10-30
 修改历史：
		日期		修改人		修改描述
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
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "    今日统计    ");
	lcddisp(2, 1, (void *)"      统计中...");


	// 打开文件
	if ( EA_ucPFOpen((uchar *)recFileName, &ucOpenID) != EM_ffs_SUCCESS )
	{
		lcddisperr("打开记录文件失败");
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
	today.youhui_num = 0;      //优惠卡张数
	today.youhui_sum = 0;      //优惠卡总额 单位分
	today.yuepiao_num = 0;     //月票卡张数
	today.yuepiao_sum = 0;     //月票卡总额 单位分
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
			break;          //不是同一天，退出
		}

		sscanf((void *)&ticket.banci[0], "%d", &m);
		cur_banci = (INT32U)m;
		if ( cur_banci > max_banci )
			max_banci = cur_banci;

		if ( strcmp((void *)&ticket.price_type[0], "1") == 0 )
		{
			//全价票
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
			//半价票
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
			//货票
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
//  		//废票
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
			//优惠票
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
			//月票
			banci[cur_banci].yuepiao_num++;
			today.yuepiao_num++;
    	}

		else
		{
			//货票不计算
		}

	}

	if ( EA_ucPFClose(ucOpenID) != EM_ffs_SUCCESS )
	{
		lcddisperr("关闭记录文件失败");
		return;
	}

	//生成菜单
	strcpy((void *)sum_menu, "今日汇总            ");
	for ( j = 1; j <= max_banci; j++ )
	{
		sprintf((void *)buf, "班次%2d              ", j);
		strcat((void *)sum_menu, (void *)buf);
	}


	for (;;)
	{
		menus = browse_menu(1, (void *)"    今日统计    ", sum_menu, TRUE);
		if ( menus == -1 )                //退出
		{
			return;
		}
		else if ( menus == 0 )            //今日汇总
		{
			EA_vCls();
//  		EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "    今日汇总    ");
//  		sprintf((void *)buf, "全票%4d张%8.1f元", today.full_num, (FP32)today.full_sum / 100.0);
//  		lcddisp(1, 1, (void *)buf);
//  		sprintf((void *)buf, "半票%4d张%8.1f元", today.half_num, (FP32)today.half_sum / 100.0);
//  		lcddisp(2, 1, (void *)buf);
			sprintf((void *)buf, "优惠%4d张%8.1f元", today.youhui_num, (FP32)today.youhui_sum / 100.0);
			lcddisp(1, 1, (void *)buf);
//  		sprintf((void *)buf, "货票%4d张%8.1f元", today.package_num, (FP32)today.package_sum / 100.0);
//  		lcddisp(2, 1, (void *)buf);
			sprintf((void *)buf, "月票%4d张%8.1f元", today.yuepiao_num, (FP32)today.yuepiao_sum / 100.0);
			lcddisp(2, 1, (void *)buf);
			sprintf((void *)buf, "总计%3d张%5.1f元", today.total_num, (FP32)today.total_sum / 100.0);
			EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, (void *)buf);


			//(void)EA_uiInkey(0);
			key = EA_uiInkey(0);
			if ( key == EM_key_ENTER )
			{
				EA_vCls();
				EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "是否确定打印？  ");
				EA_vDisp(4, 1, "确定-打印  其他-取消");
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
//  		sprintf((void *)buf, "   班次%2d   ", menus);
//  		EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, (void *)buf);
			sprintf((void *)buf, "全票%4d张%8.1f元", banci[menus].full_num, (FP32)banci[menus].full_sum / 100.0);
			lcddisp(1, 1, (void *)buf);
			sprintf((void *)buf, "半票%4d张%8.1f元", banci[menus].half_num, (FP32)banci[menus].half_sum / 100.0);
			lcddisp(2, 1, (void *)buf);
			sprintf((void *)buf, "优惠%4d张%8.1f元", today.youhui_num, (FP32)today.youhui_sum / 100.0);
			lcddisp(3, 1, (void *)buf);
//  		sprintf((void *)buf, "废票%4d张%8.1f元", banci[menus].valid_num, (FP32)banci[menus].valid_sum / 100.0);
//  		lcddisp(3, 1, (void *)buf);
			sprintf((void *)buf, "货票%4d张%8.1f元", banci[menus].package_num, (FP32)banci[menus].package_sum / 100.0);
			lcddisp(4, 1, (void *)buf);
			sprintf((void *)buf, "总计%3d张%5.1f元", banci[menus].total_num, (FP32)banci[menus].total_sum / 100.0);
//  		sprintf((void *)buf, "总计%3d张%5.1f元", today.total_num, (FP32)today.total_sum / 100.0);
			EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, (void *)buf);
//  		lcddisp(4, 1, (void *)buf);

			//(void)EA_uiInkey(0);
			key = EA_uiInkey(0);
			if ( key == EM_key_ENTER )
			{
				EA_vCls();
				EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "是否确定打印？  ");
				EA_vDisp(4, 1, "确定-打印  其他-取消");
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
 函数名称：print_sum
 功能描述：打印统计数据
 参数名称：	输入/输出？	类型		描述
 pSummary		输入		TODAY_SUM *	统计的数据，包含今日全天，和每个班次的
 
 返  回  值：无
 
 作      者：许岩/刘及华
 日      期：2012-12-02
 修改历史：
	日期		修改人		修改描述
	------		---------	-------------
******************************************************************************/
void print_summary(TODAY_SUM *pSummary)
{
	INT8U ucRet = 0;
	//  DevHandle hDprinterHandle;
	char printbuf[300];
	char buf[300];
	BUS_TIME ltime;

	// 打印设备属于I/O模块的一种，应先获得操作句柄，此时决定打印采用的模式
	ucRet = EA_ucOpenDevice("PRINT", EM_io_REALMODE, &hDprinterHandle);
	if ( ucRet != EM_SUCCESS )
	{
		lcddisperr("获得打印操作句柄失败!");
		return;
	}

	  // POS起动时,调用初始化函数初始化打印机芯状态
	  ucRet =  EA_ucInitPrinter(hDprinterHandle);
	  if ( ucRet == EM_prn_PAPERENDED )
	  {
          (void)EA_ucCloseDevice(&hDprinterHandle);
		  lcddisperr("无纸!打印失败!");
		  return;
	  }
	  else if ( ucRet != EM_SUCCESS )
	  {
    	  (void)EA_ucCloseDevice(&hDprinterHandle);
		  lcddisperr("初始化打印机芯失败!");
		  return;
	  }

	// 可进行打印的一些基本设置
	ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_XSPACE, 0);
	if ( ucRet != EM_SUCCESS )
	{
		//  	(void)EA_ucCloseDevice(&hDprinterHandle);
		lcddisperr("打印机设置失败!");
		return;
	}
//  ucRet =  EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_YSPACE, 4);
	ucRet =  EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_YSPACE, 2);
	if ( ucRet != EM_SUCCESS )
	{
		//  	(void)EA_ucCloseDevice(&hDprinterHandle);
		lcddisperr("打印机设置失败!");
		return;
	}

	//  ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_PAGESPACE, 578);
//  ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_PAGESPACE, 577);
	ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_PAGESPACE, 576);
	if ( ucRet != EM_SUCCESS )
	{
		//  	(void)EA_ucCloseDevice(&hDprinterHandle);
		lcddisperr("打印机设置失败!");
		return;
	}

	ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_ASCII_DOTTYPE, EM_prn_ASCIIDOT16X8);
	if ( ucRet != EM_SUCCESS )
	{
		//  	(void)EA_ucCloseDevice(&hDprinterHandle);
		lcddisperr("打印机设置失败!");
		return;
	}

	ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_HZ_DOTTYPE, EM_prn_HZDOT16X16);
	if ( ucRet != EM_SUCCESS )
	{
		//  	(void)EA_ucCloseDevice(&hDprinterHandle);
		lcddisperr("打印机设置失败!");
		return;
	}

	ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_HZ_PRNTYPE, EM_prn_HZPRN2X2);
	if ( ucRet != EM_SUCCESS )
	{
		//  	(void)EA_ucCloseDevice(&hDprinterHandle);
		lcddisperr("打印机设置失败!");
		return;
	}


	//  ucRet = EA_ucSetPrinterFormat(hDprinterHandle, EM_prn_ASCII_PRNTYPE, EM_prn_ASCIIPRN2X2);
	//  if ( ucRet != EM_SUCCESS )
	//  {
	//  	lcddisperr("打印机设置失败!");
	//  	return;
	//  }

	// 打印测试
	//  ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "打印测试!\n");
	//  if ( ucRet != EM_SUCCESS )
	//  {
	//  	EA_vDisplay(2, "本行打印失败!");
	//  	return;
	//  }
	//  ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "大连易达通电子技术有限公司1\n");
	//  ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "大连易达通电子技术有23\n");
	//  ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "abcd1234ABCD!@#$\n");
	//  ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "大连易达通电子技术有限公司4\n");

	//全天的票据
	//  全票  5张  45.0元
	//  半票  4张  10.0元
	// 	货票  9张   9.0元
	//  总计  18张  65.00元
	//	2012-11-04 17:06:38
	//  全天  车号：鲁P-12345		
	//  售票员号： 1234
	//	POS机号：123456

	//单趟的票据
	//  全票  5张  45.0元
	//  半票  4张  10.0元
	// 	货票  9张   9.0元
	//  总计  18张  65.00元
	//	2012-11-04 17:06:38
	//  班次 2 车号：鲁P-12345		
	//  售票员号： 1234
	//	POS机号：123456
	EA_vCls();
	EA_vDisp(4, 1, "正在打印......      ");
	sprintf(printbuf, "全票%4d张%6.1f元\n", pSummary->full_num, (FP32)pSummary->full_sum / 100.0);
	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf); 
	sprintf(printbuf, "半票%4d张%6.1f元\n", pSummary->half_num, (FP32)pSummary->half_sum / 100.0);
	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf); 
	sprintf(printbuf, "货票%4d张%6.1f元\n", pSummary->package_num, (FP32)pSummary->package_sum / 100.0);
	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf); 
	sprintf(printbuf, "优惠%4d张%6.1f元\n", pSummary->youhui_num, (FP32)pSummary->youhui_sum / 100.0);
	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf); 
	sprintf(printbuf, "月票%4d次\n", pSummary->yuepiao_num);
	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf); 
	sprintf(printbuf, "总计%4d张%6.1f元\n", pSummary->total_num, (FP32)pSummary->total_sum / 100.0);
	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);    
	           //票据统计
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
	//      sprintf(printbuf, "班次：%2s       上行\n", ticket->banci);
	//  else
	//      sprintf(printbuf, "班次：%2s       下行\n", ticket->banci);
	if ( pSummary->day_or_banci == 0 )
		sprintf(printbuf, "全天    ");
	else
		sprintf(printbuf, "班次:%2d   ", pSummary->banci);
	//(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);               //班次： 2  @406 上行
	sprintf(buf, "%s\n", DevStat.bus_number_full);
	strcat((void *)printbuf, (void *)buf);
	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);               //车号：鲁P-12345

	sprintf(printbuf, "售票员号： %s\n", DevStat.op_number);
	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);               //售票员号：1234

//  sprintf(printbuf, "票号：  %s\n", pTicket->ticket_number);
//  (void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);               //票号：1234567

	sprintf(printbuf, "POS机号：%s\n", DevStat.pos_number);
	(void)EA_ucLinePrinter(hDprinterHandle, 0, printbuf);               //POS机号：123456


	// 打印最后调用换页命令
	ucRet = EA_ucLinePrinter(hDprinterHandle, 0, "\f");
	if ( ucRet == EM_SUCCESS )
	{
		EA_vDisplay(2, "本次打印结束!");
		EA_vDelay(1);		
	}

	generate_summary_info((void *)pSummary);                 //生成记录
	store_record(&Tinfo.pos_number[0]);     //储存记录

	DevStat.cur_ticket_number++;
	if ( DevStat.cur_ticket_number > 9999999 )
		DevStat.cur_ticket_number = 0;
	sprintf((void *)&Tinfo.ticket_number[0], "%07d", DevStat.cur_ticket_number);            //票号 7

	(void)EA_ucCloseDevice(&hDprinterHandle);

}

/******************************************************************************
 函数名称：mdata
 功能描述：菜单->数据处理
 参数描述：
 参数名称：	输入/输出？	类型		描述
				
 返  回  值：无
				   
 作      者	：许岩
 日      期：2012-10-30
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void mdata(void)
{
	INT8S menus = 0;


	for (;;)
	{
		menus = browse_menu(1, (void *)"    数据处理    ", data_menu, TRUE);
		switch ( menus )
		{
		   case -1:             //取消键
			   return;

		   case 0:              //1.废票
			   invalid_ticket_process();
			   break;

		   case 1:              //2.记录和参数传输

			   send_record_process();
			   break;

			   //  	   case 2:              //3.下载参数
			   //  		   set_banci();
			   //  		   break;

		   default:
			   break;
		}

	}





}

///******************************************************************************
// 函数名称：invalid_ticket_process
// 功能描述：废票处理
// 参数描述：
// 参数名称： 输入/输出？ 类型        描述
//
// 返  回  值：无
//
// 作      者 ：许岩
// 日      期：2012-10-30
// 修改历史：
//        日期        修改人      修改描述
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
//    // 打开文件
//    if ( EA_ucPFOpen((uchar *)recFileName, &ucOpenID) != EM_ffs_SUCCESS )
//    {
//        lcddisperr("打开记录文件失败");
//        return;
//    }
//
//    (void)EA_ucPFInfo(ucOpenID, EM_ffs_VALIDREC, &uiResult);
//    total_rec_num = uiResult;
//
//    if ( total_rec_num == 0 )
//    {
//        lcddisperr("没有记录!");
//        (void)EA_ucPFClose(ucOpenID);
//        return;
//    }
//
//    (void)EA_ucPFReadRec(ucOpenID, total_rec_num, 1, &uiReadCnt, &ticket.pos_number[0]);
//
//    if ( EA_ucPFClose(ucOpenID) != EM_ffs_SUCCESS )
//    {
//        lcddisperr("关闭记录文件失败");
//        return;
//    }
//
//    //显示最后一笔售票记录
//    EA_vCls();
//    EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "      废票      ");
//    sprintf((void *)buf, "票号:%s", ticket.ticket_number);
//    lcddisp(1, 1, buf);
//    sprintf((void *)buf, "%c%c%c%c-%c%c-%c%c %c%c:%c%c:%c%c",
//            ticket.ticket_time[0], ticket.ticket_time[1], ticket.ticket_time[2], ticket.ticket_time[3],
//            ticket.ticket_time[4], ticket.ticket_time[5], ticket.ticket_time[6], ticket.ticket_time[7],
//            ticket.ticket_time[8], ticket.ticket_time[9], ticket.ticket_time[10], ticket.ticket_time[11],
//            ticket.ticket_time[12], ticket.ticket_time[13]);
//    lcddisp(2, 1, buf);
//    sprintf((void *)buf, "%s→%s\n", ticket.start_station_name, ticket.end_station_name);
//    lcddisp(3, 1, buf);
//    if ( strcmp((void *)ticket.price_type, "1") == 0 )
//    {
//        sprintf((void *)buf, "全票：%s元\n", ticket.price);
//    }
//    else if ( strcmp((void *)ticket.price_type, "2") == 0 )
//    {
//        sprintf((void *)buf, "半票：%s元\n", ticket.price);
//    }
//    else if ( strcmp((void *)ticket.price_type, "3") == 0 )
//    {
//        sprintf((void *)buf, "货票：%s元\n", ticket.price);
//    }
//    else
//    {
//        sprintf((void *)buf, "废票：%s元\n", ticket.price);
//    }
//    lcddisp(4, 1, buf);
//
//    (void)EA_uiInkeyMs(0);
//
//    menus = browse_menu(1, (void *)"确认要作为废票? ", confirm_menu, TRUE);
//    if ( menus == 0 )
//    {
//        strcpy((void *)&ticket.price_type[0], "4");
//        store_record(&ticket.pos_number[0]);     //储存记录
//        EA_ucSetInverse(EM_lcd_INVON);
//        lcddisp(4, 1, (void *)"处理完毕            ");
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
 函数名称：invalid_ticket_process
 功能描述：废票处理
 参数描述：
 参数名称：	输入/输出？	类型		描述
				
 返  回  值：无
				   
 作      者	：许岩
 日      期：2012-10-30
 修改历史：
		日期		修改人		修改描述
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


	// 打开文件
	if ( EA_ucPFOpen((uchar *)recFileName, &ucOpenID) != EM_ffs_SUCCESS )
	{
		lcddisperr("打开记录文件失败");
		return;
	}

	(void)EA_ucPFInfo(ucOpenID, EM_ffs_VALIDREC, &uiResult);
	total_rec_num = uiResult;

	if ( total_rec_num == 0 )
	{
		lcddisperr("没有记录!");
		(void)EA_ucPFClose(ucOpenID);
		return;
	}

	(void)EA_ucPFReadRec(ucOpenID, total_rec_num, 1, &uiReadCnt, &ticket.pos_number[0]);

	if ( EA_ucPFClose(ucOpenID) != EM_ffs_SUCCESS )
	{
		lcddisperr("关闭记录文件失败");
		return;
	}

	//显示最后一笔售票记录
	EA_vCls();
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "      废票      ");
	sprintf((void *)buf, "票号:%s", ticket.ticket_number);
	lcddisp(1, 1, buf);
	sprintf((void *)buf, "%c%c%c%c-%c%c-%c%c %c%c:%c%c:%c%c",
			ticket.ticket_time[0], ticket.ticket_time[1], ticket.ticket_time[2], ticket.ticket_time[3],
			ticket.ticket_time[4], ticket.ticket_time[5], ticket.ticket_time[6], ticket.ticket_time[7],
			ticket.ticket_time[8], ticket.ticket_time[9], ticket.ticket_time[10], ticket.ticket_time[11],
			ticket.ticket_time[12], ticket.ticket_time[13]);
	lcddisp(2, 1, buf);
	sprintf((void *)buf, "%s→%s\n", ticket.start_station_name, ticket.end_station_name);
	lcddisp(3, 1, buf);
	if ( strcmp((void *)ticket.price_type, "1") == 0 )
	{
		sprintf((void *)buf, "全票：%s元\n", ticket.price);
	}
	else if ( strcmp((void *)ticket.price_type, "2") == 0 )
	{
		sprintf((void *)buf, "半票：%s元\n", ticket.price);
	}
	else if ( strcmp((void *)ticket.price_type, "3") == 0 )
	{
		sprintf((void *)buf, "货票：%s元\n", ticket.price);
	}
	else
	{
		sprintf((void *)buf, "废票：%s元\n", ticket.price);
	}
	lcddisp(4, 1, buf);

	(void)EA_uiInkey(0);

	menus = browse_menu(1, (void *)"确认要作为废票? ", confirm_menu, TRUE);
	if ( menus == 0 )
	{
		strcpy((void *)&ticket.price_type[0], "4");
		store_record(&ticket.pos_number[0]);     //储存记录
		EA_ucSetInverse(EM_lcd_INVON);
		lcddisp(4, 1, (void *)"处理完毕            ");
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
 函数名称：send_record_process
 功能描述：上传售票记录处理
 参数描述：
 参数名称：	输入/输出？	类型		描述
				
 返  回  值：无
				   
 作      者	：许岩
 日      期：2012-10-30
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void send_record_process(void)
{
	INT8U ucResult = 0;
	//  DevHandle hRS232Handle;
	//  INT8U buf[30];

	EA_vCls();
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "  上传售票记录  ");
	//  lcddisp(2, 1, (void *)"      通讯中..      ");
	lcddisp(2, 1, (void *)"    数据处理中..    ");
	generate_record_file();
	lcddisp(2, 1, (void *)"      通讯中..      ");

	//  打开RS232设备
	ucResult = EA_ucOpenDevice("COM2", EM_io_RDWR, &hRS232Handle);
	if ( ucResult != EM_SUCCESS )
	{
		lcddisperr("打开串口失败");
		return;
	}

	ucResult = EA_ucInitRS232(hRS232Handle, EM_232_115200BPS, EM_232_NOPAR, EM_232_8DBS);
	if ( ucResult != EM_SUCCESS )
	{
		lcddisperr("串口初始化失败");
		(void)EA_ucCloseDevice(&hRS232Handle);
		return;
	}


	//  strcpy((void *)buf, "0123456789ABCDEF");
	//  (void)EA_ucWriteDevice(hRS232Handle, 16, 10, buf);


	communication();



	ucResult = EA_ucCloseDevice(&hRS232Handle);
	if ( ucResult != EM_SUCCESS )
	{
		lcddisperr("打开串口失败");
	}

}

/******************************************************************************
 函数名称：generate_record_file
 功能描述：生成记录文件
 参数描述：
 参数名称：	输入/输出？	类型		描述
				
 返  回  值：无
				   
 作      者	：许岩
 日      期：2012-10-30
 修改历史：
		日期		修改人		修改描述
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


	//  //先删除文件
	//  (void)EA_iCfsDelete("rec.txt");

	fHandle = EA_iCfsOpen("rec.txt", "w+");
	if ( fHandle == NULL )
	{
		lcddisperr("创建rec.txt失败");
		return;
	}

	// 打开文件
	if ( EA_ucPFOpen((uchar *)recFileName, &ucOpenID) != EM_ffs_SUCCESS )
	{
		lcddisperr("打开记录文件失败");
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

		iWriteSize = EA_i64CfsWrite(fHandle, buf, strlen((void *)buf));         //写入文件
		if ( iWriteSize < 0 )
		{
			lcddisperr("写rec.txt失败");
			(void)EA_ucPFClose(ucOpenID);
			(void)EA_iCfsClose(fHandle);
			return;
		}

	}

	i = EA_iCfsFsync(fHandle);
	if ( i < 0 )
	{
		lcddisperr("同步rec.txt失败");
		(void)EA_ucPFClose(ucOpenID);
		(void)EA_iCfsClose(fHandle);
		return;
	}

	if ( EA_ucPFClose(ucOpenID) != EM_ffs_SUCCESS )
	{
		lcddisperr("关闭记录文件失败");
		(void)EA_iCfsClose(fHandle);
		return;
	}

	i = EA_iCfsClose(fHandle);
	if ( i < 0 )
	{
		lcddisperr("关闭rec.txt失败");
		return;
	}


}

/*****************************************************************
 函数原型：read_param_table()
 功能描述：读取参数表
 参数描述：	
				 
 返回值：	无
 
 作  者：	许岩
 日  期：	2004-09-17
 修改历史：
		日期		修改人		修改描述
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
		lcddisperr("读取parm.txt文件失败");
		return;
	}
	file_len = EA_i64CfsGetFileSize(fHandle);
	if ( file_len < 0 )
	{
		lcddisperr("读取参数文件长度失败");
		(void)EA_iCfsClose(fHandle);
		return;
	}

	//动态分配空间
	pfile = (char *)EA_pvMalloc(file_len * sizeof(char) + 1);
	if ( pfile == NULL )
	{
		lcddisperr("动态分配空间错误");
		(void)EA_iCfsClose(fHandle);
		return;
	}

	//读取全部文件
	iReadSize = EA_i64CfsRead(fHandle, pfile, file_len);
	if ( iReadSize < file_len )
	{
		lcddisperr("读取文件内容失败");
		(void)EA_iCfsClose(fHandle);
		return;
	}
	(void)EA_iCfsClose(fHandle);

	//  p = pfile;

	//取出[param]段
	p = strstr(pfile, "[param]");
	sscanf(p, "%[^\x00d^\x00a]", line_buf); //得到第 0 行//遇到0D0A结束
	p = p + strlen(line_buf) + 2;

	sscanf(p, "%[^\x00d^\x00a]", line_buf); //得到第 1 行//遇到0D0A结束
	sscanf(line_buf, "version=%s", DevStat.version);
	p = p + strlen(line_buf) + 2;

	sscanf(p, "%[^\x00d^\x00a]", line_buf); //得到第 2 行//遇到0D0A结束
	sscanf(line_buf, "operator_amount=%d", &DevStat.operator_amount);
	p = p + strlen(line_buf) + 2;

	sscanf(p, "%[^\x00d^\x00a]", line_buf); //得到第 3 行//遇到0D0A结束
	sscanf(line_buf, "line_amout=%d", &DevStat.line_amount);

	//动态分配内存，初始化变量
	EA_vFree(OPinfo);
	OPinfo = (OPERATOR_INFO *)EA_pvMalloc(DevStat.operator_amount * sizeof(OPERATOR_INFO) + 1);
	if ( OPinfo == NULL )
	{
		lcddisperr("动态分配空间错误");
	}
	EA_vFree(Linfo);
	Linfo = (LINE_INFO *)EA_pvMalloc(DevStat.line_amount * sizeof(LINE_INFO) + 1);
	if ( Linfo == NULL )
	{
		lcddisperr("动态分配空间错误");
	}
	EA_vFree(STinfo);
	STinfo = (STATION_INFO *)EA_pvMalloc(DevStat.line_amount * sizeof(STATION_INFO) + 1);
	if ( STinfo == NULL )
	{
		lcddisperr("动态分配空间错误");
	}
	memset((void *)OPinfo, 0x00, DevStat.operator_amount * sizeof(OPERATOR_INFO));
	memset((void *)Linfo, 0x00, DevStat.line_amount * sizeof(LINE_INFO));
	memset((void *)STinfo, 0x00, DevStat.line_amount * sizeof(STATION_INFO));


	//取出[operator_info]段
	p = strstr(pfile, "[operator_info]");
	sscanf(p, "%[^\x00d^\x00a]", line_buf);     //取到行尾
	p = p + strlen(line_buf) + 2;
	for ( j = 0; j < DevStat.operator_amount; j++ )
	{
		sscanf(p, "%[^\x00d^\x00a]", line_buf);     //取到行尾
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


	//取出[line_info]段
	p = strstr(pfile, "[line_info]");
	sscanf(p, "%[^\x00d^\x00a]", line_buf);     //取到行尾
	p = p + strlen(line_buf) + 2;
	for ( j = 0; j < DevStat.line_amount; j++ )
	{
		sscanf(p, "%[^\x00d^\x00a]", line_buf);     //取到行尾
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


	//取出票价表
	for ( j = 0; j < DevStat.line_amount; j++ )
	{
		sprintf(search_buf, "[stations_of_line_%d]", j);
		p = strstr(pfile, search_buf);
		if ( p == NULL )
			continue;
		sscanf(p, "%[^\x00d^\x00a]", line_buf);     //取到行尾
		p = p + strlen(line_buf) + 2;

		sscanf(p, "%[^\x00d^\x00a]", line_buf);     //取到行尾
		sscanf(line_buf, "%d", &STinfo[j].forward_station_num);
		p = p + strlen(line_buf) + 2;

		sscanf(p, "%[^\x00d^\x00a]", line_buf);     //取到行尾
		sscanf(line_buf, "%d", &STinfo[j].backward_station_num);
		p = p + strlen(line_buf) + 2;

		//  	//add for debug
		//  	sprintf((void *)buf, "%d",STinfo[j].forward_station_num);
		//  	lcddisperr((void *)buf);
		//  	sprintf((void *)buf, "%d",STinfo[j].backward_station_num);
		//  	lcddisperr((void *)buf);
		//  	//debug end

		//取上行站点编号
		sscanf(p, "%[^\x00d^\x00a]", line_buf);     //取到行尾
		pl = line_buf;
		for ( i = 0; i < STinfo[j].forward_station_num; i++ )
		{
			sscanf(pl, "%[^,\x00d]", buf);          //取到,
			strcpy((void *)&STinfo[j].forward_number[i][0], buf);
			pl = pl + strlen(buf) + 1;
		}
		p = p + strlen(line_buf) + 2;

		//取下行站点编号
		sscanf(p, "%[^\x00d^\x00a]", line_buf);     //取到行尾
		pl = line_buf;
		for ( i = 0; i < STinfo[j].backward_station_num; i++ )
		{
			sscanf(pl, "%[^,\x00d]", buf);          //取到,
			strcpy((void *)&STinfo[j].backward_number[i][0], buf);
			pl = pl + strlen(buf) + 1;
		}
		p = p + strlen(line_buf) + 2;

		//取上行站点名称
		sscanf(p, "%[^\x00d^\x00a]", line_buf);     //取到行尾
		pl = line_buf;
		for ( i = 0; i < STinfo[j].forward_station_num; i++ )
		{
			sscanf(pl, "%[^,\x00d]", buf);          //取到,
			strcpy((void *)&STinfo[j].forward_name[i][0], buf);
			pl = pl + strlen(buf) + 1;
		}
		p = p + strlen(line_buf) + 2;

		//取下行站点名称
		sscanf(p, "%[^\x00d^\x00a]", line_buf);     //取到行尾
		pl = line_buf;
		for ( i = 0; i < STinfo[j].backward_station_num; i++ )
		{
			sscanf(pl, "%[^,\x00d]", buf);          //取到,
			strcpy((void *)&STinfo[j].backward_name[i][0], buf);
			pl = pl + strlen(buf) + 1;
		}
		p = p + strlen(line_buf) + 2;

		//取上行票价表
		for ( k = 0; k < STinfo[j].forward_station_num; k++ )
		{
			sscanf(p, "%[^\x00d^\x00a]", line_buf);     //取到行尾
			pl = line_buf;
			for ( i = 0; i < STinfo[j].forward_station_num; i++ )
			{
				sscanf(pl, "%[^,\x00d]", buf);          //取到,
				strcpy((void *)&STinfo[j].forward_price_table[k][i][0], buf);
				pl = pl + strlen(buf) + 1;
			}
			p = p + strlen(line_buf) + 2;

		}

		//取下行票价表
		for ( k = 0; k < STinfo[j].backward_station_num; k++ )
		{
			sscanf(p, "%[^\x00d^\x00a]", line_buf);     //取到行尾
			pl = line_buf;
			for ( i = 0; i < STinfo[j].backward_station_num; i++ )
			{
				sscanf(pl, "%[^,\x00d]", buf);          //取到,
				strcpy((void *)&STinfo[j].backward_price_table[k][i][0], buf);
				pl = pl + strlen(buf) + 1;
			}
			p = p + strlen(line_buf) + 2;

		}

	}


}

/******************************************************************************
 函数名称：send_backup_record_process
 功能描述：上传备份售票记录处理
 参数描述：
 参数名称：	输入/输出？	类型		描述
				
 返  回  值：无
				   
 作      者	：许岩
 日      期：2012-10-30
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void send_backup_record_process(void)
{
	INT8U ucResult = 0;
	//  DevHandle hRS232Handle;
	//  INT8U buf[30];

	EA_vCls();
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "  上传备份记录  ");
	lcddisp(2, 1, (void *)"    数据处理中..    ");
	generate_record_file();
	generate_backup_record_file();
	lcddisp(2, 1, (void *)"      通讯中..      ");

	//  打开RS232设备
	ucResult = EA_ucOpenDevice("COM2", EM_io_RDWR, &hRS232Handle);
	if ( ucResult != EM_SUCCESS )
	{
		lcddisperr("打开串口失败");
		return;
	}

	ucResult = EA_ucInitRS232(hRS232Handle, EM_232_115200BPS, EM_232_NOPAR, EM_232_8DBS);
	if ( ucResult != EM_SUCCESS )
	{
		lcddisperr("串口初始化失败");
		(void)EA_ucCloseDevice(&hRS232Handle);
		return;
	}



	communication();


	ucResult = EA_ucCloseDevice(&hRS232Handle);
	if ( ucResult != EM_SUCCESS )
	{
		lcddisperr("打开串口失败");
	}

}

/******************************************************************************
 函数名称：generate_backup_record_file
 功能描述：生成备份记录文件
 参数描述：
 参数名称：	输入/输出？	类型		描述
				
 返  回  值：无
				   
 作      者	：许岩
 日      期：2012-10-30
 修改历史：
		日期		修改人		修改描述
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
		lcddisperr("创建backup.txt失败");
		return;
	}

	// 打开文件
	if ( EA_ucPFOpen((INT8U *)"backup", &ucOpenID) != EM_ffs_SUCCESS )
	{
		lcddisperr("打开备份记录文件失败");
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

		iWriteSize = EA_i64CfsWrite(fHandle, buf, strlen((void *)buf));         //写入文件
		if ( iWriteSize < 0 )
		{
			lcddisperr("写backup.txt失败");
			(void)EA_ucPFClose(ucOpenID);
			(void)EA_iCfsClose(fHandle);
			return;
		}

	}

	i = EA_iCfsFsync(fHandle);
	if ( i < 0 )
	{
		lcddisperr("同步backup.txt失败");
		(void)EA_ucPFClose(ucOpenID);
		(void)EA_iCfsClose(fHandle);
		return;
	}

	if ( EA_ucPFClose(ucOpenID) != EM_ffs_SUCCESS )
	{
		lcddisperr("关闭备份记录文件失败");
		(void)EA_iCfsClose(fHandle);
		return;
	}

	i = EA_iCfsClose(fHandle);
	if ( i < 0 )
	{
		lcddisperr("关闭backup.txt失败");
		return;
	}


}

/******************************************************************************
 函数名称：format_process
 功能描述：格式化处理
 参数描述：
 参数名称：	输入/输出？	类型		描述
				
 返  回  值：无
				   
 作      者	：许岩
 日      期：2012-10-30
 修改历史：
		日期		修改人		修改描述
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
	EA_vTextOut(0, 0, EM_key_FONT8X16, 0, 1, 1, "   系统初始化   ");
	//  sprintf((void *)buf, "编号:%s", DevStat.pos_number);
	//  lcddisp(1, 1, (void *)buf);

	lcddisp(2, 1, (void *)"输入密码:");
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
		lcddisp(4, 1, (void *)"密码错误            ");
		EA_ucSetInverse(EM_lcd_INVOFF);
		(void)EA_uiInkey(1);
		return;
	}


	//初始化操作
	param_factory_default(2);

	//删除所有文件
	// 获取文件个数
	uiFileNums = EA_iCfsGetFileNum();
	if ( uiFileNums == 0 )
	{
		return;				//无文件，不用删除了
	}

	// 动态分配空间
	ptFileInfo = (E_cfs_FileDetailInfo *)EA_pvMalloc(uiFileNums * sizeof(E_cfs_FileDetailInfo));
	if ( ptFileInfo == NULL )
	{
		lcddisperr("动态分配空间错误");
		return;
	}

	// 获取文件信息
	ucResult = EA_iCfsGetFileSystemInfo(&uiFileNums, ptFileInfo, &uiFreeSpace);


	for ( j = 0; j < uiFileNums; j++ )
	{

		a = EA_iCfsDelete((char *)ptFileInfo[j].cFileName);
		if ( a < 0 )
		{
			sprintf((void *)buf, "删除%s失败", ptFileInfo[j].cFileName);
			lcddisperr((void *)buf);
			return;
		}
	}

	EA_vFree(ptFileInfo);
	ptFileInfo = NULL;
}

/******************************************************************************
 函数名称：ic_card_process
 功能描述：刷卡处理
 参数描述：
 参数名称：	输入/输出？	类型		描述
				
 返  回  值：ok(0)-成功
			 notok(0xFF)-按取消键了
				   
 作      者	：许岩
 日      期：2012-10-30
 修改历史：
		日期		修改人		修改描述
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
	lcddisp(1, 1, (void *)"   请刷月票卡..   ");

	//打开RS232设备
	ucResult = EA_ucOpenDevice("COM1", EM_io_RDWR, &hRS232Handle);
	if ( ucResult != EM_SUCCESS )
	{
		lcddisperr("打开串口失败");
		return notok;
	}

	ucResult = EA_ucInitRS232(hRS232Handle, EM_232_115200BPS, EM_232_NOPAR, EM_232_8DBS);
	if ( ucResult != EM_SUCCESS )
	{
		lcddisperr("串口初始化失败");
		(void)EA_ucCloseDevice(&hRS232Handle);
		return notok;
	}


	for( ;; )
	{
//  	key = EA_ucKBHit();
//  	if ( key == EM_key_HIT )    //有按键
//  	{
//  		key = EA_uiInkey(1);    //读取按键键值
//  		if ( key == EM_key_CANCEL )
//  		{
//  			(void)EA_ucCloseDevice(&hRS232Handle);
//  			return notok;       //退出
//  		}
//  	}
		//找卡
		memcpy(Com1SndBuf, "\xAA\x05\x00", 3);
		i = com1_snd_and_rcv(Com1SndBuf, 3, (void *)Com1RcvBuf, &rcv_len, 1000, 10);
		if ( i == 0x01 )
		{
			(void)EA_ucCloseDevice(&hRS232Handle);
			return notok;			//按取消键退出
		}
		if ( i != ok )
		{
			continue;				//通讯不上
		}
		if ( rcv_len == 9 )
		{
			//各种失败处理
			result = (INT16U)Com1RcvBuf[5] * 256 + (INT16U)Com1RcvBuf[6];
			if ( result == 0x6A01 )			//此卡未启用
			{
				lcddisp(2, 1, (void *)"    此卡未启用!!    ");
				lcddisp(3, 1, (void *)"                    ");
				lcddisp(4, 1, (void *)"                    ");
				beep_failed();
				sleep_ms(1000);
				lcddisp(2, 1, (void *)"                    ");
			}
			else if ( result == 0x6A02 )	//黑名单卡
			{
				lcddisp(2, 1, (void *)"     黑名单卡!!     ");
				lcddisp(3, 1, (void *)"                    ");
				lcddisp(4, 1, (void *)"                    ");
				beep_failed();
				sleep_ms(1000);
				lcddisp(2, 1, (void *)"                    ");
			}
			else if ( result == 0x6A03 )	//不是本系统卡
			{
				lcddisp(2, 1, (void *)"    不是本系统卡    ");
				lcddisp(3, 1, (void *)"                    ");
				lcddisp(4, 1, (void *)"                    ");
				beep_failed();
				sleep_ms(1000);
				lcddisp(2, 1, (void *)"                    ");
			}
			else							//没找到卡
			{
			}

			continue;
		}
		else if ( rcv_len == 26 )
		{
//			//找卡成功处理
//			lcddisp(2, 1, (void *)"      找卡成功      ");
//			continue;
		}
		else
		{
			continue;				//接收不正确
		}


		//找卡成功处理
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
		//各种卡的子处理流程
		////////////////////////////////////////
		switch ( CardInfo.card_type )
		{
//  	   case CARDT_YOUHUI:		//优惠卡
//  		   i = notok;
//  		   lcddisperr("不是月票卡！");
//  		   break;

		   case CARDT_YUEPIAO:		//月票卡
			   i = card_yuepiao_process();
			   break;

		   default:                //卡类型错误
			   lcddisp(2, 1, (void *)"    卡类型错误!!    ");
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

	generate_ticket_info();                 //生成记录
	store_record(&Tinfo.pos_number[0]);     //储存记录
	

	(void)EA_ucCloseDevice(&hRS232Handle);

	return ok;

}


/******************************************************************************
 函数名称：ic_card_process
 功能描述：刷卡处理
 参数描述：
 参数名称：	输入/输出？	类型		描述
				
 返  回  值：ok(0)-成功
			 notok(0xFF)-按取消键了
				   
 作      者	：许岩
 日      期：2012-10-30
 修改历史：
		日期		修改人		修改描述
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
	lcddisp(1, 1, (void *)"    请刷优惠卡..    ");

	//打开RS232设备
	ucResult = EA_ucOpenDevice("COM1", EM_io_RDWR, &hRS232Handle);
	if ( ucResult != EM_SUCCESS )
	{
		lcddisperr("打开串口失败");
		return notok;
	}

	ucResult = EA_ucInitRS232(hRS232Handle, EM_232_115200BPS, EM_232_NOPAR, EM_232_8DBS);
	if ( ucResult != EM_SUCCESS )
	{
		lcddisperr("串口初始化失败");
		(void)EA_ucCloseDevice(&hRS232Handle);
		return notok;
	}


	for( ;; )
	{
//  	key = EA_ucKBHit();
//  	if ( key == EM_key_HIT )    //有按键
//  	{
//  		key = EA_uiInkey(1);    //读取按键键值
//  		if ( key == EM_key_CANCEL )
//  		{
//  			(void)EA_ucCloseDevice(&hRS232Handle);
//  			return notok;       //退出
//  		}
//  	}

		//找卡
		memcpy(Com1SndBuf, "\xAA\x05\x00", 3);
		i = com1_snd_and_rcv(Com1SndBuf, 3, (void *)Com1RcvBuf, &rcv_len, 1000, 10);
		if ( i == 0x01 )
		{
			(void)EA_ucCloseDevice(&hRS232Handle);
			return notok;			//按取消键退出
		}
		if ( i != ok )
		{
			continue;				//通讯不上
		}
		if ( rcv_len == 9 )
		{
			//各种失败处理
			result = (INT16U)Com1RcvBuf[5] * 256 + (INT16U)Com1RcvBuf[6];
			if ( result == 0x6A01 )			//此卡未启用
			{
				lcddisp(2, 1, (void *)"    此卡未启用!!    ");
				lcddisp(3, 1, (void *)"                    ");
				lcddisp(4, 1, (void *)"                    ");
				beep_failed();
				sleep_ms(1000);
				lcddisp(2, 1, (void *)"                    ");
			}
			else if ( result == 0x6A02 )	//黑名单卡
			{
				lcddisp(2, 1, (void *)"     黑名单卡!!     ");
				lcddisp(3, 1, (void *)"                    ");
				lcddisp(4, 1, (void *)"                    ");
				beep_failed();
				sleep_ms(1000);
				lcddisp(2, 1, (void *)"                    ");
			}
			else if ( result == 0x6A03 )	//不是本系统卡
			{
				lcddisp(2, 1, (void *)"    不是本系统卡    ");
				lcddisp(3, 1, (void *)"                    ");
				lcddisp(4, 1, (void *)"                    ");
				beep_failed();
				sleep_ms(1000);
				lcddisp(2, 1, (void *)"                    ");
			}
			else							//没找到卡
			{
			}

			continue;
		}
		else if ( rcv_len == 26 )
		{
//			//找卡成功处理
//			lcddisp(2, 1, (void *)"      找卡成功      ");
//			continue;
		}
		else
		{
			continue;				//接收不正确
		}


		//找卡成功处理
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
		//各种卡的子处理流程
		////////////////////////////////////////
		switch ( CardInfo.card_type )
		{
		   case CARDT_YOUHUI:		//优惠卡
			   i = card_youhui_process();
			   break;

//           case CARDT_YUEPIAO:      //月票卡
////  		   i = card_yuepiao_process();
//               i = notok;
//               break;

		   default:                //卡类型错误
			   lcddisp(2, 1, (void *)"    卡类型错误!!    ");
			   lcddisp(3, 1, (void *)"                    ");
			   lcddisp(4, 1, (void *)"                    ");
			   beep_failed();
			   sleep_ms(1000);
			   lcddisp(2, 1, (void *)"                    ");
			   continue;
		}
		if(i != ok)
			continue;

		//刷卡成功 跳出循环
		break;

	}

	(void)EA_ucCloseDevice(&hRS232Handle);

	return ok;

}

/******************************************************************************
 函数名称：card_youhui_process
 功能描述：优惠卡处理流程
 参数描述：
 参数名称：	输入/输出？	类型		描述
				
 返  回  值：ok(0)-成功
			 notok(0xFF)-失败
				   
 作      者	：许岩
 日      期：2012-10-30
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
INT8U card_youhui_process(void)
{
	FP64 pricef = 0.0;
	INT8U buf[30];
	INT8U prices_tmp[8];
	INT8U i = 0;
	INT16U rcv_len = 0;

	//取票价
	pricef = atof((void *)&DevStat.prices[0]);
//	DevStat.iccard_price = (INT32U)(pricef * 100.0);
	CardInfo.fare = (INT32U)(pricef * 100.0);

	CardInfo.fare = CardInfo.fare * DevStat.ticket_amount * ( DevStat.zhekou / 100.0 );			//单价*张数
	sprintf((void *)prices_tmp, "%.2f", ((FP32)pricef * (DevStat.zhekou / 100.00)));
//  htoa( (void *)&DevStat.prices[0], &CardInfo.fare, 4);
//  strcpy((void *)&DevStat.prices[0], (void *)prices_tmp);

	if ( CardInfo.init_balance < CardInfo.fare )
	{
		//余额不足处理
		lcddisp(2, 1, (void *)"     余额不足!!     ");
		sprintf((void *)buf, "卡余额: %.2f元", (FP32)CardInfo.init_balance / 100.0);
		lcddisp(3, 1, (void *)buf);
		lcddisp(4, 1, (void *)"                    ");
		beep_failed();
		sleep_ms(3000);

		return notok;
	}

	//扣款
	memcpy(Com1SndBuf, "\xAA\x07\x14\x00\x00\x00\x00", 7);
	memcpy(&Com1SndBuf[7], &CardInfo._CARD_NUMBER.detail_card_number.serial_number[0], 4);
	memcpy(&Com1SndBuf[11], &CardInfo.card_in_time, 7);
	memcpy(&Com1SndBuf[18], &CardInfo.fare, 4);
	Com1SndBuf[22] = 0x06;								//交易类型
	i = com1_snd_and_rcv2(Com1SndBuf, 23, (void *)Com1RcvBuf, &rcv_len, 1000, 10);
	if ( i != ok )
	{
		lcddisp(2, 1, (void *)"      扣款失败      ");
		lcddisp(3, 1, (void *)"错误代码: 0001      ");
		lcddisp(4, 1, (void *)"                    ");
		beep_failed();
		sleep_ms(3000);
		return notok;				//通讯不上
	}
	if ( rcv_len == 9 )
	{
		//各种失败处理
		lcddisp(2, 1, (void *)"      扣款失败      ");
		sprintf((void *)buf, "错误代码: %02X%02X", Com1RcvBuf[5], Com1RcvBuf[6]);
		lcddisp(3, 1, (void *)buf);
		lcddisp(4, 1, (void *)"                    ");
		beep_failed();
		sleep_ms(3000);
		return notok;
	}
	else if ( rcv_len == 16 )
	{
		//扣款成功处理
//		memcpy(&CardInfo._CARD_NUMBER.detail_card_number.serial_number[0], &Com1RcvBuf[3], 4);
//		CardInfo.card_purchase_type = Com1RcvBuf[3];

		DevStat.ticket_type = '9';
		CardInfo.card_purchase_type = 0x06;
		memcpy(&CardInfo.balance, &Com1RcvBuf[8], 4);
		strcpy((void *)&DevStat.prices[0], (void *)prices_tmp);

		lcddisp(2, 1, (void *)"优惠卡  扣款成功    ");
		sprintf((void *)buf, "卡余额: %.2f元", (FP32)CardInfo.balance / 100.0);
		lcddisp(3, 1, (void *)buf);
		sprintf((void *)buf, "消费余额: %.2f元", (FP32)CardInfo.fare / 100.0);
		lcddisp(4, 1, (void *)buf);
//  	lcddisp(4, 1, (void *)"                    ");

		if ( CardInfo.balance <= 500 )
		{
			lcddisp(4, 1, (void *)"卡余额不多  请充值!!");
		}

		beep_success();
		sleep_ms(5500);
		return ok;
	}
	else
	{
		lcddisp(2, 1, (void *)"      扣款失败      ");
		lcddisp(3, 1, (void *)"错误代码: 0002      ");
		lcddisp(4, 1, (void *)"                    ");
		beep_failed();
		sleep_ms(3000);
		return notok;				//接收不正确
	}

}

/******************************************************************************
 函数名称：card_yuepiao_process
 功能描述：月票卡处理流程
 参数描述：
 参数名称：	输入/输出？	类型		描述
				
 返  回  值：ok(0)-成功
			 notok(0xFF)-失败
				   
 作      者	：许岩
 日      期：2012-10-30
 修改历史：
		日期		修改人		修改描述
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

	//判断有效期
	time_cur = cal_sec((void *)&CardInfo.card_in_time);
	time_incard = cal_sec((void *)&CardInfo.valid_time);

	if (time_cur > time_incard)
	{
		//超期了，扣款
		lcddisp(2, 1, (void *)"  该卡已超过有效期  ");
		beep_failed();
		sleep_ms(3000);
		return notok;				//直接退出
	}
	else 
	{
		//在有效期内，不扣钱
		//只能按照对应的金额进行消费刷卡 ,, frank 14年10月18号
		//取票价
		CardInfo.fare = 0;
		CardInfo.balance = 0;			//金额和卡内余额都是0
//  	memset((void *)&DevStat.prices[0], 0x00, 8);                    //票价为0.00元
		sprintf((void *)&DevStat.prices[0], "%.2f", 0.00);
		DevStat.ticket_amount = 1;										//张数为1

		DevStat.start_station = CardInfo.qishi;
		strcpy((void *)&DevStat.start_station_number[0], (char *)&STinfo[DevStat.line_ptr].forward_number[DevStat.start_station][0]);
		strcpy((void *)&DevStat.start_station_name[0], (char *)&STinfo[DevStat.line_ptr].forward_name[DevStat.start_station][0]);

		DevStat.end_station = CardInfo.daoda;
		strcpy((void *)&DevStat.end_station_number[0], (char *)&STinfo[DevStat.line_ptr].forward_number[DevStat.end_station][0]);
		strcpy((void *)&DevStat.end_station_name[0], (char *)&STinfo[DevStat.line_ptr].forward_name[DevStat.end_station][0]);
//  	if ( CardInfo.init_balance < CardInfo.fare )
//  	{
//  		//余额不足处理
//  		lcddisp(3, 1, (void *)"不是该区间的月票卡!!");
//  		lcddisp(4, 1, (void *)"     不能乘坐!!     ");
//  		beep_failed();
//  		sleep_ms(1000);
//  		return notok;
//  	}

		//5分钟内不允许连续刷卡
		i = Cal_Interval_Sec(300);
		if (i != ok)
		{
			lcddisp(2, 1, (void *)"      扣款失败      ");
			lcddisp(3, 1, (void *)"月票卡不能带人      ");
			lcddisp(4, 1, (void *)"间隔必须大于5分钟   ");
			beep_failed();
			sleep_ms(3000);
			return notok;				//通讯不上
		}

		//张数只能为1张
		if ( DevStat.ticket_amount > 1 )
		{
			lcddisp(2, 1, (void *)"      扣款失败      ");
			lcddisp(3, 1, (void *)"在有效期内的月票卡  ");
			lcddisp(4, 1, (void *)"只能买一张票!!      ");
			beep_failed();
			sleep_ms(3000);
			return notok;				//通讯不上
		}

		//扣款
		memcpy(Com1SndBuf, "\xAA\x07\x14\x00\x00\x00\x00", 7);
		memcpy(&Com1SndBuf[7], &CardInfo._CARD_NUMBER.detail_card_number.serial_number[0], 4);
		memcpy(&Com1SndBuf[11], &CardInfo.card_in_time, 7);
		memset(&Com1SndBuf[18], 0x00, 4);
		Com1SndBuf[22] = 0x07;			//交易类型
		i = com1_snd_and_rcv2(Com1SndBuf, 23, (void *)Com1RcvBuf, &rcv_len, 1000, 10);
		if ( i != ok )
		{
			lcddisp(2, 1, (void *)"      扣款失败      ");
			lcddisp(3, 1, (void *)"错误代码: 0001      ");
			lcddisp(4, 1, (void *)"                    ");
			beep_failed();
			sleep_ms(3000);
			return notok;				//通讯不上
		}
		if ( rcv_len == 9 )
		{
			//各种失败处理
			lcddisp(2, 1, (void *)"      扣款失败      ");
			sprintf((void *)buf, "错误代码: %02X%02X", Com1RcvBuf[5], Com1RcvBuf[6]);
			lcddisp(3, 1, (void *)buf);
			lcddisp(4, 1, (void *)"                    ");
			beep_failed();
			sleep_ms(3000);
			return notok;
		}
		else if ( rcv_len == 16 )
		{
			//扣款成功处理
//			CardInfo.card_purchase_type = Com1RcvBuf[3];
			DevStat.ticket_type = '8';
			CardInfo.card_purchase_type = 0x07;
			memcpy(&CardInfo.balance, &Com1RcvBuf[8], 4);

			lcddisp(2, 1, (void *)"月票卡  扣款成功    ");
			sprintf((void *)buf, "有效期至:%02X%02X.%02X.%02X ", CardInfo.valid_time.century, 
					CardInfo.valid_time.year, CardInfo.valid_time.month, CardInfo.valid_time.day);
			lcddisp(3, 1, (void *)buf);

			sprintf((void *)buf, "%s-%s", (char *)&DevStat.start_station_name[0], (char *)&DevStat.end_station_name[0]);  // 显示起始和到达站点信息 
			lcddisp(4, 1, (void *)buf);

			if ( time_incard - time_cur <= 15UL * 24UL * 60UL * 60UL )			//有效期提前15天提醒
			{
				lcddisp(4, 1, (void *)"有效期将至  请充值!!");
			}

			beep_success();

			sleep_ms(5500);
			return ok;
		}
		else
		{
			lcddisp(2, 1, (void *)"      扣款失败      ");
			lcddisp(3, 1, (void *)"错误代码: 0002      ");
			lcddisp(4, 1, (void *)"                    ");
			beep_failed();
			sleep_ms(3000);
			return notok;				//接收不正确
		}
	}


}

/******************************************************************************
 函数名称：Cal_Interval_Sec
 功能描述：计算刷卡间隔时间
 参数描述：
 参数名称：	输入/输出？	类型		描述
 interval	输入		INT32U		秒
 
 返  回  值：ok - 已经超过时间限制,允许继续刷卡
				notok - 不允许继续刷卡
   
 作      者	：许岩
 日      期：2006.1.9
 修改历史：
		日期		修改人		修改描述

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
	INT8U card_num_cur[10];				//当前卡号

	const char recFileName[] = "record";
	INT8U ucOpenID = 0;
	INT32U total_rec_num = 0;

	TICKET_INFO ticket;
	INT32U uiResult;
	INT32U uiReadCnt;
//	INT32U j = 0;

	// 打开文件
	if ( EA_ucPFOpen((uchar *)recFileName, &ucOpenID) != EM_ffs_SUCCESS )
	{
		lcddisperr("打开记录文件失败");
		return ok;
	}


	(void)EA_ucPFInfo(ucOpenID, EM_ffs_VALIDREC, &uiResult);
	total_rec_num = uiResult;
	DevStat.record_number = total_rec_num;

//	if ( EA_ucPFWriteRec(ucOpenID, 0, rec) != EM_ffs_SUCCESS )
//	{
//		lcddisperr("添加记录失败");
//		return ok;
//	}

//	if ( EA_ucPFClose(ucOpenID) != EM_ffs_SUCCESS )
//	{
//		lcddisperr("关闭记录文件失败");
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
			CardInfo._CARD_NUMBER.detail_card_number.serial_number[3] );			//卡号
	
	////////////////////////////////////////
	//计算刷卡间隔时间
	////////////////////////////////////////
	for ( k = 0; k < DevStat.record_number; k++ )
	{
//		get_record((INT8U *)rec, DevStat.record_number - k - 1);    //32字节记录
		(void)EA_ucPFReadRec(ucOpenID, DevStat.record_number - k, 1, &uiReadCnt, &ticket.pos_number[0]);

//		h = memcmp((void *)&CardInfo.card_in_time, (void *)&prec->card_in_time, 2); //比较世纪,年
//		if ( h != ok )
//		{
//			//年不相等,允许消费
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
		if ( (time_cur - time_rec >= (INT32S)interval) || (time_rec - time_cur >= (INT32S)interval) )   //超过时间间隔也没有找到此卡卡号
		{
			(void)EA_ucPFClose(ucOpenID);
			return ok;
		}
		else   //在时间间隔内,看卡号是否相同
		{
//			h = memcmp((void *)&CardInfo._CARD_NUMBER.detail_card_number.serial_number[0], (void *)&prec->card_number[4], 4);
			h = strcmp((char *)card_num_cur, (char *)ticket.card_number);
			if ( h == ok ) //卡号相等
			{
				(void)EA_ucPFClose(ucOpenID);
				return notok;
			}
			else //不相等则继续查询
			{
				;
			}
		}
	}

	(void)EA_ucPFClose(ucOpenID);
	return ok;
}






