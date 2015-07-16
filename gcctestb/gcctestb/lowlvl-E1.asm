;----------------------------------------------------------------------
;
; FILE :lowlvl.src
; DATE :Wed,Jul 01, 2009
; DESCRIPTION :Program of Low level
; CPU TYPE :RX
;
;----------------------------------------------------------------------
.GLB _charput
.GLB _charget
;----------------------------------------------------------------------
;register define
;----------------------------------------------------------------------
#define	FC2E0 	00084080h
#define FE2C0 	00084090h
#define DBGSTAT 000840C0h
#define RXFL0EN 00001000h
#define TXFL0EN 00000100h

.SECTION P,CODE
;----------------------------------------------------------------------
; charput:
;----------------------------------------------------------------------
_charput: 
.STACK _charput = 00000000h
_C2ESTART: 
	MOV.L #TXFL0EN,R3
	MOV.L #DBGSTAT,R4
__TXLOOP:
	MOV.L [R4],R5
	AND R3,R5
	BNZ __TXLOOP
__WRITEFC2E0:
	MOV.L #FC2E0,R2
	MOV.L R1,[R2]
__CHARPUTEXIT:
	RTS
;----------------------------------------------------------------------
; charget:
;----------------------------------------------------------------------
_charget:
.STACK _charget = 00000000h
__E2CSTART:
	MOV.L #RXFL0EN,R3
	MOV.L #DBGSTAT,R4
__RXLOOP:
	MOV.L [R4],R5
	AND R3,R5
	BZ __RXLOOP
__READFE2C0:
	MOV.L #FE2C0,R2
	MOV.L [R2],R1
__CHARGETEXIT:
	RTS

		.glb	_open
		.glb	_read
		.glb	_lseek
		.glb	_isatty
		.glb	_fstat
		.glb	_close

_open:
_read:
_lseek:
_isatty:
_fstat:
_close:
		MOV.L	#0,R1
		RTS

;----------------------------------------------------------------------
	.END
