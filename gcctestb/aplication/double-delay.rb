class Mycyclic < Ssp_cyclic
	def initialize(id,thread)
		super("cyclic",id)
		@thread = thread
	end
	def cyclic(n)
		puts "cyclic here " + n.to_s	
		@thread.iact
	end
end

class Mythread_0 < Ssp_thread
	def initialize(id,led)
		super("thread",id)
		@led = led
	end
	def thread(n)
		puts "thread here " + n.to_s
		@led.on
		p "0 start"
		Ssp_Delay(500)
		p "0 end"
		@led.off
	end
end

class Mythread_1 < Ssp_thread
	def initialize(id,led)
		super("thread",id)
		@led = led
	end
	def thread(n)
		while 1 do
			p "thread_1 here"
			@led.on
			p "1 start"
			Ssp_Delay(1000)
			p "1 end"
			@led.off
			p "1 start "
			Ssp_Delay(1000)
			p "1 end"
			GC.start
		end
	end
end


  GC.interval_ratio = 20
  GC.step_ratio = 200

  led0    = Rx_gpio.new(Rx_gpio::LED0)
  led1    = Rx_gpio.new(Rx_gpio::LED1)

  led0.off
  led1.off

	begin
	  thread_0   = Mythread_0.new(3,led0)
	  thread_1   = Mythread_1.new(4,led1)

	  thread_1.act
	
	  cyclic_0   = Mycyclic.new(4,thread_0)
	  cyclic_0.start
	rescue => e
		p e.message
	end
  puts "mruby setup finish!"

