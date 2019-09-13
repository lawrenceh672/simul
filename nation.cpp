#include "nation.h"
#include "family.h"
#include "error.h"
#include "smlutils.h"
#include "intnode.h"

Nation::Nation(IntNode *root)
{
name = NULL;
if(NATIONNAME)
   AutoName();
}

void Nation::AutoName()
{
char namebuffer[33];
name = (char *)malloc(33*sizeof(char));
name[0] = 0;
name[1] = 0;

strcat(name, "Canada");
strcat(name, _itoa(seqno, namebuffer, 10));
}

Nation::~Nation()
{
free ((void *)name);
}

void Nation::StatMember(StatGuide *sg)
{
if(sg->output == 'h')
   {
   fprintf(sg->dest, "Nation %d(%d) name: %s.\n", seqno, serial, name); //Someday this will accomondate binary/db data etc...
   fprintf(sg->dest, "Population %d\n", population);
   fprintf(sg->dest, "Families %d\n\n", members);
   }
}

unsigned int Nation::BytesUsedLocal()
{
if(name != NULL)
   return _msize(name);
else
   return 0;
}

unsigned int Nation::MallocsUsedLocal()
{
if(name != NULL)
   return 1;
else
   return 0;
}

NStats *Nation::PackageNodeInfo(NStats *out)
{
return out;
}

NStats *Nation::UnPackageNodeInfo(NStats *in)
{
if(NATIONNAME)
   AutoName();
else
   name = NULL;
population = Counter(PER);
return in;
}

IntNode *Nation::Initialize_Local(IntNode *root)
{
population = Counter(PER);
return tree;
}

