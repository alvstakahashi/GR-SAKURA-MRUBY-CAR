#include "mruby.h"
#include "mruby/variable.h"
#include "mruby/string.h"

#include <string.h>

#include "iodefine.h"


static void mtu3_init(void)
{
    /* PC7��MTIOC3A�ɐݒ� */
    MPC.PWPR.BIT.B0WI = 0;      // �s���@�\�I����
    MPC.PWPR.BIT.PFSWE = 1;     // �@�v���e�N�g����
    MPC.PC7PFS.BIT.PSEL = 0x01; // PC7��MTIOC3A�ɐ�
    MPC.PWPR.BIT.PFSWE = 0;     // �s���@�\�I����
    MPC.PWPR.BIT.B0WI = 1;      // �@�v���e�N�g�ݒ�
    PORTC.PMR.BIT.B7 = 1;       // PC7�͎��Ӌ@�\�Ƃ��Ďg�p

    SYSTEM.PRCR.WORD = 0xA502;  // �v���e�N�g����
    MSTP(MTU) = 0;              // MTU (MTU0�`MTU5)�̃X�g�b�v��ԉ���
    SYSTEM.PRCR.WORD = 0xA500;  // �v���e�N�g�ݒ�
    
    /* MTU3��TCNT�����~ */
    MTU.TSTR.BIT.CST3 = 0;

    /* TCNT�N���A */
    MTU3.TCNT = 0x0000;

    /* �J�E���^�N���b�N�ƃJ�E���^�N���A�v���̐ݒ� */
    // GR-SAKURA�ł́A�O�����U�q 12MHz, 
    // PLLCR.STC=16(16���{), PLLCR.PLIDIV=0(1����), SCKCR.PCKB=2(4����)
    // ���AMTU��PCLK�� 12*16/4 = 48MHz
//    MTU3.TCR.BIT.TPSC = 0;  // �J�E���^�N���b�N: PCLK/1 = 48MHz
    MTU3.TCR.BIT.TPSC = 3;  // �J�E���^�N���b�N: PCLK/64 = 750Khz
    MTU3.TCR.BIT.CKEG = 0;  // �����オ��G�b�W�ŃJ�E���g
    MTU3.TCR.BIT.CCLR = 1;  // TGRA�̃R���y�A�}�b�`��TCNT�N���A

    /* PWM���[�h1 */
    MTU3.TMDR.BYTE = 0x02;
    
    /* �g�`�o�̓��x���̑I�� */
    MTU3.TIORH.BIT.IOA = 1; // ������Ԃ�0�o�� �����R���y�A�}�b�`��0�o��
    MTU3.TIORH.BIT.IOB = 2; // �f���[�e�B�R���y�A�}�b�`��1�o��

    /* �����ƃf���[�e�B�[�̐ݒ� */
    						// GRA 20ms�ɂ���
    						// 20ms �́@50Hz   750KHz / 50 = 15000
    MTU3.TGRA = 15000;      
}
// pulsWidth  us���w��
static void mtu3_Start(int pulsWidth)
{
    			          // �f���[�e�B�[  1100us �ɂ���
						  //  750KHz / (100000/1100)  = 825 
						  //  750KHz / (100000/1400)  = 1050
						  //  750KHz / (100000/1500)  = 1125
						  //  750KHz / (100000/1800)  = 1350
						
	MTU3.TGRB = (15000-((750*pulsWidth)/1000));    
    /* MTU3��TCNT����J�n */
    MTU.TSTR.BIT.CST3 = 1;
}
static void mtu3_Stop(void)
{
    /* MTU3��TCNT�����~ */
    MTU.TSTR.BIT.CST3 = 0;
}


/*-----------------------------------------------------------*/
static mrb_value
mrb_rx_rcservo_initialize(mrb_state *mrb, mrb_value self)
{
	  mtu3_init();
	  return self;
}
static mrb_value
mrb_rx_rcservo_Start(mrb_state *mrb, mrb_value self)
{
	  mrb_value width;
	  mrb_get_args(mrb, "o",&width);

	  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@width"),width );

  	  mrb_int width_int = mrb_fixnum(width);

      mtu3_Start(width_int);

      return self;
}
static mrb_value
mrb_rx_rcservo_Stop(mrb_state *mrb, mrb_value self)
{
	  mtu3_Stop();
      return self;
}
void
mrb_mruby_rx_rcservo_gem_init(mrb_state* mrb) {


	struct RClass *rcservo;
	rcservo = mrb_define_class(mrb, "Rx_rcservo", mrb->object_class);

	/* methods */
	mrb_define_method(mrb, rcservo, "initialize", mrb_rx_rcservo_initialize,ARGS_NONE());
	mrb_define_method(mrb, rcservo, "Start", mrb_rx_rcservo_Start, ARGS_REQ(1));
	mrb_define_method(mrb, rcservo, "Stop", mrb_rx_rcservo_Stop, ARGS_NONE());

}

void
mrb_mruby_rx_rcservo_gem_final(mrb_state* mrb) {
  // finalizer
}
