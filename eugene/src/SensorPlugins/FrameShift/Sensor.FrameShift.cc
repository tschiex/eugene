#include "Sensor.FrameShift.h"
#define NORM(x,n) (((n)+(Max(-(n),x)))/(n))

/*************************************************************
 **                      SensorFrameShift                   **
 *************************************************************/

extern Parameters PAR;

// ----------------------
//  Default constructor.
// ----------------------
SensorFrameShift :: SensorFrameShift (int n) : Sensor(n)
{
}

// ----------------------
//  Default destructor.
// ----------------------
SensorFrameShift :: ~SensorFrameShift ()
{
}

// ----------------------
//  Init start.
// ----------------------
void SensorFrameShift :: Init (DNASeq *X)
{
  type = Type_Start;
  insProb = exp(-PAR.getD("FrameShift.Ins"));
  delProb = exp(-PAR.getD("FrameShift.Del"));
  if (PAR.getI("Output.graph")) Plot(X);
}

// -----------------------
//  GiveInfo frameshift
// -----------------------
void SensorFrameShift :: GiveInfo (DNASeq *X, int pos, DATA *d)
{
  
    d->Ins = insProb;
    d->Del = delProb;
}

// ----------------------------
//  Plot Sensor information
// ----------------------------
void SensorFrameShift :: Plot(DNASeq *X)
{
}

// ------------------
//  Post analyse
// ------------------
void SensorFrameShift :: PostAnalyse(Prediction *pred)
{
}