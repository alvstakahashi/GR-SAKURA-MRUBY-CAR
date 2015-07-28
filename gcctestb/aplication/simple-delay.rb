
  GC.interval_ratio = 20
  GC.step_ratio = 200

  led0    = Rx_gpio.new(Rx_gpio::LED0)
  led0.off
  toggle = 0

  while 1 do
	p "delay delay"
	if toggle == 0 then
		led0.on
		p "led0 on"
		toggle = 1
	else
		led0.off
		p "led0 off"
		toggle = 0
	end
	Ssp_Delay(500)
  end


  puts "mruby setup finish!"
