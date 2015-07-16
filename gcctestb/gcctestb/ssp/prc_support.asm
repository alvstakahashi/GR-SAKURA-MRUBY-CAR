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
;   上記著作権者は，以下の(1)～(4)の条件を満たす場合に限り，本ソフトウェ
;   ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
;   変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
;   (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
;       権表示，この利用条件および下記の無保証規定が，そのままの形でソー
;       スコード中に含まれていること．
;   (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
;       用できる形で再配布する場合には，再配布に伴うドキュメント（利用
;       者マニュアルなど）に，上記の著作権表示，この利用条件および下記
;       の無保証規定を掲載すること．
;   (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
;       用できない形で再配布する場合には，次のいずれかの条件を満たすこ
;       と．
;     (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
;         作権表示，この利用条件および下記の無保証規定を掲載すること．
;     (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
;         報告すること．
;   (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
;       害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
;       また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
;       由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
;       免責すること．
;  
;   本ソフトウェアは，無保証で提供されているものである．上記著作権者お
;   よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
;   に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
;   アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
;   の責任を負わない．
;  
;

;
;  プロセッサ依存モジュール アセンブリ言語部（RX600シリーズ用）
;

;
;  ディスパッチャおよび割込み(CPU例外)出入り口のルール:
;	動作モードを以下のように定義する.
;		ディスパッチャモード:
;			CPUロック状態, 割込み優先度マスク全解除状態,
;			タスクコンテキスト(__kernel_intnest = 0), タスクスタック
;		割込み(CPU例外)処理モード
;			全割込みロック状態(PSWレジスタIビット = 0),
;			割込み優先度マスク全解除でない状態(IPL != 0)
;			割込みコンテキスト(__kernel_intnest != 0), 割込みスタック
;
;	カーネル管理外割込みのサポート有無と, CPUロック状態, 割込み優先度
;	マスク全解除状態の関係は以下の通りである.
;		カーネル管理外割込み未サポート時:
;			CPUロック状態(PSWレジスタIビット = 0)
;			割込み優先度マスク全解除状態(IPL = 0)
;		カーネル管理外割込みサポート時:
;			CPUロック状態
;				(PSWレジスタIビット = 0, IPL = IPL_LOCK, lock_flag = true)
;			割込み優先度マスク全解除状態(saved_ipl = 0)
;
;	各構造体ポインタを以下のように各レジスタにアサインする.
;		r15 = p_runtsk	ただしディスパッチャの各出口では無効
;		r14 = *p_runtsk	dispatcher の中では p_runtsk 確定時に再取得する
;	各入り口から最初に変数アクセスするときに上記レジスタに保存する.
;
;	構造体アライメントへの対応
;		構造体アライメントが4Byte(アンパック)の場合:
;			一般的なレジスタ相対アドレッシングが可能
;				例:	mov.l	#__kernel_p_runtsk, r15
;					mov.l  	r0, TCB_sp[r15]
;		構造体アライメントが4Byteではない(パック)の場合:
;			mov.lのようにロングサイズ指定の場合、相対値は4の倍数のみ有効
;			このため, 一度対象アドレスを求めてからアクセスする必要がある
;				例:	mov.l	#__kernel_p_runtsk, r15
;					add		#TCB_sp, r15, r5
;					mov.l  	r0, [r5]
;		各オフセット値を判断し, 条件アセンブルによりコードを切り替える
;

;
;  各種EQU定義(Hファイルの#define定義)
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
;	割り込みベクタ登録から直接くるところ
;   ここではユーザー割り込みルーチン　_CMI0 を呼び出す
;----------------------------------------------------------------------
__kernel_default_int_handler_entry_c:
	pop		r1
	popm	r1-r15
__kernel_default_int_handler_entry:
	pushm	r1-r5								; スクラッチレジスタをタスクスタックへ退避 
;	mov.l	#0FFFFFFFFH, r1						; 割込みハンドラ番号をr1へ 
	mov.l	#_CMI0, r2							; ハンドラのアドレスをr2へ 
;	bra.a	_kernel_interrupt					; 共通ルーチンへ 

;
;  割込みの出入口処理(アセンブリ言語記述部分)
;
;  呼出し条件:
;  ・割込み発生時のH/W処理により, PSWレジスタのIビット=0, IPLは受付け
;    た割込みのIPL.
;  ・スタックは多重割り込みなら割込みスタック, そうでなければ
;    タスクスタック
;  ・割込み発生時のH/W処理により,スタックに割込みからの復帰PCとPSWが
;    保存されている.
;  ・ベクタテーブルに登録された個別の入り口処理により, スタックに
;    スクラッチレジスタ(R1-R5)が保存されている.
;
;  引数:
;  ・r1:割込み要因番号
;  ・r2:割込みハンドラのアドレス
;
;  レジスタがスタック上にどのように保存されているかを以下に示す.
;  この図では上が低位, 下が高位のアドレスで, スタックは下から
;  上方向に向かって積み上げられるものとする.
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
;  ここでは, 割込み処理モードに変更してハンドラを実行する.
;
;  ハンドラからリターンした後は, 多重割込みでなく, かつ reqflg が
;  TRUE になった時に，ret_int へ分岐(遅延ディスパッチ)する．
;
;  多重割込みかどうかは割込みネストカウンタの値で判定する.
;  __kernel_intnest != 0 ならば多重割込みであると判定する.
;
;  reqflg はCPUロック状態でチェックする. そうでないと，
;  reqflg チェック後に起動された割込みハンドラ内で
;  ディスパッチが要求された場合に，ディスパッチされない.
;
_kernel_interrupt:
	pushm	r14-r15						; スクラッチレジスタを退避
	pushc	fpsw						; FPUステータスレジスタ退避
	mvfacmi	r5
	shll	#16, r5						; ACC最下位16bitは0とする
	mvfachi	r4
	pushm	r4-r5						; アキュムレータ退避
	mov.l	#__kernel_intnest, r5
	mov.w	[r5], r4
	add		#1, r4						; ネスト回数をインクリメント
	mov.w	r4, [r5]
	setpsw	i							; 割込み許可(CPUロック解除状態)

	jsr  	r2							; Cルーチン呼び出し

	clrpsw	i							; 割込み禁止(CPUロック状態)
	mov.l	#__kernel_intnest, r5
	mov.w	[r5], r4
	sub		#1, r4						; ネスト回数をデクリメント
	mov.w	r4, [r5]
	cmp		#0, r4						; 多重割り込みかどうか
	bnz		interrupt_return			; 多重割り込みならリターン
	; 初段の割込み
	mov.l	#__kernel_reqflg, r5		; ディスパッチ要求がないか?
	mov.l	[r5], r5
	cmp		#1, r5
;	bz		ret_int						; あれば ret_int へジャンプ
	bnz		ret_int_r_rte
;
; カーネルの終了処理の呼出し
;
; モードとスタックを非タスクコンテキスト用に切り替え．
;とりあえず削除


;
;  割込み(CPU例外)からのタスクディスパッチャ入口
;
;	呼出し条件:
;	・全割込みロック状態(PSWレジスタIビット = 0)
;	・割込み優先度マスク全解除でない状態(IPL != 0)
;	・タスクコンテキスト(__kernel_intnest=0)
;	・使用スタックはタスクスタック
;	・reqflg = true
;
;  ここでは, ディスパッチャモードに変更し, reqflgをOFFにしてから, 
;  遅延ディスパッチの有無を判断する.
;

;
;  ret_int先頭でスタックに積まれているPSWレジスタへのオフセット
;  ACC + FPSW + R14--R15 + R1--R5 + PC
;
	RET_INT_GET_PSW_OFFSET	.equ	(8+4+28+4)
#define  RET_INT_GET_PSW_OFFSET		(8+4+28+4)
#define  PSW_IPL_MASK 		  	 	117440512 

ret_int:
.if	1
	mov.l	RET_INT_GET_PSW_OFFSET[r0], r5	; 割込み/CPU例外発生前のIPL値取得
	and		#PSW_IPL_MASK, r5
	mvtc	r5, psw						; 全割込みロック(CPUロック)状態
										; 割込み/CPU例外発生前の割込み優先度
.endif
	mov.l	#__kernel_reqflg, r5
	mov.l	#0, [r5]					; reqflg <--- FALSE
	bsr		__kernel_search_schedtsk	; タスクを実行
	bsr		__kernel_run_task			; search_schedtskの返り値(R1)がそのままrun_taskの引数となる

;
;  割込み(CPU例外)へのタスクディスパッチャ出口
;
;  呼び出し条件:
;	・ディスパッチャモード(ファイルヘッダ参照)
;
;  ここでは, タスク例外ハンドラ呼出しと, 割込み(CPU例外)処理モードへの
;  変更と, コンテキストの復帰を行い, 割込み(CUP例外)発生元へ戻る.
;
interrupt_return:
ret_int_r_rte:
	popm	r4-r5						; アキュムレータ復帰
	mvtaclo	r5							; ACC最下位16bitは0で復帰
	mvtachi	r4
	popc	fpsw						; FPUステータスレジスタ復帰
	popm	r14-r15						; レジスタ復帰
	popm	r1-r5
	rte									; 割込み前の処理に戻る

	.end

