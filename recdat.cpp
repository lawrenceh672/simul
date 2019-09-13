#include "recdat.h"

RecordData::RecordData(void *dta, int size)
{
if(dta != NULL)
   {
   data = malloc(size);
   memcpy(data, dta, size);
   }
else
   data = NULL;
flags = 0;
}

RecordData::~RecordData()
{
free(data);
}

void RecordData::ChangeData(void *dta, int size)
{

free(data);
data = malloc(size);
memcpy(data, dta, size);
}

void RecordData::Copy(RecordData *inp, int size)
{
flags = inp->flags;
data = malloc(size);
memcpy(data, inp->data, size);
}

unsigned int RecordData::BytesUsed()
{
return _msize(data);
}
