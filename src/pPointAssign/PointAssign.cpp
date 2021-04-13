/************************************************************/
/*    NAME: Chao-Chun Hsu                                   */
/*    ORGN: MIT, Cambridge MA                               */
/*    FILE: PointAssign.cpp                                 */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "XYPoint.h"
#include "PointAssign.h"

using namespace std;

//---------------------------------------------------------
// Constructor

PointAssign::PointAssign()
{
  assign_by_region = false;
}

//---------------------------------------------------------
// Destructor

PointAssign::~PointAssign()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool PointAssign::OnNewMail(MOOSMSG_LIST &NewMail)
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

    if(key == "VISIT_POINT"){
      string str_val = msg.GetString();

      m_travel_points.push_back(str_val);
      m_Comms.Notify("VISIT_POINT_CHECK", str_val);
    }
    else if(key != "APPCAST_REQ"){
      reportRunWarning("Unhandles Mail: " + key);
    }

   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool PointAssign::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool PointAssign::Iterate()
{
  AppCastingMOOSApp::Iterate();
 
  // Do your thing here!
  m_Comms.Notify("UTS_PAUSE", "false");
  
  list<string>::iterator it;
  for(it = m_travel_points.begin(); it != m_travel_points.end(); ++it){
    Point m_point;
    string str_val = *it;

    m_Comms.Notify("STRING_VAL", str_val);

    // message processing
    vector<string> my_vector = parseString(str_val, ',');

    for(unsigned int c = 0; c < my_vector.size(); c++){
      string param = biteStringX(my_vector[c], '=');
      string value = my_vector[c];

      if(tolower(param) == "x"){
        double x_double = atof(value.c_str());
        m_point.setX(x_double);
      }
      else if(tolower(param) == "y"){
        double y_double = atof(value.c_str());
        m_point.setY(y_double);
      }
      else if(tolower(param) == "id"){
        int id_int = atoi(value.c_str());
        m_point.setID(id_int);
      }
    }
    it = m_travel_points.erase(it);
    m_points.push_back(m_point);
  }

  // assign by region is false
  if(!assign_by_region){
    m_Comms.Notify("ASSIGN_BY_REGION", "false");
    m_Comms.Notify("VISIT_POINT_HENRY", "firstpoint");
    m_Comms.Notify("VISIT_POINT_GILDA", "firstpoint");
    
    int count_henry = 0;
    int count_gilda = 0;

    for(vector<Point>::iterator it = m_points.begin(); it != m_points.end(); it++){
      Point& point = *it;

      cout << point.getInfo() << endl;

      if((point.getID() % 2) == 0){
        count_henry++;

        string id = intToString(point.getID());
        string color = "green";
        postViewPoint(point.get_x(), point.get_y(), id, color);
        
        m_Comms.Notify("VISIT_POINT_HENRY", point.getInfo());
      }
      else{
        count_gilda++;
        
        string id = intToString(point.getID());
        string color = "blue";
        postViewPoint(point.get_x(), point.get_y(), id, color);
        
        m_Comms.Notify("VISIT_POINT_GILDA", point.getInfo());
      }
    }

    m_Comms.Notify("VISIT_POINT_HENRY_COUNT", count_henry);
    m_Comms.Notify("VISIT_POINT_GILDA_COUNT", count_gilda);
    m_Comms.Notify("VISIT_POINT_HENRY", "lastpoint");
    m_Comms.Notify("VISIT_POINT_GILDA", "lastpoint");
  }
  // assign by region is true
  else{

  }


  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool PointAssign::OnStartUp()
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
    bool bool_value;

    bool handled = false;
    if(param == "assign_by_region") {
      setBooleanOnString(bool_value, value);
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

void PointAssign::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();

  Register("VISIT_POINT", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool PointAssign::buildReport() 
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
// Procedure: postViewPoint(double x, double y, string label, string color)
void PointAssign::postViewPoint(double x, double y, string label, string color)
{
  XYPoint point(x, y);
  point.set_label(label);
  point.set_color("vertex", color);
  point.set_param("vertex_size", "2");

  string spec = point.get_spec();
  m_Comms.Notify("VIEW_POINT", spec);
}


