/***********************************************************************/
/*                                                                     */
/*  FILE        :rx63nsci0.h                                          */
/*  DATE        :Thu, Jul 04, 2015                                     */
/*  DESCRIPTION :SCI0 Driver                                           */
/*  CPU TYPE    :RX63N                                                 */
/*                                                                     */
/*                                                                     */
/***********************************************************************/

#ifndef __RX63NSCI0_H__
#define __RX63NSCI0_H__

#include <kernel.h>

extern void CPU_UartInitialize(void);
extern int  CPU_UartPutch(char data);
extern void   CPU_UartPuts(char *buffer);
extern char CPU_UartGetch(void);
extern int CPU_UartRead(char *inbuf,int sz,RELTIM tout);
extern int CPU_UartGets(char *buffer);
extern int CPU_UartGetch_t(RELTIM tout);

#endif // __RX63NSCI0_H__
