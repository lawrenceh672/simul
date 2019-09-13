#include "mscmast.h"
#include "recdat.h"
#include "chginf.h"

extern int MSCcomp(const void *key, const void *element);
extern int MSCcompDESC(const void *key, const void *element);

#ifndef MASSSTORAGE
#define MASSSTORAGE
typedef struct
{
int row;
}RecordInfo;

typedef struct HeaderInfo
{
int IDFlag; //A little four byte sequence to skim off incorrectly inputed files
int rectypes; //how many types of records.  Fixed to one for now
int headersize; //how many bytes is the header
int rows; //number of rows on disk
int RecordSize; //The size in bytes of a record
}HeaderInfo;

class MassStorage_type
{
public:
MassStorage_type();
~MassStorage_type();

void AddColumn(int len);
void SetName(const char *nme);

char *name; //name of the record type
int cols; //fields in the record
int *colsize; //the size in bytes of each column
HeaderInfo hi;
};

class MassStorage //A generic class that pulls records out of very large file(s) for easy retrieval
{
public:
MassStorage(const char *fname);
~MassStorage();

int GetRecordSize();
RecordData *GetRecord(int recnum);
int UpdateRecord(int recnum, void *data);
int DeleteRecord(int recnum);
int AddRecord(void *data);
int AddRecordType(MassStorage_type *mst); //Adds a record type to be stored
int Save(); //puts into permanent storage
int SaveAs(const char *path); //puts it somewhere else
int Load(const char *path); //prepares a file for use
int BytesUsed(); //So that the unlimited undo doesn't really become unlimited :-)
int Current_RowCount();
RecordData *GetAddress(int recnum);
RecordData *ReadRecord(int recnum);
int AddToMemory_NEW(RecordData *rd, int rowi); //Adds a new record to memory
int AddToMemory_UPD(RecordData *rd, int rowi); //Adds another change to record in memory
int OnDisk(int recnum);
void PrintStats() {printf("Records on disk: %d\nRecords in Memory %d\nRecords Added %d\nRecords Deleted %d\nRecords Updated %d\nSaving to %s\n", RowsLoaded, MemRecs, RowsAdded, RowsDeleted, RowsUpdated, filename);}
int ClearAllChanges();
int UndoLastChange(int recnum);
int UndoAllChanges(int recnum);
int ClearUndoSystem();
ChangeInfo *GetRecordHistory(int recnum);
void ClearAllUndoStacks();
void ClearUndoStack(int recnum);

int totalrows; //how many rows for all record types
int rectypes;
MassStorage_type typeindex; //contains info on the records stored
int RowsUpdated; //Updated rows
int RowsLoaded; //number of records loaded
int RowsAdded; //rows added since it was loaded
int RowsDeleted; //rows deleted since it was loaded
struct stat fileinfo;

private:
int ReadHeader(FILE *fp); //Reads a MassStorage file header
int CheckHeader(); //Checks the header information for flaws
ChangeInfo *GetIndex(int recnum); //returns the index in the current array for row recnum
int ChangesDone(int recnum); //returns the number of changes done for row recnum
int ChangesDone_Direct(int recnum); //direct cuz it doesn't have to look up the index
void Renumerate(int recnum);

ChangeInfo *current; //This manages the stored records

FILE *fp; //the actual file
int MemRecs; //Records in memory

RecordData temphold; //Temp space for one extra record
char *filename;
unsigned char MSCflags; //Global flags for all records
};
#endif
