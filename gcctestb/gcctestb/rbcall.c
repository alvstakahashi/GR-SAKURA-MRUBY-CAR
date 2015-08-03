#include <stdint.h>
#include "mruby.h"
#include "mruby/proc.h"
#include "mruby/data.h"
#include "mruby/compile.h"
#include "mruby/string.h"
#include "mruby/numeric.h"
#include "mruby/dump.h"
#include <math.h>
#include <stdlib.h>
#include <stdint.h>

#include "kernel_impl.h"
#include "task.h"

#include "kernel_cfg.h"

extern struct mrb_context *ruby_ctx_top[];
extern struct mrb_context *ruby_ctx_current[];

mrb_state *mrb_global;
mrb_irep *idx_code1 = 0;

struct mrb_context *
stack_init(mrb_state *mrb,struct mrb_context *c);

//void mrb_ci_cp(struct mrb_context *to,struct mrb_context *from)
//{
//	to->cibase = from->cibase;
//	to->ciend  = from->ciend;
//	to->ci     = from->ci;
//}

mrb_value
mrb_ssp_delay_delayDEBUG(mrb_state *mrb, mrb_value self)
{
	ER retval;
	mrb_int	tout;
    mrb_value argv;
	uint_t	current_task;
	mrb_callinfo *dummy;
	int i;

  	mrb_get_args(mrb, "o", &argv);
  	tout = mrb_fixnum(argv);
#if 1
	printf("mrb_ssp_delay tout = %d\n",tout);
#endif

//	int ai = mrb_gc_arena_save(mrb);
//	t_unlock_cpu();
	retval = dly_tsk(tout);
//	t_lock_cpu();
	return argv;
}

void mruby_call(const uint8_t *mrb_p )
{
    mrb_global = mrb_open();
	if (mrb_global == NULL) 
	{
		return ;
	}
	mrb_value return_value1;
	if (idx_code1 == 0) 
	{
		idx_code1 = mrb_read_irep(mrb_global, mrb_p);
		if (idx_code1 == 0) 
		{
			// irep_error(mrb, idx_code1);
			return ;
		}
	}
	if (idx_code1 != 0) 
	{
//		int ai = mrb_gc_arena_save(mrb_global);
//		t_lock_cpu();
	  	
		return_value1 =  mrb_run(mrb_global, mrb_proc_new(mrb_global, idx_code1),mrb_top_self(mrb_global));
//		t_unlock_cpu();
//		mrb_gc_arena_restore(mrb_global, ai);
	}
}
