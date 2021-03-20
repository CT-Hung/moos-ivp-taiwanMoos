/************************************************************/
/*    NAME: ChengXuan_Tsai                                              */
/*    ORGN: MIT, Cambridge MA                               */
/*    FILE: PrimeFactor.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef PrimeFactor_HEADER
#define PrimeFactor_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <list>
#include <string>
#include <cstdint>
#include "PrimeEntry.h"
using namespace std;
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

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();

 private: // Configuration variables
   list<string> NUM_LIST;
   list<string> NUM_LIST_LARGE;
   list<string> TOTAL_PRIME;
   list<string> CURRENT_PRIME;
   bool interrupt;
   bool IF_FINISH;
   list<string> PRIME_BACKUP;
   uint64_t backup_NUM;
   PrimeEntry Timer;
 private: // State variables
};

#endif 
