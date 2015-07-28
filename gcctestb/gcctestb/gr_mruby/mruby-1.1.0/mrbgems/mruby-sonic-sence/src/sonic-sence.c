#include "mruby.h"
#include "mruby/variable.h"
#include "mruby/string.h"

#include <kernel.h>
#include "kernel_cfg.h"

#include <string.h>
#include <stdio.h>

#include "iodefine.h"

#define	NOCOUNT		(-1)
// ����2m���J�E���g�I�[�o�[�Ƃ���
#define	COUNT_OVER	(2*100000/340)


#define PH00		0			// �������
#define	PH01		1			// �o�̓^�C�~���O�҂�
#define	PH02		2			// ��M�҂�

#define	EV00		0			//�N��������
#define	EV01		1			//�����A�b�v
#define	EV02		2			//start���\�b�h
#define	EV03		3			//stop���\�b�h

extern mrb_value sonic_self_tbl[];

extern mrb_state *mrb_global;

typedef struct 
{
	int phase;
	int event;
	int count;
	int accel_status;
	mrb_value	thread;
} Sonic_T;

extern  Sonic_T sonic_cb;	

//------------------------��������---------------------------------
#if 0
static void phase00(int cycid)
{
	mrb_value refCount;
	mrb_value self;

	switch(sonic_cb.event)
	{
	case EV02:				//start
		/* �o�̓|�[�g�Ɠ��̓|�[�g�̏����� */
		/* IO4*/
		PORT2.PDR.BIT.B4  = 1;		/*�o��*/
		PORT2.PODR.BIT.B4 = 0;		/*LOW*/

		//�ۑ��l��
		self = sonic_self_tbl[cycid-1];
		refCount = mrb_fixnum_value(NOCOUNT);
		mrb_iv_set(mrb_global, self, mrb_intern_lit(mrb_global, "@refCount"),refCount );
		
		sonic_cb.phase = PH01;
		break;
	default:
		break;
	}
}
static void phase01(int cycid)
{
	switch(sonic_cb.event)
	{
	case EV01:				//�����A�b�v
		sonic_cb.count = 0;
		PORT2.PODR.BIT.B4 = 1;		/* HIGH */
		sonic_cb.phase = PH02;
		break;
	default:
		break;
	}
}
static void phase02(int cycid)
{
	switch(sonic_cb.event)
	{
	case EV01:				//�����A�b�v
		PORT2.PODR.BIT.B4 = 0;		/* LOW */
		PORT2.PDR.BIT.B4  = 0;		/*����*/
		sonic_cb.count++;
		if (sonic_cb.count > COUNT_OVER)
		{
			//�ۑ��l��
			mrb_value self = sonic_self_tbl[cycid-1];
			mrb_value refCount = mrb_fixnum_value(NOCOUNT);
			mrb_iv_set(mrb_global, self, mrb_intern_lit(mrb_global, "@refCount"),refCount );
			
			PORT2.PDR.BIT.B4  = 1;		/*�o��*/
			PORT2.PODR.BIT.B4 = 0;		/*LOW*/
			sonic_cb.phase = PH01;
			break;
		}
		//�Z���T�I��?
		if (PORT2.PIDR.BIT.B4 != 0)
		{
			mrb_value self = sonic_self_tbl[cycid-1];
			mrb_value refCount = mrb_fixnum_value(sonic_cb.count);
			mrb_iv_set(mrb_global, self, mrb_intern_lit(mrb_global, "@refCount"),refCount );

			if (sonic_cb.accel_status != 0)
			{
				//�X���b�h�N��
				mrb_funcall(mrb_global,sonic_cb.thread, "iact",1,refCount);
			}
			PORT2.PDR.BIT.B4  = 1;		/*�o��*/
			PORT2.PODR.BIT.B4 = 0;		/*LOW*/
			sonic_cb.phase = PH01;
		}
		break;
	default:
		break;
	}
}
static void phase_proc(int cycid)
{
	switch(sonic_cb.phase)
	{
	case PH00:
			phase00(cycid);
			break;
	case PH01:
			phase01(cycid);
			break;
	case PH02:
			phase02(cycid);
			break;
	default:
		break;
	}
}
// ssp_thread �œo�^�����task�{��
void
mrb_sonic_sence_handler(intptr_t exf)
{
	sonic_cb.event = EV01;
	phase_proc((int)exf);
#if 1  	  
  	  printf("sonic_sence_handler\n");
#endif
}
//------------------------�����܂�---------------------------------
#else
extern void phase_proc(int id);
#endif

static mrb_value
mrb_sonic_sence_initialize(mrb_state *mrb, mrb_value self)
{
	// �����n���h��ID �ƃX���b�h�C���X�^���X���n�����
	  mrb_value cycid;
	  mrb_value thread;

//	  char *name_cstr;	
//	  mrb_int	id_num;

#if 0  	  
  	  printf("sonic_sence_initialize &self = %08x self = %08x %08x tbl=%08x\n",
  	  		  (int)&self,*((int*)&self),*(((int*)&self)+1),(int)task_self_tbl);
#endif

	  mrb_get_args(mrb, "oo",&cycid,&thread);

	  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@thread"),thread );
	  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@cycid"),cycid );

	  mrb_int  id_num = mrb_fixnum(cycid);
	  
	  sonic_self_tbl[id_num-1] = self;
/**/
	 memset(&sonic_cb,0,sizeof(Sonic_T));
	 mrb_value refCount = mrb_fixnum_value(NOCOUNT);
	 
	 mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@refCount"),refCount );

	sonic_cb.thread = thread;
	
/**/
	 sta_cyc(id_num);
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

	sonic_cb.event = EV02;
	phase_proc(id_num);
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
	mrb_define_method(mrb, sonic, "initialize", mrb_sonic_sence_initialize, ARGS_REQ(2));
	mrb_define_method(mrb, sonic, "start", mrb_sonic_sence_start, ARGS_NONE());
//	mrb_define_method(mrb, sonic, "stop", mrb_sonic_sence_stop, ARGS_NONE());
	mrb_define_method(mrb, sonic, "getTime", mrb_sonic_sence_gettime, ARGS_NONE());
	mrb_define_method(mrb, sonic, "setState", mrb_sonic_sence_setState, ARGS_REQ(1));

}

void
mrb_mruby_sonic_sence_gem_final(mrb_state* mrb) {
  // finalizer
}
