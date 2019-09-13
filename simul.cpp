#include "simaster.h"
#include "intnode.h"
#include "world.h"
#include "nation.h"
#include "family.h"
#include "person.h"
#include "gene.h"
#include "interf.h"

main()
{
World *earth;
Interface *inter;

if(earth->treef == NULL) //The tree file exists
   {
   _cprintf("Warning!  tree.dat cant be opened!\n");
   }
else
   {
   earth->tree = new IntNode(0, NULL); //Start it off
   earth->tree->LoadTree(earth->treef);
   }

earth = new World(NULL); //Just use the stuff from the disk
inter = new Interface(earth);
while( inter->Prompt() == 666) {}; //until the user exits it
delete Lowest_Level::tree;
delete earth;
delete inter;
return 1;
}
