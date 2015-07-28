/*
 *  関数のプロトタイプ宣言
 */
#ifndef TOPPERS_MACRO_ONLY
extern void	main(intptr_t arg);
extern void	mruby_call(intptr_t arg);
extern void	task2(intptr_t arg);

extern void 	mrb_ssp_thread_call(intptr_t arg);

extern void 	mrb_ssp_cyclic_call(intptr_t arg);
extern void 	mrb_ssp_alarm_call(intptr_t arg);

extern void mrb_sonic_sence_handler(intptr_t exf);
extern void mrb_sonic_thread(intptr_t exf);


#endif /* TOPPERS_MACRO_ONLY */
