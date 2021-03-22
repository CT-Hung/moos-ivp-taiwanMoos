/************************************************************/
/*    NAME: Chao-Chun Hsu                                              */
/*    ORGN: MIT                                             */
/*    FILE: PrimeFactor.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include <string>
#include <cstdint>
#include <sstream>
#include "MBUtils.h"
#include "PrimeFactor.h"

using namespace std;

//---------------------------------------------------------
// Constructor

PrimeFactor::PrimeFactor()
{
  m_rcv_idx = 0;
  m_cal_idx = 0;
  m_iterations = 0;
  m_timewarp = 1;
}

//---------------------------------------------------------
// Destructor

PrimeFactor::~PrimeFactor()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool PrimeFactor::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);
  
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;

#if 0 // Keep these around just for template
    string key   = msg.GetKey();
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif
    
    string key = msg.GetKey();

    if(key == "NUM_VALUE")
    {
      PrimeEntry mprime;
      uint64_t numerical_val = strtoul(msg.GetString().c_str(), NULL, 0);

      m_rcv_idx++;
      mprime.setOriginalVal(numerical_val);
      mprime.setReceivedIndex(m_rcv_idx);
      mprime.setCalculatedIndex(0);
      mprime.setDone(false);
      mprime.m_started = false;
      mprime.setRTime(MOOSTime());

      m_num_val_msg.push_back(mprime);
    }
    else
    {
      cout << "Incorrect message type for this app. See help..." << endl;
    }

   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool PrimeFactor::OnConnectToServer()
{
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool PrimeFactor::Iterate()
{
  AppCastingMOOSApp::Iterate();

  if(m_num_val_msg.size() > 0)
  {
    for(std::list<PrimeEntry>::iterator it = m_num_val_msg.begin(); it != m_num_val_msg.end();)
    {
      PrimeEntry& entry = *it;

      entry.factor(10000);
      entry.setCTime(MOOSTime());

      if(entry.done())
      {
        m_cal_idx++;
        entry.setCalculatedIndex(m_cal_idx);
        string num_result = entry.getReport();

        Notify("PRIME_RESULT", num_result);
        it = m_num_val_msg.erase(it);
      }
      else
      {
        entry.m_started = true;
        entry.m_part_way += 10000;
        ++it;
      }
    }
  }
  
  AppCastingMOOSApp::PostReport();
  
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool PrimeFactor::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();

  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string line  = *p;
      string param = tolower(biteStringX(line, '='));
      string value = line;
      
      if(param == "foo") {
        //handled
      }
      else if(param == "bar") {
        //handled
      }
    }
  }
  
  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void PrimeFactor::RegisterVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
  Register("PRIME_RESULT", 0);
  Register("NUM_VALUE", 0);
}

//------------------------------------------------------------
// Procedure: buildReport()

bool PrimeFactor::buildReport() 
{ 
  //m_msgs << "PRIME_FACTOR=" << m_output_msg << endl;

  return(true);
}