// ---------------------------------------------------------------
//   T. Schiex
//
//     File:  gdIF.h
//     Version:  1.0
//
//    Copyright (c) 2000 by Thomas Schiex All rights reserved.
//    Redistribution is not permitted without the express written
//    permission of the author.
//
//  Function for interfacing with libGD
// ---------------------------------------------------------------
#ifndef  GDIF_H_INCLUDED
#define  GDIF_H_INCLUDED

void InitPNG   (int x, int y, int offset, int From, int To,
		int olap, int ILen,char *name);
void PlotBarF  (unsigned int nuc, signed char phase, double pos,
		double width, int col);
void PlotBarI  (unsigned int nuc, signed char phase, double pos,
		int width, int col);
void PlotLine  (unsigned int nuc1, unsigned int nuc2, signed char phase1,
		signed char phase2, double pos1, double pos2, int col);
void PlotString(unsigned int nuc, signed char phase, double pos,
		char st[], int col);
void ClosePNG();
void OutputHTMLFileNames(int firstImg);

#endif
