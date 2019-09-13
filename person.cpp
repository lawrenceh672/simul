#include "person.h"
#include "gene.h"
#include "error.h"
#include "smlutils.h"
#include "intnode.h"

#ifdef RAND_MAX
#undef RAND_MAX
#define RAND_MAX 100
#endif

Person::Person(IntNode *root)
{
LevelID = PER;
}

void Person::SetName(char *NewName)
{
name = NewName;
}


int Person::Gender()
{
return GetFlag(GENDER, &flags);
}

void Person::AutoName()
{
char namebuffer[33];
name = (char *)malloc(33*sizeof(char));
name[0] = 0;
name[1] = 0;

strcat(name, "Person");
strcat(name, _itoa(getrandom(0, 32767), namebuffer, 10));
}

int Person::Disperse(int *arr, int max, int choicesize)
{
int loop, links;
links = getrandom(0, max);

//Determine now how many genes are linked to it
for(loop = 0; loop < links; loop++)
{
arr[loop] = getrandom(0, choicesize); //Now give it a random link
}
return links;
}

Person::~Person()
{
free ((void *)name);
}

void Person::StatMember(StatGuide *sg)
{
if(sg->output == 'h')
   {
   if(GetFlag(GENDER, &flags) == MALE)
      fprintf(sg->dest, "Person %d(%d) name: %s.  M\n", seqno, serial, name);
   else
      fprintf(sg->dest, "Person %d(%d) name: %s.  F\n", seqno, serial, name);
   }
}

unsigned int Person::BytesUsedLocal()
{
if(name == NULL)
   return 0;
else
   return _msize((void *)name);
}

unsigned int Person::MallocsUsedLocal()
{
if(name == NULL)
   return 0;
else
   return 1;
}

PStats *Person::PackageNodeInfo(PStats *out)
{
out->flags = flags;
return out;
}

PStats *Person::UnPackageNodeInfo(PStats *in)
{
if(PEOPLENAME)
   AutoName();
else
   name = NULL;
flags = in->flags;
return in;
}

