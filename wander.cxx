////////////////////////////////////////////////////////////////////////
// Filename    : wander.cxx
// Created by  : Deepak, John, Navin, Stephen
// Date        :  17 Aug 11
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) Carnegie Mellon University.  All rights reserved.
//
// All use of this software is subject to the terms of the revised BSD
// license.  You should have received a copy of this license along
// with this source code in a file named "LICENSE."
//
////////////////////////////////////////////////////////////////////

#include "wander.h"

/////////////////////////////////////////////////////////////////////////////////
//
// Function : rand_float
// Description : This function creates a random float point number

/////////////////////////////////////////////////////////////////////////////////

double rand_float() {
  const static double rand_max = 0x7fff;
  return ((rand()) / (rand_max + 1.0));
}

/////////////////////////////////////////////////////////////////////////////////
//
// Function : random_clamped
// Description : This function returns a random floating point number in the range
//               -1 to 1.

/////////////////////////////////////////////////////////////////////////////////

double random_clamped() {
  return  (rand_float() - rand_float());
}

Wander::Wander(AICharacter *ai_ch, double wander_radius,int flag, double aoe, float max_weight) 
: SteeringObjective(ai_ch, max_weight){
  _wander_radius = wander_radius;
  double theta = rand_float() * 2 * 3.14159;
  double si = rand_float() * 3.14159;
  _flag = flag;
  // Area around which the character should wander
  _area_of_effect = aoe;
  _init_pos = _ai_char->get_node_path().get_pos(_ai_char->get_char_render());
  // _flag is used by Wander to wander in a given axis
  // Value 0 - XY axes wander
  // Value 1 - YZ axes wander
  // Value 2 - XZ axes wander
  // Value 3 - XYZ axes wander
  // default is XY axes
  switch(_flag) {
    case 0: {
              _wander_target = LVecBase3f(_wander_radius * cos(theta), _wander_radius * sin(theta),0);
              break;
            }
    case 1: {
              _wander_target = LVecBase3f(0, _wander_radius * cos(theta), _wander_radius * sin(theta));
              break;
            }
    case 2: {
              _wander_target = LVecBase3f(_wander_radius * cos(theta), 0,  _wander_radius * sin(theta));
              break;
            }
    case 3: {
              _wander_target = LVecBase3f(_wander_radius * sin(theta) * cos(si), _wander_radius * sin(theta) * sin(si), _wander_radius * cos(theta));
              break;
            }
    default: {
              _wander_target = LVecBase3f(_wander_radius * cos(theta), _wander_radius * sin(theta),0);
              break;
             }
  }
}

Wander::~Wander() {
}

/////////////////////////////////////////////////////////////////////////////////
//
// Function : do_wander
// Description : This function performs the wander and returns the wander force which is used
//               in the calculate_prioritized function.
//               This function is not to be used by the user.

/////////////////////////////////////////////////////////////////////////////////

LVecBase3f Wander::do_wander() {
  LVecBase3f present_pos = _ai_char->get_node_path().get_pos(_ai_char->get_char_render());
  // Create the random slices to enable random movement of wander for x,y,z respectively
  double time_slice_1 = random_clamped() * 1.5;
  double time_slice_2 = random_clamped() * 1.5;
  double time_slice_3 = random_clamped() * 1.5;
  switch(_flag) {
  case 0: {
            _wander_target += LVecBase3f(time_slice_1, time_slice_2, 0);
            break;
          }
  case 1: {
            _wander_target += LVecBase3f(0, time_slice_1, time_slice_2);
            break;
          }
  case 2: {
            _wander_target += LVecBase3f(time_slice_1, 0, time_slice_2);
            break;
          }
  case 3: {
            _wander_target += LVecBase3f(time_slice_1, time_slice_2, time_slice_3);
            break;
          }

  default: {
            _wander_target = LVecBase3f(time_slice_1, time_slice_2, 0);
           }
  }
  _wander_target.normalize();
  _wander_target *= _wander_radius;
  LVecBase3f target = _ai_char->get_char_render().get_relative_vector(_ai_char->get_node_path(), LVector3f::forward());
  target.normalize();
  // Project wander target onto global space
  target = _wander_target + target;
  LVecBase3f desired_target = present_pos + target;
  LVecBase3f desired_velocity = desired_target - _ai_char->get_node_path().get_pos() ;
  desired_velocity.normalize();
  desired_velocity *= _ai_char->_max_speed;
  double distance = (present_pos - _init_pos).length();
  if(_area_of_effect > 0 && distance > _area_of_effect) {
    LVecBase3f direction = present_pos - _init_pos;
    direction.normalize();
    desired_velocity =  - direction * _ai_char->_max_speed;
    //LVecBase3f dirn = _ai_char->_steering->_steering_force;
    //dirn.normalize();
    //_ai_char->_steering->_steering_force = LVecBase3f(0.0, 0.0, 0.0);
  }
  return desired_velocity;
}
