#include "mscmast.h"

#ifndef CHANGEINFO
#define CHANGEINFO
class ChangeInfo
{
public:
ChangeInfo();
~ChangeInfo();
void AddChange(RecordData *rd);
int RemoveLastChange();
void RemoveAllChanges();
unsigned int BytesUsed();
RecordData *GetLastChange();
RecordData *GetChange(int recnum);
void Initialize(RecordData *rd, int size, int rowi);
int ClearUndoStack();

int changes;
int row;
private:
RecordData *data;
int size;
};
#endif
