require 'orocos'
include Orocos

if !ARGV[0]
    STDERR.puts "usage: test.rb <device name> <baud rate>"
    exit 1
end

Orocos.initialize

Orocos::Process.run 'digiquartz_pressure::Task' => "digiquartz_pressure" do
    driver = TaskContext.get 'digiquartz_pressure'
    #Orocos.log_all_ports

    driver.device = ARGV[0]
    driver.baudrate = Integer(ARGV[1])
    driver.integration_time = 0.188
    driver.configure
    driver.start

    loop do        
	sleep 0.01
    end
end

