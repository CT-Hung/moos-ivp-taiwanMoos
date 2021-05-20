/************************************************************/
/*    NAME: Chao-Chun Hsu                                              */
/*    ORGN: MIT, Cambridge MA                               */
/*    FILE: SensorRequest.cpp                                        */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#include <iterator>
#include <string>
#include "MBUtils.h"
#include "ACTable.h"
#include "SensorRequest.h"

using namespace std;

//---------------------------------------------------------
// Constructor

SensorRequest::SensorRequest()
{
   m_name = "";
   m_nav_x = 0;
   m_nav_y = 0;
   m_is_trigger = 0;
}

//---------------------------------------------------------
// Destructor

SensorRequest::~SensorRequest()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool SensorRequest::OnNewMail(MOOSMSG_LIST &NewMail)
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

    if(key == "START_TEMP_MEASURE"){
      m_is_trigger = msg.GetDouble();
    }
    if(key == "V_NAME"){
      m_name = msg.GetString();
    }
    if(key == "NAV_X"){
      m_nav_x = msg.GetDouble();
    }
    if(key == "NAV_Y"){
      m_nav_y = msg.GetDouble();
    }

     if(key == "FOO") 
       cout << "great!";

     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool SensorRequest::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool SensorRequest::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!
  if(m_is_trigger == 1){
    string m_string;
    m_string += "vname=" + m_name;
    m_string += ",x=" + to_string(m_nav_x);
    m_string += ",y=" + to_string(m_nav_y);
    Notify( "UCTD_SENSOR_REQUEST", m_string);
  }
  // Do your thing above!
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool SensorRequest::OnStartUp()
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

void SensorRequest::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
  Register("START_TEMP_MEASURE", 0);
  Register("V_NAME", 0);
  Register("NAV_X", 0);
  Register("NAV_Y", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool SensorRequest::buildReport() 
{
  m_msgs << "============================================" << endl;
  m_msgs << "File:                                       " << endl;
  m_msgs << "============================================" << endl;

  ACTable actab(4);
  actab << "Alpha | Bravo | Charlie | Delta | Echo | Foxtrot | Golf";
  actab.addHeaderLines();
  actab << "one" << "two" << "three" << "four" << "five" << "six" << "seven";
  m_msgs << actab.getFormattedString();

  return(true);
}




