#include "family.h"
#include "error.h"
#include "intnode.h"
#include "smlutils.h"
#include "person.h"

Family::Family(IntNode *root)
{
name = NULL;
if(FAMILYNAME)
   AutoName();
}

void Family::AutoName()
{
char namebuffer[33]; //Just tacks on the Family Id number to the word Family
name = (char *)malloc(33*sizeof(char));
name[0] = 0;
name[1] = 0;

strcat(name, "Family");
strcat(name, _itoa(seqno, namebuffer, 10));
}

unsigned int Family::RetMem()
{
return members;
}

Family::~Family()
{
free ((void *)name);
}

void Family::StatMember(StatGuide *sg)
{
if(sg->output == 'h')
   {
   fprintf(sg->dest, "Family %d(%d)\n", seqno, serial);
   fprintf(sg->dest, "Members: %d\n\n", members);
   }
}

unsigned int Family::BytesUsedLocal()
{
if(name != NULL)
   return _msize(name);
else
   return 0;
}

unsigned int Family::MallocsUsedLocal()
{
if(name != NULL)
   return 1;
else
   return 0;
}

FStats *Family::PackageNodeInfo(FStats *out)
{
return out;
}

FStats *Family::UnPackageNodeInfo(FStats *in)
{
if(FAMILYNAME)
   AutoName();
else
   name = NULL;
return in;
}

