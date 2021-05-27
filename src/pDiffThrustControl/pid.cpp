#ifndef _PID_SOURCE_
#define _PID_SOURCE_

#include <iostream>
#include <cmath>
#include "pid.h"

using namespace std;


PID::PID()
{
    m_kp = 1;
    m_ki = 0;
    m_kd = 0;
    m_max = 100;
    m_min = -100;
    m_old_error = 0;
    m_old_time = 0;
    m_integral = 0;
    m_pidOutput = 0;
}
PID::~PID() 
{
}

bool PID::run( double setpoint, double pv , double current_time)
{
    m_dt = current_time-m_old_time;
    if(m_dt == 0)
    {
        m_old_time = current_time;
        return true;
    }
    else if(m_dt < 0) 
    {
        return false;
    }
    // Calculate error
    double error = setpoint - pv;

    // Proportional term
    double Pout = m_kp * error;

    // Integral term
    m_integral += error * m_dt;
    double Iout = m_ki * m_integral;

    // Derivative term
    double derivative = (error - m_old_error) / m_dt;
    double Dout = m_kd * derivative;

    // Calculate total output
    m_pidOutput = Pout + Iout + Dout;

    // Restrict to max/min
    if( m_pidOutput > m_max )
        m_pidOutput = m_max;
    else if( m_pidOutput < m_min )
        m_pidOutput = m_min;

    // Save error to previous error
    m_old_error = error;
    m_old_time = current_time;

    return true;
}

bool PID::setKp(double kp)
{
    m_kp = kp;
    return true;
}

bool PID::setKi(double ki)
{
    m_ki = ki;
    return true;
}

bool PID::setKd(double kd)
{
    m_kd = kd;
    return true;
}

bool PID::setMax(double pidMax)
{
    m_max = pidMax;
    return true;
}

bool PID::setMin(double pidMin)
{
    m_min = pidMin;
    return true;
}

bool PID::setStartTime(double startTime)
{
    m_old_time = startTime;
    return true;
}
#endif
