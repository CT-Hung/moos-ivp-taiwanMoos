/************************************************************/
/*    NAME: Chao-Chun Hsu                                   */
/*    ORGN: MIT, Cambridge MA                               */
/*    FILE: CommunicationAngle.h                            */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef CommunicationAngle_HEADER
#define CommunicationAngle_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <math.h>
#include <iostream>

class CommunicationAngle : public AppCastingMOOSApp
{
 public:
   CommunicationAngle();
   ~CommunicationAngle();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   std::string m_vName;
   std::string m_cName;
   double m_vx;
   double m_vy;
   double m_vd;
   double m_vh;
   double m_vs;
   double m_cx;
	 double m_cy;
	 double m_cd;
	 double m_ch;
	 double m_cs;
	 double m_g;
	 double m_c0;
	 double m_depth;
	 double m_t;
	 double m_degreR;
	 double m_r;
	 double m_eleAngle;
	 double m_R;
	 double m_TL;
	 double m_chd;

   void getPosition();
   void soundTrack();
   void tranLoss();
   void RegisterColl();

   void registerVariables();

 private: // Configuration variables

 private: // State variables
   unsigned int m_iterations;
   double       m_timewarp;
};

#endif 
