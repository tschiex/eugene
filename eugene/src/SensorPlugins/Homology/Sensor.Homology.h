#ifndef SENSOR_HOMOLOGY_H_INCLUDED
#define SENSOR_HOMOLOGY_H_INCLUDED

#include "../../EuGene/Sensor.h"
#include "../0_SensorTk/markov.h"

/*************************************************************
 **                     SensorHomology                        **
 *************************************************************/
class SensorHomology : public Sensor
{
 private:
  int   **TblastxNumber;
  REAL  **TblastxScore;
  double TblastxP;
  double TblastxB;

 public:
  SensorHomology (int);
  virtual ~SensorHomology ();
  virtual void Init       (DNASeq *);
  virtual void GiveInfo   (DNASeq *, int, DATA *);
  REAL tblastxupdate (int hitnb, REAL hitscore, double pen, double base);
  int  PhaseAdapt (char);
  char ph06       (char);
  virtual void Plot  (DNASeq *);
  virtual void PlotAt(int pos);
  virtual void PostAnalyse(Prediction *);

};

extern "C" SensorHomology * builder0( int n ) {  return new SensorHomology(n);}

#endif

