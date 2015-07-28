#include "mruby.h"
#include "mruby/variable.h"
#include "mruby/string.h"

#include <kernel.h>
#include "kernel_cfg.h"

#include <string.h>
#include <stdio.h>


extern mrb_value task_self_tbl[];

extern mrb_state *mrb_global;


static mrb_value
mrb_ssp_thread_initialize(mrb_state *mrb, mrb_value self)
{
	  mrb_value id;
	  mrb_value name;
	  char *name_cstr;	
	  mrb_int	id_num;

#if 1  	  
  	  printf("thread_initialize &self = %08x self = %08x %08x tbl=%08x\n",
  	  		  (int)&self,*((int*)&self),*(((int*)&self)+1),(int)task_self_tbl);
#endif

	  mrb_get_args(mrb, "So",&name, &id);

	  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@task_id"),id );
	  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@task_name"),name );

	  id_num = mrb_fixnum(id);
	  
	  task_self_tbl[id_num-1] = self;

//	  memcpy(&task_self_tbl[id_num-1],&self,sizeof(mrb_value));


  	  name_cstr = mrb_str_to_cstr(mrb, name);

#if 1
	printf("mrb_ssp_thread_initialize cstr=%s id = %d\n",name_cstr,id_num);
#endif
	  return self;
}
static mrb_value
mrb_ssp_thread_act(mrb_state *mrb, mrb_value self)
{
	ER retval;
	mrb_value id   = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@task_id"));
	mrb_int id_num = mrb_fixnum(id);

	retval = act_tsk(id_num);

	if (retval != E_OK)
	{
		return(mrb_false_value());
	}
	return(mrb_true_value());
}
static mrb_value
mrb_ssp_thread_iact(mrb_state *mrb, mrb_value self)
{
	ER retval;
	mrb_value id   = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@task_id"));
	mrb_int id_num = mrb_fixnum(id);
	retval = iact_tsk(id_num);

	if (retval != E_OK)
	{
		return(mrb_false_value());
	}
	return(mrb_true_value());
}

// ssp_thread ‚Å“o˜^‚³‚ê‚étask–{‘Ì
void
mrb_ssp_thread_call(intptr_t exf)
{
	mrb_int task_id = (mrb_int)exf;		// taskID
	mrb_value self;
	char name_cstr[20];
	
//	memcpy(&self,&task_self_tbl[task_id-1],sizeof(mrb_value));

	self = task_self_tbl[task_id-1];


#if 1  	  
  	  printf("thread_call &self = %08x self = %08x %08x\n",
  	  		  (int)&self,*((int*)&self),*(((int*)&self)+1));
#endif

	
	mrb_value name = mrb_iv_get(mrb_global, self, mrb_intern_lit(mrb_global, "@task_name"));
	mrb_value id   = mrb_iv_get(mrb_global, self, mrb_intern_lit(mrb_global, "@task_id"));
// 	char *name_cstr = mrb_str_to_cstr(mrb_global, name);	
    strncpy(name_cstr, RSTRING_PTR(name), RSTRING_LEN(name));
    name_cstr[RSTRING_LEN(name)]='\0';

    
#if 1
	printf("mrb_ssp_thread_call exf = %d cstr=%s id = %d\n",
			exf,name_cstr,mrb_fixnum(id));
#endif
	
#if 0
	mrb_funcall(mrb_global, mrb_top_self(mrb_global), "thread",1, id);
#else
	mrb_funcall(mrb_global, self, (const char *)name_cstr,1, id);
//	mrb_funcall(mrb_global, self, "thread",1, id);
#endif
}
	
void
mrb_mruby_ssp_thread_gem_init(mrb_state* mrb) {


	struct RClass *ssp;
	ssp = mrb_define_class(mrb, "Ssp_thread", mrb->object_class);

	/* methods */
	mrb_define_method(mrb, ssp, "initialize", mrb_ssp_thread_initialize, ARGS_REQ(2));
	mrb_define_method(mrb, ssp, "act" , mrb_ssp_thread_act, ARGS_NONE());
	mrb_define_method(mrb, ssp, "iact", mrb_ssp_thread_iact, ARGS_NONE());

}

void
mrb_mruby_ssp_thread_gem_final(mrb_state* mrb) {
  // finalizer
}
