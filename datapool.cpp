#include "datapool.h"
#include "gene.h"
#include "person.h"
#include "family.h"
#include "nation.h"
#include "world.h"
#include "intnode.h"
#include "error.h"

template DataPool<Lowest_Level, LStats>;
template DataPool<Gene, GStats>;
template DataPool<Person, PStats>;
template DataPool<Family, FStats>;
template DataPool<Nation, NStats>;
template DataPool<World, WStats>;

template <class D, class S> DataPool<D,S>::DataPool(const char *bsname)
{
IntNode BlankTree(0, NULL);
int loop;

BlankTree.LineTree(NUMCOMPONENTS); //make a big line of them

RowsLoaded = 0;
RowsDeleted = 0;
RowsUpdated = 0;
RowsAdded = 0;
files = 0;

if(bsname == NULL)
   return;
bname = (char *)malloc(strlenBytes(bsname) + 8);
memcpy(bname, bsname, strlenBytes(bsname));
strcat(bname, "0.dat");
//open the first file
ms = (MassStorage **)malloc(sizeof(MassStorage *));
ms[0] = new MassStorage(bname);
files = 1;

for(loop = 0;loop < files;loop++)
   RowsLoaded += ms[loop]->RowsLoaded;

Output = new D();
}

template <class D, class S> DataPool<D,S>::~DataPool()
{
int loop;

free(bname);
for(loop = 0;loop < files;loop++)
   delete ms[loop];
free(ms);
}

template <class D, class S> D *DataPool<D,S>::GetData(int ID)
{
return Convert(File(ID)->GetRecord(ID));
}

template <class D, class S> D *DataPool<D,S>::Convert(RecordData *inp)
{
if(inp == NULL || Output == NULL) //output can be null when the datapool is being created
   return NULL;
memcpy(&InfoPack, inp->data, sizeof(S));
Output->UnPackageNodeInfo_General(&InfoPack); //Get the generic stuff
Output->UnPackageNodeInfo(&InfoPack); //Then the local stuff(b/c it'll probably be based on the generic stuff)
return Output;
}

template <class D, class S> int DataPool<D,S>::Deleted(int ID)
{
RecordData *rd;
rd = File(ID)->GetRecord(ID);

if(rd != NULL) //its there
   return GetFlag(RecFlagDEL, &rd->flags); //and its been deleted
else
   return -1; //musta been an invalid record
}

template <class D, class S> int DataPool<D,S>::Updated(int ID)
{
RecordData *rd;
rd = File(ID)->GetRecord(ID);

if(rd != NULL) //its there
   return  GetFlag(RecFlagUPD, &rd->flags); //and its been changed
else
   return -1; //musta been an invalid record
}

template <class D, class S> int DataPool<D,S>::Added(int ID)
{
RecordData *rd;
rd = File(ID)->GetRecord(ID);

if(rd != NULL) //its there
   return !GetFlag(RecFlagDSC, &rd->flags); //if its on the disc, its not added
else
   return -1; //musta been an invalid record
}


template <class D, class S> int DataPool<D,S>::Save()
{
int loop, retval;

for(loop = 0;loop < files;loop++)
   {
   if((retval = ms[loop]->Save()) != MSS0000P)
      return retval;
   }
return retval;
}

template <class D, class S> void DataPool<D,S>::CloseFiles()
{
int loop;

free(bname);
for(loop = 0;loop < files;loop++)
   delete ms[loop];
free(ms);
}

template <class D, class S> int DataPool<D,S>::OpenPool(const char *bsname)
{
int loop;
RowsLoaded = 0;
RowsDeleted = 0;
RowsUpdated = 0;
RowsAdded = 0;
files = 0;

if(bsname == NULL) //File open error
   return SML0004N;
bname = (char *)malloc(strlenBytes(bsname) + 8);
memcpy(bname, bsname, strlenBytes(bsname));
strcat(bname, "0.dat");
//open the first file
ms = (MassStorage **)malloc(sizeof(MassStorage *));
ms[0] = new MassStorage(bname);
files = 1;

for(loop = 0;loop < files;loop++)
   RowsLoaded += ms[loop]->RowsLoaded;
return SML0001P;
}

template <class D, class S> void DataPool<D,S>::CreateTemplate(int recs)
{
int loop;
D tmp;

for(loop = 0;loop < recs;loop++)
   AddData(&tmp);
}

template <class D, class S> int DataPool<D,S>::DeleteAllData()
{
int loop, retval, iloop = 0, lastrow;
MassStorage *fle;
int crc;

for(loop = 0;loop < files;loop++)
   {
   fle = File(loop);
   crc = fle->RowsLoaded + fle->RowsAdded; //It doesn't hurt to delete already deleted records
   lastrow = fle->RowsDeleted; //If something with the delete goes wrong, the rows deleted will remain the same
   for(iloop = 0;iloop < crc;iloop++)
      {
      if((retval = fle->DeleteRecord(iloop)) == lastrow)
         return retval;
      else
         lastrow = retval;
      }
   }
return MSS0000P; //Unless rowsdeleted == MSS0000P there should be no problem
}

template <class D, class S> MassStorage *DataPool<D,S>::File(int rec)
{
return ms[0];
}

/*
template <class D, class S> D *DataPool<D,S>::UpdateData(int ID, D *data)
{
S dta;
int hold;
MassStorage *fle;

fle = File(ID)->RowsUpdated;
hold = fle->RowsUpdated;

data->PackageNodeInfo_General(&dta);
if(fle->UpdateRecord(ID, data->PackageNodeInfo(&dta)) == hold)
   return NULL;
else
   return Convert(ms[0]->UpdateRecord(ID, data->PackageNodeInfo(&dta)));
}
*/

template <class D, class S> unsigned int DataPool<D,S>::BytesUsed()
{
int loop;
unsigned int retval = 0;

for(loop = 0;loop < files;loop++)
   {
   retval += ms[0]->BytesUsed();
   }
return retval;
}