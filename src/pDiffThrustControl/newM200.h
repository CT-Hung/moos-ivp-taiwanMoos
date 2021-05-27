/***************************************************************/
/*  NAME: Alon Yaari                                           */
/*  ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*  FILE: M200.h                                               */
/*  DATE: Dec 2014                                             */
/*  Rewriter : CTHung at 2019                                  */
/***************************************************************/

#ifndef NEWM200_H
#define NEWM200_H

#include <sys/socket.h>
#include <string>

#define MAX_RUDDER      50.0
#define MAX_THRUST     100.0

class iM200{

public:
        iM200();
        ~iM200() {};
public:
  bool  ThrustRudderToLR();
  void  setMaxRudder(double);
  void  setMaxThrust(double);
  void  setDesiredRudder(double);
  void  setDesiredThrust(double);
  void  setZAngularV(double);
  void setAngularMax(double);
  void  setRudderGain(double);
  double getMaxRudder(){return m_dMaxRudder;};
  double getMaxThrust(){return m_dMaxThrust;};
  double getDesiredLeftMotor(){return m_des_L;};
  double getDesiredRightMotor(){return m_des_R;};
  double getZAngularV(){return m_zAngularV;};
  double getDesiredThrust(){return m_des_thrust;};
  double getDesiredRudder(){return m_des_rudder;};

private:
  double m_dMaxRudder;       // MAX_RUDDER         Maximum rudder angle that will be attempted
  double m_dMaxThrust;       // MAX_THRUST         Maximum thrust allowed
  double m_zAngularV;
  double m_angularMax;
  double m_rudderGain;

  // Motor related
  double m_des_rudder;
  double m_des_thrust;
  double m_des_L;
  double m_des_R;
};

#endif
