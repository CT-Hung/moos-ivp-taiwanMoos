/************************************************************/
/*    NAME: Chao-Chun Hsu                                              */
/*    ORGN: MIT                                             */
/*    FILE: PrimeFactor.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef PrimeFactor_HEADER
#define PrimeFactor_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "PrimeEntry.h"


class PrimeFactor : public AppCastingMOOSApp
{
 public:
   PrimeFactor();
   ~PrimeFactor();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   bool buildReport();

 protected:
   void RegisterVariables();

 protected: //member variables 

 private: // Configuration variables

 private: // State variables
   unsigned int m_rcv_idx;
   unsigned int m_cal_idx;
   std::list<PrimeEntry> m_num_val_msg;

   unsigned int m_iterations;
   double m_timewarp;
};

#endif 
