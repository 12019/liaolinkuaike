/******************************************************************************
Copyright 2005 �����״�ͨ�������޹�˾
All rights riserved.

�ļ���  ��data_type.h
ģ�����ƣ����������е���������

��ǰ�汾��0.0.1
�޸���	������
������ڣ�2005.04.06
����˵����create
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
*                    ������������޹ص���������
**********************************************************************************************************/

typedef unsigned char  BOOLEAN;                 /* ��������                                 */
typedef unsigned char  INT8U;                   /* �޷���8λ���ͱ���                        */
typedef signed   char  INT8S;                   /* �з���8λ���ͱ���                        */
typedef unsigned short INT16U;                  /* �޷���16λ���ͱ���                       */
typedef signed   short INT16S;                  /* �з���16λ���ͱ���                       */
typedef unsigned int   INT32U;                  /* �޷���32λ���ͱ���                       */
typedef signed   int   INT32S;                  /* �з���32λ���ͱ���                       */
//typedef int   INT32S;                           /* �з���32λ���ͱ���                       */
typedef float          FP32;                    /* �����ȸ�������32λ���ȣ�                 */
typedef double         FP64;                    /* ˫���ȸ�������64λ���ȣ�                 */

typedef INT32U         OS_STK;                  /* ��ջ��32λ���                           */


typedef unsigned char  	uint8;                   /* �޷���8λ���ͱ���                        */
typedef signed   char  	int8;                    /* �з���8λ���ͱ���                        */
typedef unsigned short 	uint16;                  /* �޷���16λ���ͱ���                       */
typedef signed   short 	int16;                   /* �з���16λ���ͱ���                       */
typedef unsigned int   	uint32;                  /* �޷���32λ���ͱ���                       */
typedef signed   int   	int32;                   /* �з���32λ���ͱ���                       */
typedef float          	fp32;                    /* �����ȸ�������32λ���ȣ�                 */
typedef double         	fp64;                    /* ˫���ȸ�������64λ���ȣ�                 */

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
//    QUEUE_DATA_TYPE     *Out;                   /* ָ���������λ��         */
//    QUEUE_DATA_TYPE     *In;                    /* ָ����������λ��         */
//    QUEUE_DATA_TYPE     *End;                   /* ָ��Buf�Ľ���λ��        */
//    uint16                      NData;                  /* ���������ݸ���           */
//    uint16                      MaxData;                /* ����������洢�����ݸ��� */
//    uint8                       (* ReadEmpty)();        /* ���մ�����             */
//    uint8                       (* WriteFull)();        /* д��������             */
//    QUEUE_DATA_TYPE     Buf[1];                 /* �洢���ݵĿռ�           */
//} DataQueue;



//���ػ�ʱ��ṹ
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
//���ӹ���Ա������Ͻṹ����
/////////////////////////////////////////////
typedef union _MANAGER_NUMBER_
{
	INT8U setter_number[3];//����Ա���
	INT8U gather_number[3];//�ɼ�Ա���
	INT8U checker_number[3];//���Ա���
	INT8U signer_number[3];//ǩ��Ա���
	INT8U check_number[3];//����Ա���
}_MANAGER_NUMBER;

//CARD_INFO���õ���struct
typedef struct _CARD_NUM
{
	INT8U city_code[2];	//���д���
	INT8U calling_code[2];//��ҵ����
	INT8U serial_number[4];		//���к�
}CARD_NUM;

//��Ƭ��Ϣ�����׼�¼�ɴ˱�������
typedef struct _CARD_INFO
{
	union _CARD_NUMBER_
	{
		INT8U card_number[8];//����
		CARD_NUM detail_card_number;
	}_CARD_NUMBER;

	BUS_TIME card_in_time;							//����ʱ��
	INT8U card_type;								//������
	INT32U fare;									//���ѽ��
	INT32U init_balance;							//ԭ���
	INT32U balance;									//���׺����
	INT8U card_purchase_type;						//��������

	INT8U csn[4];									//Ӳ������

	INT8U kind_of_card;								//���࣬CPU_CARD=CPU��		M1_CARD=M1��
	BUS_TIME valid_time;							//��Ч��
	INT8U qishi;
	INT8U daoda; 
}CARD_INFO;




//#define resetpos() (*(void(*)())0)()
//#define OS_ENTER_CRITICAL()
//#define OS_EXIT_CRITICAL()


#endif
