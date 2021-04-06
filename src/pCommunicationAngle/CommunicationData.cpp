#include "CommunicationData.h"
#include <cstdlib>
#include <math.h>
using namespace std;

 CommunicationData::CommunicationData()
{
  my_nav_x = 0;
  my_nav_y = 0;
  my_nav_depth = 0;
  my_nav_heading = 0;
  my_nav_speed = 0;
  my_nav_heading = 0;
  
  co_nav_x = 0;
  co_nav_y = 0;
  co_nav_depth = 0;
  co_nav_heading = 0;
  co_nav_speed = 0;
  co_nav_heading = 0;

  midpoint_x = 0;
  midpoint_y = 0;
  midpoint_z = 0;

  vector_x = 0;
  vector_y = 0;
  vector_z = 0;

  center_x = 0;
  center_y = 0;
  center_z = 0;

  surface_sound_speed = 0;
  sound_speed_gradient = 0;
  water_depth = 0;
  time_interval = 0;

  ifpath = false;
 }
void CommunicationData::set_midpoint()
{
  double x_my = read_my_nav_x();
  double y_my = read_my_nav_y();
  double z_my = read_my_nav_depth();
  double x_co = read_co_nav_x();
  double y_co = read_co_nav_y();
  double z_co = read_co_nav_depth();
  set_midpoint_x(x_my,x_co);
  set_midpoint_y(y_my,y_co);
  set_midpoint_z(z_my,z_co);
}
void CommunicationData::set_vector()
{ double x_my = read_my_nav_x();
  double y_my = read_my_nav_y();
  double z_my = read_my_nav_depth();
  double x_co = read_co_nav_x();
  double y_co = read_co_nav_y();
  double z_co = read_co_nav_depth();
  double vector_x,vector_y,vector_z;
  vector_x = x_my - x_co;
  vector_y = y_my - y_co;
  vector_z = (vector_x*vector_x+vector_y*vector_y)/(z_my - z_co);
  set_vector_x(vector_x);
  set_vector_y(vector_y);
  set_vector_z(vector_z);
}

void CommunicationData::set_center()
{
  double x_mid = read_midpoint_x();
  double y_mid = read_midpoint_y();
  double z_mid = read_midpoint_z();
  double x_vec = read_vector_x();
  double y_vec = read_vector_y();
  double z_vec = read_vector_z();
  double c = read_surface_sound_speed();
  double g = read_sound_speed_gradient();

  double k = ((-1)*c/g-z_mid)/z_vec;
  double center_z = (-1)*c/g;
  double center_x = x_mid + k*x_vec;
  double center_y = y_mid + k*y_vec;

  set_center_x(center_x);
  set_center_y(center_y);
  set_center_z(center_z); 

}

void CommunicationData::set_angle()
{
  double center_x = read_center_x();
  double center_y = read_center_y();
  double center_z = read_center_z();
  double x_my = read_my_nav_x();
  double y_my = read_my_nav_y();
  double z_my = read_my_nav_depth();

   angle = 90-atan((center_z-z_my)/sqrt((center_x-x_my)*(center_x-x_my)+(center_y-y_my)*(center_y-y_my)));
}

void CommunicationData::set_ifpath()
{
  double center_x = read_center_x();
  double center_y = read_center_y();
  double center_z = read_center_z();
  double my_x = read_my_nav_x();
  double my_y = read_my_nav_y();
  double my_z = read_my_nav_depth();
  double co_x = read_co_nav_x();
  double co_y = read_co_nav_y();
  double co_z = read_co_nav_depth();
  double depth = read_water_depth();

  double angle_a = atan(sqrt((my_x-center_x)*(my_x-center_x)+(my_y-center_y)*(my_y-center_y))/(my_z-center_z));
  double angle_b = atan(sqrt((co_x-center_x)*(co_x-center_x)+(co_y-center_y)*(co_y-center_y))/(co_z-center_z));
  double angle_max, angle_min;
  if (angle_a > angle_b)
  {
    angle_max = angle_a;
    angle_min = angle_b;
  }
  else
  {
    angle_max = angle_b;
    angle_min = angle_a;
  }


  ifpath = true;
 for (double i = angle_min; i < angle_max; i += 0.1)
  {
    if (depth < sqrt((my_x-center_x)*(my_x-center_x)+(my_y-center_y)*(my_y-center_y)+(my_z-center_z)*(my_z-center_z))*cos(i))
    {
      ifpath = false;
      break;
    }
  } 
}


void CommunicationData::set_transmission_loss()
{
  double c = read_surface_sound_speed();
  double d = read_water_depth();
  double g = read_sound_speed_gradient();
  double cz = c + d * g;
  double angle = read_angle();
  transmission_loss = -20*log10(sqrt(cz*cos(angle)/c));
}

string CommunicationData::get_report_ACOUSTIC_PATH()
{
  double angle = read_angle();
  double transloss = read_transmission_loss();
  string angle_s = to_string(angle);
  string transloss_s = to_string(transloss);
  if (ifpath)
    return "elev_angle="+angle_s+", transmission_loss="+ transloss_s;
  else 
    return "NaN";
}

string CommunicationData::get_report_CONNECTIVITY_LOCATION()
{
 double x_my = read_my_nav_x();
 double y_my = read_my_nav_y();
 double x_co = read_co_nav_x();
 double y_co = read_co_nav_y();
 double d = read_my_nav_depth();

 string x_s = to_string((x_my+x_co)/2);
 string y_s = to_string((y_my+y_co)/2);
 string d_s = to_string(d);
 return "x="+ x_s +",y="+ y_s + ",depth=" + d_s;
}
