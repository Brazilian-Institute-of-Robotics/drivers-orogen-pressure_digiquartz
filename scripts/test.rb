require 'orocos'
include Orocos


Orocos.initialize

Orocos.run 'digiquartz_pressure::Task' => "digiquartz_pressure" do
    driver = TaskContext.get 'digiquartz_pressure'
    #Orocos.log_all_ports

    puts 'applying config'
    driver.apply_conf_file("/home/diego/flat_fish/dev/bundles/flat_fish/config/orogen/live/digiquartz_pressure::Task.yml", ['default'])
    puts 'configuring....'
    driver.configure
    puts 'starting'
    driver.start

    Orocos.watch(driver)
end

