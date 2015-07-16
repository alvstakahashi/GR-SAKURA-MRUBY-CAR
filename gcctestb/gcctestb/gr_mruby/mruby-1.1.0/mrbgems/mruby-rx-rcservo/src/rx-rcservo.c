#include "mruby.h"
#include "mruby/variable.h"
#include "mruby/string.h"

#include <string.h>

#include "iodefine.h"


static void mtu3_init(void)
{
    /* PC7をMTIOC3Aに設定 */
    MPC.PWPR.BIT.B0WI = 0;      // ピン機能選択の
    MPC.PWPR.BIT.PFSWE = 1;     // 　プロテクト解除
    MPC.PC7PFS.BIT.PSEL = 0x01; // PC7をMTIOC3Aに設
    MPC.PWPR.BIT.PFSWE = 0;     // ピン機能選択の
    MPC.PWPR.BIT.B0WI = 1;      // 　プロテクト設定
    PORTC.PMR.BIT.B7 = 1;       // PC7は周辺機能として使用

    SYSTEM.PRCR.WORD = 0xA502;  // プロテクト解除
    MSTP(MTU) = 0;              // MTU (MTU0～MTU5)のストップ状態解除
    SYSTEM.PRCR.WORD = 0xA500;  // プロテクト設定
    
    /* MTU3のTCNT動作停止 */
    MTU.TSTR.BIT.CST3 = 0;

    /* TCNTクリア */
    MTU3.TCNT = 0x0000;

    /* カウンタクロックとカウンタクリア要因の設定 */
    // GR-SAKURAでは、外部発振子 12MHz, 
    // PLLCR.STC=16(16逓倍), PLLCR.PLIDIV=0(1分周), SCKCR.PCKB=2(4分周)
    // より、MTUのPCLKは 12*16/4 = 48MHz
//    MTU3.TCR.BIT.TPSC = 0;  // カウンタクロック: PCLK/1 = 48MHz
    MTU3.TCR.BIT.TPSC = 3;  // カウンタクロック: PCLK/64 = 750Khz
    MTU3.TCR.BIT.CKEG = 0;  // 立ち上がりエッジでカウント
    MTU3.TCR.BIT.CCLR = 1;  // TGRAのコンペアマッチでTCNTクリア

    /* PWMモード1 */
    MTU3.TMDR.BYTE = 0x02;
    
    /* 波形出力レベルの選択 */
    MTU3.TIORH.BIT.IOA = 1; // 初期状態で0出力 周期コンペアマッチで0出力
    MTU3.TIORH.BIT.IOB = 2; // デューティコンペアマッチで1出力

    /* 周期とデューティーの設定 */
    						// GRA 20msにする
    						// 20ms は　50Hz   750KHz / 50 = 15000
    MTU3.TGRA = 15000;      
}
// pulsWidth  usを指定
static void mtu3_Start(int pulsWidth)
{
    			          // デューティー  1100us にする
						  //  750KHz / (100000/1100)  = 825 
						  //  750KHz / (100000/1400)  = 1050
						  //  750KHz / (100000/1500)  = 1125
						  //  750KHz / (100000/1800)  = 1350
						
	MTU3.TGRB = (15000-((750*pulsWidth)/1000));    
    /* MTU3のTCNT動作開始 */
    MTU.TSTR.BIT.CST3 = 1;
}
static void mtu3_Stop(void)
{
    /* MTU3のTCNT動作停止 */
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
