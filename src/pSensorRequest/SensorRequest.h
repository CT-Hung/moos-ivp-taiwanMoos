/************************************************************/
/*    NAME: Chao-Chun Hsu                                              */
/*    ORGN: MIT, Cambridge MA                               */
/*    FILE: SensorRequest.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef SensorRequest_HEADER
#define SensorRequest_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "MOOS/libMOOS/MOOSLib.h"

class SensorRequest : public AppCastingMOOSApp
{
 public:
   SensorRequest();
   ~SensorRequest();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();
   std::string m_name;
   double m_nav_x;
   double m_nav_y;
   double m_is_trigger;

 private: // Configuration variables

 private: // State variables
};

#endif 
