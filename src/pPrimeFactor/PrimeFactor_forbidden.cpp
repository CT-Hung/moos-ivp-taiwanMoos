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
  backup_NUM = 0;
  uint64_t i, j, k;
  list<string>::iterator p;
  bool     IF_PRIME;

  TOTAL_PRIME.push_back("2");
  for(i=3;i<1000000;i++)
   {
     IF_PRIME = true;
     for(p=TOTAL_PRIME.begin(); p!=TOTAL_PRIME.end();p++)
     {
      j= stoull(*p);
      if(!(i%j))
      {
	IF_PRIME = false;
        break;	
      }
      else if (j>sqrt(i))
      {
	IF_PRIME = true;
	break;
      }
     }
     if(IF_PRIME == true)
      {
	TOTAL_PRIME.push_back(to_string(i));
      }
   }
  for(p=TOTAL_PRIME.begin(); p!=TOTAL_PRIME.end();p++)
     {
      cout<< stoi(*p)<<endl;
     }
    
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
    list<string>::iterator p;  
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
  if (NUM_LIST.size()!=0)
    {
      small_num = true;
    }
 list<string>::iterator k,p,q,k_l,p_l,q_l;
    uint64_t NUM,NUM_l,PRIME,PRIME_l;
    bool FINISH,FINISH_l = true;
    stringstream m_prime,m_prime_l;
    string m_result,m_result_l,PRIME_string,PRIME_string_l,backup;
 
 /* if (FINISH_l == false)
    {
       cout<<"interrupt";
       interrupt = true;
       PRIME_BACKUP =  CURRENT_PRIME;
       backup_NUM = NUM_l;
    }*/

while (NUM_LIST.size() != 0|| NUM_LIST.size() != 0)
{
  if(small_num==true)
   {
     for(k=NUM_LIST.begin();k!=NUM_LIST.end();k++)
    {
     cout<<"checkpoint4"<<endl;
     FINISH = false;
     NUM = stoull(*k);
     for(p=TOTAL_PRIME.begin(); p!=TOTAL_PRIME.end();)
     {
      PRIME= stoull(*p);
      cout<<NUM<<endl;
      if(NUM == 1)
      {
	FINISH = true;
	break;	
      }
      else if (NUM%PRIME==0)
      {
	cout<<"push !!"<<endl;
	PRIME_string = to_string(PRIME);
	CURRENT_PRIME.push_back(PRIME_string);
	NUM = NUM/PRIME;
	continue;
      }
      else if (PRIME>sqrt(NUM))
      {
	PRIME_string = to_string(NUM);
	CURRENT_PRIME.push_back(PRIME_string);
        FINISH = true;
	break;
      }
      else
      {
	p++;
      }
     }
     if(FINISH)
     {	     
       m_prime<<"primes=";      
       for(q=CURRENT_PRIME.begin();q!=CURRENT_PRIME.end();q++)
       {
         m_prime<<*q<<":";
	 cout<<*q<<"in list"<<endl;
       }
       m_prime<<",username=shane";
       m_prime>>m_result;
       cout<<m_result<<"<--- result"<<endl;
       Notify("NUM_RESULT",m_result);
       m_prime.str("");
       m_prime.clear();
       CURRENT_PRIME.clear();
     }
    }
   }
    else if(small_num==false)
   {
    for(k_l=NUM_LIST_LARGE.begin();k_l!=NUM_LIST_LARGE.end();k_l++)
    {
      cout<<"checkpoint4_l"<<endl;
      FINISH_l = false;
      NUM_l = stoull(*k_l);
      if(interrupt == true)
     {
       CURRENT_PRIME = PRIME_BACKUP;
       NUM_l = backup_NUM;
       interrupt = false;
     }
      for(p_l=TOTAL_PRIME.begin(); p_l!=TOTAL_PRIME.end();)
      {
       PRIME_l= stoull(*p_l);
       cout<<NUM_l<<endl;
       if(NUM_l == 1)
       {
       	 FINISH_l = true;
	 break;	
       }
       else if (NUM_l%PRIME_l==0)
       {
	 cout<<"push !!"<<endl;
	 PRIME_string_l = to_string(PRIME_l);
	 CURRENT_PRIME.push_back(PRIME_string_l);
	 NUM_l = NUM_l/PRIME_l;
	 continue;
       }
       else if (PRIME_l>sqrt(NUM_l))
       {
	 PRIME_string_l = to_string(NUM_l);
	 CURRENT_PRIME.push_back(PRIME_string_l);
         FINISH_l = true;
	 break;
       }
       else
       {
	 p_l++;
       }  
      }
      if(FINISH_l)
      {	
        m_prime_l<<"primes=";      
        for(q_l=CURRENT_PRIME.begin();q_l!=CURRENT_PRIME.end();q_l++)
        {
          m_prime_l<<*q_l<<":";
	  cout<<*q_l<<"in list"<<endl;
        }
        m_prime_l<<",username=shane";
        m_prime_l>>m_result_l;
        cout<<m_result_l<<"<--- result"<<endl;
        Notify("NUM_RESULT",m_result_l);
        m_prime_l.str("");
        m_prime_l.clear();
        CURRENT_PRIME.clear();
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




