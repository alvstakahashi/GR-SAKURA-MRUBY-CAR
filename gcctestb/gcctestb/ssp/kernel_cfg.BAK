/* kernel_cfg.h */
#ifndef TOPPERS_KERNEL_CFG_H
#define TOPPERS_KERNEL_CFG_H

#define TNUM_TSKID	5
#define TNUM_CYCID	5
#define TNUM_ALMID	4

#define MAIN_TASK	1
#define TASK2_ID	2
#define RUBY_TSK3	3
#define RUBY_TSK4	4
#define RUBY_TSK5	5

#define RUBY_CYC1	1
#define RUBY_CYC2	2
#define RUBY_CYC3	3
#define RUBY_CYC4	4
#define RUBY_CYC5	5

#define RUBY_ALM1	1
#define RUBY_ALM2	2
#define RUBY_ALM3	3
#define RUBY_ALM4	4

//#define	TASK_STACKSZ		1024
#define	TASK_STACKSZ		4096
#define TOPPERS_TASKSTKPT(task_id ) (( STK_T  )(( char * )( task_stack[task_id] ) + ( TASK_STACKSZ )))
//STK_T *const	_kernel_istkpt = TOPPERS_ISTKPT(TOPPERS_STK, TOPPERS_STKSZ);
extern STK_T	task_stack[TNUM_TSKID][COUNT_STK_T(TASK_STACKSZ)];

extern unsigned char task_wait[TNUM_TSKID];

extern RELTIM	tskTout[TNUM_TSKID];


#endif /* TOPPERS_KERNEL_CFG_H */

