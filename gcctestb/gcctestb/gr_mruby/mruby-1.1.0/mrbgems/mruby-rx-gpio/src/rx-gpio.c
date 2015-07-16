#include "mruby.h"
#include "mruby/variable.h"
#include "mruby/string.h"

#include <string.h>

#include "iodefine.h"

static const unsigned char BITTBL[8] = { 0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80 };

static void *get_port(int pin,int *bit,int *ddr)
{
	void *ret;
	switch(pin)
	{
		case 100:
			*bit = 0;
			ret = (void*)&PORTA;
			break;
		case 101:
			*bit = 1;
			ret = (void*)&PORTA;
			break;
		case 102:
			*bit = 2;
			ret = (void*)&PORTA;
			break;
		case 103:
			*bit = 6;
			ret = (void*)&PORTA;
			break;
		case 202:
		case 302:
			*bit = 2;
			ret = (void*)&PORT2;
			break;
		case 203:
		case 303:
			*bit = 3;
			ret = (void*)&PORT2;
			break;
		case 204:
		case 304:
			*bit = 4;
			ret = (void*)&PORT2;
			break;
		case 205:
		case 305:
			*bit = 5;
			ret = (void*)&PORT2;
			break;

		case 210:
		case 310:
			*bit = 0;
			ret = (void*)&PORTE;
			break;
		case 211:
		case 311:
			*bit = 1;
			ret = (void*)&PORTE;
			break;
		case 212:
		case 312:
			*bit = 2;
			ret = (void*)&PORTE;
			break;
		case 213:
		case 313:
			*bit = 3;
			ret = (void*)&PORTE;
			break;
		case 214:
		case 314:
			*bit = 4;
			ret = (void*)&PORTE;
			break;
		case 215:
		case 315:
			*bit = 5;
			ret = (void*)&PORTE;
			break;
		case 216:
		case 316:
			*bit = 6;
			ret = (void*)&PORTE;
			break;
		case 217:
		case 317:
			*bit = 7;
			ret = (void*)&PORTE;
			break;
		case 250:
		case 350:
			*bit = 7;
			ret = (void*)&PORTC;
			break;
		default:
		return((void*)0);
	}
	if (pin > 300)
	{
		*ddr = 0;	// IN
	}
	else
	{
		*ddr = 1;	//OUT
	}
	return(ret);
}			

	

static mrb_value
mrb_rx_gpio_initialize(mrb_state *mrb, mrb_value self)
{
	  mrb_value pin;
	  mrb_get_args(mrb, "o",&pin);

	  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@pin"),pin );

	  mrb_int pin_no = mrb_fixnum(pin);

#if MRB_DEBUG1
	printf("rx_gpio_initialize pin = %d\n",pin_no);
#endif
	int bit;
	int ddr;
	volatile struct st_port0   *port = (volatile struct st_port0   *)get_port(pin_no,&bit,&ddr);

	if (ddr == 1)	//OUTPUT
	{
		port->PDR.BYTE |= BITTBL[bit];
	}
	else
	{
		port->PDR.BYTE &= ~BITTBL[bit];
	}
	return self;
}
static mrb_value
mrb_rx_gpio_on(mrb_state *mrb, mrb_value self)
{
	mrb_value pin   = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@pin"));
	mrb_int pin_no = mrb_fixnum(pin);

	int bit;
	int ddr;
	
	volatile struct st_port0   *port = (volatile struct st_port0   *)get_port(pin_no,&bit,&ddr);

#if MRB_DEBUG1
	printf("rx_gpio_on port = %08x bit = %d\n",(int)port,bit);
#endif
	if (ddr == 1)
	{	
		port->PODR.BYTE |= BITTBL[bit];
	}
    return self;
}
static mrb_value
mrb_rx_gpio_off(mrb_state *mrb, mrb_value self)
{
	mrb_value pin   = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@pin"));
	mrb_int pin_no = mrb_fixnum(pin);

	int bit;
	int ddr;
	volatile struct st_port0   *port = (volatile struct st_port0   *)get_port(pin_no,&bit,&ddr);

#if MRB_DEBUG1
	printf("rx_gpio_off port = %08x bit = %d\n",(int)port,bit);
#endif
	if (ddr == 1)
	{
		port->PODR.BYTE &= ~BITTBL[bit];
	}
    return self;
}

static mrb_value
mrb_rx_gpio_input(mrb_state *mrb, mrb_value self)
{
	mrb_value pin   = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@pin"));
	mrb_int pin_no = mrb_fixnum(pin);

	int bit;
	int ddr;
	int status=0;
	volatile struct st_port0   *port = (volatile struct st_port0   *)get_port(pin_no,&bit,&ddr);

#if MRB_DEBUG1
	printf("rx_gpio_input port = %08x bit = %d\n",(int)port,bit);
#endif
	if (ddr == 0)	// IN
	{
		status = port->PIDR.BYTE & BITTBL[bit];

		if (status != 0)
		{
			return(mrb_true_value());
		}
	}
    return(mrb_false_value());
}
	
void
mrb_mruby_rx_gpio_gem_init(mrb_state* mrb) {


	struct RClass *gpio;
	gpio = mrb_define_class(mrb, "Rx_gpio", mrb->object_class);

	mrb_define_const(mrb,gpio, "LED0", mrb_fixnum_value(100));
	mrb_define_const(mrb,gpio, "LED1", mrb_fixnum_value(101));
	mrb_define_const(mrb,gpio, "LED2", mrb_fixnum_value(102));
	mrb_define_const(mrb,gpio, "LED3", mrb_fixnum_value(103));

	mrb_define_const(mrb,gpio, "IO2", mrb_fixnum_value(202));
	mrb_define_const(mrb,gpio, "IO3", mrb_fixnum_value(203));
	mrb_define_const(mrb,gpio, "IO4", mrb_fixnum_value(204));
	mrb_define_const(mrb,gpio, "IO5", mrb_fixnum_value(205));

	mrb_define_const(mrb,gpio, "IO44", mrb_fixnum_value(210));
	mrb_define_const(mrb,gpio, "IO45", mrb_fixnum_value(211));
	mrb_define_const(mrb,gpio, "IO46", mrb_fixnum_value(212));
	mrb_define_const(mrb,gpio, "IO47", mrb_fixnum_value(213));
	mrb_define_const(mrb,gpio, "IO48", mrb_fixnum_value(214));
	mrb_define_const(mrb,gpio, "IO49", mrb_fixnum_value(215));
	mrb_define_const(mrb,gpio, "IO50", mrb_fixnum_value(216));
	mrb_define_const(mrb,gpio, "IO51", mrb_fixnum_value(217));

	mrb_define_const(mrb,gpio, "IO12", mrb_fixnum_value(250));


	mrb_define_const(mrb,gpio, "IO2IN", mrb_fixnum_value(302));
	mrb_define_const(mrb,gpio, "IO3IN", mrb_fixnum_value(303));
	mrb_define_const(mrb,gpio, "IO4IN", mrb_fixnum_value(304));
	mrb_define_const(mrb,gpio, "IO5IN", mrb_fixnum_value(305));

	mrb_define_const(mrb,gpio, "IO44IN", mrb_fixnum_value(310));
	mrb_define_const(mrb,gpio, "IO45IN", mrb_fixnum_value(311));
	mrb_define_const(mrb,gpio, "IO46IN", mrb_fixnum_value(312));
	mrb_define_const(mrb,gpio, "IO47IN", mrb_fixnum_value(313));
	mrb_define_const(mrb,gpio, "IO48IN", mrb_fixnum_value(314));
	mrb_define_const(mrb,gpio, "IO49IN", mrb_fixnum_value(315));
	mrb_define_const(mrb,gpio, "IO50IN", mrb_fixnum_value(316));
	mrb_define_const(mrb,gpio, "IO51IN", mrb_fixnum_value(317));
	mrb_define_const(mrb,gpio, "IO12IN", mrb_fixnum_value(350));

	/* methods */
	mrb_define_method(mrb, gpio, "initialize", mrb_rx_gpio_initialize, ARGS_REQ(1));
	mrb_define_method(mrb, gpio, "on", mrb_rx_gpio_on, ARGS_NONE());
	mrb_define_method(mrb, gpio, "off", mrb_rx_gpio_off, ARGS_NONE());
	mrb_define_method(mrb, gpio, "in", mrb_rx_gpio_input, ARGS_NONE());

}

void
mrb_mruby_rx_gpio_gem_final(mrb_state* mrb) {
  // finalizer
}
