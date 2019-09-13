#include "smlutils.h"
#include "world.h"
#include "intnode.h"


IntNode *BuildTree(int *list, World *earth)
{
IntNode *par = NULL, *sib = NULL, *ret = NULL;
int loop, ct[NUMCOMPONENTS], il2,ctp,iloop, chilp = 0, TotalNodes;
par = earth->tree;
ret = par;
ret->SetFlagArrays(); //Must be done everytime the tree is changed and flags are needed
TotalNodes = ret->Count_All_Children() + 1;
SetFlagOn(ret->GetSerial(), ret->SelectFLG);

//Go through each level, for each object selected attach all its lower level children selected to it
for(loop = 1;loop < NUMCOMPONENTS;loop++) //iteration zero is the world(done above)
   {
   if(list[loop] == WILDCARD) //get every Nat/Fam from each of the prev selected Wor/Nat
      {
      sib = par;
      iloop = 0;
      il2 = par->children;
      while((((sib = sib->nextSib(ct)) != par) && (iloop < il2)) || (iloop == 0))
         {
         if(!GetFlag(sib->GetSerial(), ret->SelectFLG))
            continue;
         ctp = sib->children;
         for(chilp = 0;chilp < ctp;chilp++)
            SetFlagOn(sib->chilptr[chilp]->GetSerial(), ret->SelectFLG);
         iloop++;
         }
      }
   else
      {
      if(list[loop] >= 0)
         {
         sib = par;
         iloop = 0;
         il2 = par->children;
         if((list[loop] >= par->children))
            return NULL;
         while((((sib = sib->nextSib(ct)) != par) && (iloop < il2)) || (iloop == 0))
            {
            if(!GetFlag(sib->GetSerial(), ret->SelectFLG)) //Nothing below this level can be selected
               continue;
            if((chilp = sib->GetSerial()) > TotalNodes) //Invalid node selection
               continue;
            SetFlagOn(sib->chilptr[list[loop]]->GetSerial(), ret->SelectFLG);
            iloop++;
            }
         } //end not Wildcard check
      else //its some negative number
         {
         if(list[loop] == NOTSELECTED)
            return ret; //its done here
         else
            return NULL;
         }
      }
   par = par->chilptr[0]; //Set up the next round of additions to the selection tree
   } //end level loop
return ret;
}

StatGuide::StatGuide(World *earthi)
{
level = 0;
flags = 0;
dest = NULL;
selTree = NULL;
earth = earthi;
}

StatGuide::~StatGuide()
{
if(selTree != NULL)
   delete selTree;
}
