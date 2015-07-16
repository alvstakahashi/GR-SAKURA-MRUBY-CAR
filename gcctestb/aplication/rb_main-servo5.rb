class Myalarm < Ssp_alarm
	def initialize(id,gpio)
		super("alarm",id)
		@gpio = gpio
	end
	def alarm(n)
		@gpio.off
	end

end


class Gpiocyclic < Ssp_cyclic
	def initialize(id,alarm,gpio,led)
		super("cyclic",id)
		@alarm = alarm
		@gpio  = gpio
		@led  = led
	end
	def start(tout)
		@tout = tout
		super
		@led.on
	end
	def change_tout(value)		#moter‚Í@100‚©‚ç141
		@tout += value
		p "tout = "+ @tout.to_s
	end
	def stop
		super
		@alarm.stop
		@led.off
	end
	def cyclic(n)
		@gpio.on
		@alarm.istart(@tout)
	end
end


class MainThread < Ssp_thread
	def initialize(id)
		super("thread",id)
	end
	def thread(n)
		puts "start mainThread"

  		led1    = Rx_gpio.new(Rx_gpio::LED1)		#accell
  		accel    = Rx_gpio.new(Rx_gpio::IO2)

  		led1.off

	  	accel_timer   = Myalarm.new(1,accel)
  		accel_loop    = Gpiocyclic.new(1,accel_timer,accel,led1)

  		stear         = Rx_rcservo.new()


###---------------------------------

		
		  while 1 do
    		width = 0

		    p "please hit any key!"
			command = Rx_getc()
			p "getc = " + command.to_s
			case command
		
			when 'w' then		#accell start
				accel_loop.start(140)
			when 's' then		#accell stop
				accel_loop.stop
			when '1' then		# Speed down
				accel_loop.change_tout(1)
			when '3' then 		# speed up
				accel_loop.change_tout(-1)
		
			when '4' then		# ¶
				width = 1050
			when '5' then		# ’†
				width = 1550
			when '6' then		# ‰E
				width = 1900
			end
			if width != 0 then
				p "width =" + width.to_s
				stear.Start(width)
			end
		    GC.start
		  end
###---------------------------------
	end
end


  GC.interval_ratio = 20
  GC.step_ratio = 200

  led0    = Rx_gpio.new(Rx_gpio::LED0)

  led0.on				#“®ì’†

  thread_main = MainThread.new(5)

  puts "mruby setup finish!"

  thread_main.act()

