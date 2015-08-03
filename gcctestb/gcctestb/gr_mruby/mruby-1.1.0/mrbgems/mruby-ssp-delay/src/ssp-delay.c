#include "mruby.h"
#include "mruby/string.h"
#include <stdio.h>
#include <string.h>
#include "kernel_impl.h"
#include "task.h"

#include "kernel_cfg.h"

mrb_value
mrb_ssp_delay_delayDEBUG(mrb_state *mrb, mrb_value self);

mrb_value
mrb_ssp_delay_delay(mrb_state *mrb, mrb_value self)
{
}

void
mrb_mruby_ssp_delay_gem_init(mrb_state* mrb)
{
  	struct RClass *krn;
  	krn = mrb->kernel_module;
  	mrb_define_method(mrb, krn, "Ssp_delay", mrb_ssp_delay_delayDEBUG, MRB_ARGS_REQ(1));
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
