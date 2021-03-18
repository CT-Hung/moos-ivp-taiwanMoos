/************************************************************/
/*    NAME: Bory Huang                                              */
/*    ORGN: MIT, Cambridge MA                               */
/*    FILE: PrimeFactor.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef PrimeFactor_HEADER
#define PrimeFactor_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include <list>
#include "entry.h" 


using namespace std;

class PrimeFactor : public CMOOSApp
{
 public:
   PrimeFactor();
   ~PrimeFactor();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   
 protected:
   void RegisterVariables();

 private: // Configuration variables
  
 private: // State variables
   list<PrimeEntry> m_num_val_messages; //list entries
   unsigned int m_recieved_index;
   unsigned int m_calculated_index; 
};

#endif 
