#include <string>
#include <iostream>
#include <cstdlib>
#include <math.h>
using namespace std;

#ifndef _COMMUNICATIONDATA_H
#define _COMMUNICATIONDATA_H


class CommunicationData
{
public:
  
  CommunicationData();
  ~CommunicationData(){};
  void set_my_name(string s) {my_name = s;}
  void set_my_nav_x(double x) {my_nav_x = x;}
  void set_my_nav_y(double y) {my_nav_y = y;}
  void set_my_nav_depth(double d) {my_nav_depth = d;}
  void set_my_nav_heading(double h) {my_nav_heading = h;}
  void set_my_nav_speed(double v) {my_nav_speed = v;}
  
  void set_co_name(string s) {co_name = s;}
  void set_co_nav_x(double x) {co_nav_x = x;}
  void set_co_nav_y(double y) {co_nav_y = y;}
  void set_co_nav_depth(double d) {co_nav_depth = d;}
  void set_co_nav_heading(double h) {co_nav_heading = h;}
  void set_co_nav_speed(double v) {co_nav_speed = v;}
  
  string read_my_name() {return my_name;}
  double read_my_nav_x() {return my_nav_x;}
  double read_my_nav_y() {return my_nav_y;}
  double read_my_nav_depth() {return my_nav_depth;}
  double read_my_nav_heading() {return my_nav_heading;}
  double read_my_nav_speed() {return my_nav_speed;}
  
  string read_co_name() {return co_name;}
  double read_co_nav_x() {return co_nav_x;}
  double read_co_nav_y() {return co_nav_y;} 
  double read_co_nav_depth() {return co_nav_depth;}
  double read_co_nav_heading() {return co_nav_heading;}
  double read_co_nav_speed() {return co_nav_speed;}

///////Set environment parameter////  

  void set_surface_sound_speed() {surface_sound_speed = 1480;}
  void set_sound_speed_gradient() {sound_speed_gradient = 0.016;}
  void set_water_depth() {water_depth = 6000;}
  void set_time_interval() {time_interval = 0.1;}
  
  double read_surface_sound_speed() {return surface_sound_speed;}
  double read_sound_speed_gradient() {return sound_speed_gradient;}
  double read_water_depth() {return water_depth;}
  double read_time_interval() {return time_interval;}
  

///////Geometry calculation/////////

  void set_midpoint_x(double x_my, double x_co) {midpoint_x = (x_my + x_co)/2.0;}
  void set_midpoint_y(double y_my, double y_co) {midpoint_y = (y_my + y_co)/2.0;}
  void set_midpoint_z(double z_my, double z_co) {midpoint_z = (z_my + z_co)/2.0;}
  void set_midpoint(); 
  double read_midpoint_x() {return midpoint_x;}
  double read_midpoint_y() {return midpoint_y;}
  double read_midpoint_z() {return midpoint_z;}
  
  void set_vector_x(double x) {vector_x = x;}
  void set_vector_y(double y) {vector_y = y;}
  void set_vector_z(double z) {vector_z = z;}

  double read_vector_x() {return vector_x;}
  double read_vector_y() {return vector_y;}
  double read_vector_z() {return vector_z;}
///This function will call the three functions above
  void set_vector();
  
  void set_center_x(double x) {center_x = x;}
  void set_center_y(double y) {center_y = y;}
  void set_center_z(double z) {center_z = z;}
  
  double read_center_x() {return center_x;}
  double read_center_y() {return center_y;}
  double read_center_z() {return center_z;}

  void set_center();

  void set_angle();
  double read_angle() {return angle;}
  
  void set_transmission_loss();
  double read_transmission_loss() {return transmission_loss;}
////Logical parameter

  void set_ifpath();
  bool read_ifpath() {return ifpath;}

  string get_report_ACOUSTIC_PATH();
  string get_report_CONNECTIVITY_LOCATION();

////Define variable
protected:
  string my_name;
  string co_name;
  double my_nav_x;
  double my_nav_y;
  double my_nav_depth;
  double my_nav_heading;
  double my_nav_speed;

  double co_nav_x;
  double co_nav_y;
  double co_nav_depth;
  double co_nav_heading;
  double co_nav_speed;

  double midpoint_x;
  double midpoint_y;
  double midpoint_z;

  double vector_x;
  double vector_y;
  double vector_z;

  double center_x;
  double center_y;
  double center_z;
  
  double angle;
  double surface_sound_speed;
  double sound_speed_gradient;
  double water_depth;
  double time_interval;
  double transmission_loss;

  bool ifpath;
} ;

#endif
