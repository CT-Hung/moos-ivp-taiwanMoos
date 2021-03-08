/************************************************************/
/*    NAME: ChengXuan_Tsai                                              */
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
// Initialize the member variables
  m_first_reading = true;
  m_current_x = 0.0;
  m_current_y = 0.0;
  m_previous_x = 0.0;
  m_previous_y = 0.0;
  m_total_distance = 0.0;
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

//Read the x,y data 
     if (key =="NAV_X")
       m_current_x = msg.GetDouble();
     else if (key =="NAV_Y")
       m_current_y = msg.GetDouble();		

     if(key == "NAV_X") 
       cout << "great!";

     else if(key != "NAV_X"&& key != "NAV_Y") // handled by AppCastingMOOSApp
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
//

//Define a function called "dist()" to calculate the distance
double dist(double cx,double px, double cy, double py)
{
  double distance = sqrt((cx-px)*(cx-px)+(cy-py)*(cy-py));
  return distance;
}
bool Odometry::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!
//Calculate distance by function "dist( )" defined above  
  if(m_first_reading)
   m_first_reading = false;
  else
   m_total_distance += dist(m_current_x,m_previous_x,m_current_y,m_previous_y);
//Assign the current x,y value to the previous ones in order to iterate the calcullation   
  m_previous_x = m_current_x;
  m_previous_y = m_current_y;
  
  Notify("ODOMETRY_DIST",m_total_distance);
  
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
  m_MissionReader.EnableVerbatimQuoting(true);
  if(!m_MissionReader.GetConfiguration(GetAppName(), sParams))
    reportConfigWarning("No config block found for " + GetAppName());
 STRING_LIST::iterator p;
  for(p=sParams.begin(); p!=sParams.end(); p++) {
    string orig  = *p;
    string line  = *p;
    string param = tolower(biteStringX(line, '='));
    string value = line;
    bool handled = false;
 
    if(param == "nav_x") {
      handled = true;
    }
    else if(param == "nav_y") {
      handled = true;
    }

//    if(!handled)
//      reportUnhandledConfigWarning(orig);

  }
  
  registerVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void Odometry::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  Register("NAV_X", 0);
  Register("NAV_Y", 0);
  // Register("FOOBAR", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool Odometry::buildReport() 
{
  m_msgs << "============================================" << endl;
  m_msgs << "File:                                       " << endl;
  m_msgs << "============================================" << endl;

  m_msgs << "Total distance is : " <<m_total_distance<< endl;
  ACTable actab(4);
  actab << "Alpha | Bravo | Charlie | Delta";
  actab.addHeaderLines();
  actab << "one" << "two" << "three" << "four";
  m_msgs << actab.getFormattedString();

  return(true);
}




