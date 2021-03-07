/************************************************************/
/*    NAME: Jane Doe                                              */
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
  m_first_reading = 1; 
  m_current_x = 0;
  m_current_y = 0;
  m_previous_x = 0;
  m_previous_y = 0;
  m_total_distance = 0;
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
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif
    string key    = msg.GetKey();
    if(key == "NAV_X"){
      m_previous_x = m_current_x;
      m_current_x = msg.GetDouble();
    }
    if(key == "NAV_Y"){
      m_previous_y = m_current_y;
      m_current_y = msg.GetDouble();
    }
     if(key == "FOO") 
       cout << "great!";
     else if((key != "NAV_Y")&&(key != "NAV_X")) // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool Odometry::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool Odometry::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!

  if(m_first_reading == 1) // -- first time recieve 
  {
    m_total_distance = 0;
    Notify( "ODOMETRY_DIST", m_total_distance);
    m_first_reading = 0;
  }
  else
  {
    double delta_x = m_current_x - m_previous_x;
    double delta_y = m_current_y - m_previous_y;
    m_total_distance += sqrt(delta_x*delta_x + delta_y*delta_y);
    Notify( "ODOMETRY_DIST", m_total_distance);
  }

  // Do your thing above!
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

  STRING_LIST::iterator p;
  for(p=sParams.begin(); p!=sParams.end(); p++) {
    string orig  = *p;
    string line  = *p;
    string param = tolower(biteStringX(line, '='));
    string value = line;

    bool handled = false;
    if(param == "foo") {
      handled = true;
    }
    else if(param == "bar") {
      handled = true;
    }

    if(!handled)
      reportUnhandledConfigWarning(orig);

  }
  
  registerVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void Odometry::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  Register("NAV_X", 0); //-- register a specified topic --
  Register("NAV_Y", 0); //
  // Register("FOOBAR", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool Odometry::buildReport() 
{
  m_msgs << "Distance: " << m_total_distance << endl;

  return(true);
}




