// ------------------------------------------------------------------
// Copyright (C) 2004 INRA <eugene@ossau.toulouse.inra.fr>
//
// This program is open source; you can redistribute it and/or modify
// it under the terms of the Artistic License (see LICENSE file).
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of Artistic License along with
// this program; if not, please see http://www.opensource.org
//
// $Id$
// ------------------------------------------------------------------
// File:     Prediction.h
// Contents: class Prediction
// ------------------------------------------------------------------

#ifndef  PREDICTION_H_INCLUDED
#define  PREDICTION_H_INCLUDED

#include <cstdio>
#include <vector>
#include <algorithm>

#include "Const.h"
extern "C"{
#include "GDIF/gdIF.h"
}
#include "SensorIF.h"

/*************************************************************
 **                        Prediction                       **
 ************************************************************/
class Prediction
{
 private:
  int index;
  int nb;
  std::vector <int> vPos;
  std::vector <signed char> vState;
  
 public:
  Prediction  ();
  ~Prediction ();

  double OptimalPath;

  void  add           (int, signed char);
  void  print         ();
  void  setPos        (int, int);
  char  getNextState  (int); 
  char  getStateForPos(int);
  char  getState      (int i) { return vState[i]; }
  int   getPos        (int i) { return vPos[i];   }
  int   size          ()      { return nb;        } 
  void  plotPred      ();
  void  resetPred     ();
  int   nbExon        (int);
  void  reversePred   ();
  char* isStart       (int);
  char* isStop        (int);
  char* isDon         (int);
  char* isAcc         (int);
  bool IsState (DATA::SigType sig_type, int pos, char strand);
};

enum Tracks {
  InitF1 = 0, InitF2 = 1, InitF3 = 2,
  InitR1 = 3, InitR2 = 4, InitR3 = 5,
  SnglF1 = 6, SnglF2 = 7, SnglF3 = 8,
  SnglR1 = 9, SnglR2 = 10, SnglR3 = 11,
  IntrF1 = 12, IntrF2 = 13, IntrF3 = 14,
  IntrR1 = 15, IntrR2 = 16, IntrR3 = 17,
  TermF1 = 18, TermF2 = 19, TermF3 = 20,
  TermR1 = 21, TermR2 = 22, TermR3 = 23,
  IntronF1 = 24,IntronF2 = 25, IntronF3 = 26,
  IntronR1 = 27, IntronR2 = 28, IntronR3 = 29,
  IntronF2T = 30, IntronF3TG = 31, IntronF3TA = 32,
  IntronR3G = 33, IntronR3A = 34, IntronR2AG = 35,
  InterGen = 36, 
  UTR5F = 37, UTR3F = 38, 
  UTR5R = 39,UTR3R = 40, 
  IntronU5F = 41, IntronU5R = 42, 
  IntronU3F = 43, IntronU3R = 44, 
  NbTracks = 45};

const short int State2Phase[NbTracks] = {1,2,3,-1,-2,-3,
					 1,2,3,-1,-2,-3,
					 1,2,3,-1,-2,-3,
					 1,2,3,-1,-2,-3,
					 4,4,4,
					 -4,-4,-4,
					 4,4,4,
					 -4,-4,-4,
					 0,0,0,0,0,
					 4,-4,4,-4};

const short int State2Frame[NbTracks] = {1,2,3,-1,-2,-3,
					 1,2,3,-1,-2,-3,
					 1,2,3,-1,-2,-3,
					 1,2,3,-1,-2,-3,
					 4,5,6,
					 -4,-5,-6,
					 5,6,6,
					 -6,-6,-5,
					 0,0,0,0,0,
					 4,-4,4,-4};

inline int PhaseAdapt(char p) {return State2Frame[(int)p];}

#endif
