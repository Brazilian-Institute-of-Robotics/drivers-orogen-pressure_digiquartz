/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "Task.hpp"

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

void Task::processIO()
{
    double pressure;
    if(driver->readMeasurement(pressure))
    {
        _pressure_psi.write(pressure);
        _pressure_bar.write(pressure * psi2bar);
        base::samples::RigidBodyState depth_sample;
        depth_sample.invalidate();
        depth_sample.time = base::Time().now();
        depth_sample.position.z() = -(pressure * psi2pa) / (_density_of_water.value() * gravity);
        if(!last_measurement_time.isNull())
        {
            depth_sample.velocity(2) = (depth_sample.position.z() - last_measurement) / (depth_sample.time - last_measurement_time).toSeconds();
            depth_sample.cov_velocity(2,2) =  pow(depth_sample.velocity(2), 2.0);
        }
        depth_sample.cov_position(2,2) = pow(_sigma.value(), 2.0);
        _depth_samples.write(depth_sample);

        last_measurement_time = depth_sample.time;
        last_measurement = depth_sample.position.z();
    }
    else
    {
        RTT::log(RTT::Error) << "Failed to read new measurement." << RTT::endlog();
    }
}


/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See Task.hpp for more detailed
// documentation about them.

bool Task::configureHook()
{
    driver.reset(new digiquartz_pressure::Driver);
    last_measurement_time.microseconds = 0;

    if (!_io_port.value().empty())
        driver->open(_io_port.value());
    
    driver->setIntegrationTime((int)(_integration_time.get()*1000.0));
    _integration_time.set(((double)(driver->getIntegrationTime()))/1000.0);

    setDriver(driver.get());
    
    if (! TaskBase::configureHook())
        return false;
    return true;
}

bool Task::startHook()
{
    if (! TaskBase::startHook())
        return false;

    driver->startAcquisition();

    return true;
}

void Task::updateHook()
{
    TaskBase::updateHook();
}

void Task::errorHook()
{
    TaskBase::errorHook();
}

void Task::stopHook()
{
    TaskBase::stopHook();
    
    driver->stopAcquisition();
}

void Task::cleanupHook()
{
    TaskBase::cleanupHook();
    
    driver.reset();
}