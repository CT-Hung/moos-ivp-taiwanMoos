// File: PrimeEntry.h
#include <string>
#include <vector>
#include <cstdint>
#include <math.h>
#include <iostream>
#include <sstream>

#ifndef PrimeEntry_HEADER
#define PrimeEntry_HEADER

class PrimeEntry
{
    public:
        PrimeEntry() {};
        ~PrimeEntry() {};

        void setOriginalVal(unsigned long int v) {m_orig=v;};
        void setReceivedIndex(unsigned int v) {m_received_index=v;}
        void setCalculatedIndex(unsigned int v) {m_calculated_index=v;}
        void setDone(bool v) {m_done=v;}
        
        // Time related functions
        void setStartTime(double v) {m_start_time=v;}
        void setEndTime(double v) {m_end_time=v;}
        
        double getElapsedTime() {return m_orig;};
        bool done() {return(m_done);}
        bool factor(unsigned long int max_steps);
        std::string getReport();

        uint64_t getFactor() {return m_factors.back();};
        void     setFactor(uint64_t v) {m_factors.push_back(v);};
        bool     started() {return m_factors.size() > 0;};
        double   m_part_way;
        bool     m_started;
    
    protected:
        uint64_t m_start_index;
        uint64_t m_orig;
        bool     m_done;

        unsigned int m_received_index;
        unsigned int m_calculated_index;
        
        std::vector<uint64_t> m_factors;
        
        // Time related member variables
        double m_start_time;
        double m_end_time;
};
#endif