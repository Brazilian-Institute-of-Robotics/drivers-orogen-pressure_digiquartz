require 'orocos'
include Orocos

if !ARGV[0]
    STDERR.puts "usage: test.rb 'serial://<device name>:<baud rate>'"
    exit 1
end

Orocos.initialize

Orocos.run 'digiquartz_pressure::Task' => "digiquartz_pressure" do
    driver = TaskContext.get 'digiquartz_pressure'
    #Orocos.log_all_ports

    driver.io_port = ARGV[0]
    driver.integration_time = 0.188
    driver.configure
    driver.start

    Orocos.watch driver
end

