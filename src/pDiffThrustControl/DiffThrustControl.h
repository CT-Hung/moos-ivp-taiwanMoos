/*********************************************************/
/*    NAME: Chao-Chun Hsu                                */
/*    ORGN: MIT, Cambridge MA                            */
/*    FILE: DiffThrustControl.h                          */
/*    DATE: December 29th, 1963                          */
/************************************************************/

#ifndef DiffThrustControl_HEADER
#define DiffThrustControl_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "newM200.h"
#include "pid.h"

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "newM200.h"
#include "pid.h"

class DiffThrustControl : public AppCastingMOOSApp
{
 public:
   DiffThrustControl();
   ~DiffThrustControl();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();
   iM200 m_m200;
   PID m_bow_thrust_PID;
   void notifyThrust();
   void stationKeep();

 private: // Configuration variables
   double m_des_rudder;
   double m_des_thrust;
   double m_des_bow_thrust;
   double m_bow_speed;
   double m_rudder_gain;
   
   bool m_turn_bow_thrust;
   double m_bow_thrust_max;
   double m_bow_thrust_min;
   double m_bow_thrust_kp;
   double m_bow_thrust_ki;
   double m_bow_thrust_kd;
   double m_bow_thrust_ratio;

   //for station keep
   double m_st_x;
   double m_st_y;
   double m_x;
   double m_y;
   double m_heading;
   double m_inner_radius; 
   std::string m_keep_hdg;

 private: // State variables
};

#endif 
