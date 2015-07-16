/***********************************************************************/
/*                                                                     */
/*  FILE        :rx63nsci0.c                                           */
/*  DATE        :Thu, Jul 04, 2015                                     */
/*  DESCRIPTION :SCI0 Driver                                           */
/*  CPU TYPE    :RX63N                                                 */
/*                                                                     */
/***********************************************************************/


#include "iodefine.h"
#include <kernel.h>
#include "ssp/kernel_cfg.h"
#include "rx63nsci0.h"


/* UART�R���g���[���ݒ� */
#define	USE_BPS		115200	/* �ʐM���x 57600 or 115200 */
#define	USE_UART_CH		0	/* UART�`���l��0 �` 2 */
/*
 *  �N���b�NPCLK(kHz)
 *
 *  �^�C�}�C�V���A���Ȃǎ��Ӄ��W���[���p
 */
#define FREQ_PCLK  (96000)
#define BAUD_115200BPS			( FREQ_PCLK * 1000 / 32 / 115200 - 1 )
#define BAUD_4800BPS			( FREQ_PCLK * 1000 / 32 /4 / 4800 - 1 )
#define SCBRR_SET BAUD_115200BPS

/* �R���\�[�����̓o�b�t�@�`�F�b�N */
static int CPU_UartRecvCheck(void)
{
	if (SCI0.SSR.BYTE & 0x40) {	/* �f�[�^����M������ */
		return 1;	/* ��M�f�[�^���� */
	} else {
		return 0;	/* ��M�f�[�^�Ȃ� */
	}
}

/* �R���\�[��(UART)������ */
void CPU_UartInitialize(void)
{
	int i;

	PORT2.PMR.BYTE = 0x00;

    //MPC �ݒ�
	MPC.PWPR.BIT.B0WI = 0;
	MPC.PWPR.BIT.PFSWE = 1;
	MPC.P20PFS.BIT.PSEL = 10;
	MPC.P21PFS.BIT.PSEL = 10;
	MPC.PWPR.BIT.B0WI = 1;
	MPC.PWPR.BIT.PFSWE = 0;
	 
	PORT2.PMR.BYTE = 0x03;


	PORT2.PMR.BYTE = 0x03;
	
	/* �|�[�g�ݒ�(RXD0(P21))���͕��� */
	PORT2.PIDR.BIT.B1 = 1;

	/* SCI0�X�^���o�C���� */
	SYSTEM.MSTPCRB.BIT.MSTPB31 = 0;

	/* SCI�ݒ� */
	SCI0.SCR.BYTE = 0x00;	/* TE=0,RE=0,CKE1=0 */
	SCI0.SMR.BYTE = 0x00;	/* CHR=0,PE=0,STOP=0,CKS=01(P��/4) */
	SCI0.BRR = SCBRR_SET;	/* �����ݒ� */
	SCI0.SEMR.BIT.ABCS = 1;	/* ABCS=1 */
	for(i=0;i<0x800000;i++);/* 1bit���ԃE�F�C�g */
	SCI0.SCR.BYTE = 0x30;	/* TIE=0,RIE=0,TE=1,RE=1 */

}

/* �R���\�[��1�����o�� */
int CPU_UartPutch(char data)
{
	while(1){
		if (SCI0.SSR.BYTE & 0x80) {	/* ���M�o�b�t�@���󂢂Ă��邩 */
			SCI0.TDR = data;
			SCI0.SSR.BYTE = SCI0.SSR.BYTE & 0x40;
			break;
		}
	}
	return(1);
}

/* �R���\�[��������o��(\n������ CR+LF�o��) */
void CPU_UartPuts(char *buffer)
{
	char data;
	while( (data = *(buffer++)) != 0){	/* null�܂ŏo�� */
		if (data == 0x0a) {
			CPU_UartPutch(0x0d);	/* CR�o�� */
			CPU_UartPutch(0x0a);	/* LF�o�� */
		} else {
			CPU_UartPutch(data);	/* 1�����o�� */
		}
	}
}



/* �R���\�[��1��������(�G�R�[�o�b�N�Ȃ�) */
char CPU_UartGetch(void)
{
	char data;
	while(1){
		if (CPU_UartRecvCheck()) {	/* �f�[�^����M������ */
			data = SCI0.RDR;
			SCI0.SSR.BYTE = SCI0.SSR.BYTE & 0x80;
			break;
		}
	}
	return data;
}
int CPU_UartGetch_t(RELTIM tout)
{
	char data;
	SYSTIM targetTime,nowTime;
	
	get_tim(&targetTime);		//���ݎ���
	targetTime += (tout*100);	// ������10us��1  �w���ms�P��
	
	
	while(1){
		if (CPU_UartRecvCheck()) {	/* �f�[�^����M������ */
			data = SCI0.RDR;
			SCI0.SSR.BYTE = SCI0.SSR.BYTE & 0x80;
			break;
		}
		get_tim(&nowTime);
		if (targetTime <= nowTime)
		{
			return(-1);
		}
	}
	return ((int)(data & 0x00ff));
}
#define STATUSBYTES		32			// 32�o�C�g��.

int CPU_UartRead(char *inbuf,int sz,RELTIM tout)
{
	int readsz = 0;
	int getdata = 0;
	
	int statusCount = STATUSBYTES;
	
	while(sz--)
	{
		getdata = CPU_UartGetch_t(tout);
		if (getdata < 0)
		{
			return(readsz);
		}
		*inbuf++ = (char)(getdata & 0x00ff);
		readsz++;
#if 1
		if (statusCount--)
		{
			CPU_UartPuts(".");
			statusCount = STATUSBYTES;
		}
#endif
	}
	return(readsz);
}
/* �R���\�[�����������(CR�R�[�h�܂�/�ő�255����/�G�R�[�o�b�N����) */
int CPU_UartGets(char *buffer)
{
	char data;
	int i;
	i=0;
	while(1){
		data = CPU_UartGetch();	/* 1�������� */
		*buffer = data;
		CPU_UartPutch(data);	/* 1�����o��(�G�R�[�o�b�N) */
		buffer++;
		i++;
		if (i > 255) break;		/* �ő啶�����ɒB���� */
		if (data == 0x0D) break;/* CR�R�[�h�܂Ŏ�M */
	}
	*buffer=(unsigned char)0;	/* null�R�[�h */
	return i;					/* ���͕����� */
}

int write(int fd, void *buf, int count)
{
	int lastCount = count;
	char *p = (char*)buf;
	while(count--)
	{
		CPU_UartPutch(*p++);
	}
	return(lastCount-count);
}
