#include "mruby.h"
#include "mruby/string.h"
#include <stdio.h>
#include <string.h>

#include <kernel.h>

mrb_value
mrb_ssp_delay_delay(mrb_state *mrb, mrb_value self)
{
	ER retval;
	mrb_int	tout;
    mrb_value argv;
    
    SYSTIM  startTime;
    SYSTIM  targetTime;
    SYSTIM	currentTime;

  	mrb_get_args(mrb, "o", &argv);
  	tout = mrb_fixnum(argv);
#if 1
	printf("mrb_ssp_delay tout = %d\n",tout);
#endif
//	int ai = mrb_gc_arena_save(mrb);
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
}


void
mrb_mruby_ssp_delay_gem_final(mrb_state* mrb)
{
}
