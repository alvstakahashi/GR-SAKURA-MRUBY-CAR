/***********************************************************************/
/*                                                                     */
/*  FILE        :mrb_read.c                                           */
/*  DATE        :Thu, Jul 07, 2015                                     */
/*  DESCRIPTION :mrb binary SCI0 read to buffer                        */
/*  CPU TYPE    :RX63N                                                 */
/*                                                                     */
/***********************************************************************/

#include <stdint.h>
#include <string.h>

#include "iodefine.h"
#include <kernel.h>
#include "ssp/kernel_cfg.h"
#include "rx63nsci0.h"

#include "mrb_read.h"

#define MRBSZ		4096		// 4096バイト
uint8_t	prg_buff[MRBSZ];
uint8_t	cmd_buff[256];


#define TIMEOUT		3000		// 3秒
#define	START_TOUT	10000		//　最初は10秒
#define HEADERSZ	10			// ヘッダサイズ10バイト
#define	SIZESZ		4			// SIZEフィールドサイズ4バイト
int read_mrb(char *buff,int bfsz)
{
	int readCount = 0;
	uint32_t mrb_size = 0;

#if 0
	int bfszORG = bfsz;		//もともとのバッファサイズ

	// ヘッダ10バイト読む
	readCount = CPU_UartRead(buff,HEADERSZ,START_TOUT);
	if (readCount < 0)
	{
		return(-1);
	}
	bfsz -= readCount;
	buff += readCount;
	readCount = CPU_UartRead(buff,SIZESZ,TIMEOUT);
	if (readCount < 0)
	{
		return(-1);
	}
	memcpy(&mrb_size,buff,SIZESZ);
	bfsz -= readCount;
	buff += readCount;
	
	if (bfszORG < mrb_size)
	{
		return(-2);
	}

	bfsz = mrb_size - (HEADERSZ + SIZESZ);
#endif
	readCount = CPU_UartRead(buff,bfsz,START_TOUT);
	if (readCount < 0)
	{
		return(-1);
	}
#if 0
	readCount += (HEADERSZ + SIZESZ);
#endif
	return(readCount);
}

uint8_t *mrb_read_ui(void)
{
	int commandCount = 0;

	int loadCount = 0;

	printf("\nmrubyOS ver0.002 By AliveVisionSoftwareKK\n");
	while(1)
	{
		printf("for Command L to load mrb binary\n");
		printf("E to exit & go load program \n");
		printf("Q to exit & run for internal program \n");
		if (loadCount)
		{
			printf("Loaded now\n");
		}
		else
		{
			printf("No program load\n");
		}
		CPU_UartPuts(">>>");

		commandCount = CPU_UartGets(cmd_buff);
		if (commandCount > 0)
		{
			switch(cmd_buff[0])
			{
			case 'L':			// load
				loadCount = read_mrb(prg_buff,MRBSZ);
				printf("Load Finish!! LoadCount = %d\n",loadCount);
				break;
			case 'E':
				if (loadCount)
				{
					return(prg_buff);
				}
				else
				{
					return((uint8_t *)NULL);
				}
				break;
			case 'Q':
				return((uint8_t *)NULL);
				break;
			default:
				printf("command err\n");
				break;
			}
		}
	}
}
