////////////////////////////////////////////////////////////////////////
// Filename    : pursue.h
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

#ifndef _PURSUE_H
#define _PURSUE_H

#include "aiGlobals.h"
#include "aiCharacter.h"

class AICharacter;

class EXPCL_PANDAAI Pursue : public SteeringObjective {

public:
  NodePath _pursue_target;
  LVecBase3f _pursue_direction;
  bool _pursue_done;

  Pursue(AICharacter *ai_ch, NodePath target_object, float max_weight);
  ~Pursue();
  LVecBase3f do_pursue();
};

#endif
