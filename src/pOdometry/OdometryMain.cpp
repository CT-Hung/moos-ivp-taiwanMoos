/************************************************************/
/*    NAME: Max                                              */
/*    ORGN: MIT                                             */
/*    FILE: OdometryMain.cpp                                    */
/*    DATE:                                                 */
/************************************************************/

#include <string>
#include "Odometry.h"

using namespace std;

int main(int argc, char *argv[])
{
  // default parameters file
  string sMissionFile = "Odometry.moos";
        
  //under what name shoud the application register with the MOOSDB?
  string sMOOSName = "pOdometry";
  
  switch(argc)
    {
    case 3:
      //command line says don't register with default name              
      sMOOSName = argv[2];
    case 2:
      //command line says don't use default config file
      sMissionFile = argv[1];
    }
  
  //make an application
  Odometry OdometryApp;

  //run it
  OdometryApp.Run(sMOOSName.c_str(), sMissionFile.c_str());
  
  return(0);
}

