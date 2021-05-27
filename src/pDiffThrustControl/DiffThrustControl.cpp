/************************************************************/
/*    NAME: Chao-Chun Hsu                                   */
/*    ORGN: MIT, Cambridge MA                               */
/*    FILE: DiffThrustControl.cpp                           */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "DiffThrustControl.h"
#include "string"
#include "AngleUtils.h"
#include "GeomUtils.h"

using namespace std;

//---------------------------------------------------------
// Constructor

DiffThrustControl::DiffThrustControl()
{
    m_des_rudder = 0;
    m_des_thrust = 0;
    m_des_bow_thrust = 0;
    m_bow_thrust_kd = 1;
    m_bow_thrust_ki = 0.3;
    m_bow_thrust_kd = 0;
    m_bow_thrust_max = 1;
    m_bow_thrust_min = -1;
    m_bow_speed = 0;
    m_turn_bow_thrust = false;
    m_bow_thrust_PID.setStartTime(MOOSTime());
    m_keep_hdg = "false";
    m_inner_radius = -1;
    m_rudder_gain = 0;
    m_bow_thrust_ratio = 3;
}

//---------------------------------------------------------
// Destructor

DiffThrustControl::~DiffThrustControl()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool DiffThrustControl::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();

#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif

     if(key == "DESIRED_RUDDER"){ 
         m_des_rudder = msg.GetDouble();
     }
     else if(key == "DESIRED_THRUST"){
         m_des_thrust = msg.GetDouble();
     }
     else if(key == "NAV_BOW_SPEED")
         m_bow_speed = msg.GetDouble();

     else if(key == "NAV_Z_ANGULAR_V")
         m_m200.setZAngularV(msg.GetDouble());

     else if(key == "DESIRED_BOW_THRUST")
         m_des_bow_thrust = msg.GetDouble();

     else if(key == "BOW_THRUST_RATIO")
         m_bow_thrust_ratio = msg.GetDouble();

     else if(key == "NAV_HEADING")
         m_heading = msg.GetDouble();

     // from BHV_DPHdg
     else if(key == "KEEP_HDG")
         m_keep_hdg = msg.GetString();
     else if(key == "NAV_X")
         m_x = msg.GetDouble();
     else if(key == "NAV_Y")
         m_y = msg.GetDouble();
     else if(key == "STAT_X")
         m_st_x = msg.GetDouble();
     else if(key == "STAT_Y")
         m_st_y = msg.GetDouble();
     else if(key == "STAT_INNER_RADIUS")
         m_inner_radius = msg.GetDouble();

     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool DiffThrustControl::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool DiffThrustControl::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!
  if (m_keep_hdg == "true")
      stationKeep();
  else 
      m_des_bow_thrust = 0;

    notifyThrust();
  AppCastingMOOSApp::PostReport();
  return(true);
}

void DiffThrustControl::notifyThrust()
{
    m_m200.setDesiredThrust(m_des_thrust);
    m_m200.setDesiredRudder(m_des_rudder);
  if(!m_m200.ThrustRudderToLR())
      reportRunWarning("Error of changing thrust and rudder to LR");
  else{
    Notify("ROS_THRUST_L", m_m200.getDesiredLeftMotor()/100);
    Notify("ROS_THRUST_R", m_m200.getDesiredRightMotor()/100);
  }
  if(m_turn_bow_thrust == true){
    if(!m_bow_thrust_PID.run(m_des_bow_thrust, m_bow_speed/m_bow_thrust_ratio, MOOSTime()))
        reportRunWarning("Error of Bow Thrust Control");
    else{
        Notify("ROS_THRUST_BOW", m_bow_thrust_PID.getPIDOutput());
    }
  }
}

void DiffThrustControl::stationKeep()
{
    double angle_to_station = relAng(m_x, m_y, m_st_x, m_st_y);
    double dist_to_station = hypot((m_x-m_st_x), (m_y-m_st_y));
    int quadrant = angle_to_station-m_heading;
    if (quadrant < 0)
        quadrant = quadrant+360;
    quadrant = quadrant/90;

    m_m200.setDesiredRudder(m_des_rudder*1.2);
    if (m_inner_radius == -1)
        m_des_bow_thrust = cos(angle_to_station*M_PI/180);
    else
        m_des_bow_thrust = dist_to_station/m_inner_radius;

    switch (quadrant)
    {
        case 0:
            m_des_bow_thrust = -m_des_bow_thrust;
            break;
        case 1:
            m_des_bow_thrust = -m_des_bow_thrust;
            m_des_thrust = -m_des_thrust;
            break;
        case 2:
            m_des_thrust = -m_des_thrust;
            break;
    }

}
//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool DiffThrustControl::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();

  STRING_LIST sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(!m_MissionReader.GetConfiguration(GetAppName(), sParams))
    reportConfigWarning("No config block found for " + GetAppName());

  STRING_LIST::iterator p;
  for(p=sParams.begin(); p!=sParams.end(); p++) {
    string orig  = *p;
    string line  = *p;
    string param = tolower(biteStringX(line, '='));
    string value = line;

    bool handled = false;
    if (param == "max_rudder"){
        double tmp = stod(value);
        m_m200.setMaxRudder(tmp);
        handled = true;
    }
    else if (param == "max_thrust"){
        double tmp = stod(value);
        m_m200.setMaxThrust(tmp);
        handled = true;
    }
    else if(param == "bow_thrust_kd"){
        double tmp = stod(value);
        handled = m_bow_thrust_PID.setKd(tmp);
    }
    else if(param == "bow_thrust_ki"){
        double tmp = stod(value);
        handled = m_bow_thrust_PID.setKi(tmp);
    }
    else if(param == "bow_thrust_kp"){
        double tmp = stod(value);
        handled = m_bow_thrust_PID.setKp(tmp);
    }
    else if(param == "bow_thrust_max"){
        double tmp = stod(value);
        handled = m_bow_thrust_PID.setMax(tmp);
    }
    else if(param == "bow_thrust_min"){
        double tmp = stod(value);
        handled = m_bow_thrust_PID.setMin(tmp);
    }
    else if(param == "bow_thrust"){
        if(value == "true")
            m_turn_bow_thrust = true;
        else
            m_turn_bow_thrust = false;
        handled = true;
    }
    else if(param == "bow_thrust_ratio"){
        double tmp = stod(value);
        m_bow_thrust_ratio = tmp;
        handled = true;
    }
    else if(param == "max_angular"){
        double tmp = stod(value);
        m_m200.setAngularMax(tmp);
        handled = true;
    }
    else if(param == "rudder_gain"){
        double tmp = stod(value);
        m_m200.setRudderGain(tmp);
        m_rudder_gain = tmp;
        handled = true;
    }

    if (!handled)
      reportUnhandledConfigWarning(orig); 
  }
  registerVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void DiffThrustControl::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  Register("DESIRED_RUDDER", 0);
  Register("DESIRED_THRUST", 0);
  Register("DESIRED_BOW_THRUST", 0);
  Register("NAV_BOW_SPEED", 0);
  Register("NAV_Z_ANGULAR_V", 0);
  Register("REVERSE_SPEED", 0);
  Register("KEEP_HDG", 0);
  Register("NAV_Y", 0);
  Register("NAV_X", 0);
  Register("NAV_HEADING", 0);
  Register("STAT_X", 0);
  Register("STAT_Y", 0);
  Register("STAT_INNER_RADIUS", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool DiffThrustControl::buildReport() 
{
  m_msgs << "============================================ \n";
  m_msgs << "File:                                        \n";
  m_msgs << "============================================ \n";
  if(m_turn_bow_thrust == true)
      m_msgs << "Bow thrust turn on" << "\n";
  else
      m_msgs << "Bow thrust turn off" << "\n";
  m_msgs << "Desired thrust :      " << m_m200.getDesiredThrust() << "\n";
  m_msgs << "Desired rudder :      " << m_m200.getDesiredRudder() << "\n";
  m_msgs << "Z Angular V  :        " << m_m200.getZAngularV() << "\n";
  m_msgs << "Keep hdg :            " << m_keep_hdg << "\n";
  m_msgs << "Desired bow thrust :  " << m_des_bow_thrust << "\n";
  m_msgs << "Bow speed :           " << m_bow_speed << "\n";
  
  ACTable actab(3);
  actab << "Thrust_L | Thrust_Bow | Thrust_R ";
  actab.addHeaderLines();
  actab << m_m200.getDesiredLeftMotor() << m_bow_thrust_PID.getPIDOutput() << m_m200.getDesiredRightMotor();
  m_msgs << actab.getFormattedString();

  return(true);
}
