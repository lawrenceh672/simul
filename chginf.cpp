#include "chginf.h"
#include "recdat.h"

ChangeInfo::ChangeInfo()
{
data = NULL;
row = -1;
changes = 0;
}

ChangeInfo::~ChangeInfo()
{
int loop;

for(loop = 0;loop < changes;loop++)
   delete &data[loop];

free(data);
}

void ChangeInfo::Initialize(RecordData *rd, int sizei, int rowi)
{
data = NULL;
//Set up the info
row = rowi;
size = sizei;
changes = 0;
//Then put the record in
AddChange(rd);
}

void ChangeInfo::AddChange(RecordData *rd)
{
data = (RecordData *)realloc(data, sizeof(RecordData) * (changes + 1));
data[changes].Copy(rd, size);
changes++;
}

int ChangeInfo::RemoveLastChange()
{
//Delete the last record in the array
data = (RecordData *)realloc(data, sizeof(RecordData) * (changes - 1));
changes--;

if(changes <= 0) //Can't delete a record if it was added
   return 1;
else
   return 0;
}

void ChangeInfo::RemoveAllChanges()
{
int loop;

for(loop = 0;loop < changes;loop++)
   {
   free(data[loop].data);
   }
free(data);
changes = 0;
}

unsigned int ChangeInfo::BytesUsed()
{
int loop, retval = 0;

for(loop = 0;loop < changes;loop++)
   {
   retval += data[loop].BytesUsed();
   }
retval += _msize(data);
return retval;
}

RecordData *ChangeInfo::GetLastChange()
{
if(changes == 0)
   return NULL;
return &data[changes - 1];
}

RecordData *ChangeInfo::GetChange(int recnum)
{
if(recnum > changes)
   return NULL;
else
   return &data[recnum];
}

int ChangeInfo::ClearUndoStack()
{
int loop, retval;

if(changes > 1)
   {
   for(loop = 0;loop < changes - 1;loop++) //kill all records except the last
      {
      data[loop].Deallocate();
      }
   data[0] = data[loop];
   data = (RecordData *)realloc(data, sizeof(RecordData)); //After the last recorddata pointer is moved to the front, drop the rest
   changes = 1; //the only change is the last

   //Now send the signal to update the statistics
   SetFlagOn(RecFlagADD, &data[0].flags); //this is like the first record added now
   if(data[0].IsDeleted())
      {
      retval = RecFlagDEL;
      }
   else
      {
      if(data[0].IsUpdated())
         {
         retval = RecFlagUPD;
         SetFlagOff(RecFlagUPD, &data[0].flags);
         }
      }
   }
return retval;
}