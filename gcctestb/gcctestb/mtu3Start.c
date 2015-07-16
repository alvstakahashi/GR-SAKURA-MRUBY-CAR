/***********************************************************************/
/*                                                                     */
/*  FILE        :mtu3Start.c                                           */
/*  DATE        :Thu, Jul 06, 2015                                     */
/*  DESCRIPTION :MTU3 Driver                                           */
/*  CPU TYPE    :RX63N                                                 */
/*                                                                     */
// PC7 �ɏo�͂��܂�.
// pulsWidth == 825 ��
//           == 1125 ���S
//			 == 1350 �E
/***********************************************************************/

#include "iodefine.h"

void mtu3_init(void)
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
void mtu3_Start(int pulsWidth)
{
    			          // �f���[�e�B�[  1100us �ɂ���
						  //  750KHz / (100000/1100)  = 825 
						  //  750KHz / (100000/1400)  = 1050
						  //  750KHz / (100000/1500)  = 1125
						  //  750KHz / (100000/1800)  = 1350
						
	MTU3.TGRB = (15000-pulsWidth);    
    /* MTU3��TCNT����J�n */
    MTU.TSTR.BIT.CST3 = 1;
}
void mtu3_Stop(void)
{
    /* MTU3��TCNT�����~ */
    MTU.TSTR.BIT.CST3 = 0;
}

