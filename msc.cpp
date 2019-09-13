#include "simaster.h"
#include "msc.h"
#include "utils.h"
#include "error.h"

MassStorage::~MassStorage()
{
int loop;

for(loop = 0;loop < MemRecs;loop++)
   {
   current[loop].RemoveAllChanges();
   }
free(current);
free(filename);
fclose(fp);
}

MassStorage::MassStorage(const char *fname)
{
MassStorage_type *mst;
int randcol, iloop;

MemRecs = 0;
rectypes = 1; //constant for now
RowsUpdated = 0;
current = NULL;
RowsLoaded = 0;
RowsDeleted = 0;
RowsAdded = 0;
filename = (char *)malloc(strlenBytes(fname));
memcpy(filename, fname, strlenBytes(fname));

if (0 == stat(fname, &fileinfo)) //It exists
   {
   if((fp = fopen(fname, "rb")) == NULL)
      printf("Cataclysmic error opening the data file %s\n", fname);
   //Is it a genuine data file?
   if(ReadHeader(fp) != MSS0000P) //some sort of error
      {
      printf("Cataclysmic error reading header in file %s\n", fname);
      printf("Generating test information\n");
      fclose(fp);
      fp = NULL;
      if((fp = fopen(fname, "wb")) == NULL)
         {
         printf("Cataclysmic error creating the data file %s.  Outputing to standard output\n", fname);
         fp = stdout;
         }
      mst = new MassStorage_type();
      mst->SetName("Default Record Type");
      randcol = getrandom(1, 5);
      for(iloop = 0;iloop < randcol;iloop++)
         {
         mst->AddColumn(getrandom(1, 50));
         }
      AddRecordType(mst);
      }
   else
      {
      if(CheckHeader() != MSS0000P)
         {
         printf("Cataclysmic error.  I will not proceed\n");
         fclose(fp);
         fp = NULL;
         }
      else //everything is good
         {
         RowsLoaded = typeindex.hi.rows;
         }
      }
   //Read in header information
   }
else //it don't
   {
   if((fp = fopen(fname, "wb")) == NULL)
      printf("Cataclysmic error creating the data file %s\n", fname);
   mst = new MassStorage_type();
   mst->SetName("Default Record Type");
   randcol = getrandom(1, 5);
   for(iloop = 0;iloop < randcol;iloop++)
      {
      mst->AddColumn(getrandom(1, 50));
      }
   AddRecordType(mst);
   }
}

int MassStorage::ReadHeader(FILE *inp)
{
HeaderInfo hi;
int loop, nlp = 0, iloop;
char nme[MAX_RECTYPE_NAME_SIZE], currch;

if(fileinfo.st_size == 0 || inp == NULL) //Just nonsense
   return MSS0001N;

//Read in the General Info
rewind(inp);
loop = sizeof(HeaderInfo);
loop = fread(&hi, sizeof(HeaderInfo), 1, inp);
if(hi.IDFlag != 666) //can't be a MassStorage file then
   return MSS0001N;

for(loop = 0;loop < hi.rectypes;loop++)
   {
   //Read in the text name
   do
      {
      currch = fgetc(inp);
      nme[nlp] = currch;
      nlp++;
      } while(nlp < MAX_RECTYPE_NAME_SIZE && currch != 0);
   if(nlp == MAX_RECTYPE_NAME_SIZE)
      {
      printf("MSS0001P error\n");
      }
   //now put the name into memory
   typeindex.name = (char *)malloc(strlenBytes(nme));
   strcpy(typeindex.name, nme);
   fread(&typeindex.cols, sizeof(int), 1, inp);
   typeindex.colsize = (int *)malloc(sizeof(int) * typeindex.cols); //read in the column sizes
   for(loop = 0;loop < typeindex.cols;loop++)
      {
      fread(&typeindex.colsize[loop], sizeof(int), 1, inp);
      }
   }

//make sure its all good
for(loop = 0;loop < rectypes;loop++)
   {
   printf("Record %s(%d columns - %d record length):\n", typeindex.name, typeindex.cols, typeindex.hi.RecordSize);
   for(iloop = 0;iloop < typeindex.cols;iloop++)
      printf("Column %d size: %d\n",iloop, typeindex.colsize[iloop]);
   }
printf("%d rows.\n", hi.rows);
memcpy(&typeindex.hi, &hi, sizeof(HeaderInfo)); //save the info for later
return MSS0000P; //good signal
}

int MassStorage::CheckHeader()
{
//Check if the file size is appropriate for the number of records
if(((fileinfo.st_size - (typeindex.hi.headersize + sizeof(HeaderInfo))) % typeindex.hi.RecordSize) != 0)
   return MSS0004N;
return MSS0000P;
}

int MassStorage::GetRecordSize()
{
return 0;
}

RecordData *MassStorage::GetRecord(int recnum)
{
RecordData *rd;
//Check if its currently in memory
if(recnum < 0)
   return NULL;
else
   {
   if((rd = GetAddress(recnum)) != NULL) //If so, return its address
      {
      temphold.Copy(rd, typeindex.hi.RecordSize);
      return &temphold;
      }
   else //otherwise, read it in, and give that address
      {
      return ReadRecord(recnum); //All record returning function use the static temphold variable
      }
   }
return NULL;
}

RecordData *MassStorage::GetAddress(int recnum)
{
ChangeInfo *ret;

if((recnum >= RowsLoaded + RowsAdded) || (recnum < 0) || (current == NULL)) //strictly beyond the range or nothing in memory
   return NULL;
if(!GetFlag(RecFlagSOR, &MSCflags))  //If its been changed since the last lookup, sort it now
    {
    qsort(current, MemRecs, sizeof(ChangeInfo), MSCcompDESC);
    SetFlagOn(RecFlagSOR, &MSCflags);
    }
ret = (ChangeInfo *)bsearch(&recnum, current, MemRecs, sizeof(ChangeInfo), MSCcomp);
if(ret == NULL)
   return NULL;
else
   return (*ret).GetLastChange();
}

ChangeInfo *MassStorage::GetIndex(int recnum)
{
ChangeInfo *ret;

if((recnum >= RowsLoaded + RowsAdded) || (recnum < 0) || (current == NULL)) //strictly beyond the range or nothing in memory
   return NULL;
if(!GetFlag(RecFlagSOR, &MSCflags))  //If its been changed since the last lookup, sort it now
    {
    qsort(current, MemRecs, sizeof(RecordData *), MSCcompDESC);
    SetFlagOn(RecFlagSOR, &MSCflags);
    }
ret = (ChangeInfo *)bsearch(&recnum, current, MemRecs, sizeof(ChangeInfo), MSCcomp);
return ret;
}


RecordData *MassStorage::ReadRecord(int recnum)
{
void *tp;
int seek;

tp = malloc(typeindex.hi.RecordSize);
seek = typeindex.hi.headersize + sizeof(HeaderInfo) + (typeindex.hi.RecordSize * recnum);
if(seek > (fileinfo.st_size - typeindex.hi.RecordSize)) //Can't let it just pick anything
   return NULL;
fseek(fp, seek, SEEK_SET); //Seek and Destroy!
fread(tp, typeindex.hi.RecordSize, 1, fp);
temphold.ChangeData(tp, typeindex.hi.RecordSize);
SetFlagOff(RecFlagDEL, &temphold.flags); //this is a verbatim record from disc
SetFlagOn(RecFlagDSC, &temphold.flags); //this record exists on disc
SetFlagOff(RecFlagUPD, &temphold.flags);
SetFlagOff(RecFlagMEM, &temphold.flags); //its not in the persistent memory system
SetFlagOff(RecFlagADD, &temphold.flags); //it wasn't added, it was loaded
free(tp);
return &temphold;
}

int MassStorage::AddRecord(void *data)
{
int hold;

temphold.ChangeData(data, typeindex.hi.RecordSize);
SetFlagOff(RecFlagDEL, &temphold.flags);//it hasn't been deleted or is it on the disc
SetFlagOff(RecFlagDSC, &temphold.flags);
SetFlagOff(RecFlagUPD, &temphold.flags); //but it has been changed(just created)
SetFlagOn(RecFlagADD, &temphold.flags);
hold = MemRecs;
if(AddToMemory_NEW(&temphold, RowsLoaded + RowsAdded) > hold) //this record exists purely in memory
   RowsAdded++;
return RowsAdded;
}

int MassStorage::AddToMemory_NEW(RecordData *rd, int rowi)
{
int chgrow;

SetFlagOn(RecFlagMEM, &rd->flags); //its now part of the persistent memory system
SetFlagOff(RecFlagSOR, &MSCflags); //The array of changed/deleted/added records is not sorted
//Add another pointer for current array and set up the first record
current = (ChangeInfo *)realloc(current, (MemRecs + 1) * sizeof(ChangeInfo));
chgrow = MemRecs;
current[chgrow].Initialize(rd, typeindex.hi.RecordSize, rowi); //Get the current row set up, and a copy of the first row in there
MemRecs++;
return MemRecs;
}

int MassStorage::AddToMemory_UPD(RecordData *rd, int rowi)
{
ChangeInfo *chgrow;

SetFlagOn(RecFlagMEM, &rd->flags); //its now part of the persistent memory system
if((chgrow = GetIndex(rowi)) == NULL) //if its not in the memory
   {
   SetFlagOff(RecFlagSOR, &MSCflags); //The array of changed/deleted/added records is not sorted
   current = (ChangeInfo *)realloc(current, (MemRecs + 1) * sizeof(ChangeInfo));
   chgrow = &current[MemRecs];
   chgrow->Initialize(rd, typeindex.hi.RecordSize, rowi); //Get the current row set up, and a copy of the first row in there
   MemRecs++;
   }
else //Just add the newest record in
   {
   chgrow->AddChange(rd);
   }
return MemRecs;
}


int MassStorage::ChangesDone(int recnum)
{
return 0;
}

int MassStorage::ChangesDone_Direct(int recnum)
{
return 0;
}

int MassStorage::OnDisk(int recnum)
{
RecordData *rd;

if(recnum < 0 || recnum > Current_RowCount())
   return NULL;

rd = GetAddress(recnum);
if(rd != NULL)
   return GetFlag(RecFlagDSC, &rd->flags);
else //if its not in memory, it'll be on the disc if its record number is less than the number of records loaded
   {
   if(recnum < RowsLoaded)
      return 1;
   else
      return 0;
   }
}

int MassStorage::UpdateRecord(int recnum, void *data)
{
RecordData *rd;

if((rd = GetRecord(recnum)) == NULL)
   return RowsUpdated; //The record don't exist
else
   rd->ChangeData(data, typeindex.hi.RecordSize); //If its exists, update it

//Switch the flags correctly
if(GetFlag(RecFlagDEL, &rd->flags))
   {
   SetFlagOff(RecFlagDEL, &rd->flags);
   RowsDeleted--; //Switch back from being deleted to updated
   RowsUpdated++;
   }
 else
   {
   if(!GetFlag(RecFlagUPD, &rd->flags))
      RowsUpdated++;
   if(GetFlag(RecFlagADD, &rd->flags)) //This row does not count in the history as being added
      SetFlagOff(RecFlagADD, &rd->flags);
   }

SetFlagOn(RecFlagUPD, &rd->flags); //set the update flag
AddToMemory_UPD(rd, recnum); //Add it into memory
return RowsUpdated;
}

int MassStorage::DeleteRecord(int recnum)
{
RecordData *rd;

rd = GetRecord(recnum);

if(rd == NULL || GetFlag(RecFlagDEL, &rd->flags)) //if its not there or already deleted
   return RowsDeleted; //The record don't exist

if(!GetFlag(RecFlagDEL, &rd->flags)) //If it hasn't been deleted already
   {
   SetFlagOn(RecFlagDEL, &rd->flags); //its been deleted
   RowsDeleted++;
   if(GetFlag(RecFlagUPD, &rd->flags)) //And if its been changed
      {
      RowsUpdated--; //it no longer counts as changed
      SetFlagOff(RecFlagUPD, &rd->flags);
      }
   if(GetFlag(RecFlagADD, &rd->flags)) //This row does not count in the history as being added
      SetFlagOff(RecFlagADD, &rd->flags);
   }
AddToMemory_UPD(rd, recnum);
return RowsDeleted;
}

void MassStorage::ClearAllUndoStacks()
{
int loop, rv;

for(loop = 0;loop < MemRecs;loop++)
   {
   rv = current[loop].ClearUndoStack();
   if(rv == RecFlagUPD)
      {
      RowsUpdated--;
      }
   }
}

void MassStorage::ClearUndoStack(int recnum)
{
ChangeInfo *ci;
int rv;

ci = GetIndex(recnum);
rv = ci->ClearUndoStack();
if(rv == RecFlagUPD)
   {
   RowsUpdated--;
   }
}


int MassStorage::AddRecordType(MassStorage_type *mst)
{
memcpy(&typeindex, mst, sizeof(MassStorage_type));
return 0;
}

int MassStorage::Save()
{
int loop, crc, lastrow = -1, rowtow = -1, lastrec = -1;
FILE *temp;
RecordData *rd;

crc = Current_RowCount();
//Write the header in
rewind(fp);
if((temp = fopen("temp.dat", "wb")) == NULL)
   {
   printf("Cataclysmic error saving\n");
   return MSS0003N;
   }

//Calculate the header size
typeindex.hi.headersize = strlenBytes(typeindex.name) + _msize(typeindex.colsize) + (sizeof(int) * 1);
typeindex.hi.rows = crc;

fwrite(&typeindex.hi, sizeof(HeaderInfo), 1, temp);
fwrite(typeindex.name, strlenBytes(typeindex.name), 1, temp);
fwrite(&typeindex.cols, sizeof(int), 1, temp);
fwrite(typeindex.colsize, _msize(typeindex.colsize), 1, temp);

for(loop = 0;loop < RowsLoaded + RowsAdded;loop++)
   {
   rd = GetRecord(loop);
   if(rd != NULL) //its been deleted
      {
      if(!GetFlag(RecFlagUPD, &rd->flags)) //and not changed
         {
         fwrite(rd->data, typeindex.hi.RecordSize, 1, temp); //this is here until I can figure a way of changing data w/o wrecking a file
         }
      else //not deleted but changed
         {
         fwrite(rd->data, typeindex.hi.RecordSize, 1, temp);
         }
      }
   }
//Close, delete and then rename the temp file
fclose(fp);
fclose(temp);
remove(filename);
rename("temp.dat", filename);
if((fp = fopen(filename, "rb")) == NULL)
   return MSS0002N;
stat(filename, &fileinfo);
ClearAllChanges();

RowsLoaded = crc; //But theres still records existing on disc

current = NULL;
return 0;
}

int MassStorage::SaveAs(const char *path)
{
free(filename);
filename = (char *)malloc(strlenBytes(path));
memcpy(filename, path, strlenBytes(path));
return Save(); //now the file name is right.  Save it
}

int MassStorage::Load(const char *path)
{
return 0;
}

int MassStorage::BytesUsed()
{
int count = 0, loop;

count += sizeof(typeindex);
if(current != NULL)
   count += _msize(current);

for(loop = 0;loop < MemRecs;loop++)
   {
   count += current[loop].BytesUsed();
   }
count += sizeof(MassStorage);
count += strlen(filename);
return count;
}

int MassStorage::Current_RowCount()
{
return RowsLoaded + RowsAdded - RowsDeleted;
}

int MassStorage::ClearAllChanges()
{
int loop;

for(loop = 0;loop < MemRecs;loop++)
   {
      current[loop].RemoveAllChanges();
   }
free(current);
RowsUpdated = 0;
RowsDeleted = 0;
RowsAdded = 0;
MemRecs = 0;
return MSS0000P;
}

ChangeInfo *MassStorage::GetRecordHistory(int recnum)
{
return GetIndex(recnum);
}

int MassStorage::UndoLastChange(int recnum)
{
ChangeInfo *locm;
RecordData *rd;
int index, rlen;

locm = GetIndex(recnum); //get the set of records that represent the changes for it
if(locm == NULL) //its not in memory
   return MSS0000P;

rd = locm->GetLastChange(); //Get the last change in that set of changes

//Now update the statistics, since the last change is being deleted we have to
if(rd->IsAdded())
   RowsAdded--;
else
   {
   if(rd->IsDeleted())
      {
      RowsDeleted--;
      }
   else
      {
      if(rd->IsUpdated())
         {
         RowsUpdated--;
         }
      }
   }


if(locm->RemoveLastChange() == TRUE) //If true, that was the last change
   {
   //Remove that record from the memory list
   index = ((int)locm - (int)current) / sizeof(ChangeInfo);
   rlen = sizeof(ChangeInfo) * (MemRecs - 1 - index);
   MemRecs--;
   if(MemRecs == 0) //All records are changed
      {
      free(current);
      current = NULL;
      return MSS0002P;
      }
   //Copy everything to the left of the index, then to the right
   memmove(&current[index], &current[index + 1], rlen);
   current = (ChangeInfo *)realloc(current, sizeof(ChangeInfo) * MemRecs);
   //Now update the record numbers in memory
   Renumerate(index);
   }
else
   {
   rd = locm->GetLastChange(); //Get the last change in that set of changes
   if(rd->IsDeleted())
      {
      RowsDeleted++;
      }
   else
      {
      if(rd->IsUpdated())
         {
         RowsUpdated++;
         }
      }
   }
return MSS0000P;
}

/*
*     MassStorage_type Section
*
*
*/

MassStorage_type::MassStorage_type()
{
name = NULL;
cols = 0;
colsize = NULL;
hi.RecordSize = 0;
hi.rows = 0;
hi.IDFlag = 666;
hi.rectypes = 1; //Pre-fixed
hi.headersize = 0;
}

void MassStorage_type::SetName(const char *nme)
{
name = (char *)malloc(strlenBytes(nme));
strcpy(name, nme);
}

MassStorage_type::~MassStorage_type()
{
delete colsize;
free(name);
}

void MassStorage_type::AddColumn(int len)
{
colsize = (int *)realloc(colsize, sizeof(int) * (cols + 1));
colsize[cols] = len;
hi.RecordSize += len;
cols++;
}

void MassStorage::Renumerate(int recnum)
{
int loop;

for(loop = recnum;loop < MemRecs;loop++)
   {
   current[loop].row--; //the record below it is no longer in memory
   }
}

/*
*
*
*        MSC helper functions
*
*
*
*/

int MSCcompDESC(const void *key, const void *element)
{
ChangeInfo *keyn;
ChangeInfo *elen;

keyn = (ChangeInfo *)key;
elen = (ChangeInfo *)element;

if(elen->row == keyn->row)
   return 0;
if(keyn->row > elen->row)
   return 1;
else
   return -1;
}

int MSCcomp(const void *key, const void *element)
{
int keyn;
ChangeInfo *elen;

keyn = *(int *)key;
elen = (ChangeInfo *)element;

if(elen->row == keyn)
   return 0;
if(keyn > elen->row)
   return 1;
else
   return -1;
}

