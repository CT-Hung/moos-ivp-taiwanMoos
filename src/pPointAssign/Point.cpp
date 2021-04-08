# include "Point.h"

using namespace std;

Point::Point()
{
  m_id = 0;
  m_x  = 0;
  m_y  = 0;
}

string Point::getInfo()
{
  ostringstream x_ostr;
  ostringstream y_ostr;
  ostringstream id_ostr;
  string info_str = "x=";

  x_ostr << this->m_x;
  info_str += x_ostr.str();

  info_str += ", y=";

  y_ostr << this->m_y;
  info_str += y_ostr.str();

  info_str += ", id=";

  id_ostr << this->m_id;
  info_str += id_ostr.str();

  return info_str;
}