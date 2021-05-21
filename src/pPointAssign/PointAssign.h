/************************************************************/
/*    NAME: tychien                                              */
/*    ORGN: MIT                                             */
/*    FILE: PointAssign.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef PointAssign_HEADER
#define PointAssign_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <list>
class PointAssign : public AppCastingMOOSApp
{
 public:
   PointAssign();
   ~PointAssign();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();
    void postViewPoint(double x, double y, std::string label, std::string color);
 private: // Configuration variables
    std::list<std::string> visit_point_list;
    std::string m_vname1;
    std::string m_vname2;
    int input_index;
    int output_index_1;
    int output_index_2;
    int output_index_f;
    int output_index_l;
    std::string  by_region;
    std::string index_1;
    std::string index_2;
    std::string index_l; 
    std::string index_f;
 private: // State variables
};

#endif 
