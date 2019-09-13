#include "hier.h" //Hierarchy base class
#include "gene.h"
#include "error.h"
#include "smlutils.h"
#include "intnode.h"

Gene::Gene(IntNode *root)
{
type = 0;
linked = NULL;
links = 0;
seqno = root->val;
}

Gene::~Gene()
{
free(linked);
}

void Gene::StatMember(StatGuide *sg)
{
int loop = 0, iloop = 0;

fprintf(sg->dest, "Gene %d(%d): has %d links\n", seqno, serial, links);
if(links != 0)
   {
   fprintf(sg->dest, "Linked Genes\n");
   for(iloop = 0; iloop < links; iloop++)
      {
      fprintf(sg->dest, "%d\n", linked[iloop]->index);
      }
   }
}

unsigned int Gene::BytesUsedLocal()
{
return _msize(linked);
}

unsigned int Gene::MallocsUsedLocal()
{
return 1;
}

GStats *Gene::PackageNodeInfo(GStats *out)
{
int loop;

out->Gene_Index = index;
out->Gene_Links = links;
for(loop = 0;loop < links;loop++)
   out->Gene_Link_Index[loop] = linked[loop]->index;
out->flags = flags;
out->enviroID = enviroID;
out->PartNumber = partNumber;
return out;
}

GStats *Gene::UnPackageNodeInfo(GStats *in)
{
index = in->Gene_Index;
links = in->Gene_Links;
flags = in->flags;
enviroID = in->enviroID;
partNumber = in->PartNumber;
return in;
}
