#include "Sensor.Repeat.h"

/*************************************************************
 **                        SensorRepeat                     **
 *************************************************************/

extern Parameters PAR;

// ----------------------
// Default constructor.
// ----------------------
SensorRepeat :: SensorRepeat (int n) : Sensor(n)
{
}

// ----------------------
//  Default destructor.
// ----------------------
SensorRepeat :: ~SensorRepeat ()
{
  vDeb.clear();
  vFin.clear();
}

// ----------------------
//  Init Repeat.
// ----------------------
void SensorRepeat :: Init (DNASeq *X)
{
  char tempname[FILENAME_MAX+1];
  FILE* ncfile;
  int deb, fin;

  type = Type_Content;

  index = 0;

  vDeb.clear();
  vFin.clear();

  UTRPenalty = PAR.getD("Repeat.UTRPenalty");
  intronPenalty = PAR.getD("Repeat.IntronPenalty");
  exonPenalty = PAR.getD("Repeat.ExonPenalty");
  
  fprintf(stderr,"Reading Intergenic regions...");
  fflush(stderr);
  
  strcpy(tempname,PAR.getC("fstname"));
  strcat(tempname,".ig");
  ncfile = FileOpen(NULL,tempname, "r");
  
  while (fscanf(ncfile,"%d %d\n", &deb, &fin) != EOF)  {
    deb = Max(1,deb)-1;
    fin = Min(X->SeqLen,fin)-1;
      vDeb.push_back( deb );
      vFin.push_back( fin );
  }

  fprintf(stderr,"done\n");
  if (PAR.getI("Output.graph")) Plot(X);
}

// -----------------------
//  ResetIter.
// -----------------------
void SensorRepeat :: ResetIter ()
{
  index = 0;
}

// --------------------------
//  GiveInfo Content Repeat.
// --------------------------
void SensorRepeat :: GiveInfo (DNASeq *X, int pos, DATA *d)
{
  // si le bloc courant est depasse, il faut avancer !
  if (index < (int)vDeb.size()  && vFin[index] < pos) index++;

  // est on dedans ?
  if (index < (int)vDeb.size()  &&  vDeb[index] <= pos && vFin[index] >= pos) {
    // penaliser !
    for(int i=0; i<6; i++)   // Exon(6)
      d->ContentScore[i] += exonPenalty;
    for(int i=7; i<8; i++)   // Intron (2)
      d->ContentScore[i] += intronPenalty; 
    for(int i=9; i<13; i++)   // UTR (4)
      d->ContentScore[i] += UTRPenalty; 
  }
 }

// ----------------------------
//  GiveInfoAt Content Repeat.
// ----------------------------
void SensorRepeat :: GiveInfoAt (DNASeq *X, int pos, DATA *d)
{
  iter = lower_bound(vFin.begin(), vFin.end(), pos);
  if (vDeb[iter-vFin.begin()] <= pos) {
    for(int i=0; i<6; i++)   // Exon(6)
      d->ContentScore[i] += exonPenalty;
    for(int i=7; i<8; i++)   // Intron (2)
      d->ContentScore[i] += intronPenalty; 
    for(int i=9; i<13; i++)  // UTR (4)
      d->ContentScore[i] += UTRPenalty; 
  }
}
// ----------------------------
//  Plot Sensor information
// ----------------------------
void SensorRepeat :: Plot(DNASeq *TheSeq)
{
  int i,j;
  
  for (i =0; i < (int)vDeb.size(); i++)
    for (j = vDeb[i]; j<= vFin[i]; j++)
      PlotBarI(j,0,0.25,2,6);
}
