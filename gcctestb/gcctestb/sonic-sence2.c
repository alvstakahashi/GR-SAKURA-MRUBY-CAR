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

#define LOW_P		0
#define	HIGH_P		1

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

Sonic_T sonic_cb;	



void phase00(int cycid)
{
	mrb_value refCount;
	mrb_value self;

	switch(sonic_cb.event)
	{
	case EV02:				//start
		/* �o�̓|�[�g�Ɠ��̓|�[�g�̏����� */
		/* IO4*/
		PORT2.PDR.BIT.B4  = 1;		/*�o��*/
		PORT2.PODR.BIT.B4 = LOW_P;		/*LOW*/

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
void phase01(int cycid)
{
	switch(sonic_cb.event)
	{
	case EV01:				//�����A�b�v
		sonic_cb.count = 0;
		PORT2.PODR.BIT.B4 = HIGH_P;		/* HIGH */
		sonic_cb.phase = PH02;
		break;
	default:
		break;
	}
}
void phase02(int cycid)
{
	switch(sonic_cb.event)
	{
	case EV01:				//�����A�b�v
		if (PORT2.PDR.BIT.B4 !=0)	//�o�͒�
		{
			PORT2.PODR.BIT.B4 = LOW_P;		/* LOW */
			PORT2.PDR.BIT.B4  = 0;		/*����*/
		}
		sonic_cb.count++;
		if (sonic_cb.count > COUNT_OVER)
		{
			//�ۑ��l��
			mrb_value self = sonic_self_tbl[cycid-1];
			mrb_value refCount = mrb_fixnum_value(NOCOUNT);
			mrb_iv_set(mrb_global, self, mrb_intern_lit(mrb_global, "@refCount"),refCount );
			
			PORT2.PDR.BIT.B4  = 1;		/*�o��*/
			PORT2.PODR.BIT.B4 = LOW_P;		/*LOW*/

			sonic_cb.phase = PH01;
			break;
		}
		//�Z���T�I��?
		if ((PORT2.PIDR.BIT.B4 != 0) && (sonic_cb.count > 200))
		{
			mrb_value self = sonic_self_tbl[cycid-1];
			mrb_value refCount = mrb_fixnum_value(sonic_cb.count);
			mrb_iv_set(mrb_global, self, mrb_intern_lit(mrb_global, "@refCount"),refCount );

			if (sonic_cb.accel_status != 0)
			{
				//�X���b�h�N��
				mrb_funcall(mrb_global,sonic_cb.thread, "iact",1,refCount);
				sonic_cb.accel_status = 0;
			}

			PORT2.PDR.BIT.B4  = 1;		/*�o��*/
			PORT2.PODR.BIT.B4 = LOW_P;		/*LOW*/

			sonic_cb.phase = PH01;
		}
		break;
	default:
		break;
	}
}
void phase_proc(int cycid)
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
#if 0  	  
  	  printf("sonic_sence_handler\n");
#endif
}

