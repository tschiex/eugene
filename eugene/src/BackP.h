#ifndef  BACKP_H_INCLUDED
#define  BACKP_H_INCLUDED

// Une liste doublement chainee et circulaire de point de retour
// arriere pour le plus court chemin avec contrainte de duree minimum.
// Le premier element A ne doit pas etre efface. A.Next est le dernier
// insere (le moins couteux de tous), A.Prev est le plus vieux (le
// plus cher).

#include "Const.h"
#include "Prediction.h"

const unsigned char SwitchStart      = 0x1;
const unsigned char SwitchStop       = 0x2;
const unsigned char SwitchAcc        = 0x4;
const unsigned char SwitchDon        = 0x8;
const unsigned char SwitchTransStart = 0x10;
const unsigned char SwitchTransStop  = 0x20;
const unsigned char SwitchIns        = 0x40;
const unsigned char SwitchDel        = 0x80;
const unsigned char SwitchAny        = 0xFF; 

class BackPoint
  {
  private:
    bool Optimal;
    signed char State;
    unsigned char SwitchType;
    int StartPos;  
    double Cost;
    double Additional;
    BackPoint *Origin;
    
  public:
    BackPoint *Next;
    BackPoint *Prev;    

    BackPoint ();
    BackPoint  (char state, int pos, double cost);
    ~BackPoint();
    void InsertNew(char state, unsigned char Switch,int pos, double cost,BackPoint *Or,bool opt = true);
    void Print();
    void Dump();
    Prediction* BackTrace();
    inline void Update(double cost) {  Next->Additional += cost; };
    BackPoint *BestUsable(int pos, unsigned char mask, int len,REAL *cost, int len2 = 0);
    BackPoint *StrictBestUsable(int pos, int len,REAL *cost);
    void Zap();
  };


#endif
