/************************************************************/
/*    NAME: ChengXuan_Tsai                                              */
/*    ORGN: MIT, Cambridge MA                               */
/*    FILE: PrimeFactor.cpp                                        */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "PrimeFactor.h"
#include "PrimeEntry.h"
#include <cstdint>

using namespace std;

//---------------------------------------------------------
// Constructor

PrimeFactor::PrimeFactor()
{
  NUM_LIST.clear();
  NUM_LIST_LARGE.clear();
  CURRENT_PRIME.clear();
  TOTAL_PRIME.clear();
  PRIME_BACKUP.clear();
  interrupt = false;
  IF_FINISH = true;
  backup_NUM = 0;
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
    string key    = msg.GetKey();
    string sval   = msg.GetString(); 
#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif
    cout<<"checkpoint2"<<endl;
    uint64_t value;
    if(key == "NUM_VALUE")
     {
       value = stoull(sval);
       cout<<value<<endl;
       Timer.setReceivedIndex(1);
       if(value<=1000)
	{
          NUM_LIST.push_back(sval);
	}
       else
	{
          NUM_LIST_LARGE.push_back(sval);
        }
     }
    if(key == "NUM_VALUE") 
       cout << "great!";

     else if(key != "NUM_VALUE") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool PrimeFactor::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool PrimeFactor::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!
  cout<<"checkpoint3"<<endl;
  bool small_num=false;
  bool stop = false;
  list<string>::iterator k,p,q,k_l,p_l,q_l;
  uint64_t NUM,NUM_l,PRIME,PRIME_l,i,j;
  double time;  
  stringstream m_prime,m_prime_l;
  string m_result,m_result_l,PRIME_string,PRIME_string_l,backup;
 
  if (IF_FINISH == false)
    {
       cout<<"interrupt"<<endl;
       interrupt = true;
       PRIME_BACKUP =  CURRENT_PRIME;
       backup_NUM = NUM_l;
    }

if ((NUM_LIST.size() != 0|| NUM_LIST_LARGE.size() != 0)&& stop==false)
{
   Timer.setStartTime(MOOSTime());
   if (NUM_LIST.size()!=0)
    {
      small_num = true;
    }
  else
    {
      small_num = false;
    }
  if(small_num==true)
   {
     cout<< "checkpoint6"<<endl;
     for(p=NUM_LIST.begin();p!=NUM_LIST.end();p++)
   {
     NUM = stoull(*p);
     Timer.setOriginalVal(NUM);
     IF_FINISH = false;
     for(i=2;i<10000000;)
     {
      if(!(NUM%i))
      {
	CURRENT_PRIME.push_back(to_string(i));
	NUM = NUM/i;
      }
      else if (i>sqrt(NUM))
      {
	if(NUM!=1)      
	CURRENT_PRIME.push_back(to_string(NUM));
	IF_FINISH = true;
	break;
      }
      else
      {
	i++;
      }
     }
     
    if(IF_FINISH)
     {	     
      	Timer.setCalculatedIndex(1);
	Timer.setEndTime(MOOSTime());
	m_prime<<Timer.getReport();
	m_prime<<",solve_time="<<Timer.getElapsedTime();
        m_prime<<",primes=";      
        for(q=CURRENT_PRIME.begin();q!=CURRENT_PRIME.end();q++)
       {
         m_prime<<*q<<":";
	 cout<<*q<<"in list"<<endl;
       }
       m_prime<<",username=shane";
       m_prime>>m_result;
       cout<<m_result<<"<--- result"<<endl;
       Notify("PRIME_RESULT",m_result);
       NUM_LIST.pop_front();
       m_prime.str("");
       m_prime.clear();
       CURRENT_PRIME.clear();
       break;
     }
    }
   }
    else if(small_num==false)
   {
     for(p_l=NUM_LIST_LARGE.begin();p_l!=NUM_LIST_LARGE.end();p_l++)
   {
     NUM_l = stoull(*p_l);
     IF_FINISH = false;
     Timer.setOriginalVal(NUM_l);
     if (interrupt)
     {
	cout<< "checkpoint 5";
	NUM_l = backup_NUM;
	CURRENT_PRIME = PRIME_BACKUP;
	interrupt = false;
     }
     for(j=2;j<100000000;)
     {
      cout<<"checkpoint 4"<<endl;
      if(!(NUM_l%j))
      {
	CURRENT_PRIME.push_back(to_string(j));
	NUM_l = NUM_l/j;
	Timer.setEndTime(MOOSTime());
        time = Timer.getElapsedTime();
	if(time>=3)
	{
	//  stop = true;
	  break;
	}
      }
      else if (j>sqrt(NUM_l))
      {
	if(NUM_l!=1)
	CURRENT_PRIME.push_back(to_string(NUM_l));
	IF_FINISH = true;
	break;
      }
      else
      {
	j++;
      }
     }
       if(IF_FINISH)
      {	
	Timer.setCalculatedIndex(1);
	Timer.setEndTime(MOOSTime());
	m_prime_l<<Timer.getReport();
	m_prime_l<<",solve_time="<<Timer.getElapsedTime();
        m_prime_l<<",primes=";      
        for(q_l=CURRENT_PRIME.begin();q_l!=CURRENT_PRIME.end();q_l++)
        {
          m_prime_l<<*q_l<<":";
	  cout<<*q_l<<"in list"<<endl;
        }
        m_prime_l<<",username=shane";
        m_prime_l>>m_result_l;
        cout<<m_result_l<<"<--- result"<<endl;
        Notify("PRIME_RESULT",m_result_l);
        m_prime_l.str("");
        m_prime_l.clear();
	NUM_LIST_LARGE.pop_front();
        CURRENT_PRIME.clear();
	break;
      }
    }
    }
}
 /*    for(p=NUM_LIST.begin(); p!=NUM_LIST.end(); p++)
     {
      string str = *p;
      NUM = stoull(str);
      if(dval_int%2)
      {
        m_num_result="NUM_RESULT = \""+ to_string(dval_int) + ", odd\"";
	Notify("NUM_RESULT",m_num_result);
      }
      else 
      {
	m_num_result="NUM_RESULT = \""+ to_string(dval_int) + ",even\"";
	Notify("NUM_RESULT",m_num_result);
      }
      
     }*/
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool PrimeFactor::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();

  STRING_LIST sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(!m_MissionReader.GetConfiguration(GetAppName(), sParams))
    reportConfigWarning("No config block found for " + GetAppName());

  STRING_LIST::iterator p;
  for(p=sParams.begin(); p!=sParams.end(); p++) {
    string orig  = *p;
    string line  = *p;
    string param = tolower(biteStringX(line, '='));
    string value = line;

    bool handled = false;
    if(param == "num_value") {
      handled = true;
    }
   // else if(param == "bar") {
   //   handled = true;
   // }

    if(!handled)
      reportUnhandledConfigWarning(orig);

  }
  registerVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void PrimeFactor::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
  Register("NUM_VALUE", 0);
  cout<<"checkpoint1"<<endl;
}


//------------------------------------------------------------
// Procedure: buildReport()

bool PrimeFactor::buildReport() 
{
  m_msgs << "============================================" << endl;
  m_msgs << "File:                                       " << endl;
  m_msgs << "============================================" << endl;

  ACTable actab(4);
  actab << "Alpha | Bravo | Charlie | Delta";
  actab.addHeaderLines();
  actab << "one" << "two" << "three" << "four";
  m_msgs << actab.getFormattedString();

  return(true);
}




