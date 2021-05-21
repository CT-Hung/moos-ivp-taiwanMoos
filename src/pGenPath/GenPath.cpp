/************************************************************/
/*    NAME: tychien                                              */
/*    ORGN: MIT                                             */
/*    FILE: GenPath.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "XYSegList.h"
#include "GenPath.h"

using namespace std;

//---------------------------------------------------------
// Constructor

GenPath::GenPath()
{
    m_input_index = 0;
    input_raw="";
    input_f ="";
    input_l ="";
    output="";
    final_visit_list.empty();
}

//---------------------------------------------------------
// Destructor

GenPath::~GenPath()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool GenPath::OnNewMail(MOOSMSG_LIST &NewMail)
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
    //---------------------------------------------------------------
    if(key == "VISIT_ASSIGN"){ 
        //temp_point=orig_pos;
        string input_point = msg.GetString();
        visit_point_list.push_back(input_point);
        m_input_index++;
        input_raw += visit_point_list.front();
        cout << "m_input_index = "<<m_input_index << endl;   
        if(!visit_point_list.empty() && visit_point_list.front()=="first_point"){
            input_f += visit_point_list.front();
            visit_point_list.pop_front();
        }
        if(!visit_point_list.empty()&& visit_point_list.front()!="lastpoint"){
            string get_visit_point = visit_point_list.front();
            //find x,y,id---------------------------------- 
            string my_x   = tokStringParse(get_visit_point, "x",',','=');
            string my_y   = tokStringParse(get_visit_point, "y",',','=');
            string my_id  = tokStringParse(get_visit_point, "id",',','=');
            cout << "id=" << my_id << endl;
            //exchage datatype----------------------------- 
            x = atof(my_x.c_str());
            y = atof(my_y.c_str());
            id= atof(my_id.c_str());
            distance = sqrt(((x-navx)*(x-navx))+((y-navy)*(y-navy)));
            cout << "x=" << x << endl;
            cout << "y=" << y << endl;
            //store the new datatype into the list -------------
            
            Buffer buffer;
            buffer.position[0]=x;
            buffer.position[1]=y;
            //buffer.position[2]=id;
            //buffer.position[3]=distance;
            buffer.traveled = false;
            m_xyid_list.push_back(buffer);
            visit_point_list.pop_front(); 
            //if(distance<temp_dis){
            //    temp_dis=distance;
            //    near = buffer;
            //}
        }
        if(!visit_point_list.empty() && visit_point_list.front()=="lastpoint"){
            input_l += visit_point_list.front();
            //visit_point_list.pop_front();
        }
    } 
    //-----------------------------------------------
    else if(key == "NAV_X"){
        navx = msg.GetDouble();
        orig_pos.position[0]=navx;
    } 
    else if(key == "NAV_Y"){
        navy = msg.GetDouble();
        orig_pos.position[1]=navy;
    }
    else if(key == "ASSIGN_FINAL")
        assign_final = msg.GetString();
    else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool GenPath::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second
//bool Same(Buffer& value){return(value==near);}
bool GenPath::Iterate()
{
  AppCastingMOOSApp::Iterate();
  //Notify("UTS_PAUSE",false);
  // Do your thing here!
/*
  //1st Gen---------------------------------------------------------------
    nearest_x=CalMin(m_xyid_list).position[0];
    nearest_y=CalMin(m_xyid_list).position[1];
    temp = m_xyid_list.front();
    temp_dis=temp.position[2]; 
    if(visit_point_list.front()=="lastpoint"){
        list<Buffer>::iterator p;
        for(p=m_xyid_list.begin(); p!=m_xyid_list.end();p++)
        {
            Buffer& xydouble = *p;
            list<Buffer>::iterator q;
            for(q=next(m_xyid_list.begin());q!=m_xyid_list.end();q++)
            {
                Buffer& xydoubletwo = *q; 
                cout << xydouble.position[3] << endl;
                if(xydouble.position[3]>xydoubletwo.position[3])
                {
                    largest_distance_temp_position = xydouble;
                    xydouble=xydoubletwo;
                    xydoubletwo=largest_distance_temp_position;
                }
            } 
        }
        //publish the waypoint----------------------------------
        XYSegList my_seglist;
        list<Buffer>::iterator r;
        for(r=m_xyid_list.begin(); r!=m_xyid_list.end();)
        {
            Buffer& finalvisit = *r;
            my_seglist.add_vertex(finalvisit.position[0],finalvisit.position[1]);  
            r++;  
        }
        m_update_str = "points=";
        m_update_str+= my_seglist.get_spec();
        Notify("WPT_UPDATE",m_update_str);    
        //------------------------------------------------------ 
    }
    //-------------------------------------------------------------------
*/ 
    // 2nd Gen.------------------------------------------------------
    //temp_point=orig_pos;
    if(visit_point_list.front()=="lastpoint"){
        //bool same (const Buffer& value){return(value==near);}
        list<Buffer>::iterator it;
        double temp_dis=300;
        for(list<Buffer>::iterator p=m_xyid_list.begin();p!=m_xyid_list.end();)
        {
            Buffer& list_point = *p;
            double list_point_x = list_point.position[0];
            double list_point_y = list_point.position[1];
            double temp_point_x = temp_point.position[0];
            double temp_point_y = temp_point.position[1];
            double distance_between_list_point =sqrt(pow((list_point_x-temp_point_x),2)+pow((list_point_y-temp_point_y),2));        
            cout << list_point_x << "," << list_point_y << "||" << distance_between_list_point << endl;
            if(list_point.traveled==false && distance_between_list_point<temp_dis){
                temp_dis=distance_between_list_point;
                near =list_point;
                it=p;
            } 
            p++;
        }
        //What if m_xyid_list is empty ?????????????
        //
        //
        final_visit_list.push_back(near);
        it->traveled = true; // crash if add this line.
        //m_xyid_list.erase(it); //crash if add this line.
        temp_point = near;
        cout << "temp_dis=" << temp_dis << endl;  //but temp_dis is now always zero.
        //publish the waypoint----------------------------------
        XYSegList my_seglist;
        list<Buffer>::iterator r;
        for(r=final_visit_list.begin(); r!=final_visit_list.end();r++)
        {
            Buffer& finalvisit = *r;
            my_seglist.add_vertex(finalvisit.position[0],finalvisit.position[1]);  
        }
        m_update_str = "points=";
        m_update_str+= my_seglist.get_spec();
        Notify("WPT_UPDATE",m_update_str);    
    
    }


  AppCastingMOOSApp::PostReport();
  return(true);
}



//calculate to find the nearest point 
    //first: find the initial point
Buffer GenPath::CalMin(list<Buffer> input)
{ //Find the nearest point of this list from the current position. 
    Buffer nearest_point;
    for(list<Buffer>::iterator p=input.begin();p!=input.end();p++)
    {
        Buffer& xydouble = *p;
        if(xydouble.position[3]<nearest_distance)
        {
        nearest_distance=xydouble.position[3];
        Buffer nearest_point = xydouble;
        }
    }
    return nearest_point;
}










//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool GenPath::OnStartUp()
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

void GenPath::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
    
     Register("VISIT_ASSIGN",0);
     Register("NAV_X",0);
     Register("NAV_Y",0);
     Register("ASSIGN_FINAL",0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool GenPath::buildReport() 
{
  m_msgs << "============================================" << endl;
    m_msgs << "input_raw="<< input_raw << endl;
    m_msgs << "input_f=" << input_f << endl;
    m_msgs << "input_l=" << input_l << endl;
    m_msgs << "x=" << x << endl;
    m_msgs << "y=" << y << endl;
    m_msgs << "m_input_index=" << m_input_index << endl;
    m_msgs << "m_update_str=" << m_update_str << endl;
    m_msgs << "visit_point_list= "<< visit_point_list.front() << endl;
    m_msgs << "distance = "<< distance << endl;
    m_msgs << "NAV_X = " << navx << endl; 
    m_msgs << "NAV_Y = " << navy << endl;
    m_msgs << "my_xyid_length = " << m_xyid_list.size()<< endl;
    m_msgs << "visit_point_list.size()=" << visit_point_list.size() << endl;
    m_msgs << "nearest point =(" << nearest_x << "," << nearest_y <<")"<< endl;
    //m_msgs << "temp_dis=" << temp_dis<< endl;
    m_msgs << "temp_point =(" << temp_point.position[0] << "," << temp_point.position[1] << ")"<<endl;
    m_msgs << "orig_posx=" << orig_pos.position[0]<< endl;
    m_msgs << "ASSIGN_FINAL = " << assign_final << endl;
    m_msgs << "output=" << output << endl;
    m_msgs << "final_visit_list=" << final_visit_list.front().position[0]<<","<<final_visit_list.front().position[1]<<endl;
    return(true);
}




