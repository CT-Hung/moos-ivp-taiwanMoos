/************************************************************/
/*    NAME: tychien                                              */
/*    ORGN: MIT                                             */
/*    FILE: PointAssign.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "PointAssign.h"
#include "XYPoint.h"
using namespace std;

//---------------------------------------------------------
// Constructor

PointAssign::PointAssign()
{
    input_index=0;
    output_index_f=0;
    output_index_1=0;
    output_index_2=0;
    output_index_l=0;
    index_2="";
    index_f="";
    index_1="";
    index_l="";
    m_vname1 = "";
    m_vname2 = "";
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
        string input_point= msg.GetString();
        visit_point_list.push_back(input_point);
        input_index++;
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
    Notify("UTIMERSTART", "true");
    Notify("UTS_PAUSE","false");
    string visit_point_vname1= "VISIT_ASSIGN_"+toupper(m_vname1);
    string visit_point_vname2= "VISIT_ASSIGN_"+toupper(m_vname2);
   // cout <<input_index << endl;
    if(!visit_point_list.empty()){
        
        string get_visit_point = visit_point_list.front();
        /*
        //to find x y id  
        size_t find_x = get_visit_point.find("x=");
        size_t find_xcomma = get_visit_point.find(",");
        string my_x= get_visit_point.substr(find_x+2, find_xcomma-find_x-2);
        size_t find_y  = get_visit_point.find("y=");
        size_t find_ycomma = get_visit_point.find(",",11);
        string my_y= get_visit_point.substr(find_y+2, find_ycomma-find_y-2);
        size_t find_id  = get_visit_point.find("id=");
        string my_id = get_visit_point.substr(find_id+3,3);
        string id_modify = stripBlankEnds(my_id);
        cout << "id_modify=" << id_modify <<endl;
        */
        
        string my_x     = tokStringParse(get_visit_point, "x",',','=');
        string my_y     = tokStringParse(get_visit_point, "y",',','=');
        string my_id    = tokStringParse(get_visit_point, "id",',','=');
        
        double x = atof(my_x.c_str());
        double y = atof(my_y.c_str());
        int id;
        stringstream id_ss;
        id_ss<<my_id;
        id_ss>>id;
        //------------------------------------------------------
        //if by region 
        if(by_region=="TRUE"){
            Notify("VISIT_BYREGION","by_region"); 
            //-------------------------------------------------------- 
            if(visit_point_list.front()=="first_point"){
                Notify(visit_point_vname1,"first_point");
                Notify(visit_point_vname2,"first_point");
                visit_point_list.pop_front();
                index_f += get_visit_point;
                output_index_f ++;
            } 
            
            if(x>87.5){
                Notify(visit_point_vname1,get_visit_point);
                output_index_1 ++; 
                cout <<  "get_visit_point = " << get_visit_point<< endl;
                cout << "visit_point_vname1= " << visit_point_vname1 << endl;
                postViewPoint(x,y,my_id,"red");
                visit_point_list.pop_front();
                index_1 += get_visit_point;
            }
            if((x<87.5) && (x>-25) && (visit_point_list.front() !="first_point") && (visit_point_list.front() !="lastpoint")){
                if(visit_point_list.front()!="first_point"){
                Notify(visit_point_vname2,get_visit_point);
                visit_point_list.pop_front();
                index_2 += get_visit_point;
                cout << "pop" << endl;
                output_index_2 ++;
                postViewPoint(x,y,my_id,"yellow");
                }
            }
            else if(visit_point_list.front()=="lastpoint"){
                Notify(visit_point_vname1,"lastpoint");
                Notify(visit_point_vname2,"lastpoint");
                Notify("ASSIGN_FINAL","true");
                output_index_l ++;
                index_l += get_visit_point;
                visit_point_list.pop_front();
            }

        }
        
        //-------------------------------------------------------
        //if not by region
        if(by_region=="FALSE"){
            Notify("VISIT_BYREGION","by_ID");
            if(id%2==1){
                Notify(visit_point_vname1,get_visit_point);
                postViewPoint(x,y,my_id,"red");
            }
            else{
                Notify(visit_point_vname2,get_visit_point);
                postViewPoint(x,y,my_id,"yellow");
            }
        }
        
        //-------------------------------------------------------- 
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
    string param = toupper(biteStringX(line, '=')); //default is tolower
    string value = line;

    bool handled = false;
    if(param == "ASSIGN_BY_REGION") {
        by_region = value;
        handled = true;
    }
    else if(param == "VNAME1") {
        m_vname1 = value;
        handled = true;
    }
    else if(param == "VNAME2") {
        m_vname2 = value;
        handled = true;
    }
    else if(!handled)
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
  // Register("FOOBAR", 0);
    Register("VISIT_POINT",0);

}


//------------------------------------------------------------
// Procedure: buildReport()

bool PointAssign::buildReport() 
{
//  m_msgs << "============================================" << endl;
//  m_msgs << "File:                                       " << endl;
//  m_msgs << "============================================" << endl;
  m_msgs << "VNAME1="<< m_vname1 << endl;
  m_msgs << "VNAME2="<< m_vname2 << endl;
  m_msgs << "REGION=" << by_region << endl;
  m_msgs << "input_index=" << input_index << endl;
  m_msgs << "output_index1=" << output_index_1 << endl;
  m_msgs << "output_index2=" << output_index_2 << endl;
  m_msgs << "output_index_f="<< output_index_f << endl;
  m_msgs << "output_index_l="<< output_index_l << endl;
  m_msgs << "index_2=" << index_2 << endl;
  m_msgs << "index_1=" << index_1 << endl; 
  m_msgs << "index_f=" << index_f << endl;
  m_msgs << "index_l=" << index_l << endl;
  //  ACTable actab(4);
//  actab << "Alpha | Bravo | Charlie | Delta";
//  actab.addHeaderLines();
//  actab << "one" << "two" << "three" << "four";
//  m_msgs << actab.getFormattedString();

  return(true);
}

void PointAssign::postViewPoint(double x, double y, string label, string color)
 {
   XYPoint point(x, y);
   point.set_label(label);
   point.set_color("vertex", color);
   point.set_param("vertex_size", "5");

   string spec = point.get_spec();    // gets the string representation of a point
   Notify("VIEW_POINT", spec);
 }


