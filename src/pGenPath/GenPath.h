/************************************************************/
/*    NAME: tychien                                              */
/*    ORGN: MIT                                             */
/*    FILE: GenPath.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef GenPath_HEADER
#define GenPath_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <list>
struct Buffer
{
    double position[4];
    bool traveled;
};

class GenPath : public AppCastingMOOSApp
{
 public:
   GenPath();
   ~GenPath();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();
    Buffer CalMin(std::list<Buffer> input);
    //bool Same(Buffer& value){return(value==near);}
 private: // Configuration variables
//   std::string update_str;
    
    std::list<std::string> visit_point_list; //to store the raw data
    Buffer buffer; //to made a data type for xy array {position[2]}
    Buffer near;
    std::list<Buffer> m_xyid_list; //to store the array {position[2]} in to the list 
    int m_input_index;
    double x;
    double y;
    double id;
    double navx;
    double navy;
    std::string assign_final;
    Buffer orig_pos;
    double distance;

    
    Buffer largest_distance_temp_position;
    std::string m_update_str;
    double nearest_distance =300;
    double nearest_x;
    double nearest_y;
    Buffer temp;
    //double temp_dis=300;
    Buffer temp_point;
    std::list<Buffer> final_visit_list;
    std::string input_raw;
    std::string input_f;
    std::string input_l;
    std::string output;
   private: // State variables
};

#endif 
