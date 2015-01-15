/******************************************************************************
Copyright 2005 大连易达通电子有限公司
All rights riserved.

文件名  ：data_type.h
模块名称：定义了所有的数据类型

当前版本：0.0.1
修改人	：许岩
完成日期：2005.04.06
升级说明：create
******************************************************************************/
#ifndef __DATA_TYPE_H__
#define __DATA_TYPE_H__

//#include "OS_CPU.H"

#ifndef NULL
#define NULL    ((void *)0)
#endif

#ifndef FALSE
#define FALSE   (0)
#endif

#ifndef TRUE
#define TRUE    (1)
#endif

typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long  DWORD;
typedef unsigned int   BOOL;


/**********************************************************************************************************
*                    定义与编译器无关的数据类型
**********************************************************************************************************/

typedef unsigned char  BOOLEAN;                 /* 布尔变量                                 */
typedef unsigned char  INT8U;                   /* 无符号8位整型变量                        */
typedef signed   char  INT8S;                   /* 有符号8位整型变量                        */
typedef unsigned short INT16U;                  /* 无符号16位整型变量                       */
typedef signed   short INT16S;                  /* 有符号16位整型变量                       */
typedef unsigned int   INT32U;                  /* 无符号32位整型变量                       */
typedef signed   int   INT32S;                  /* 有符号32位整型变量                       */
//typedef int   INT32S;                           /* 有符号32位整型变量                       */
typedef float          FP32;                    /* 单精度浮点数（32位长度）                 */
typedef double         FP64;                    /* 双精度浮点数（64位长度）                 */

typedef INT32U         OS_STK;                  /* 堆栈是32位宽度                           */


typedef unsigned char  	uint8;                   /* 无符号8位整型变量                        */
typedef signed   char  	int8;                    /* 有符号8位整型变量                        */
typedef unsigned short 	uint16;                  /* 无符号16位整型变量                       */
typedef signed   short 	int16;                   /* 有符号16位整型变量                       */
typedef unsigned int   	uint32;                  /* 无符号32位整型变量                       */
typedef signed   int   	int32;                   /* 有符号32位整型变量                       */
typedef float          	fp32;                    /* 单精度浮点数（32位长度）                 */
typedef double         	fp64;                    /* 双精度浮点数（64位长度）                 */

/*define	pin	macro*/
//#define	BIT0	0x00000001
//#define	BIT1	0x00000002
//#define	BIT2	0x00000004
//#define	BIT3	0x00000008
//
//#define	BIT4	0x00000010
//#define	BIT5	0x00000020
//#define	BIT6	0x00000040
//#define	BIT7	0x00000080
//
//#define	BIT8	0x00000100
//#define	BIT9	0x00000200
//#define	BIT10	0x00000400
//#define	BIT11	0x00000800
//
//#define	BIT12	0x00001000
//#define	BIT13	0x00002000
//#define	BIT14	0x00004000
//#define	BIT15	0x00008000
//
//#define	BIT16	0x00010000
//#define	BIT17	0x00020000
//#define	BIT18	0x00040000
//#define	BIT19	0x00080000
//
//#define	BIT20	0x00100000
//#define	BIT21	0x00200000
//#define	BIT22	0x00400000
//#define	BIT23	0x00800000
//
//#define	BIT24	0x01000000
//#define	BIT25	0x02000000
//#define	BIT26	0x04000000
//#define	BIT27	0x08000000
//
//#define	BIT28	0x10000000
//#define	BIT29	0x20000000
//#define	BIT30	0x40000000
//#define	BIT31	0x80000000
//
//#define BANK0 0x80000000
//#define BANK1 0x81000000
//#define BANK2 0x82000000
//#define BANK3 0x83000000
//
//#ifndef QUEUE_DATA_TYPE
//    #define QUEUE_DATA_TYPE             uint8
//#endif
//
//typedef struct
//{
//    QUEUE_DATA_TYPE     *Out;                   /* 指向数据输出位置         */
//    QUEUE_DATA_TYPE     *In;                    /* 指向数据输入位置         */
//    QUEUE_DATA_TYPE     *End;                   /* 指向Buf的结束位置        */
//    uint16                      NData;                  /* 队列中数据个数           */
//    uint16                      MaxData;                /* 队列中允许存储的数据个数 */
//    uint8                       (* ReadEmpty)();        /* 读空处理函数             */
//    uint8                       (* WriteFull)();        /* 写满处理函数             */
//    QUEUE_DATA_TYPE     Buf[1];                 /* 存储数据的空间           */
//} DataQueue;



//车载机时间结构
//__packed typedef struct _TIME_
typedef struct _TIME_
{
	INT8U century;
	INT8U year;
	INT8U month;
	INT8U day;
	INT8U hour;
	INT8U minute;
	INT8U second;
//}_TIME, BUS_TIME;
}BUS_TIME;

////////////////////////////////////////////
//增加管理员编号联合结构变量
/////////////////////////////////////////////
typedef union _MANAGER_NUMBER_
{
	INT8U setter_number[3];//设置员编号
	INT8U gather_number[3];//采集员编号
	INT8U checker_number[3];//监测员编号
	INT8U signer_number[3];//签点员编号
	INT8U check_number[3];//稽查员编号
}_MANAGER_NUMBER;

//CARD_INFO中用到的struct
typedef struct _CARD_NUM
{
	INT8U city_code[2];	//城市代码
	INT8U calling_code[2];//行业代码
	INT8U serial_number[4];		//序列号
}CARD_NUM;

//卡片信息，交易记录由此变量生成
typedef struct _CARD_INFO
{
	union _CARD_NUMBER_
	{
		INT8U card_number[8];//卡号
		CARD_NUM detail_card_number;
	}_CARD_NUMBER;

	BUS_TIME card_in_time;							//交易时间
	INT8U card_type;								//卡类型
	INT32U fare;									//消费金额
	INT32U init_balance;							//原余额
	INT32U balance;									//交易后余额
	INT8U card_purchase_type;						//交易类型

	INT8U csn[4];									//硬件卡号

	INT8U kind_of_card;								//卡类，CPU_CARD=CPU卡		M1_CARD=M1卡
	BUS_TIME valid_time;							//有效期
	INT8U qishi;
	INT8U daoda; 
}CARD_INFO;




//#define resetpos() (*(void(*)())0)()
//#define OS_ENTER_CRITICAL()
//#define OS_EXIT_CRITICAL()


#endif
