; 
;   TOPPERS/SSP Kernel
;       Smallest Set Profile Kernel
;  
;   Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
;                               Toyohashi Univ. of Technology, JAPAN
;   Copyright (C) 2007      by Embedded and Real-Time Systems Laboratory
;               Graduate School of Information Science, Nagoya Univ., JAPAN
;  
;   Copyright (C) 2010 by Witz Corporation, JAPAN
;   ��L���쌠�҂́C�ȉ���(1)�`(4)�̏����𖞂����ꍇ�Ɍ���C�{�\�t�g�E�F
;   �A�i�{�\�t�g�E�F�A�����ς������̂��܂ށD�ȉ������j���g�p�E�����E��
;   �ρE�Ĕz�z�i�ȉ��C���p�ƌĂԁj���邱�Ƃ𖳏��ŋ�������D
;   (1) �{�\�t�g�E�F�A���\�[�X�R�[�h�̌`�ŗ��p����ꍇ�ɂ́C��L�̒���
;       ���\���C���̗��p��������щ��L�̖��ۏ؋K�肪�C���̂܂܂̌`�Ń\�[
;       �X�R�[�h���Ɋ܂܂�Ă��邱�ƁD
;   (2) �{�\�t�g�E�F�A���C���C�u�����`���ȂǁC���̃\�t�g�E�F�A�J���Ɏg
;       �p�ł���`�ōĔz�z����ꍇ�ɂ́C�Ĕz�z�ɔ����h�L�������g�i���p
;       �҃}�j���A���Ȃǁj�ɁC��L�̒��쌠�\���C���̗��p��������щ��L
;       �̖��ۏ؋K����f�ڂ��邱�ƁD
;   (3) �{�\�t�g�E�F�A���C�@��ɑg�ݍ��ނȂǁC���̃\�t�g�E�F�A�J���Ɏg
;       �p�ł��Ȃ��`�ōĔz�z����ꍇ�ɂ́C���̂����ꂩ�̏����𖞂�����
;       �ƁD
;     (a) �Ĕz�z�ɔ����h�L�������g�i���p�҃}�j���A���Ȃǁj�ɁC��L�̒�
;         �쌠�\���C���̗��p��������щ��L�̖��ۏ؋K����f�ڂ��邱�ƁD
;     (b) �Ĕz�z�̌`�Ԃ��C�ʂɒ�߂���@�ɂ���āCTOPPERS�v���W�F�N�g��
;         �񍐂��邱�ƁD
;   (4) �{�\�t�g�E�F�A�̗��p�ɂ�蒼�ړI�܂��͊ԐړI�ɐ����邢���Ȃ鑹
;       �Q������C��L���쌠�҂����TOPPERS�v���W�F�N�g��Ɛӂ��邱�ƁD
;       �܂��C�{�\�t�g�E�F�A�̃��[�U�܂��̓G���h���[�U����̂����Ȃ闝
;       �R�Ɋ�Â�����������C��L���쌠�҂����TOPPERS�v���W�F�N�g��
;       �Ɛӂ��邱�ƁD
;  
;   �{�\�t�g�E�F�A�́C���ۏ؂Œ񋟂���Ă�����̂ł���D��L���쌠�҂�
;   ���TOPPERS�v���W�F�N�g�́C�{�\�t�g�E�F�A�Ɋւ��āC����̎g�p�ړI
;   �ɑ΂���K�������܂߂āC�����Ȃ�ۏ؂��s��Ȃ��D�܂��C�{�\�t�g�E�F
;   �A�̗��p�ɂ�蒼�ړI�܂��͊ԐړI�ɐ����������Ȃ鑹�Q�Ɋւ��Ă��C��
;   �̐ӔC�𕉂�Ȃ��D
;  
;

;
;  �v���Z�b�T�ˑ����W���[�� �A�Z���u�����ꕔ�iRX600�V���[�Y�p�j
;

;
;  �f�B�X�p�b�`������ъ�����(CPU��O)�o������̃��[��:
;	���샂�[�h���ȉ��̂悤�ɒ�`����.
;		�f�B�X�p�b�`�����[�h:
;			CPU���b�N���, �����ݗD��x�}�X�N�S�������,
;			�^�X�N�R���e�L�X�g(__kernel_intnest = 0), �^�X�N�X�^�b�N
;		������(CPU��O)�������[�h
;			�S�����݃��b�N���(PSW���W�X�^I�r�b�g = 0),
;			�����ݗD��x�}�X�N�S�����łȂ����(IPL != 0)
;			�����݃R���e�L�X�g(__kernel_intnest != 0), �����݃X�^�b�N
;
;	�J�[�l���Ǘ��O�����݂̃T�|�[�g�L����, CPU���b�N���, �����ݗD��x
;	�}�X�N�S������Ԃ̊֌W�͈ȉ��̒ʂ�ł���.
;		�J�[�l���Ǘ��O�����ݖ��T�|�[�g��:
;			CPU���b�N���(PSW���W�X�^I�r�b�g = 0)
;			�����ݗD��x�}�X�N�S�������(IPL = 0)
;		�J�[�l���Ǘ��O�����݃T�|�[�g��:
;			CPU���b�N���
;				(PSW���W�X�^I�r�b�g = 0, IPL = IPL_LOCK, lock_flag = true)
;			�����ݗD��x�}�X�N�S�������(saved_ipl = 0)
;
;	�e�\���̃|�C���^���ȉ��̂悤�Ɋe���W�X�^�ɃA�T�C������.
;		r15 = p_runtsk	�������f�B�X�p�b�`���̊e�o���ł͖���
;		r14 = *p_runtsk	dispatcher �̒��ł� p_runtsk �m�莞�ɍĎ擾����
;	�e���������ŏ��ɕϐ��A�N�Z�X����Ƃ��ɏ�L���W�X�^�ɕۑ�����.
;
;	�\���̃A���C�����g�ւ̑Ή�
;		�\���̃A���C�����g��4Byte(�A���p�b�N)�̏ꍇ:
;			��ʓI�ȃ��W�X�^���΃A�h���b�V���O���\
;				��:	mov.l	#__kernel_p_runtsk, r15
;					mov.l  	r0, TCB_sp[r15]
;		�\���̃A���C�����g��4Byte�ł͂Ȃ�(�p�b�N)�̏ꍇ:
;			mov.l�̂悤�Ƀ����O�T�C�Y�w��̏ꍇ�A���Βl��4�̔{���̂ݗL��
;			���̂���, ��x�ΏۃA�h���X�����߂Ă���A�N�Z�X����K�v������
;				��:	mov.l	#__kernel_p_runtsk, r15
;					add		#TCB_sp, r15, r5
;					mov.l  	r0, [r5]
;		�e�I�t�Z�b�g�l�𔻒f��, �����A�Z���u���ɂ��R�[�h��؂�ւ���
;

;
;  �e��EQU��`(H�t�@�C����#define��`)
;
;	.include	asm_config.inc
#include "asm_config.inc"

;	.glb	__kernel_default_int_handler	; takahashi
	.glb 	__kernel_default_int_handler_entry
	.glb 	__kernel_default_int_handler_entry_c
	.glb	_CMI0



	.glb	__kernel_istkpt
	
	.glb	__kernel_reqflg

	
	.glb	__kernel_search_schedtsk
	.glb	__kernel_run_task

	.glb	_kernel_interrupt
	.glb	__kernel_intnest
	.glb	_current_psw

	.section	P, CODE


_current_psw:
	mvfc	psw, r1
	rts

;----------------------------------------------------------------------
;	���荞�݃x�N�^�o�^���璼�ڂ���Ƃ���
;   �����ł̓��[�U�[���荞�݃��[�`���@_CMI0 ���Ăяo��
;----------------------------------------------------------------------
__kernel_default_int_handler_entry_c:
	pop		r1
	popm	r1-r15
__kernel_default_int_handler_entry:
	pushm	r1-r5								; �X�N���b�`���W�X�^���^�X�N�X�^�b�N�֑ޔ� 
;	mov.l	#0FFFFFFFFH, r1						; �����݃n���h���ԍ���r1�� 
	mov.l	#_CMI0, r2							; �n���h���̃A�h���X��r2�� 
;	bra.a	_kernel_interrupt					; ���ʃ��[�`���� 

;
;  �����݂̏o��������(�A�Z���u������L�q����)
;
;  �ďo������:
;  �E�����ݔ�������H/W�����ɂ��, PSW���W�X�^��I�r�b�g=0, IPL�͎�t��
;    �������݂�IPL.
;  �E�X�^�b�N�͑��d���荞�݂Ȃ犄���݃X�^�b�N, �����łȂ����
;    �^�X�N�X�^�b�N
;  �E�����ݔ�������H/W�����ɂ��,�X�^�b�N�Ɋ����݂���̕��APC��PSW��
;    �ۑ�����Ă���.
;  �E�x�N�^�e�[�u���ɓo�^���ꂽ�ʂ̓���������ɂ��, �X�^�b�N��
;    �X�N���b�`���W�X�^(R1-R5)���ۑ�����Ă���.
;
;  ����:
;  �Er1:�����ݗv���ԍ�
;  �Er2:�����݃n���h���̃A�h���X
;
;  ���W�X�^���X�^�b�N��ɂǂ̂悤�ɕۑ�����Ă��邩���ȉ��Ɏ���.
;  ���̐}�ł͏オ���, �������ʂ̃A�h���X��, �X�^�b�N�͉�����
;  ������Ɍ������Đςݏグ������̂Ƃ���.
;
;	-------------------------
;	|       ACC-HI(4byte)   |
;	-------------------------(SP + 0)
;	|       ACC-LO(4byte)   |
;	-------------------------(SP + 4)
;	|       FPSW(4byte)     |
;	-------------------------(SP + 8)
;	|       R14(4byte)      |
;	-------------------------(SP + 12)
;	|       R15(4byte)      |
;	-------------------------(SP + 16)
;	|       R1(4byte)       |
;	-------------------------(SP + 20)
;	|       R2(4byte)       |
;	-------------------------(SP + 24)
;	|       R3(4byte)       |
;	-------------------------(SP + 28)
;	|       R4(4byte)       |
;	-------------------------(SP + 32)
;	|       R5(4byte)       |
;	-------------------------(SP + 36)
;	|       PC(4byte)       |
;	-------------------------(SP + 40)
;	|       PSW(4byte)      |
;	-------------------------(SP + 44)
;
;  �����ł�, �����ݏ������[�h�ɕύX���ăn���h�������s����.
;
;  �n���h�����烊�^�[���������, ���d�����݂łȂ�, ���� reqflg ��
;  TRUE �ɂȂ������ɁCret_int �֕���(�x���f�B�X�p�b�`)����D
;
;  ���d�����݂��ǂ����͊����݃l�X�g�J�E���^�̒l�Ŕ��肷��.
;  __kernel_intnest != 0 �Ȃ�Α��d�����݂ł���Ɣ��肷��.
;
;  reqflg ��CPU���b�N��ԂŃ`�F�b�N����. �����łȂ��ƁC
;  reqflg �`�F�b�N��ɋN�����ꂽ�����݃n���h������
;  �f�B�X�p�b�`���v�����ꂽ�ꍇ�ɁC�f�B�X�p�b�`����Ȃ�.
;
_kernel_interrupt:
	pushm	r14-r15						; �X�N���b�`���W�X�^��ޔ�
	pushc	fpsw						; FPU�X�e�[�^�X���W�X�^�ޔ�
	mvfacmi	r5
	shll	#16, r5						; ACC�ŉ���16bit��0�Ƃ���
	mvfachi	r4
	pushm	r4-r5						; �A�L�������[�^�ޔ�
	mov.l	#__kernel_intnest, r5
	mov.w	[r5], r4
	add		#1, r4						; �l�X�g�񐔂��C���N�������g
	mov.w	r4, [r5]
	setpsw	i							; �����݋���(CPU���b�N�������)

	jsr  	r2							; C���[�`���Ăяo��

	clrpsw	i							; �����݋֎~(CPU���b�N���)
	mov.l	#__kernel_intnest, r5
	mov.w	[r5], r4
	sub		#1, r4						; �l�X�g�񐔂��f�N�������g
	mov.w	r4, [r5]
	cmp		#0, r4						; ���d���荞�݂��ǂ���
	bnz		interrupt_return			; ���d���荞�݂Ȃ烊�^�[��
	; ���i�̊�����
	mov.l	#__kernel_reqflg, r5		; �f�B�X�p�b�`�v�����Ȃ���?
	mov.l	[r5], r5
	cmp		#1, r5
;	bz		ret_int						; ����� ret_int �փW�����v
	bnz		ret_int_r_rte
;
; �J�[�l���̏I�������̌ďo��
;
; ���[�h�ƃX�^�b�N���^�X�N�R���e�L�X�g�p�ɐ؂�ւ��D
;�Ƃ肠�����폜


;
;  ������(CPU��O)����̃^�X�N�f�B�X�p�b�`������
;
;	�ďo������:
;	�E�S�����݃��b�N���(PSW���W�X�^I�r�b�g = 0)
;	�E�����ݗD��x�}�X�N�S�����łȂ����(IPL != 0)
;	�E�^�X�N�R���e�L�X�g(__kernel_intnest=0)
;	�E�g�p�X�^�b�N�̓^�X�N�X�^�b�N
;	�Ereqflg = true
;
;  �����ł�, �f�B�X�p�b�`�����[�h�ɕύX��, reqflg��OFF�ɂ��Ă���, 
;  �x���f�B�X�p�b�`�̗L���𔻒f����.
;

;
;  ret_int�擪�ŃX�^�b�N�ɐς܂�Ă���PSW���W�X�^�ւ̃I�t�Z�b�g
;  ACC + FPSW + R14--R15 + R1--R5 + PC
;
	RET_INT_GET_PSW_OFFSET	.equ	(8+4+28+4)
#define  RET_INT_GET_PSW_OFFSET		(8+4+28+4)
#define  PSW_IPL_MASK 		  	 	117440512 

ret_int:
.if	1
	mov.l	RET_INT_GET_PSW_OFFSET[r0], r5	; ������/CPU��O�����O��IPL�l�擾
	and		#PSW_IPL_MASK, r5
	mvtc	r5, psw						; �S�����݃��b�N(CPU���b�N)���
										; ������/CPU��O�����O�̊����ݗD��x
.endif
	mov.l	#__kernel_reqflg, r5
	mov.l	#0, [r5]					; reqflg <--- FALSE
	bsr		__kernel_search_schedtsk	; �^�X�N�����s
	bsr		__kernel_run_task			; search_schedtsk�̕Ԃ�l(R1)�����̂܂�run_task�̈����ƂȂ�

;
;  ������(CPU��O)�ւ̃^�X�N�f�B�X�p�b�`���o��
;
;  �Ăяo������:
;	�E�f�B�X�p�b�`�����[�h(�t�@�C���w�b�_�Q��)
;
;  �����ł�, �^�X�N��O�n���h���ďo����, ������(CPU��O)�������[�h�ւ�
;  �ύX��, �R���e�L�X�g�̕��A���s��, ������(CUP��O)�������֖߂�.
;
interrupt_return:
ret_int_r_rte:
	popm	r4-r5						; �A�L�������[�^���A
	mvtaclo	r5							; ACC�ŉ���16bit��0�ŕ��A
	mvtachi	r4
	popc	fpsw						; FPU�X�e�[�^�X���W�X�^���A
	popm	r14-r15						; ���W�X�^���A
	popm	r1-r5
	rte									; �����ݑO�̏����ɖ߂�

	.end

