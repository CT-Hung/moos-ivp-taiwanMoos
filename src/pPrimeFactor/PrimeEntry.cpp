#include <iterator>
#include <math.h>
#include <iostream>
#include <sstream>
#include "PrimeEntry.h"

using namespace std;

//---------------------------------------------------------
// Constructor

PrimeEntry::PrimeEntry()
{
  m_start_index      = 0;
  m_orig             = 0;
  m_done             = false;
  m_received_index   = 0;
  m_calculated_index = 0;
  m_factors          = {};

};

bool PrimeEntry::factor(unsigned long int max_steps)
{
  uint64_t numerical_val;

  int i;

  // started & not prime
  if(this->m_started == true && this->m_factors.size() > 0)
  {
    numerical_val = this->getOriginVal()/this->m_factors.back();
    i = this->m_part_way;
  }
  // started & is prime
  else if(this->m_start_index == true)
  {
    numerical_val = this->getOriginVal();
    i = this->m_part_way;
  }
  // first iteration
  else
  {
      numerical_val = this->getOriginVal();
      i = 2;
  }

  for(; i <= int(sqrt(numerical_val)) && max_steps > 0; i++)
  {
    max_steps--;
    if(numerical_val % 1 == 0 && i == int(sqrt(numerical_val)))
    {
      this->setFactor(i);
      this->setFactor(i);
      this->setDone(true);

      return false; 
    }
    else if(i == int(sqrt(numerical_val)))
    {
      this->setFactor(numerical_val);
      this->setDone(true);

      return false;
    }
    else if(numerical_val % i == 0)
    {
      this->setFactor(i);
      cout << i << endl;
      numerical_val/=i;
      i = 2;

    }
    else
    {
      this->m_start_index = i;
    }
  }
  return false;
}
  
std::string  PrimeEntry::getReport()
{
  string num_result = "orig=";

  ostringstream origss;
  origss << this->m_orig;

  num_result += origss.str();
  num_result += ", received=";

  ostringstream recvss;
  recvss << this->m_received_index;
  
  num_result += recvss.str();
  num_result += ", calculated=";

  ostringstream calcss;
  calcss << this->m_calculated_index;

  num_result += calcss.str();
  num_result += ", solve_time=";

  double calcTime = this->m_ctime - this->m_rtime;
  ostringstream time;

  time << calcTime;

  num_result += time.str();

  num_result += ", primes=";
  for(std::vector<uint64_t>::iterator it = m_factors.begin(); it != m_factors.end(); ++it)
  {
      ostringstream factor;
      factor << *it;

      num_result += factor.str();
      if(it < m_factors.end() - 1)
      {
        num_result += ":";
      }

  }

  num_result += ", username=cchsu";

  return num_result;

}