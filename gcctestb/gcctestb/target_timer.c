/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2003-2004 by Naoki Saito
 *             Nagoya Municipal Industrial Research Institute, JAPAN
 *  Copyright (C) 2003-2004 by Platform Development Center
 *                                          RICOH COMPANY,LTD. JAPAN
 *  Copyright (C) 2008-2010 by Witz Corporation, JAPAN
 * 
 *  ��L���쌠�҂́C�ȉ���(1)�`(4)�̏����𖞂����ꍇ�Ɍ���C�{�\�t�g�E�F
 *  �A�i�{�\�t�g�E�F�A�����ς������̂��܂ށD�ȉ������j���g�p�E�����E��
 *  �ρE�Ĕz�z�i�ȉ��C���p�ƌĂԁj���邱�Ƃ𖳏��ŋ�������D
 *  (1) �{�\�t�g�E�F�A���\�[�X�R�[�h�̌`�ŗ��p����ꍇ�ɂ́C��L�̒���
 *      ���\���C���̗��p��������щ��L�̖��ۏ؋K�肪�C���̂܂܂̌`�Ń\�[
 *      �X�R�[�h���Ɋ܂܂�Ă��邱�ƁD
 *  (2) �{�\�t�g�E�F�A���C���C�u�����`���ȂǁC���̃\�t�g�E�F�A�J���Ɏg
 *      �p�ł���`�ōĔz�z����ꍇ�ɂ́C�Ĕz�z�ɔ����h�L�������g�i���p
 *      �҃}�j���A���Ȃǁj�ɁC��L�̒��쌠�\���C���̗��p��������щ��L
 *      �̖��ۏ؋K����f�ڂ��邱�ƁD
 *  (3) �{�\�t�g�E�F�A���C�@��ɑg�ݍ��ނȂǁC���̃\�t�g�E�F�A�J���Ɏg
 *      �p�ł��Ȃ��`�ōĔz�z����ꍇ�ɂ́C���̂����ꂩ�̏����𖞂�����
 *      �ƁD
 *    (a) �Ĕz�z�ɔ����h�L�������g�i���p�҃}�j���A���Ȃǁj�ɁC��L�̒�
 *        �쌠�\���C���̗��p��������щ��L�̖��ۏ؋K����f�ڂ��邱�ƁD
 *    (b) �Ĕz�z�̌`�Ԃ��C�ʂɒ�߂���@�ɂ���āCTOPPERS�v���W�F�N�g��
 *        �񍐂��邱�ƁD
 *  (4) �{�\�t�g�E�F�A�̗��p�ɂ�蒼�ړI�܂��͊ԐړI�ɐ����邢���Ȃ鑹
 *      �Q������C��L���쌠�҂����TOPPERS�v���W�F�N�g��Ɛӂ��邱�ƁD
 *      �܂��C�{�\�t�g�E�F�A�̃��[�U�܂��̓G���h���[�U����̂����Ȃ闝
 *      �R�Ɋ�Â�����������C��L���쌠�҂����TOPPERS�v���W�F�N�g��
 *      �Ɛӂ��邱�ƁD
 * 
 *  �{�\�t�g�E�F�A�́C���ۏ؂Œ񋟂���Ă�����̂ł���D��L���쌠�҂�
 *  ���TOPPERS�v���W�F�N�g�́C�{�\�t�g�E�F�A�Ɋւ��āC����̎g�p�ړI
 *  �ɑ΂���K�������܂߂āC�����Ȃ�ۏ؂��s��Ȃ��D�܂��C�{�\�t�g�E�F
 *  �A�̗��p�ɂ�蒼�ړI�܂��͊ԐړI�ɐ����������Ȃ鑹�Q�Ɋւ��Ă��C��
 *  �̐ӔC�𕉂�Ȃ��D
 * 
 */

/*
 *		�^�C�}�h���C�o�iFRK-RX62N�p�j
 */

#include "kernel_impl.h"
#include "time_event.h"
#include <sil.h>
#include "target_timer.h"

/*
 *  �^�C�}�̏���������
 */
void
target_timer_initialize(intptr_t exinf)
{
	/*
	 * ���W���[���X�g�b�v�@�\�̐ݒ�(CMT�ASCI1)
	 */
	*SYSTEM_MSTPCRA_ADDR &= ~(SYSTEM_MSTPCRA_MSTPA15_BIT); /* CMT0 */

	/* 
	 * �^�C�}��~ 
	 */
	*CMT_CMSTR0_ADDR &= ~CMT_CMSTR0_STR0_BIT;
	
	/*
 	 * �J�E���g�A�b�v�ɗp������N���b�N�ݒ�
	 * PCLK/32��I��
	 */
	*CMT0_CMCR_ADDR = CMT_PCLK_DIV_8;

	/* 
	 * �R���y�A�}�b�`�^�C�}�J�E���^�ݒ�
	 */
	*CMT0_CMCNT_ADDR = 0U;

	/* 
	 * �R���y�A�}�b�`�^�C�}�����ݒ�
	 */
	*CMT0_CMCOR_ADDR = CMCOR_PERIOD;

	/*
	 * �R���y�A�}�b�`�^�C�}���荞�ݗv����ݒ背�W�X�^�iISELR28�j
	 */
	*ICU_ISELR028_ADDR = ICU_ISEL_CPU;

	/*
	 *  �^�C�}����J�n�O�̊����ݗv�����N���A
	 */
	x_clear_int(INTNO_TIMER);

	/* 
	 * �R���y�A�}�b�`�^�C�}���荞�݂�����
	 */
	*CMT0_CMCR_ADDR |= CMT0_CMCR_CMIE_BIT;
	
	/* 
	 * �^�C�}����J�n
	 */
	*CMT_CMSTR0_ADDR |= CMT_CMSTR0_STR0_BIT;
}

/*
 *  �^�C�}�̒�~����
 */
void
target_timer_terminate(intptr_t exinf)
{
	/* 
	 * �^�C�}��~ 
	 */
	*CMT_CMSTR0_ADDR &= ~CMT_CMSTR0_STR0_BIT;
}

/*
 *  �^�C�}�����݃n���h��
 */
void target_timer_handler(void)
{
	i_begin_int(INTNO_TIMER);
	
	signal_time();				/* �^�C���e�B�b�N�̋��� */
	
	i_end_int(INTNO_TIMER);
}
