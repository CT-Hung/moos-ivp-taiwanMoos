/************************************************************/
/*    NAME: ChengXuan_Tsai                                              */
/*    ORGN: MIT, Cambridge MA                               */
/*    FILE: CommunicationAngle.cpp                                        */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "CommunicationAngle.h"
#include "CommunicationData.h"
#include <string>
#include <math.h>
using namespace std;

//---------------------------------------------------------
// Constructor

CommunicationAngle::CommunicationAngle()
{
}

//---------------------------------------------------------
// Destructor

CommunicationAngle::~CommunicationAngle()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool CommunicationAngle::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();
    string sval = msg.GetString();
    double dval = msg.GetDouble();
#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif
     if(key == "NAV_X")
       Data.set_my_nav_x(dval);
     else if(key == "NAV_Y")
       Data.set_my_nav_y(dval);
     else if(key == "NAV_DEPTH")
       Data.set_my_nav_depth(dval);
     else if(key == "NAV_HEADING")
       Data.set_my_nav_heading(dval);
     else if(key == "NAV_SPEED")
       Data.set_my_nav_speed(dval);
     else if(key == "'collaborator'_NAV_X")
       Data.set_co_nav_x(dval);
     else if(key == "'collaborator'_NAV_Y")
       Data.set_co_nav_y(dval);
     else if(key == "'collaborator'_NAV_DEPTH")
       Data.set_co_nav_depth(dval);
     else if(key == "'collaborator'_NAV_HEADING")
       Data.set_co_nav_heading(dval);
     else if(key == "'collaborator'_NAV_SPEED")
       Data.set_co_nav_speed(dval);
     else if (key == "VEHICLE_NAME")
       Data.set_my_name(sval);
     else if (key == "COLLABORATOR_NAME")
       Data.set_co_name(sval);
 /*   if(key == "FOO") 
       cout << "great!";

     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   */
  }	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool CommunicationAngle::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool CommunicationAngle::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!
  Data.set_midpoint();
  Data.set_vector();
  Data.set_center();
  Data.set_angle();
  Data.set_transmission_loss();
  Data.set_ifpath();
  string a = Data.get_report_ACOUSTIC_PATH();
  string c = Data.get_report_CONNECTIVITY_LOCATION();
  Notify("ACOUSTIC_PATH",a);
  Notify("CONNECTIVITY_LOCATION",c);
AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool CommunicationAngle::OnStartUp()
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

void CommunicationAngle::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  double t = Data.read_time_interval();
  Register("NAV_X",t);
  Register("NAV_Y",t);
  Register("NAV_DEPTH",t);
  Register("NAV_HEADING",t);
  Register("NAV_SPEED",t);
  Register("'collaborator'_NAV_X",t);
  Register("'collaborator'_NAV_Y",t);
  Register("'collaborator'_NAV_DEPTH",t);
  Register("'collaborator'_NAV_HEADING",t);
  Register("'collaborator'_NAV_SPEED",t);
  Register("VEHICLE_NAME",t);
  Register("COLLABORATOR_NAME",t);
  // Register("FOOBAR", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool CommunicationAngle::buildReport() 
{
  m_msgs << "============================================" << endl;
  m_msgs << "File:                                       " << endl;
  m_msgs << "============================================" << endl;

  ACTable actab(4);
  actab << "Alpha | Bravo | Charlie | Delta";
  actab.addHeaderLines();
  actab << "one" << "two" << "three" << "four";
  m_msgs << actab.getFormattedString();

  return(true);
}




