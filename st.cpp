#include "interf.h"
#include "smlutils.h"
#include "st_hl.h"
#include "utils.h"

int Stat(Param *pm, World *earth)
{
//st object [-r(recurse)] [-l(how many levels)] [-f(h|b|d)]

int temp, loop = 0, last = 0, iloop = 0, ctp = 0, OFlag = 0, il2 = 0;
int ObjIDHold[NUMCOMPONENTS+1]; //one extra for the destination ID
int ct[NUMCOMPONENTS];
ObjIDHold[NUMCOMPONENTS + 1] = STATMESSAGE; //default file destination
StatGuide *sg;
IntNode *sib = NULL, *par = NULL;

sg = new StatGuide(earth);
sg->output = 'h'; //human readable is a good default

sg->dest = earth->OutputMessage(STATMESSAGE);
sg->flags = 0;

//Set up the Object Identifier numbers properly
for(loop = 0;loop < NUMCOMPONENTS;loop++)
   {
   ObjIDHold[loop] = NOTSELECTED;
   ct[loop] = NOTSELECTED;
   }
ObjIDHold[0] = 0; //The World node

if(pm->parmc == 1)
   return SML0001N; //No input was given

//Check if first argument is valid Object Identifier
if((temp = ParseObjectID(pm->parms[1], ObjIDHold)) > 0) //If the Object ID is valid
   {
   while(ObjIDHold[last] != NOTSELECTED  && last < NUMCOMPONENTS)
      {
      //if(ObjIDHold[last] == WILDCARD)
         SetFlagOn(last, &sg->flags);
      last++;
      }
   SetFlagOn(last - 1, &sg->flags);
   sg->selTree = BuildTree(ObjIDHold, earth);
   if(sg->selTree == NULL) //something was wrong
      return SML0003N;
   //Now that the parameter has been processed, process the switch(es)
   for(loop = 0;loop < pm->switchc;loop++)
      {
      if( (strcmp(pm->switches[loop], "-h")) == 0) //check for the output type switch
         {
         sg->dest = earth->OutputMessage(STATMESSAGE);
         sg->output = 'h'; //human readable to file
         }
      if( (strcmp(pm->switches[loop], "-s")) == 0)
         {
         sg->dest = stdout;
         sg->output = 'h'; //human readable to screen
         }
      }
   earth->Statter(sg, sg->selTree);
   }
else
   return temp;
return temp;
}
