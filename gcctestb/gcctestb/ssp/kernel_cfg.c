/* kernel_cfg.c */
//#include "kernel_int.h"
#include "kernel_impl.h"
#include "task.h"
#include "kernel_unrename.h"

#include "kernel_cfg.h"
#include "time_event.h"

#include "sample1.h"
#include "mruby.h"
#include "mruby/variable.h"

/*
 *  Task Management Functions
 */

const ID _kernel_tmax_tskid = (TMIN_TSKID + TNUM_TSKID - 1);

const ATR     	_kernel_tinib_tskatr[TNUM_TSKID]    = {(TA_ACT),(TA_NULL),(TA_NULL),(TA_NULL),(TA_NULL)};
const uint_t	_kernel_init_rdypmap = 1U;
const intptr_t	_kernel_tinib_exinf[TNUM_TSKID]     = {(intptr_t)(1),(intptr_t)(2),(intptr_t)(3),(intptr_t)(4),(intptr_t)(5)};
const TASK    	_kernel_tinib_task[TNUM_TSKID]      = {(main),(task2),(mrb_sonic_thread),(mrb_ssp_thread_call),(mrb_ssp_thread_call)};
const uint_t  	_kernel_tinib_epriority[TNUM_TSKID] = {INT_PRIORITY(1),INT_PRIORITY(2),INT_PRIORITY(3),INT_PRIORITY(4),INT_PRIORITY(5)};

/*
 *  Cyclic Handler Functions
 */

const ID _kernel_tmax_cycid = (TMIN_CYCID + TNUM_CYCID - 1);
const uint_t _kernel_cycevtid_offset = 0;

const uint_t _kernel_cycinib_cycact = 0;
const intptr_t _kernel_cycinib_exinf[TNUM_CYCID] = {(intptr_t)(1),(intptr_t)(2),(intptr_t)(3),(intptr_t)(4),(intptr_t)(5)};
const CYCHDR _kernel_cycinib_cychdr[TNUM_CYCID] = {(mrb_ssp_cyclic_call),(mrb_ssp_cyclic_call),(mrb_sonic_sence_handler),(mrb_ssp_cyclic_call),(mrb_ssp_cyclic_call)};
const RELTIM _kernel_cycinib_cyctim[TNUM_CYCID] = {(2000),(2000),(10000),(100000),(50000)};
const RELTIM _kernel_cycinib_cycphs[TNUM_CYCID] = {(0),(0),(0),(0),(0)};

uint_t _kernel_cyccb_cycact;
EVTTIM _kernel_cyccb_evttim[TNUM_CYCID];


/*
 *  Alarm Handler Functions
 */

const ID _kernel_tmax_almid = (TMIN_ALMID + TNUM_ALMID - 1);
const uint_t _kernel_almevtid_offset = TNUM_CYCID;

const ALMHDR _kernel_alminib_almhdr[TNUM_ALMID] = {(mrb_ssp_alarm_call),(mrb_ssp_alarm_call),(mrb_ssp_alarm_call),(mrb_ssp_alarm_call)};
const intptr_t _kernel_alminib_exinf[TNUM_ALMID] = {(intptr_t)(1),(intptr_t)(2),(intptr_t)(3),(intptr_t)(4)};

uint_t _kernel_almcb_almact;

/*
 *  Time Event Management
 */

#define TNUM_TMEVT 9

const uint_t _kernel_tnum_tmevt_queue = 9;

QUEUE _kernel_tmevt_queue[TNUM_TMEVT+1];
EVTTIM _kernel_tmevt_time[TNUM_TMEVT];
CBACK _kernel_tmevt_callback[TNUM_TMEVT];
uintptr_t _kernel_tmevt_arg[TNUM_TMEVT];


#define MAX_TASK	8
#define MAX_CYCLIC	8
#define MAX_ALARM	8
#define MAX_SONIC	8

mrb_value task_self_tbl[MAX_TASK];
mrb_value cyclic_self_tbl[MAX_CYCLIC];
mrb_value alarm_self_tbl[MAX_ALARM];
mrb_value sonic_self_tbl[MAX_SONIC];

typedef struct 
{
	int phase;
	int event;
	int count;
	int accel_status;
	mrb_value	thread;
	int	cycid;
	int limit;
} Sonic_T;

Sonic_T sonic_cb;	


/*
 *  Stack Area for System
 */

#define TOPPERS_ISTKSZ		DEFAULT_ISTKSZ
static STK_T          		_kernel_stack[COUNT_STK_T(TOPPERS_ISTKSZ)];
#define TOPPERS_STK   		_kernel_stack
#define TOPPERS_STKSZ		ROUND_STK_T(TOPPERS_ISTKSZ)


const SIZE		_kernel_stksz = TOPPERS_STKSZ;
STK_T *const	_kernel_stk = TOPPERS_STK;

/*
 *  ��^�X�N�R���e�L�X�g�p�̃X�^�b�N�����l
 */

#define TOPPERS_ISTKPT( istk, istksz ) (( STK_T * )(( char * )( istk ) + ( istksz )))

#ifdef TOPPERS_ISTKPT
STK_T *const	_kernel_istkpt = TOPPERS_ISTKPT(TOPPERS_STK, TOPPERS_STKSZ);
#endif /* TOPPERS_ISTKPT */

/*
 *  Module Initialization Function
 */

void
_kernel_initialize_object(void)
{
	_kernel_initialize_time_event();
	_kernel_initialize_task();			//�K�v
//	_kernel_initialize_interrupt();		
//	_kernel_initialize_exception();		
	_kernel_initialize_cyclic();
	_kernel_initialize_alarm();

}


#ifndef TARGET_COPYRIGHT
#define TARGET_COPYRIGHT
#endif /* TARGET_COPYRIGHT */




