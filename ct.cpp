#include "interf.h"
#include "st_hl.h"
#include "smlutils.h"

int Count(Param *pm, World *earth)
{
int loop, last = 0, ObjIDHold[NUMCOMPONENTS], ct[NUMCOMPONENTS], temp;
StatGuide *sg;

sg = new StatGuide(earth);
sg->dest = stdout;

//Set up the Object Identifier numbers properly
for(loop = 0;loop < NUMCOMPONENTS;loop++)
   {
   ObjIDHold[loop] = NOTSELECTED;
   ct[loop] = NOTSELECTED;
   }
ObjIDHold[0] = 0; //The World node

if(pm->parmc == 1)
   {
   if(pm->switchc == 0)
      return SML0001N; //No input was given
   else
      {
      ObjIDHold[0] = 0;
      temp = 1;
      }
   }
else
   temp = ParseObjectID(pm->parms[1], ObjIDHold);

//Check if first argument is valid Object Identifier
if(temp > 0) //If the Object ID is valid
   {
   while(ObjIDHold[last] != NOTSELECTED  && last < NUMCOMPONENTS)
      last++;
   SetFlagOn(last - 1, &sg->flags);
   sg->selTree = BuildTree(ObjIDHold, earth);
   //Now that the parameter has been processed, process the switch(es)
   for(loop = 0;loop < pm->switchc;loop++)
      {
      if( (strcmp(pm->switches[loop], "-g")) == 0) //check for the output type switch
         {
         sg->level = GEN;
         }
      if( (strcmp(pm->switches[loop], "-p")) == 0)
         {
         sg->level = PER;
         }
      if( (strcmp(pm->switches[loop], "-f")) == 0)
         {
         sg->level = FAM;
         }
      if( (strcmp(pm->switches[loop], "-n")) == 0)
         {
         sg->level = NAT;
         }
      if( (strcmp(pm->switches[loop], "-w")) == 0)
         {
         sg->level = WOR;
         }
      }
   last = earth->Counts(sg, sg->selTree);
   fprintf(sg->dest, "%d\n", last);
   }
else
   return temp;
return temp;

}
