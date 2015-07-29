#include "mruby.h"
#include "mruby/variable.h"
#include "mruby/string.h"

#include <kernel.h>
#include "kernel_cfg.h"

#include <string.h>
#include <stdio.h>

#include "iodefine.h"

#define LOW_P		0
#define	HIGH_P		1


extern void us_delay(int tout);
extern void ms_delay(int tout);

extern mrb_value sonic_self_tbl[];

extern mrb_state *mrb_global;

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

extern  Sonic_T sonic_cb;	


void
mrb_sonic_sence_handler(intptr_t exf)
{
	iact_tsk(SONIC_TSK);
}
// 周期ハンドラで起動される
// 開始のコマンドを送って終了する。
void mrb_sonic_thread(intptr_t exf)
{
	PORT2.PDR.BIT.B4  = 1;		/*出力モード*/
	PORT2.PODR.BIT.B4 = LOW_P;	/*LOW*/
	us_delay(2);
	PORT2.PODR.BIT.B4 = HIGH_P;	/* HIGH */
	us_delay(5);
	PORT2.PODR.BIT.B4 = LOW_P;	/*LOW*/
	PORT2.PDR.BIT.B4  = 0;		/*入力モード*/

	sonic_cb.count = 0;
	while(PORT2.PIDR.BIT.B4 == LOW_P)
	{
		us_delay(1);
		if (++sonic_cb.count > 500)
		{
			return;
		}
	}
	sonic_cb.count = 0;
	while(PORT2.PIDR.BIT.B4 == HIGH_P)
	{
		us_delay(1);
		if (++sonic_cb.count > sonic_cb.limit)	// 1M以上は未検地
		{
			return;
		}
	}
	printf("sonic count = %d\n",sonic_cb.count);
		
	mrb_value self = sonic_self_tbl[sonic_cb.cycid-1];
	mrb_value refCount = mrb_fixnum_value(sonic_cb.count);
	mrb_iv_set(mrb_global, self, mrb_intern_lit(mrb_global, "@refCount"),refCount );
	
	if (sonic_cb.accel_status != 0)
	{
		//スレッド起動
		mrb_value status = mrb_funcall(mrb_global,sonic_cb.thread, "act",1,refCount);
		if (mrb_fixnum(status) != 0)
		{
			sonic_cb.accel_status = 0;
		}
	}
}

static mrb_value
mrb_sonic_sence_initialize(mrb_state *mrb, mrb_value self)
{
	// 周期ハンドラID とスレッドインスタンスと閾値が渡される
	  mrb_value cycid;
	  mrb_value thread;
	  mrb_value limit;

//	  char *name_cstr;	
//	  mrb_int	id_num;

#if 0  	  
  	  printf("sonic_sence_initialize &self = %08x self = %08x %08x tbl=%08x\n",
  	  		  (int)&self,*((int*)&self),*(((int*)&self)+1),(int)task_self_tbl);
#endif

	  mrb_get_args(mrb, "ooo",&cycid,&thread,&limit);

	  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@thread"),thread );
	  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@cycid"),cycid );

	  mrb_int  id_num = mrb_fixnum(cycid);
	  
	  sonic_self_tbl[id_num-1] = self;
/**/
	memset(&sonic_cb,0,sizeof(Sonic_T));
	sonic_cb.thread = thread;
	sonic_cb.cycid  = id_num;
	sonic_cb.limit  = mrb_fixnum(limit);
/**/
#if 1
	printf("mrb_sonic_sence_initialize id = %d\n",id_num);
#endif
	  return self;
}
static mrb_value
mrb_sonic_sence_start(mrb_state *mrb, mrb_value self)
{
	
//	ER retval;
	mrb_value id   = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@cycid"));
	mrb_int id_num = mrb_fixnum(id);

	sta_cyc(id_num);
}	
static mrb_value
mrb_sonic_sence_gettime(mrb_state *mrb, mrb_value self)
{
	mrb_value time   = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@refCount"));
	return time;
}
static mrb_value
mrb_sonic_sence_setState(mrb_state *mrb, mrb_value self)
{
	mrb_get_args(mrb, "i",&sonic_cb.accel_status);
	return self;
}
	
void
mrb_mruby_sonic_sence_gem_init(mrb_state* mrb) {


	struct RClass *sonic;
	sonic = mrb_define_class(mrb, "SonicSence", mrb->object_class);

	/* methods */
	mrb_define_method(mrb, sonic, "initialize", mrb_sonic_sence_initialize, ARGS_REQ(3));
	mrb_define_method(mrb, sonic, "start", mrb_sonic_sence_start, ARGS_NONE());
//	mrb_define_method(mrb, sonic, "stop", mrb_sonic_sence_stop, ARGS_NONE());
	mrb_define_method(mrb, sonic, "getTime", mrb_sonic_sence_gettime, ARGS_NONE());
	mrb_define_method(mrb, sonic, "setState", mrb_sonic_sence_setState, ARGS_REQ(1));

}

void
mrb_mruby_sonic_sence_gem_final(mrb_state* mrb) {
  // finalizer
}
