// ---------------------------------------------------------------
//   T. Schiex
//
//     File:  Hits.h
//     Version:  1.0
//
//    Copyright (c) 2000 by Thomas Schiex All rights reserved.
//    Redistribution is not permitted without the express written
//    permission of the authors.
//
//  Definitions for a class representing alignements with genomic seq.
// ---------------------------------------------------------------


#ifndef  HITS_H_INCLUDED
#define  HITS_H_INCLUDED
#include <stdio.h>


class Block
{
 private:

 public:

  Block ();
  Block(int Start, int End,int LStart,int LEnd);
  ~Block ();
  
  void AddBlockAfter(int Start,int End,int LStart,int LEnd);
  
  int Start;
  int End;

  int LStart;
  int LEnd;
  Block *Prev,*Next;
  
};

class  Hits
{
 private:

 public:
  Hits ();
  Hits  (char* name, int length, char strand,int deb,int fin,int ldeb,int lfin);

  ~Hits ();

  char *Name;
  char Strand;
  int Length;
  int NGaps;
    
  Block *Match;
  Hits *Next;
};
#endif