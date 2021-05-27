#ifndef _PID_H_
#define _PID_H_

class PID
{
    public:
        PID();
        // Returns the manipulated variable given a setpoint and current process value
        ~PID();
        bool run(double, double, double);
        bool setKp(double);
        bool setKi(double);
        bool setKd(double);
        bool setMax(double);
        bool setMin(double);
        bool setStartTime(double);
        double getPIDOutput(){return m_pidOutput;};

    private:
        double m_kp;
        double m_ki;
        double m_kd;
        double m_max;
        double m_min;
        double m_dt;
        // Kp -  proportional gain
        // Ki -  Integral gain
        // Kd -  derivative gain
        // dt -  loop interval time
        // max - maximum value of manipulated variable
        // min - minimum value of manipulated variable
        double m_old_time;
        double m_old_error;
        double m_integral;
        double m_pidOutput;
};

#endif
