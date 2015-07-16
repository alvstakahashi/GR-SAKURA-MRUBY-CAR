#include "mruby.h"
#include "mruby/string.h"
#include <stdio.h>
#include <string.h>

#include "rx63nsci0.h"

mrb_value
mrb_rx_getc(mrb_state *mrb, mrb_value self)
{
  char tbl[2];
  tbl[0] = CPU_UartGetch();
  tbl[1] = 0x00;

  mrb_value result_str = mrb_str_new(mrb, tbl, strlen(tbl));

  return result_str;
}

void
mrb_mruby_rx_getc_gem_init(mrb_state* mrb)
{
  struct RClass *krn;
  krn = mrb->kernel_module;
  mrb_define_method(mrb, krn, "Rx_getc", mrb_rx_getc,ARGS_NONE());
}

void
mrb_mruby_rx_getc_gem_final(mrb_state* mrb)
{
}
