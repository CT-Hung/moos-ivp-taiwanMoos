/************************************************************/
/*    NAME: Kiat                                            */
/*    ORGN: NYCU                                            */
/*    FILE: PrimeEntry.cpp                                 */
/*    DATE:                                                 */
/************************************************************/

#include "stdint.h"
#include <cstdlib>
#include "PrimeEntry.h"

using namespace std;

bool  PrimeEntry::factor(unsigned long int max_steps)
{
    uint64_t numerical_val;
    int i; //incrementer for counting through loop
    
    if (this->m_started == true && this->m_factors.size()>0)
    { //started & not prime
        numerical_val = this-> getElapsedTime()/this->m_factors.back();
        i = this->m_part_way;
    }
    else if (this->m_started == true) //started & prime
    {
        numerical_val = this-> getElapsedTime();
        i = this->m_part_way;
    }
    else //first iteration
    {
        numerical_val = this-> getElapsedTime(); // don't really need the accessor here
        i = 2;
    }


  
    for (;i <= int(sqrt(numerical_val)) && max_steps > 0; i++) 
    {
        max_steps = max_steps -1;
        if (numerical_val % i ==0 && i == int(sqrt(numerical_val)))
        {
            this->setFactor(i);
            this->setFactor(i);
            this->setDone(true);
            return false;
        }
        else if (i == int(sqrt(numerical_val)))
        {
            this->setFactor(numerical_val);
            this->setDone(true);
            return false;
        }
        else if (numerical_val % i == 0) 
        { 
            this->setFactor(i);
            cout << i << endl;
            numerical_val = numerical_val/i;
            i = 2;
        }
        else
        {
            this->m_start_index = i;
        }
    }
    return false;
}

string PrimeEntry::getReport()
{
    string num_result = "orig=";
    
    ostringstream origss;
    origss << this->m_orig;

    num_result += origss.str();

    num_result += ",received=";

    ostringstream recvss;
    recvss << this->m_received_index;

    num_result += recvss.str();

    num_result += ",calculated=";

    ostringstream calcss;
    calcss << this->m_calculated_index;

    num_result += calcss.str();

    num_result += ",solve_time=";

    double calcTime = (this->m_end_time - this->m_start_time);
    ostringstream time;

    time << calcTime;

    num_result += time.str();
    

    //solve time??
    num_result += "primes=";

    for (std::vector<uint64_t>::iterator it = m_factors.begin(); it != m_factors.end(); ++ it)
    {
        ostringstream factor;
        factor << *it;
        
        num_result += factor.str();
        if (it < m_factors.end()-1) 
        {    
            num_result += ":";
        }
    }
    num_result += ",username=ltencate";
    return num_result; 
}