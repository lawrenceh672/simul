#include "interf.h"

int Help(Param *pm, World *earth)
{
if(pm->parmc == 1) //just hl
   {
   _cprintf("hl [keyword].\n");
   _cprintf("ex - exit.\n");
   _cprintf("st - statistics.\n");
   _cprintf("va - variables.\n");
   return SML0001P;
   }

if(pm->parmc > 2) //hl [something] [and more things]
   return SML0001N;

if(strcmp(pm->parms[1], "ex")==0)
   {
   _cprintf("Just Type ex and press enter to quit SIMUL.\n");
   return SML0001P;
   }

if(strcmp(pm->parms[1], "va")==0)
   {
   _cprintf("va [Variable] [NewValue].  Variables include:\n");
   _cprintf("stat - File for receiving statistical information.\n");
   _cprintf("log - File for receiving Log information.\n");
   _cprintf("error - File for receiveing Error Information.\n");
   _cprintf("out - File for receiving General Output.\n");
   _cprintf("defaults - Default world settings.  Can't be changed.\n");
   _cprintf("prompt - prompt String default.\n");
   return SML0001P;
   }

if(strcmp(pm->parms[1], "st")==0)
   {
   _cprintf("st object [Form].\n");
   _cprintf("object is an object id of the form N.F.P.G\n");
   _cprintf("(H)uman (B)inary (I)xf (D)atabase].\n");
   return SML0001P;
   }
return SML0001N;
}

