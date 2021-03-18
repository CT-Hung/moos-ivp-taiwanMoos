

#ifndef PrimeFactor_HEADER
#define PrimeFactor_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include <iostream>
#include <cstdint>
#include <list>
#include "PrimeEntry.h"

class PrimeFactor : public CMOOSApp
{
 public:
   PrimeFactor();
   ~PrimeFactor(){};

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

   void setPrimeFactors(uint64_t &integer);
   double calcSolvingTime(PrimeEntry &entry) const;
   std::string getGlobalReport(PrimeEntry &entry) const;
   void setCurrentPrime(uint64_t &integer)  {m_current_prime = integer;};


 protected:
   void RegisterVariables();

   bool                 m_first_reading;  // registers first mail
   uint64_t             m_current_prime;  // current num to calculate primes 
   std::list<uint64_t>  m_prime_factors;  // hold
   std::string          m_prime_name;     // name of MOOS variable to publish
   uint64_t             m_max_iterations; // max iterations per Iterate-loop
   uint64_t             m_solved_primes;  // number of solved primes
   uint64_t             m_received_primes;// number of received primes
   std::list<PrimeEntry> m_all_entries;   // list of all current numbers 

};

#endif 