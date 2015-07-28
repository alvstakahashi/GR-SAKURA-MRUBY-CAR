#include "mruby.h"
#include "mruby/string.h"
#include <stdio.h>
#include <string.h>

#include <kernel.h>

#include <iodefine.h>

void us_delay(int tout);
void ms_delay(int tout);

#define 	US_COUNT		6		// このカウントで1us
#define 	US_MAX			60		// 最大カウント
void us_delay(int tout)
{
    unsigned short  startTime;
    unsigned short  targetTime;
    unsigned short	currentTime;

	startTime = CMT0.CMCNT;
	targetTime = startTime + tout*US_COUNT;
	if (targetTime >= US_MAX)
	{
		targetTime -= US_MAX;
	}
	if (targetTime < startTime)		// オーバーフロー
	{
		while(1)
		{
			currentTime = CMT0.CMCNT;
			if (startTime > currentTime)	//オーバーフロー
			{
				break;
			}
		}
	}
	while(1)
	{
		currentTime = CMT0.CMCNT;
		if (currentTime >= targetTime)
		{
			break;
		}
	}
}
void ms_delay(int tout)
{
    SYSTIM  startTime;
    SYSTIM  targetTime;
    SYSTIM	currentTime;

	get_tim(&startTime);
	targetTime = startTime + tout*100;
	if (targetTime < startTime)		// オーバーフロー
	{
		while(1)
		{
			get_tim(&currentTime);
			if (startTime > currentTime)	//オーバーフロー
			{
				break;
			}
		}
	}
	while(1)
	{
		get_tim(&currentTime);
		if (currentTime >= targetTime)
		{
			break;
		}
	}
}

mrb_value
mrb_ssp_delay_delay(mrb_state *mrb, mrb_value self)
{
	ER retval;
	mrb_int	tout;
    mrb_value argv;
    

  	mrb_get_args(mrb, "o", &argv);
  	tout = mrb_fixnum(argv);
#if 1
	printf("mrb_ssp_delay tout = %d\n",tout);
#endif
//	int ai = mrb_gc_arena_save(mrb);
	ms_delay(tout);

//	retval = dly_tsk(tout);
//  	mrb_gc_arena_restore(mrb, ai);
	return argv;
}

mrb_value
mrb_ssp_delay_us(mrb_state *mrb, mrb_value self)
{
	mrb_int	tout;
    mrb_value argv;
    

  	mrb_get_args(mrb, "o", &argv);
  	tout = mrb_fixnum(argv);
#if 0
	printf("mrb_ssp_delay_us tout = %d\n",tout);
#endif
//	int ai = mrb_gc_arena_save(mrb);
	us_delay(tout);

//	retval = dly_tsk(tout);
//  	mrb_gc_arena_restore(mrb, ai);
	return argv;
}

void
mrb_mruby_ssp_delay_gem_init(mrb_state* mrb)
{
  	struct RClass *krn;
  	krn = mrb->kernel_module;
  	mrb_define_method(mrb, krn, "Ssp_Delay", mrb_ssp_delay_delay, MRB_ARGS_REQ(1));
  	mrb_define_method(mrb, krn, "Ssp_usDelay", mrb_ssp_delay_us, MRB_ARGS_REQ(1));
}


void
mrb_mruby_ssp_delay_gem_final(mrb_state* mrb)
{
}
