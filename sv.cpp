#include "interf.h"
#include "smlutils.h"

int Save(Param *pm, World *earth)
{
IntNode *root;
int ObjIDHold[NUMCOMPONENTS], loop;

if(pm->parmc > 2) //sv filename (some other stuff)
   return SML0001N;

for(loop = 0;loop < NUMCOMPONENTS;loop++)
   ObjIDHold[loop] = WILDCARD;

root = BuildTree(ObjIDHold, earth);

_cprintf("Warning. Overwriting tree.dat\n");
earth->treef = fopen("tree.dat", "wb");
earth->SaveTree(root); //Save the representation of the structure
return earth->ArchiveMember();
}

