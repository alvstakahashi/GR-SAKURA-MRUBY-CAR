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


/* UARTコントローラ設定 */
#define	USE_BPS		115200	/* 通信速度 57600 or 115200 */
#define	USE_UART_CH		0	/* UARTチャネル0 ～ 2 */
/*
 *  クロックPCLK(kHz)
 *
 *  タイマ，シリアルなど周辺モジュール用
 */
#define FREQ_PCLK  (96000)
#define BAUD_115200BPS			( FREQ_PCLK * 1000 / 32 / 115200 - 1 )
#define BAUD_4800BPS			( FREQ_PCLK * 1000 / 32 /4 / 4800 - 1 )
#define SCBRR_SET BAUD_115200BPS

/* コンソール入力バッファチェック */
static int CPU_UartRecvCheck(void)
{
	if (SCI0.SSR.BYTE & 0x40) {	/* データを受信したか */
		return 1;	/* 受信データあり */
	} else {
		return 0;	/* 受信データなし */
	}
}

/* コンソール(UART)初期化 */
void CPU_UartInitialize(void)
{
	int i;

	PORT2.PMR.BYTE = 0x00;

    //MPC 設定
	MPC.PWPR.BIT.B0WI = 0;
	MPC.PWPR.BIT.PFSWE = 1;
	MPC.P20PFS.BIT.PSEL = 10;
	MPC.P21PFS.BIT.PSEL = 10;
	MPC.PWPR.BIT.B0WI = 1;
	MPC.PWPR.BIT.PFSWE = 0;
	 
	PORT2.PMR.BYTE = 0x03;


	PORT2.PMR.BYTE = 0x03;
	
	/* ポート設定(RXD0(P21))入力方向 */
	PORT2.PIDR.BIT.B1 = 1;

	/* SCI0スタンバイ解除 */
	SYSTEM.MSTPCRB.BIT.MSTPB31 = 0;

	/* SCI設定 */
	SCI0.SCR.BYTE = 0x00;	/* TE=0,RE=0,CKE1=0 */
	SCI0.SMR.BYTE = 0x00;	/* CHR=0,PE=0,STOP=0,CKS=01(Pφ/4) */
	SCI0.BRR = SCBRR_SET;	/* 分周設定 */
	SCI0.SEMR.BIT.ABCS = 1;	/* ABCS=1 */
	for(i=0;i<0x800000;i++);/* 1bit時間ウェイト */
	SCI0.SCR.BYTE = 0x30;	/* TIE=0,RIE=0,TE=1,RE=1 */

}

/* コンソール1文字出力 */
int CPU_UartPutch(char data)
{
	while(1){
		if (SCI0.SSR.BYTE & 0x80) {	/* 送信バッファが空いているか */
			SCI0.TDR = data;
			SCI0.SSR.BYTE = SCI0.SSR.BYTE & 0x40;
			break;
		}
	}
	return(1);
}

/* コンソール文字列出力(\nだけで CR+LF出力) */
void CPU_UartPuts(char *buffer)
{
	char data;
	while( (data = *(buffer++)) != 0){	/* nullまで出力 */
		if (data == 0x0a) {
			CPU_UartPutch(0x0d);	/* CR出力 */
			CPU_UartPutch(0x0a);	/* LF出力 */
		} else {
			CPU_UartPutch(data);	/* 1文字出力 */
		}
	}
}



/* コンソール1文字入力(エコーバックなし) */
char CPU_UartGetch(void)
{
	char data;
	while(1){
		if (CPU_UartRecvCheck()) {	/* データを受信したか */
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
	
	get_tim(&targetTime);		//現在時刻
	targetTime += (tout*100);	// 時刻は10usが1  指定はms単位
	
	
	while(1){
		if (CPU_UartRecvCheck()) {	/* データを受信したか */
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
#define STATUSBYTES		32			// 32バイト毎.

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
/* コンソール文字列入力(CRコードまで/最大255文字/エコーバックあり) */
int CPU_UartGets(char *buffer)
{
	char data;
	int i;
	i=0;
	while(1){
		data = CPU_UartGetch();	/* 1文字入力 */
		*buffer = data;
		CPU_UartPutch(data);	/* 1文字出力(エコーバック) */
		buffer++;
		i++;
		if (i > 255) break;		/* 最大文字数に達した */
		if (data == 0x0D) break;/* CRコードまで受信 */
	}
	*buffer=(unsigned char)0;	/* nullコード */
	return i;					/* 入力文字数 */
}

int write(int fd, void *buf, int count)
{
	int lastCount = count;
	char *p = (char*)buf;
	while(count--)
	{
#ifdef HEWSIM
		charput(*p++);
#else
		CPU_UartPutch(*p++);
#endif
	}
	return(lastCount-count);
}
