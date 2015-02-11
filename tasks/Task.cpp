/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "Task.hpp"
#include <rtt/extras/FileDescriptorActivity.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace digiquartz_pressure;

Task::Task(std::string const& name)
    : TaskBase(name)
{
}

Task::Task(std::string const& name, RTT::ExecutionEngine* engine)
    : TaskBase(name, engine)
{
}

Task::~Task()
{
}



/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See Task.hpp for more detailed
// documentation about them.




bool Task::configureHook()
{
    
    if (! TaskBase::configureHook())
        return false;
    
    std::stringstream file;
    file << "serial://";
    file <<  _device.get();
    file <<  ":";
    file << _baudrate.get();
    driver.open(file.str()); 


    driver.setIntegrationTime((int)(_integration_time.get()*1000));
    _integration_time.set(((double)(driver.getIntegrationTime()))/1000);

    fd_device = driver.getFileDescriptor();

    zero_value = 14.503773773;

    return true;

}



bool Task::startHook()
{
    
    if (! TaskBase::startHook())
        return false;
     
    // Here, "fd" is the file descriptor of the underlying device
    // it is usually created in configureHook()
    RTT::extras::FileDescriptorActivity* activity =
        getActivity<RTT::extras::FileDescriptorActivity>();
    
    if (activity)
        activity->watch(fd_device);

    driver.startAcquisition();

    this->setZeroDepth();

    return true;
    
}



void Task::updateHook()
{
    
    TaskBase::updateHook();
    
    RTT::extras::FileDescriptorActivity* fd_activity =
            getActivity<RTT::extras::FileDescriptorActivity>();
    if (fd_activity){
        if (fd_activity->hasError()){
        
        } else if (fd_activity->hasTimeout()){
        
        }
        else{
            double value;
            base::samples::RigidBodyState depth_sample;

            if(driver.readMeasurement(value)){
       //         std::cout << "psi: " << value << "     bar: " << value * 0.06894757 <<  "       depth: " << (value-14.503773773) * -0.7030696 << std::endl;
                _pressure_psi.write(value);
                _pressure_bar.write(value * 0.06894757);
                depth_sample.time = base::Time().now();
                depth_sample.position(2) = (value-zero_value) * -0.7030696;
                depth_sample.velocity(2) = ((value-last_value) * -0.7030696) / (depth_sample.time-last_time).toSeconds();
                _depth_samples.write(depth_sample);
                _sample_periode.write((depth_sample.time-last_time).toSeconds());

                last_time = depth_sample.time;
                last_value = value;
            }
        }
    }
}



void Task::errorHook()
{
    
    TaskBase::errorHook();
    

    

    
}



void Task::stopHook()
{
    
    TaskBase::stopHook();
    
    RTT::extras::FileDescriptorActivity* activity =
        getActivity<RTT::extras::FileDescriptorActivity>();
    
    if (activity)
        activity->clearAllWatches();
    

    
}



void Task::cleanupHook()
{
    
    TaskBase::cleanupHook();
    

    

    
}

void Task::setZeroDepth()
{
    this->updateHook();
    zero_value = last_value;
}
