/***************************************************************/
/*  NAME: Alon Yaari                                           */
/*  ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*  FILE: M200.cpp                                             */
/*  DATE: Dec 2014                                             */
/*  Rewriter : CTHung at 2019                                  */
/***************************************************************/

#include "newM200.h"
#include <math.h>
#include <iostream>
using namespace std;

// Procedure: clamp()
//   Purpose: Clamps the value of v between minv and maxv
double clamp(double v, double minv, double maxv)
{
    return min(maxv,max(minv, v));
}

iM200::iM200()
{
  // Motor related
  m_des_thrust            = 0.0;                  // Last requested desired thrust
  m_des_rudder            = 0.0;                  // Last requested desired rudder
  m_des_L                 = 0.0;                  // Last requested desired thrust to L motor
  m_des_R                 = 0.0;                  // Last requested desired thrust to R motor
  m_zAngularV = 0;
  m_angularMax = 100;
  m_rudderGain = 0;
}
void iM200::setMaxRudder(double maxRudder)
{
    m_dMaxRudder = maxRudder;
}

void iM200::setMaxThrust(double maxThrust)
{
    m_dMaxThrust = maxThrust;
}

void iM200::setDesiredRudder(double desiredRudder)
{
    m_des_rudder = desiredRudder;
}

void iM200::setDesiredThrust(double desiredThrust)
{
    m_des_thrust = desiredThrust;
}

void iM200::setZAngularV(double zAngularV)
{
    m_zAngularV = zAngularV;
}

void iM200::setAngularMax(double angularMax)
{
    m_angularMax = angularMax;
}

void iM200::setRudderGain(double rudderGain)
{
    m_rudderGain = rudderGain;
}
bool iM200::ThrustRudderToLR()
{
  // 1. Constrain Values
  //      DESIRED_RUDDER value to MAX_RUDDER
  //          - Anything more extreme than +/-50.0 is turn-in-place
  //      DESIRED_THRUST value to MAX_THRUST
  //          - Anything greater than +/-100.0% makes no sense
  double desiredRudder = clamp (m_des_rudder, (-1.0 * m_dMaxRudder), m_dMaxRudder);
  double desiredThrust = clamp (m_des_thrust, (-1.0 * MAX_THRUST), MAX_THRUST);

  if (fabs(m_zAngularV) >= m_angularMax && m_zAngularV > 0 && m_des_rudder > 0)
    m_des_rudder = m_des_rudder*m_rudderGain;
  else if (fabs(m_zAngularV) >= m_angularMax && m_zAngularV < 0 && m_des_rudder < 0)
    m_des_rudder = m_des_rudder*m_rudderGain;
  // 2. Calculate turn
  //      - ADD rudder to left thrust
  //      - SUBTRACT rudder from right thrust
  double percentLeft  = desiredThrust + desiredRudder;
  double percentRight = desiredThrust - desiredRudder;

  // 3. Map desired thrust values to motor bounds
  //      - Range of DESIRED_THRUST: [-MAX_THRUST, MAX_THRUST]
  //      -          ...map to...
  //      - Range of valid thrust values: [-m_MaxThrustValue, m_MaxThrustValue]
  double fwdOrRevL   = (percentLeft  > 0.0) ? 1.0 : -1.0;
  double fwdOrRevR   = (percentRight > 0.0) ? 1.0 : -1.0;
  double pctThrustL  = fabs(percentLeft)  / MAX_THRUST;
  double pctThrustR  = fabs(percentRight) / MAX_THRUST;
  double mappedLeft  = pctThrustL * m_dMaxThrust * fwdOrRevL;
  double mappedRight = pctThrustR * m_dMaxThrust * fwdOrRevR;

  // 4. Deal with overages
  //      - Any value over m_MaxThrustValue gets subtracted from both sides equally
  //      - Constrain to [-m_MaxThrustValue, m_MaxThrustValue]
  double maxThrustNeg = -1.0 * m_dMaxThrust;
  if (mappedLeft  > m_dMaxThrust)
    mappedRight -= (mappedLeft  - m_dMaxThrust);
  if (mappedLeft  < maxThrustNeg)
    mappedRight -= (mappedLeft  + m_dMaxThrust);
  if (mappedRight > m_dMaxThrust)
    mappedLeft  -= (mappedRight - m_dMaxThrust);
  if (mappedRight < maxThrustNeg)
    mappedLeft  -= (mappedRight + m_dMaxThrust);

  m_des_L  = clamp (mappedLeft,  (-1.0 * m_dMaxThrust), m_dMaxThrust);
  m_des_R  = clamp (mappedRight, (-1.0 * m_dMaxThrust), m_dMaxThrust);
  return true;
}

