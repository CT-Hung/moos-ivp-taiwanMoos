# include <string>
# include <iostream>
# include <sstream>

using namespace std;

class Point
{
 public:
  Point();
  ~Point() {};

  void   setID(int    val) {m_id = val; };
  void   setX (double val) {m_x  = val; };
  void   setY (double val) {m_y  = val; };
  
  int    getID()           {return m_id;};
  double get_x()           {return m_x; };
  double get_y()           {return m_y; };

  string getInfo();

 private:
  int    m_id;
  double m_x;
  double m_y; 
};