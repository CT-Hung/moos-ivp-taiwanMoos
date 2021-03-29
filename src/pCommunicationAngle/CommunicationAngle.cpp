/************************************************************/
/*    NAME: Chao-Chun Hsu                                   */
/*    ORGN: MIT, Cambridge MA                               */
/*    FILE: CommunicationAngle.cpp                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "CommunicationAngle.h"

using namespace std;

//---------------------------------------------------------
// Constructor

CommunicationAngle::CommunicationAngle()
{
    m_vName = "";
    m_cName = "";
    m_vx = 0;;
    m_vy = 0;
    m_vd = 0;
    m_vh = 0;
    m_vs = 0;
    m_cx = 0;
    m_cy = 0;
    m_cd = 0;
    m_ch = 0;
    m_cs = 0;
    m_g = 0.016;      // sound speed gradient
    m_c0 = 1480;      // surface sound speed
    m_depth = 6000;   // water depth
    m_t = 0.25;
    m_degreR = 0;
    m_r = 0;
    m_eleAngle = 0;
    m_R = 0;
    m_TL = 0;
    m_chd = 0;

    m_iterations = 0;
    m_timewarp = 1;
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
    
    if(key == "VEHICLE_NAME"){
      m_vName = msg.GetString();
    }
    else if(key == "COLLABORATOR_NAME"){
      m_cName = msg.GetString();
      RegisterColl();
    }
    else if(key == "NAV_X"){
      m_vx = msg.GetDouble();
    }
    else if(key == "NAV_Y"){
      m_vy = msg.GetDouble();
    }
    else if(key == "NAV_DEPTH"){
      m_vd = msg.GetDouble();
    }
    else if(key == "NAV_HEADING"){
      m_vh = msg.GetDouble();
    }
    else if(key == "NAV_SPEED"){
      m_vs = msg.GetDouble();
    }
    else if(key == m_cName+"_NAV_X"){
      m_cx = msg.GetDouble();
    }
    else if(key == m_cName+"_NAV_Y"){
      m_cy = msg.GetDouble();
    }
    else if(key == m_cName+"_NAV_DEPTH"){
      m_cd = msg.GetDouble();
    }
    else if(key == m_cName+"_NAV_HEADING"){
      m_ch = msg.GetDouble();
    }
    else if(key == m_cName+"_NAV_SPEED"){
      m_cs = msg.GetDouble();
    }
    else
    {
      cout << "Incorrect message type for this app. See help..." << endl;
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
  Register("VEHICLE_NAME", 0);
  Register("COLLABORATOR_NAME", 0);
  Register("NAV_X", 0);
  Register("NAV_Y", 0);
  Register("NAV_DEPTH", 0);
  Register("NAV_HEADING", 0);
  Register("NAV_SPEED", 0);
  Register("'collaborator'_NAV_X", 0);
  Register("'collaborator'_NAV_Y", 0);
  Register("'collaborator'_NAV_DEPTH", 0);
  Register("'collaborator'_NAV_HEADING", 0);
  Register("'collaborator'_NAV_SPEED", 0);
  //Register("ACOUSTIC PATH", 0);
  //Register("CONNECTIVITY LOCATION", 0);
  //Register("ACOUSTIC PATH_REF", 0);
  //Register("CONNECTIVITY LOCATION_REF", 0);

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

//------------------------------------------------------------
// Procedure: RegisterColl()

void CommunicationAngle::RegisterColl()
{
  Register(m_cName+"_NAV_X", 0);
  Register(m_cName+"_NAV_Y", 0);
  Register(m_cName+"_NAV_DEPTH", 0);
  Register(m_cName+"_NAV_HEADING", 0);
  Register(m_cName+"_NAV_SPEED", 0);
}

