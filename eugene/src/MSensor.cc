#include "MSensor.h"
#include <strings.h>

inline bool Prior(const UseSensor *A, const UseSensor *B)
{ return(A->Priority < B->Priority); }

/*************************************************************
 **                        UseSensor                        **
 *************************************************************/
// -------------------------
//  Default constructor.
// -------------------------
UseSensor :: UseSensor ()
{
  Priority = 0;
  Name[0] = '\000';
}

// -------------------------
//  Default constructor.
// -------------------------
UseSensor :: UseSensor (int priority, char name[FILENAME_MAX+1])
{
  Priority = priority;
  strcpy(Name,name);
}

// -------------------------
//  Default destructor.
// -------------------------
UseSensor :: ~UseSensor () {}



/*************************************************************
 **                       MasterSensor                      **
 *************************************************************/

extern Parameters PAR;

// ------------------------
// Default constructor.
// ------------------------
MasterSensor :: MasterSensor ()
{
}

// ------------------------
//  Default destructor.
// ------------------------
MasterSensor :: ~MasterSensor ()
{
  msList.clear();
  theSensors.clear();
}

// ------------------------
//  Init Master.
// ------------------------
void MasterSensor :: InitMaster ()
{
  char *key_name;
  int  val_prior;
  int i;
  
  // On r�cup�re les couples nom de sensor/priorit� du .par
  while(PAR.getUseSensor(&key_name, &val_prior))
    msList.push_back(new UseSensor(val_prior, key_name));

  // On les tri
  sort(msList.begin(), msList.end(), Prior);
  
  // Liste des plugins devant se trouver dans ./PLUGINS
  soList  = new char*[(int)msList.size()];
  // Liste des plugins � utiliser (key pour attaquer PAR)
  useList = new char*[(int)msList.size()];
  
  for(i=0; i<(int)msList.size(); i++) {
    soList[i] = new char[FILENAME_MAX+1];
    strcpy(soList[i], "./PLUGINS/");
    strcat(soList[i], msList[i]->Name);
    strcat(soList[i], ".so");
    useList[i] = new char[FILENAME_MAX+1];
    strcpy(useList[i], msList[i]->Name);
    strcat(useList[i], ".use");
  }
  
  // On indique les plugins ignor�s
  int nb0 = 0;
  while (msList[nb0]->Priority == 0) {
    fprintf(stderr,"WARNING: Ignored information : %s\n", msList[nb0]->Name);
    nb0++;
  }
  
  dllList = new (DLLFactory *)[(int)msList.size()];
  
  for(i=nb0; i<(int)msList.size(); i++) {
    if(PAR.getI(useList[i])) {
      dllList[i] = new DLLFactory ( soList[i] );
      if(dllList[i]->factory)
	theSensors.push_back( dllList[i]->factory->CreateSensor() );
      else fprintf(stderr,"WARNING: Plugin not valid or not found : %s\n",soList[i]);
    }
  }
}

// ------------------------
//  Init. the sensors.
// ------------------------
void MasterSensor :: InitSensors (DNASeq *X)
{
  for(int i=0; i<(int)theSensors.size(); i++)
    theSensors[i]->Init(X);
}

// --------------------------
//  Get informations at pos.
// --------------------------
void MasterSensor :: GetInfoAt (DNASeq *X, int pos, DATA *d)
{
  int i;
  for(i=0; i<2;  i++) d->Stop[i] = d->Start[i] = d->Acc[i] = d->Don[i] = 0.0;
  for(i=0; i<13; i++) d->ContentScore[i] = 0.0;

  d->ESTMATCH_TMP = 0; // WARNING temporaire : EST -> on est dans intron
  
  for(i=0; i<(int)theSensors.size(); i++) {
    theSensors[i]->GiveInfo(X, pos, d);
  }
}

// --------------------------------------------
//  Get special info at pos.
//  Retourne TRUE si les sensors sont porteurs
//  d'infos de type "type" FALSE sinon.
// --------------------------------------------
int MasterSensor :: GetInfoSpAt (TYPE_SENSOR type,
				 DNASeq *X, int pos, DATA *d)
{
  int i;
  int info = FALSE;  // Aucune info

  for(i=0; i<2;  i++) d->Stop[i] = d->Start[i] = d->Acc[i] = d->Don[i] = 0.0;
  for(i=0; i<13; i++) d->ContentScore[i] = 0.0;

  d->ESTMATCH_TMP = 0; // WARNING temporaire : EST -> on est dans intron

  for(i=0; i<(int)theSensors.size(); i++) {
    if(theSensors[i]->type == type || theSensors[i]->type == Type_Multiple) {
      theSensors[i]->GiveInfo(X, pos, d);
      info = TRUE;
    }
    else if(theSensors[i]->type == Type_Unknown)
      return info;  // Aucune info pour ce type
  }
  return info;
}

// --------------------------
//  Reset type.
// --------------------------
void MasterSensor :: ResetType ()
{
  for(int i=0; i<(int)theSensors.size(); i++)
    theSensors[i]->type = Type_Unknown;
}

// -------------------------
//  Destroy the sensors.
// -------------------------
void MasterSensor :: ResetSensors ()
{
  for(int i=0; i<(int)theSensors.size(); i++)
    delete theSensors[i];
}