class Mycyclic < Ssp_cyclic
	def initialize(id,thread)
		super("cyclic",id)
		@thread = thread
	end
	def cyclic(n)
		puts "cyclic here " + n.to_s
		begin	
			@thread.iact
		rescue => e
			p e.message
		end
	end
end

class Mythread < Ssp_thread
	def initialize(id,led)
		super("thread",id)
		@led = led
	    @status = 0;
	end
	def thread(n)
		puts "thread here " + n.to_s
		if @status == 0 then
			@status =1
			@led.on
		else
			@status = 0
			@led.off
		end
	end
end

class Mythread_gc < Mythread
	def thread(n)
		super(n)
		GC.start
		puts n.to_s + "GC Finish!"
	end
end


  GC.interval_ratio = 20
  GC.step_ratio = 200

  led0    = Rx_gpio.new(Rx_gpio::LED0)
  led1    = Rx_gpio.new(Rx_gpio::LED1)

  led0.off
  led1.off

  thread_0   = Mythread.new(3,led0)
  thread_1   = Mythread_gc.new(4,led1)
  cyclic_0   = Mycyclic.new(4,thread_0)
  cyclic_1   = Mycyclic.new(5,thread_1)
  cyclic_0.start
  cyclic_1.start

  puts "mruby setup finish!"
