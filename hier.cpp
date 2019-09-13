#include "error.h"
#include "hier.h"
#include "gene.h"
#include "person.h"
#include "family.h"
#include "nation.h"
#include "world.h"
#include "intnode.h"
#include "smlutils.h"
#include "geo.h"

#ifndef HIER_CPP_DEFINED
//This prevents NULL checks that are only needed for the intitial static variable assignments
IntNode BlankTree(1, NULL); //1 child so that the same constructors don't run into NULL pointers

//Now assign the static variables correctly
//Lowest Level stuff
DataPool<Lowest_Level, LStats> Lowest_Level::DP = DataPool<Lowest_Level, LStats>("gnfknr"); //Theres no info in the lowest level, its just a placeholder
FILE *Lowest_Level::error = fopen( "error.txt", "w");  //Standard IO streams
FILE *Lowest_Level::logf = fopen( "logf.txt", "w");
FILE *Lowest_Level::statf = fopen( "stat.txt", "w");
FILE *Lowest_Level::screen = stdout;
FILE *Lowest_Level::treef = fopen("tree.dat", "rb");
Geography Lowest_Level::Map = Geography(20, 20);  //20x20 Blocks
IntNode *Lowest_Level::tree = new IntNode(0, NULL); //a default node for the world

//Explicitly instantiate the classes
template level<Lowest_Level, Gene, GStats>; //Gene Class
template level<Gene, Person, PStats>; //Person
template level<Person, Family, FStats>; //Family
template level<Family, Nation, NStats>; //Nation
template level<Nation, World, WStats>; //World

//Level ID's
unsigned char level<Lowest_Level, Gene, GStats>::LevelID = GEN;
unsigned char level<Gene, Person, PStats>::LevelID = PER;
unsigned char level<Person, Family, FStats>::LevelID = FAM;
unsigned char level<Family, Nation, NStats>::LevelID = NAT;
unsigned char level<Nation, World, WStats>::LevelID = WOR;

//The DataPools
DataPool<Gene, GStats> level<Lowest_Level, Gene, GStats>::DP = DataPool<Gene, GStats>("gene");
DataPool<Person, PStats> level<Gene, Person, PStats>::DP = DataPool<Person, PStats>("person");
DataPool<Family, FStats> level<Person, Family, FStats>::DP = DataPool<Family, FStats>("family");
DataPool<Nation, NStats> level<Family, Nation, NStats>::DP = DataPool<Nation, NStats>("nation");
DataPool<World, WStats> level<Nation, World, WStats>::DP = DataPool<World, WStats>("world");

//Members of lower levels for reference to static variables
Lowest_Level level<Lowest_Level, Gene, GStats>::LowerInfo = Lowest_Level();
Gene level<Gene, Person, PStats>::LowerInfo = Gene();
Person level<Person, Family, FStats>::LowerInfo = Person();
Family level<Family, Nation, NStats>::LowerInfo = Family();
Nation level<Nation, World, WStats>::LowerInfo = Nation();

/*
Lowest Level Specfic Routines
*/
FILE *Lowest_Level::OutputMessage(char dest)
{
if(dest == ERRORMESSAGE)
   return error;
if(dest == LOGMESSAGE)
   return logf;
if(dest == GENERALMESSAGE)
   return stdout;
if(dest == STATMESSAGE)
   return statf;
return NULL;
}

void Lowest_Level::SaveTree(IntNode *root)
{
root->SaveTree(treef);
}

/*
Generic Level Routines
*/
template <class L, class M, class T> level<L,M,T>::level()
{
members = 0;
seqno = 0;
offset = 0;
serial = 0;
mof = 0;
here = NULL;
}


template <class L, class M, class T> level<L,M,T>::~level()
{
members = 0;
seqno = 0;
offset = 0;
serial = 0;
mof = 0;
}


template <class L, class M, class T> void level<L,M,T>::Statter(StatGuide *sg, IntNode *root)
{
int num, loop, rser;
IntNode *sib;
L *dta;

if(root == NULL) //simple checks to prevent exceptions
   return;
num = root->children;
//level = root->Level();
rser = root->GetSerial();
if(GetFlag(rser,root->SelectFLG)) //whether this node needs to be seen or not
   StatMember(sg);
if(num == 0)
   {
   return;
   }
sib = root->chilptr[0]; //Now check all its children
for(loop = 0;loop < num;loop++)
   {
   dta = GetData(loop);
   if(dta != NULL)
      dta->Statter(sg, root->chilptr[loop]);
   else
      fprintf(sg->dest, "There was an error at node %d\n", sib->GetSerial());
   sib = sib->nextSib(NULL);
   }
}

template <class L, class M, class T> void level<L,M,T>::StatMember(StatGuide *sg)
{
}

template <class L, class M, class T> unsigned int level<L,M,T>::BytesUsedLocal()
{
return 666;
}

template <class L, class M, class T> unsigned int level<L,M,T>::BytesUsed(int leveli)
{
int count = 0, loop;

if(leveli == LevelID) //if this is the level, count it all and return it
   {
   count = (sizeof(L) * members); //this is the same for all level subclasses
   return BytesUsedLocal() + count + DP.BytesUsed();
   }
if(leveli < LevelID || leveli == NOTSELECTED) //trying to count an unselected level, or a greater level is nonsense(the same level is ok)
   return 0;

//otherwise get the count from everymember
for(loop = 0;loop < members;loop++)
   count += GetData(loop)->BytesUsed(leveli);
return count;
}

template <class L, class M, class T> unsigned int level<L,M,T>::MallocsUsedLocal()
{
return 666;
}

template <class L, class M, class T> unsigned int level<L,M,T>::MallocsUsed(int leveli)
{
int count = 0, loop;

if(leveli == LevelID) //if this is the level, count it all and return it
   {
   count = members; //this is the same for all level subclasses
   return MallocsUsedLocal() + count;
   }
if(leveli < LevelID || leveli == NOTSELECTED) //trying to count an unselected level, or a greater level is nonsense(the same level is ok)
   return 0;

//otherwise get the count from everymember
for(loop = 0;loop < members;loop++)
   count += GetData(loop)->MallocsUsed(leveli);
return count;
}

template <class L, class M, class T> unsigned int level<L,M,T>::Counts(StatGuide *sg, IntNode *root)
{
int lev, num, loop, retval = 0;
IntNode *sib;

if(root == NULL) //simple checks to prevent exceptions
   return 0;
num = root->children;
lev = root->Level();
if(sg->OutputLevel(lev)) //whether this level needs to be seen or not
   return Counter(sg->level);
if(num == 0)
   {
   return 0;
   }
sib = root->chilptr[0];
for(loop = 0;loop < num;loop++)
   {
   retval += GetData(sib->val)->Counts(sg, root->chilptr[loop]);
   sib = sib->nextSib(NULL);
   }
return retval;
}

template <class L, class M, class T> unsigned int level<L,M,T>::Counter(int leveli)
{
if(here != NULL)
   return here->Count_Level(leveli);
else
   return tree->GetNode(serial)->Count_Level(leveli);
}

template <class L, class M, class T> int level<L,M,T>::ArchiveMember()
{
//Tells its datapool to save itself
DP.Save();
//Then tell the lower one to do likewise
LowerInfo.ArchiveMember();
return SML0001P;
}

template <class L, class M, class T> void *level<L,M,T>::GetSchema(int *path)
{
void *tp;

if(path[LevelID] == NOTSELECTED || LevelID == NUMCOMPONENTS) //we want the level above to be returned
   return NULL;

if(path[LevelID] > members || path[LevelID] < 0 || members == 0) //these pick this node conditions(< 0 is to get the WILDCARD)
   return this;

else
   {
   if((tp = GetData(path[LevelID + 1])->GetSchema(path)) == NULL)
      return this;
   else
      return tp; //pass along the returned value
   }
return NULL;
}

template <class L, class M, class T> L *level<L,M,T>::GetData(int ID)
{
return LowerInfo.DP.GetData(ID + offset);
}

template <class L, class M, class T> void level<L,M,T>::DeleteMembers()
{
//First delete its own children
LowerInfo.DP.DeleteAllData();
LowerInfo.DP.ClearAllChanges();
members = 0;
seqno = 0;
offset = 0;
serial = 0;
mof = 0;
LowerInfo.DeleteMembers();
}

template <class L, class M, class T> void level<L,M,T>::ChangeStructure(IntNode *root)
{
unsigned int loop;

for(loop = 0;loop < root->children;loop++) //make the overall tree correct
   here->Merge(root->chilptr[loop]);

Initialize_DataPool(root); //change the datapool to reflect the new changes
}

template <class L, class M, class T> int level<L,M,T>::AddMembers(IntNode *root, IntNode *curr_tree, int new_serial)
{
int loop, newchil;
L *newc;

if(root == NULL) //simple checks to prevent exceptions
   {
   if(tree != NULL) //If something has been loaded
      {
      here = tree;
      serial = here->GetSerial(); //get the id variables down pat
      offset = here->Offset();
      seqno = here->GetIndex();
      members = here->children;
      Initialize_Local(here); //Set up some initial stuff
      return members;
      }
   else
      return 0;
   }
newchil = root->children; //To keep from reaccessing root->children
members += newchil;
if(root->parent == NULL) //world node processing
   {
   mof = -1;
   here = tree;
   serial = 0;
   seqno = 0;
   tree->Merge(root); //Put the tree onto the existing one
   }
else
   {
   //Set up the values correctly
   here = curr_tree; //get the variables set correctly
   serial = new_serial;
   offset = here->Offset();
   seqno = here->GetIndex();
   mof = here->parent->GetSerial(); //Find the parent node
   }

for(loop = 0;loop < newchil;loop++) //and dump in a new object of that class
   {
   newc = new L;
   new_serial++; //addmembers adds nodes in consecutive serial order(depth first)
   newc->AddMembers(root->chilptr[loop], curr_tree->chilptr[(members - newchil) + loop], new_serial); //tell the temporary instance to store everything
   LowerInfo.DP.AddData(newc);  //Put it in the datapool
   delete newc;
   }

Initialize_Local(here); //Set up some initial stuff
return members;
}

template <class L, class M, class T> MapBlock *level<L,M,T>::Location(int x, int y)
{
return Map.location(x,y);
}

template <class L, class M, class T> T *level<L,M,T>::PackageNodeInfo_General(T *out)
{
out->gs.mof = mof;
out->gs.members = members;
out->gs.offset = offset;
out->gs.seqno = seqno;
out->gs.serial = serial;
return out;
}

template <class L, class M, class T> T *level<L,M,T>::UnPackageNodeInfo_General(T *out)
{
mof = out->gs.mof;
members = out->gs.members;
offset = out->gs.offset;
seqno = out->gs.seqno;
serial = out->gs.serial;
here = tree->GetNode(serial);
return out;
}

template <class L, class M, class T> void level<L,M,T>::Initialize_DataPool(IntNode *root)
{
int recs;

recs = root->Count_Level(LevelID + 1);
LowerInfo.DP.CreateTemplate(recs);
LowerInfo.Initialize_DataPool(root);
}

template <class L, class M, class T> void level<L,M,T>::Clear_DataPool()
{
DP.ClearAllChanges();
LowerInfo.Clear_DataPool();
}

#define HIER_CPP_DEFINED
#endif
