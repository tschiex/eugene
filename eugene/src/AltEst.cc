// ------------------------------------------------------------------
// Copyright (C) 2005 INRA <eugene@ossau.toulouse.inra.fr>
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
// File:     AltEst.cc
// Contents: class Alternative Est
// ------------------------------------------------------------------

#include "AltEst.h"

/*************************************************************
 **                      OneAltEst
 *************************************************************/
// ----------------------
//  Default constructor.
// ----------------------
OneAltEst :: OneAltEst()
{
  id[0] = 0;
  start = end = index = exonIndex = exonsNumber = 0;
  totalLength = altSplicingEvidence = 0;
}

OneAltEst :: OneAltEst(char *ID, int i, int j)
{
  strcpy(id, ID);
  this->start               = i;
  this->index               = -1;
  this->exonIndex           = 0;
  this->exonsNumber         = 0;
  this->altSplicingEvidence = 0;
  this->totalLength         = 0;
  this->AddExon(i, j);
}

// ----------------------
//  Default destructor.
// ----------------------
OneAltEst :: ~OneAltEst ()
{
}

// ------------------------
//  ResetEst
// ------------------------
void OneAltEst :: Reset ()
{
  this->start               = -1;
  this->end                 = -1;
  this->index               = -1;
  this->exonIndex           = 0;
  this->exonsNumber         = 0;
  this->altSplicingEvidence = 0;
  this->totalLength         = 0;
  vi_ExonStart.clear();
  vi_ExonEnd.clear();
}

// ----------------------
//  Push an exon
// ----------------------
void OneAltEst :: AddExon(int i, int j)
{
  vi_ExonStart.push_back(i);
  vi_ExonEnd.push_back(j);
  this->end = j;
  this->exonsNumber++;
  this->totalLength += (j-i+1);
}

// ----------------------
//  Remove an exon
// ----------------------
void OneAltEst :: RemoveExon(int i)
{
  totalLength -= (vi_ExonEnd[i] - vi_ExonStart[i] + 1);
  vi_ExonStart.erase(vi_ExonStart.begin() + i);
  vi_ExonEnd.erase(vi_ExonEnd.begin() + i);
  exonsNumber--;
  if ((i==0) || (i==exonsNumber))
    UpdateBoundaries();
}

// ------------------------------------------
//  ExtremitiesTrim
//   To avoid alignment errors, clean the est 
//   by "trimming" the extremities 
//   like a pac-man or an exonuclease...
// ------------------------------------------
void OneAltEst :: ExtremitiesTrim(int exonuclen)
{
  if (exonuclen == 0) return;
  // shortening first exon
  //print();
  if (vi_ExonEnd[0] - vi_ExonStart[0] > exonuclen) {
    vi_ExonStart[0] += exonuclen;
    totalLength     -= exonuclen;
  }
  else { // tiny first exon, it has to be removed
    //fprintf(stdout,"\n1 %s %d %d\n", id, vi_ExonStart[0], vi_ExonEnd[0]);
    if (exonsNumber>1) // security test
      RemoveExon(0);
  }

  // same shortening for the last exon
  if (vi_ExonEnd[exonsNumber-1]-vi_ExonStart[exonsNumber-1] > exonuclen) {
    vi_ExonEnd[exonsNumber-1] -= exonuclen;
    totalLength               -= exonuclen;
  }
  else { // tiny last exon, it has to be removed
    //fprintf(stdout,"2 %s\n", id);
    if (exonsNumber>1) // security test
      RemoveExon(exonsNumber-1);
  }
  UpdateBoundaries();
  //Print();
}

// -------------------------------------------
//  IsFiltered
//   Return 0 if no filtered
//          1 if filtered because of unspliced
//          2 if filtered because of exon len
// -------------------------------------------
int OneAltEst :: IsFiltered(bool unspliced, bool extremelen, bool verbose,
			    int  minIn,     int  maxIn,      int  maxEx, int minEx,
			    int  minEstLen, int  maxEstLen)
{
  // remove if unspliced
  if ((unspliced) && (exonsNumber == 1)) {
    if (verbose) fprintf(stderr," %s removed (unspliced) ...", id);
    return 1;
  }
  if (extremelen) {
    // remove if est too short or too long (after trimming!)
    if (totalLength < minEstLen) {
      if (verbose) fprintf(stderr," %s removed (too short) ...", id);
      return 2;
    }
    if (totalLength > maxEstLen) {
      if (verbose) fprintf(stderr," %s removed (too long) ...", id);
      return 2;
    }
    if ((exonsNumber>1) && (vi_ExonEnd[0] - vi_ExonStart[0]-1 > maxEx)) {
      if (verbose) fprintf(stderr," %s removed (exon too long) ...", id);
      return 2;
    }

    // remove if one intron or exon is too short or too long,
    for (int j=1; j<exonsNumber;j++) {
      if ((vi_ExonStart[j] - vi_ExonEnd[j-1] -1) < minIn) {
	if (verbose) fprintf(stderr," %s removed (intron too short) ...", id);
	return 2;
      }
      if ((vi_ExonStart[j] - vi_ExonEnd[j-1] -1) > maxIn) {
	if (verbose) fprintf(stderr," %s removed (intron too long) ...", id);
	return 2;
      }
      if ((vi_ExonEnd[j] - vi_ExonStart[j] -1) > maxEx) {
	if (verbose) fprintf(stderr," %s removed (exon too long) ...", id);
	return 2;
      }
      if((vi_ExonEnd[j] - vi_ExonStart[j] -1 < minEx) && (j<exonsNumber-1)) {
	// internal exon too short
	if (verbose) fprintf(stderr," %s removed (int. exon too short) ...", id);
	return 2;
      }
    }
  }
  return 0;
}

// -----------------------------------------------------
//  Test splice sites identity of 2 Est
//  they have to be sorted: "this" starts before "other"
// -----------------------------------------------------
bool OneAltEst :: IsInconsistentWith(OneAltEst *other)
{
  int i, j = 0, k;
  //printf("%s IsInconsistentWith %s ? ... ",this->ID,other->ID);

  // if this and other are not overlaping they can't be inconsistent
  if (other->start >= this->end) return false;

  // i reaches the first exon that ends after the begin of other,
  // that is the first exon overlaping other
  for (k=0; k<this->exonsNumber; k++) {
    if (vi_ExonEnd[k] >= other->start) break;
  }
  // control if other starts in an intron of this
  if (other->start < this->vi_ExonStart[k]) return true;

  for (i=k; i<this->exonsNumber; i++) {
    // for each exon i of this (other starts at exon j=0, because this is before other)
    if ((i < (this->exonsNumber-1)) && (j < (other->exonsNumber-1))) {
      // they have each at least one remaining exon
      if ( (this->vi_ExonEnd[i]     != other->vi_ExonEnd[j]) || 
	   (this->vi_ExonStart[i+1] != other->vi_ExonStart[j+1]))
	return true; // a difference in the pairs donor - acceptor
      // same coordinates :
      j++;
      continue; // OK, increment the next exon of this (loop "for" upper)
    }
    else { // at least one est is ending
      if (i == this->exonsNumber-1) { // end of this
	if ((other->vi_ExonEnd[j] >= this->end) || (j == other->exonsNumber-1)) return false;
	// current exon of other ends after the last exon of this, so there can't be any inconsistency
	// if not, it has to be the last exon of other
	return true;
	// an intron begin in other while i is still in its last exon
      }
      else { // only other is in its last exon
	if (this->vi_ExonEnd[i] >= other->end) return false;
	// idem than upper, the last exon of other ends in an exon of this,
	// so no inconsistency is possible. 
	// If not, the last exon of other continue in an intron of this
	return true;
      }
    }
  }
  // We're not supposed to reach this line!...
  fprintf(stderr, "\n\n INTERNAL ERROR in AlternativeEst::IsInconsistentWith\n");
  return false; // just to avoid a compilation warning
}

// --------------------------------------
//  Print the OneAltEst (nuc coordinates)
// --------------------------------------
void OneAltEst :: Print()
{
  //fprintf(stdout,"%10s n�%4d %6d -> %6d %de:",
  //        id, index, start+1, end+1, exonsNumber);
  fprintf(stdout," %s:", id);
  for(int i=0; i<(int)vi_ExonStart.size(); i++) {
    fprintf(stdout,"\t%d - %d", vi_ExonStart[i]+1, vi_ExonEnd[i]+1);
  }
  //fprintf(stdout,"\t%d", altSplicingEvidence);
  fprintf(stdout,"\n");
}

/*************************************************************
 **                        AltEst
 *************************************************************/
// ----------------------
//  Default constructor.
// ----------------------
AltEst :: AltEst()
{
}

// ----------------------
//  Default destructor.
// ----------------------
AltEst :: ~AltEst ()
{
}

// ----------------------
//  Read .alt file.
// ----------------------
int AltEst :: ReadAltFile (char name[FILENAME_MAX+1], int &nbUnspliced, int &nbExtremLen)
{
  int  read, deb, fin, poids, brin, EstDeb, EstFin, filtertype, totalread;
  char *EstId, *PEstId, *tmp;
  char A[128], B[128];
  FILE *fp;
  OneAltEst oaetmp;
  bool first = 1;

  A[0]   = B[0] = 0;
  EstId  = A;
  PEstId = B;

  fp = FileOpen(NULL, name, "r");
  totalread = 0;

  while ((read=fscanf(fp,"%d %d %d %*s %d %s %d %d\n",
		      &deb, &fin, &poids, &brin, EstId, &EstDeb, &EstFin)) == 7) {
    if (strcmp(EstId, PEstId) == 0) {
      //voae_AltEst[totalAltEstNumber-1].AddExon(deb-1,fin-1);
      oaetmp.AddExon(deb-1, fin-1);
    }
    else {
      totalread++;
      if (first) first = 0;
      else {
	oaetmp.ExtremitiesTrim(exonucleasicLength);
	filtertype = oaetmp.IsFiltered(unsplicedFilter, extremeLenFilter, verbose,
				       minIn, maxIn, maxEx, minEx,
				       minEstLength, maxEstLength);
	if (filtertype == 0) {
	  voae_AltEst.push_back(oaetmp);
	  totalAltEstNumber++;
	}
	else {
	  if (filtertype == 1) nbUnspliced++;
	  if (filtertype == 2) nbExtremLen++;
	}
	oaetmp.Reset();
      }
      oaetmp = OneAltEst(EstId, deb-1, fin-1);
      tmp    = PEstId;
      PEstId = EstId;
      EstId  = tmp;
    }
  }
  // last est
  oaetmp.ExtremitiesTrim(exonucleasicLength);
  filtertype = oaetmp.IsFiltered(unsplicedFilter, extremeLenFilter, verbose,
				 minIn, maxIn, maxEx, minEx,
				 minEstLength, maxEstLength);
  if (filtertype == 0) {
    voae_AltEst.push_back(oaetmp);
    totalAltEstNumber++;
  }
  else{
    if (filtertype == 1) nbUnspliced++;
    if (filtertype == 2) nbExtremLen++;
  }

  if (read != EOF) fprintf(stderr,"Incorrect ALTEST file !\n");
  fclose(fp);
  return totalread;
}

// -----------------------------------------------------------------
// Filter the AltEst:
//  Remove Est strictly included in another one,
//  Set a list of Est clusters, 
//   (a cluster can be seen as a connex component in a graph 
//   built with est as edges and overlap as vertices ;
//   it allows to reduce the time complexity by restricting the
//   pairwise est-est comparisons to est of a same cluster.)
//  And keep only Est confering an evidence of alternative splicing,
//  (that is all est that is inconsistent with another one).
// -----------------------------------------------------------------
void AltEst :: Compare(int &nbIncomp, int &nbNoevidence, int &nbIncluded)
{
  int i, j, k = 0;
  // Until now, only one cluster is considered (time max)
  // test all pairwise est comparisons in the cluster
  // NxN comparisons could be tested, but it has been reduced to
  // ((NxN)-N)/2 , only one comparison per pair, without the diag. (cf. k)
  // WARNING : voae_AltEst[0] is the special INIT (counted in totalAltEstNumber)
  for(i=1; i<totalAltEstNumber; i++) {
    if (compatibleFilter || includedFilter) {
      // Compare this est with the others to check incompatibility or inclusion
      for (j=2+k; j<totalAltEstNumber; j++) {
	char *iID = voae_AltEst[i].GetId();
	char *jID = voae_AltEst[j].GetId();
	if (voae_AltEst[i].IsInconsistentWith(&voae_AltEst[j])) {
	  if (verbose) fprintf(stderr," incompatibility: %s vs. %s ...", jID, iID);
	  voae_AltEst[i].PutAltSplE(true);
	  voae_AltEst[j].PutAltSplE(true);
	  nbIncomp++;
	}
	else { // no inconsistency
	  // j strictly included in i
	  if ((includedFilter) && (voae_AltEst[j].GetEnd() <= voae_AltEst[i].GetEnd())) {
	    if (verbose) fprintf(stderr," %s removed (included in %s) ...", jID, iID);
	    voae_AltEst.erase(voae_AltEst.begin() + j);
	    totalAltEstNumber--;
	    j--;
	    nbIncluded++;
	  }
	}
      }
      k++;
    }
  }

  if (compatibleFilter) {
    for (i=1; i<totalAltEstNumber; i++) {
      if  (! voae_AltEst[i].GetAltSplE()) {
	if (verbose) fprintf(stderr," %s removed (no alt.spl. evidence) ...", voae_AltEst[i].GetId());
	voae_AltEst.erase(voae_AltEst.begin() + i);
	totalAltEstNumber--;
	i--;
	nbNoevidence++;
      }
    }
  }
}