#include "mscmast.h"

#ifndef RECORDDATA
#define RECORDDATA
class RecordData
{
public:
RecordData() {data = NULL;};
RecordData(void *data, int size);
~RecordData();
void ChangeData(void *dta, int size);
void Copy(RecordData *inp, int size); //Copies rd into temphold, duplicates all data
unsigned int BytesUsed();
int IsDeleted() {return GetFlag(RecFlagDEL, &flags);};
int IsUpdated() {return GetFlag(RecFlagUPD, &flags);};
int IsAdded() {return GetFlag(RecFlagADD, &flags);};
char *AsString() {return (char *)data;};
void Deallocate() {free(data);};

unsigned char flags; //Hold the characteristics of this here record
void *data;
/*
Flag Zero: 0/1 Data doesn't exist/does exist in the physical file
Flag One:  If its on disc, has it been changed? 0/1 = n/y
Flag Two:  Deleted/not deleted, 1/0
Flag Three: Is it in memory now?/Not? 1/0
Flag Four:  Has it been added?
Flag Five:  Is the new record memory block sorted
*/
};
#endif