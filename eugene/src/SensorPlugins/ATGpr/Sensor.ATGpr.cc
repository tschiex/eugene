/*****************************************************************************/
/*             Copyright (c) 2002 by INRA. All rights reserved.              */
/*                 Redistribution is not permitted without                   */
/*                 the express written permission of INRA.                   */
/*                     Mail : tschiex@toulouse.inra.fr                       */
/*---------------------------------------------------------------------------*/
/* File         : EuGeneTk/SensorPlugins/ATGpr/Sensor.ATGpr.cc               */
/* Description  : Sensor ATGpr                                               */
/* Authors      : P.Bardou, S.Foissac, M.J.Cros, A.Moisan, T.Schiex          */
/* History      : May 2003                                                   */
/*****************************************************************************/

#include "Sensor.ATGpr.h"
#define NORM(x,n) (((n)+(Max(-(n),x)))/(n))

/*************************************************************
 **                        SensorATGpr                      **
 *************************************************************/

extern Parameters PAR;

// ----------------------
// Default constructor.
// ----------------------
SensorATGpr :: SensorATGpr (int n, DNASeq *X) : Sensor(n)
{
  char tempname[FILENAME_MAX+1];

  type = Type_Start;
  
  vPosF.clear();
  vValF.clear();
  vPosR.clear();
  vValR.clear();
  
  fprintf(stderr, "Reading start file (ATGpr)....................");
  fflush(stderr);
  strcpy(tempname,PAR.getC("fstname"));
  strcat(tempname,".atgpr");
  ReadATGprF(tempname, X->SeqLen);
  fprintf(stderr,"forward,");
  fflush(stderr);
  
  strcpy(tempname,PAR.getC("fstname"));
  strcat(tempname,".atgprR");
  ReadATGprR(tempname, X->SeqLen);
  fprintf(stderr," reverse done\n");
  
  CheckStart(X,vPosF, vPosR);

  // vectors for reverse are put in the increasing order
  reverse(vPosR.begin(), vPosR.end()); 
  reverse(vValR.begin(), vValR.end()); 
}

// ----------------------
//  Default destructor.
// ----------------------
SensorATGpr :: ~SensorATGpr ()
{
  vPosF.clear();
  vValF.clear();
  vPosR.clear();
  vValR.clear();
}

// ----------------------
//  Init start.
// ----------------------
void SensorATGpr :: Init (DNASeq *X)
{
  startP = PAR.getD("ATGpr.startP*",GetNumber());
  startB = PAR.getD("ATGpr.startB*",GetNumber());
  
  indexF = indexR = 0;
  PositionGiveInfo = -1;

  if (PAR.getI("Output.graph")) Plot(X);
}

// --------------------------
//  Read start forward file.
// --------------------------
void SensorATGpr :: ReadATGprF (char name[FILENAME_MAX+1], int Len)
{
  FILE *fp;
  int i,j = -1;
  double force;
  
  if (!(fp = fopen(name, "r"))) {
    fprintf(stderr, "cannot open ATGpr file %s\n", name);
    exit(2);
  }
  
  while (1) {
    i = fscanf(fp,"%*s %lf %*s %*s %d %*s %*s %*s\n", &force, &j); 
    if (i == EOF) break;
    if (i < 2) {
      fprintf(stderr, "Error in ATGpr file %s, position %d\n", name, j);
      exit(2);
    }
    vPosF.push_back( j-1 );
    vValF.push_back( force );
  }
  if (j == -1) fprintf(stderr,"WARNING: empty ATGpr file !\n");
  fclose(fp);
}

// --------------------------
//  Read start reverse file.
// --------------------------
void SensorATGpr :: ReadATGprR (char name[FILENAME_MAX+1], int Len)
{
  FILE *fp;
  int i,j = -1;
  double force;
  
  if (!(fp = fopen(name, "r"))) {
    fprintf(stderr, "cannot open start file %s\n", name);
    exit(2);
  }

  while (1) {
    i = fscanf(fp,"%*s %lf %*s %*s %d %*s %*s %*s\n", &force, &j);
    if (i == EOF) break;
    if (i < 2) {
      fprintf(stderr, "Error in ATGpr file %s, position %d\n", name, j);
      exit(2);
    }

    //if (force > 0.1) {
    j = Len-j+2;
    vPosR.push_back( j-1 );
    vValR.push_back( force );
    //}
  }
  if (j == -1) fprintf(stderr,"WARNING: empty ATGpr file !\n");
  fclose(fp);
}

// ------------------------
//  GiveInfo signal start.
// ------------------------
void SensorATGpr :: GiveInfo (DNASeq *X, int pos, DATA *d)
{
  bool update = false;
  double f;
  
  // update indexes on vectors
  if ( (PositionGiveInfo == -1) || (pos != PositionGiveInfo+1) ) update = true;
  PositionGiveInfo = pos;
  
  // Start Forward
  if(!vPosF.empty()) {
    if (update) 
      indexF = lower_bound(vPosF.begin(), vPosF.end(), pos)-vPosF.begin();
    
    if((indexF<(int)vPosF.size()) && (vPosF[indexF] == pos)) {
      // correct read score  to be in ]0 inf[
      vValF[indexF] = fabs(vValF[indexF]); 
      if (vValF[indexF]==0) vValF[indexF]=0.001;

      f = pow(vValF[indexF], startB) * exp(-startP);
      d->sig[DATA::Start].weight[Signal::Forward] += log(f);
      d->sig[DATA::Start].weight[Signal::ForwardNo] += log(1.0-f);
      indexF++;
    }
  }
  
  // Start Reverse
  if (!vPosR.empty()) {
    if (update) 
      indexR = lower_bound(vPosR.begin(),vPosR.end(),pos)-vPosR.begin();
    
    if((indexR<(int)vPosR.size()) && (vPosR[indexR] == pos)) {
      // correct read score  to be in ]0 inf[
      if (vValR[indexR]<0) vValR[indexR] = -vValR[indexR];
      if (vValR[indexR]==0) vValR[indexR]=0.001;

      f = pow(vValR[indexR], startB) * exp(-startP);
      d->sig[DATA::Start].weight[Signal::Reverse]   += log(f);
      d->sig[DATA::Start].weight[Signal::ReverseNo] += log(1.0-f);
      indexR++;
    }
  }
}

// ----------------------------
//  Plot Sensor information
// ----------------------------
void SensorATGpr :: Plot(DNASeq *X)
{
  for (int i =0; i < (int)vPosF.size(); i++)
    PlotBarF(vPosF[i],(vPosF[i]%3)+1,0.5,NORM(log(vValF[i]),4.0),2);

  for (int i =0; i < (int)vPosR.size(); i++)
    PlotBarF(vPosR[i],-((X->SeqLen-vPosR[i])%3)-1,0.5,NORM(log(vValR[i]),4.0),2);
}

// ------------------
//  Post analyse
// ------------------
void SensorATGpr :: PostAnalyse(Prediction *pred)
{
}
