/************************************************************/
/*    NAME: Max                                              */
/*    ORGN: MIT                                             */
/*    FILE: Odometry.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <list>
#include <iterator>
#include "MBUtils.h"
#include "Odometry.h"

using namespace std;

//---------------------------------------------------------
// Constructor

Odometry::Odometry()
{
  bool   m_first_reading = false;
  double m_current_x = 0;
  double m_current_y = 0;
  double m_previous_x = m_current_x;
  double m_previous_y = m_current_y;
  double m_total_distance = 0;

}

//---------------------------------------------------------
// Destructor

Odometry::~Odometry()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

 //Set previous_x to current_x, and current_x to the new NAV_X value. Same for Y. 


bool Odometry::OnNewMail(MOOSMSG_LIST &NewMail)
{
   MOOSMSG_LIST::iterator p;
   
   for(p=NewMail.begin(); p!=NewMail.end(); p++) {
      CMOOSMsg &msg = *p;

      string key = msg.GetKey();
      double dval = msg.GetDouble();

      if(key == "NAV_X") {
        m_previous_x = m_current_x;
        m_current_x = dval;
      }
      else if(key == "NAV_Y") {
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

bool Odometry::Iterate()
{
   // happens AppTick times per second
   //    Edit the Odometry::Iterate() method to calculate the new distance. Handle the special case of the first navigation measurement. Post the total distance to the MOOS variable ODOMETRY_DIST. 
   double dist;

   dist = sqrt( pow(m_previous_x-m_current_x, 2) + pow(m_previous_y-m_current_y, 2) ); 
   m_total_distance += dist;

	 Notify("ODOMETRY_DIST", m_total_distance);

   return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
// happens before connection is open

bool Odometry::OnStartUp()
{
  list<string> sParams;                                                                                         
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {         
    list<string>::iterator p;                                           
    for(p=sParams.begin(); p!=sParams.end(); p++) {                 
      string original_line = *p;                                    
      string line = *p;                                    
      string param = stripBlankEnds(toupper(biteString(line, '=')));  
      string value = stripBlankEnds(line);
                                                           
      if(param == "FOO") {                                      
        //handled                                               
      }                                                             
      else if(param == "BAR") {                                  
        //handled                                     
      }                            
    }
  } 

  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void Odometry::RegisterVariables()
{
  // Register("FOOBAR", 0);
  Register("NAV_X", 0);
  Register("NAV_Y", 0); 
}

