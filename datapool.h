#include "msc.h"
#include "simaster.h"

#ifndef DATAPOOL
#define DATAPOOL
template <class D, class S>
class DataPool
{
public:
DataPool(const char *bsname);
~DataPool();
D *GetData(int ID);
int UpdateData(int ID, D *data) {S dta; data->PackageNodeInfo_General(&dta); return File(ID)->UpdateRecord(ID, data->PackageNodeInfo(&dta)); };
int DeleteData(int ID) { return ms[0]->DeleteRecord(ID); };
int DeleteAllData();
int AddData(D *data) {S dta; data->PackageNodeInfo_General(&dta); return File(RowsLoaded + RowsAdded)->AddRecord(data->PackageNodeInfo(&dta)); };
int RowsLoaded;
int RowsUpdated;
int RowsDeleted;
int RowsAdded;
int Deleted(int ID); //whether or not its been deleted/updated
int Updated(int ID);
int Added(int ID);
int InMemory(int ID) { return (int)ms[0]->GetAddress(ID); };
int Save();
void CloseFiles();
void ClearAllChanges() {ms[0]->ClearAllChanges();}
int OpenPool(const char *bsname);
void CreateTemplate(int recs); //creates recs blank records
MassStorage *File(int rec);
unsigned int BytesUsed();

private:
D *Convert(RecordData *data);
MassStorage **ms; //the files that contain the data
char *bname; //the basename for all the file
int filesize; //the file size each chunk should be
int files; //the number of files on this objects control
D *Output;
S InfoPack;
};
#endif
