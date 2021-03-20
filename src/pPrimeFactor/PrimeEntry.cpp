

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "PrimeFactor.h"
#include "PrimeEntry.h"
#include <cstdint>

using namespace std;


PrimeEntry::PrimeEntry()
{
  m_done = false;
  m_start_time = 0;
  m_end_time = 0;
  m_received_index = 0;
  m_calculated_index = 0;
}
double PrimeEntry::getElapsedTime()
{
 return(m_end_time-m_start_time);
}
std::string PrimeEntry::getReport()
{
return("orig="+to_string(m_orig)+",received="+to_string(m_received_index)+",calculated="+to_string(m_calculated_index));
}
