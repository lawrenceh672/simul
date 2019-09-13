#include <direct.h>
#include "world.h"
#include "nation.h"
#include "person.h"
#include "family.h"
#include "gene.h"
#include "utils.h"
#include "intnode.h"
#include "smlutils.h"
#include "error.h"

World::World(IntNode *root)
{
int tg = 0, tp = 0, tf = 0, tn = 0, tw = 0;
int tgm, tpm, tfm, tnm, twm, rowsd;

strcpy(name, "Test World");
//Initialize all the members
population = 0;
families = 0;
members = 0;

if(root != NULL)
   {
   seqno = root->val;
   offset = 0;
   }

AddMembers(root, tree, 0); //Actually make it
rowsd = DP.DeleteData(0); //Delete the pre-existing world node
DP.AddData(this); //And add this one in

tn = BytesUsed(NAT);
tf = BytesUsed(FAM);
tp = BytesUsed(PER);
tg = BytesUsed(GEN);
tw = BytesUsed(WOR);
tnm = MallocsUsed(NAT);
tfm = MallocsUsed(FAM);
tpm = MallocsUsed(PER);
tgm = MallocsUsed(GEN);
twm = MallocsUsed(WOR);
fprintf(OutputMessage(LOGMESSAGE), "Size Of a Gene %d.\n", gs);
fprintf(OutputMessage(LOGMESSAGE), "Size Of a Person %d.\n", ps);
fprintf(OutputMessage(LOGMESSAGE), "Size of a Family %d.\n", fs);
fprintf(OutputMessage(LOGMESSAGE), "Size of a Nation %d.\n", ns);
fprintf(OutputMessage(LOGMESSAGE), "Size of a World %d.\n", ws);
fprintf(OutputMessage(LOGMESSAGE), "Bytes used by Gene Class %d(%d allocations).\n", tg, tgm);
fprintf(OutputMessage(LOGMESSAGE), "Bytes used by Person Class %d(%d allocations).\n", tp, tpm);
fprintf(OutputMessage(LOGMESSAGE), "Bytes used by Family Class %d(%d allocations).\n", tf, tfm);
fprintf(OutputMessage(LOGMESSAGE), "Bytes used by Nation Class %d(%d allocations).\n", tn, tnm);
fprintf(OutputMessage(LOGMESSAGE), "Bytes used by World Class %d(%d allocations).\n", tw, twm);
fprintf(OutputMessage(LOGMESSAGE), "Total Bytes: %d(%d mallocs)\n", tg + tp + tf + tn + tw, tgm + tpm + tfm + tnm + twm);
}

void World::StatMember(StatGuide *sg)
{
if(sg->output == 'h')
   {
   fprintf(sg->dest, "World name: %s\n", name);
   fprintf(sg->dest, "Total population:%d\n", population);
   fprintf(sg->dest, "Total Nations: %d\n", members);
   fprintf(sg->dest, "Total Families: %d\n", families);
   }
}

unsigned int World::BytesUsedLocal()
{
return 0;
}

unsigned int World::MallocsUsedLocal()
{
return 0;
}

unsigned int World::BytesUsedTotal()
{
return BytesUsed(WOR);
}

World::~World()
{
if(error != NULL) fclose(error);
if(logf != NULL) fclose(logf);
if(statf != NULL)fclose(statf);
}

int World::count(int typeof, int *inds)
{
int loop = 0, count = 0;
void *tp;

if(inds[typeof] == WILDCARD)
   {
   inds[typeof] = NOTSELECTED;
   tp = GetSchema(inds);
   inds[typeof] = WILDCARD;
   }
else
   {
   tp = GetSchema(inds);
   }

switch(typeof - 1) //gotta count from the above level
   {
   case GEN:
      count = ((Gene *)tp)->Counter(typeof);
      break;
   case PER:
      count = ((Person *)tp)->Counter(typeof);
      break;
   case FAM:
      count = ((Family *)tp)->Counter(typeof);
      break;
   case NAT:
      count = ((Nation *)tp)->Counter(typeof);
      break;
   case WOR:
      count = ((World *)tp)->Counter(typeof);
      break;
   }
return count;
}

void World::KillEveryone()
{
population = 0;
families = 0;
nations = 0;
tree->DeleteAllChildren(); //Deletes the guiding tree structure
DeleteMembers(); //Clears out the datapool and destroys those changes
}

WStats *World::PackageNodeInfo(WStats *out)
{
strcpy(out->name, name);
return out;
}

WStats *World::UnPackageNodeInfo(WStats *in)
{
strcpy(name, in->name);
return in;
}

int World::LoadData(char *path)
{
//Close up the datapools
level<Lowest_Level, Gene, GStats>::DP.CloseFiles();
level<Gene, Person, PStats>::DP.CloseFiles();
level<Person, Family, FStats>::DP.CloseFiles();
level<Family, Nation, NStats>::DP.CloseFiles();
level<Nation, World, WStats>::DP.CloseFiles();
fclose(Lowest_Level::treef);
tree->DeleteAllChildren();
if(chdir(path) == -1)
   return SML0006N;
//Reload them
level<Lowest_Level, Gene, GStats>::DP.OpenPool("gene");
level<Gene, Person, PStats>::DP.OpenPool("person");
level<Person, Family, FStats>::DP.OpenPool("family");
level<Family, Nation, NStats>::DP.OpenPool("nation");
level<Nation, World, WStats>::DP.OpenPool("world");
//Reload the tree structure
Lowest_Level::treef = fopen("tree.dat", "rb");
tree->LoadTree(Lowest_Level::treef);
return SML0000P; //All good
}

IntNode *World::Initialize_Local(IntNode *root)
{
population = Counter(PER);
families = Counter(FAM);
nations = Counter(NAT);
return tree;
}
