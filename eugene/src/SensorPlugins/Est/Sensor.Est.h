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
// File:     Sensor.Est.h
// Contents: Sensor Est
// ------------------------------------------------------------------

#ifndef  SENSOR_EST_H_INCLUDED
#define  SENSOR_EST_H_INCLUDED

#include "../../Sensor.h"
#include "../../Hits.h"

/*************************************************************
 **                       SensorEst                         **
 *************************************************************/
class SensorEst : public Sensor
{
 private:
  std::vector<int>           vPos;
  std::vector<unsigned char> vESTMatch;
  std::vector<int>::iterator iter;
  std::vector<char>   vSplicedStartF; // vSplicedStartF[i] = true if a spliced start codon begins at pos i on forward
  std::vector<char>   vSplicedStartR; // vSplicedStartR[i] = true if a spliced start codon begins at pos i on reverse
  
  int    index;
  unsigned char *ESTMatch;
  Hits   **HitTable;
  int    NumEST;
  double estP, utrP;
  double DonorThreshold;
  double cdsBoost;
  double spliceBoost;
  double spliceNonCanP; // penalty if there is a non canonical splice site
  double spliceStartP;  // penalty if there is a spliced start
  int    estM, utrM;
  int    ppNumber;
  int    initid;
  int    stepid;
  int    N;
  int mRNAOnly;  // 0 or 1: 
                 // 1 if the hits are just mrna hits => ncRNA are penalized
                 // 0: a hit can be a mrna hit or a ncrna hit: tracks 'ncrna' are not penalized
  char  *fileExt; // File name extension

  int MinDangling;
  int MaxIntron;
  int MaxIntIntron;
  
  Hits** ESTAnalyzer(Hits *AllEST, unsigned char *ESTMatch,
		     int EstM, int *NumEST, DNASeq *X);
  void   ESTSupport (Prediction *, FILE *, int Tdebut,    int Tfin,
		     int debut, int fin, Hits **HitTable, int Size);
  void   FEASupport (Prediction *, FILE *, int Tdebut,    int Tfin,
		     int debut, int fin, Hits **HitTable, int Size, int NumG);
  int    LenSup(Hits **HitTable, unsigned char *Sup, 
		std::vector<int> vSupEstI, int &additionalsup,
                int index, int beg, int end);
  void Print (char name[FILENAME_MAX+1]);
 public:
  SensorEst               (int n, DNASeq *X);
  virtual ~SensorEst      ();
  virtual void Init       (DNASeq *);
  virtual void GiveInfo   (DNASeq *, int, DATA *);
  virtual void Plot       (DNASeq *);
  virtual void PostAnalyse(Prediction *, FILE *);
};

extern "C" SensorEst * builder0( int n, DNASeq *X) { return new SensorEst(n, X);}

#endif
