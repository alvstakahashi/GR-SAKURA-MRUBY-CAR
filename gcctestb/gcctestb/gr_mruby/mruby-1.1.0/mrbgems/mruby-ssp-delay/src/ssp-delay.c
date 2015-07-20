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

  	mrb_get_args(mrb, "o", &argv);
  	tout = mrb_fixnum(argv);
#if 0
	printf("mrb_ssp_delay tout = %d\n",tout);
#endif
//	int ai = mrb_gc_arena_save(mrb);
	retval = dly_tsk(tout);
//  	mrb_gc_arena_restore(mrb, ai);
	return argv;
}

void
mrb_mruby_ssp_delay_gem_init(mrb_state* mrb)
{
  	struct RClass *krn;
  	krn = mrb->kernel_module;
  	mrb_define_method(mrb, krn, "Ssp_delay", mrb_ssp_delay_delay, MRB_ARGS_REQ(1));
}

//  struct RClass *krn;
//  krn = mrb->kernel_module;
//  mrb_define_module_function(mrb, mrb->kernel_module, "eval", f_eval, MRB_ARGS_ARG(1, 3));
//  	mrb_define_method(mrb, mrb->kernel_module, "delay", mrb_ssp_delay,ARGS_NONE());
//  	 MRB_ARGS_REQ(1));
//    mrb_define_module_function(mrb, mrb->kernel_module, "Ssp_delay", mrb_ssp_delay, MRB_ARGS_REQ(1));
//  mrb_define_method(mrb, krn, "Ssp_delay", mrb_ssp_delay, MRB_ARGS_REQ(1));

void
mrb_mruby_ssp_delay_gem_final(mrb_state* mrb)
{
}
