#include "interf.h"
#include "intnode.h"
#include "utils.h"
#include <ctype.h>

int Init(Param *pm, World *earth) //Initializes a world, or loads a previous one
{
IntNode *root;
StopWatch TimeIt;
int resp;

if(earth != NULL)
   {
   _cprintf("(D)estroy (M)erge Anything else cancels\n");
   resp = _getche();
   _putch('\n');
   resp = tolower(resp);
   switch(resp)
      {
      case 'd':
         earth->KillEveryone();
         break;
      case 'm':
         break;
      default:
         return SML0000P;
      }
   }
//Merge this tree onto the existing one but not the root node, cuz there is only one world
root = WorldSpecs(); //Make it accordingly
TimeIt.Start();
earth->AddMembers(root, earth->tree, 0); //Now do it.  Add in the new members(and bunches of other stuff) 0 is the world not serial number
printf("%f seconds.", TimeIt.Stop());

_cprintf("%d Genes\n", earth->Counter(GEN));
if(pm->parmc == 2) // the user name was included
   strcpy(earth->name, pm->parms[1]);
delete root;
return SML0001P;
}

IntNode *WorldSpecs()
{
IntNode *tree, *sib, *par, *root;
int loop, iloop, maxes[NUMCOMPONENTS], mins[NUMCOMPONENTS], size;

maxes[GEN] = GENESPERPERSON;
mins[GEN] = GENESPERPERSON;

maxes[PER] = GetInteger("Enter maximum people per family\n");
mins[PER] = GetInteger("Enter minimum people per family\n");
maxes[FAM] = GetInteger("Enter maximum families per nation\n");
mins[FAM] = GetInteger("Enter minimum families per nation\n");
maxes[NAT] = GetInteger("Enter maximum nations per world\n");
mins[NAT] = GetInteger("Enter minimum nations per world\n");

tree = new IntNode(0, NULL); //the world node
tree->set(0);
root = tree; //keep the root node in memory
for(loop = 1;loop < NUMCOMPONENTS;loop++)
   {
   par = tree; //now add in stuff for all its siblings
   sib = par;
   do
      {
      size = getrandom(mins[loop], maxes[loop]);
      for(iloop = 0;iloop < size;iloop++)
         {
         sib->add(iloop);
         }
      }
   while((sib = sib->nextSib(NULL) ) != par);
   tree = sib->chilptr[0]; //go to the next level
   }
return root;
}

