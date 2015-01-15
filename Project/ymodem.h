/*******************************************************************************
** �ļ���: 		ymodem.H
** �汾��  		1.0
** ��������: 	RealView MDK-ARM 4.21
** ����: 		
** ��������: 	2011-04-29
** ����:		��Ymodem.c����ص�ͷ�ļ�
** ����ļ�:	Ymodem.c
** �޸���־��	2011-04-29   �����ĵ�
*******************************************************************************/

/* ��ֹ�ض��� *****************************************************************/
#ifndef _YMODEM_H_
#define _YMODEM_H_

/* �� ------------------------------------------------------------------------*/
#define PACKET_SEQNO_INDEX      (1)
#define PACKET_SEQNO_COMP_INDEX (2)

#define PACKET_HEADER           (3)
#define PACKET_TRAILER          (2)
#define PACKET_OVERHEAD         (PACKET_HEADER + PACKET_TRAILER)
#define PACKET_SIZE             (128)
#define PACKET_1K_SIZE          (1024)

#define FILE_NAME_LENGTH        (128)
#define FILE_SIZE_LENGTH        (16)

#define SOH                     (0x01)  //128�ֽ����ݰ���ʼ
#define STX                     (0x02)  //1024�ֽڵ����ݰ���ʼ
#define EOT                     (0x04)  //��������
#define ACK                     (0x06)  //��Ӧ
#define NAK                     (0x15)  //û��Ӧ
#define CA                      (0x18)  //�����������ֹת��
#define CRC16                   (0x43)  //'C' == 0x43, ��Ҫ 16-bit CRC 

#define ABORT1                  (0x41)  //'A' == 0x41, �û���ֹ 
#define ABORT2                  (0x61)  //'a' == 0x61, �û���ֹ

#define NAK_TIMEOUT             (0x100000)
#define MAX_ERRORS              (5)

/* �������� ------------------------------------------------------------------*/

#endif  /* _YMODEM_H_ */

/*******************************�ļ�����***************************************/