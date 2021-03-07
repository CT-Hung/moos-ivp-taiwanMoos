/************************************************************/
/*    NAME: Jane Doe                                              */
/*    ORGN: MIT                                             */
/*    FILE: Odometry.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "Odometry.h"

using namespace std;

//---------------------------------------------------------
// Constructor

Odometry::Odometry()
{
}

Odometry::Odometry(bool first_reading, double current_x, double current_y, double previous_x, double previous_y, double total_distance)
{
  m_first_reading = first_reading;
  m_current_x = current_x;
  m_current_y = current_y;
  m_previous_x = previous_x;
  m_previous_y = previous_y;
  m_total_distance = total_distance;
}

//---------------------------------------------------------
// Destructor

Odometry::~Odometry()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool Odometry::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);
  
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;

#if 0 // Keep these around just for template
    string key   = msg.GetKey();
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif
    
    string key = msg.GetKey();
    double dval = msg.GetDouble();
    double dtim = msg.GetTime();

    if(m_current_x == 0 && m_current_y == 0)
      m_first_reading = true;
    if(key == "NAV_X"){
      m_previous_x = m_current_x;
      m_current_x = dval;
    }
    if(key == "NAV_Y"){
      m_previous_y = m_current_y;
      m_current_y = dval;
    }
   }

  return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool Odometry::OnConnectToServer()
{
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool Odometry::Iterate()
{
  AppCastingMOOSApp::Iterate();
  double last_distance = sqrt(pow(m_current_x-m_previous_x, 2) + pow(m_current_y-m_previous_y, 2));

  if(!m_first_reading){
    m_total_distance = last_distance;
  }else{
    m_total_distance += last_distance;
  }

  Notify("ODOMETRY_DIST", m_total_distance);

  AppCastingMOOSApp::PostReport();
  
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool Odometry::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();
  
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string line  = *p;
      string param = tolower(biteStringX(line, '='));
      string value = line;

      if(MOOSStrCmp(param, "NAV_X")){
	      m_previous_x = stod(value);
	      m_current_x = stod(value);
      }
      if(MOOSStrCmp(param, "NAV_Y")){
	      m_previous_y = stod(value);
	      m_current_y = stod(value);
      }
    }
  }
  m_total_distance = 0;
  
  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void Odometry::RegisterVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  
  m_Comms.Register("NAV_X", 0.1);
  m_Comms.Register("NAV_Y", 0.1);
}

bool Odometry::buildReport()
{
  m_msgs << "ODOMETRY_DIST=" << m_total_distance << endl;

  return(true);
}
