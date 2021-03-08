/************************************************************/
/*    NAME: Chao-Chun Hsu                                              */
/*    ORGN: MIT, Cambridge MA                               */
/*    FILE: Odometry.cpp                                        */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "Odometry.h"

using namespace std;

//---------------------------------------------------------
// Constructor

Odometry::Odometry()
{
  m_first_reading = true;
  m_current_x = 0;
  m_current_y = 0;
  m_previous_x = 0;
  m_previous_y = 0;
  m_total_distance = 0;
  m_iterations = 0;
  m_timewarp = 1;
}

Odometry::Odometry(bool first_reading, double current_x, double current_y, double previous_x, double previous_y, double total_distance, unsigned int iterations, int timewarp)
{
  m_first_reading = first_reading;
  m_current_x = current_x;
  m_current_y = current_y;
  m_previous_x = previous_x;
  m_previous_y = previous_y;
  m_total_distance = total_distance;
  m_iterations = iterations;
  m_timewarp = timewarp;
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

  m_previous_x = m_current_x;
  m_previous_y = m_current_y;

  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;

    string key = msg.GetKey();

    if(key == "NAV_X"){
      m_current_x = msg.GetDouble();
    }
    if(key == "NAV_Y"){
      m_current_y = msg.GetDouble();
    }
  }
  #if 0 // Keep these around just for template
      string comm  = msg.GetCommunity();
      double dval  = msg.GetDouble();
      string sval  = msg.GetString(); 
      string msrc  = msg.GetSource();
      double mtime = msg.GetTime();
      bool   mdbl  = msg.IsDouble();
      bool   mstr  = msg.IsString();
  #endif
	
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
  m_total_distance += last_distance;

  Notify("ODOMETRY_DIST", m_total_distance);
  m_iterations++;

  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool Odometry::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();

  STRING_LIST sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(!m_MissionReader.GetConfiguration(GetAppName(), sParams))
    reportConfigWarning("No config block found for " + GetAppName());
  else
  {
    STRING_LIST::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string orig_line  = *p;
      string param = tolower(biteStringX(orig_line, '='));
      string value = orig_line;

      bool handled = false;
      if(param == "foo") {
        handled = true;
      }
      else if(param == "bar") {
        handled = true;
      }

      if(!handled)
        reportUnhandledConfigWarning(orig_line);

    }
  }

  m_total_distance = 0;
  
  RegisterVariables();
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void Odometry::RegisterVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
  Register("NAV_X", 0);
  Register("NAV_Y", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool Odometry::buildReport() 
{
  //m_msgs << "============================================" << endl;
  //m_msgs << "File:                                       " << endl;
  //m_msgs << "============================================" << endl;

  //ACTable actab(4);
  //actab << "Alpha | Bravo | Charlie | Delta";
  //actab.addHeaderLines();
  //actab << "one" << "two" << "three" << "four";
  //m_msgs << actab.getFormattedString();
  
  m_msgs << "ODOMETRY_DIST=" << m_total_distance << endl;

  return(true);
}




