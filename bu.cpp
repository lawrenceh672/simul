#include "interf.h"

int BytesUsed(Param *pm, World *earth)
{
if(pm->parmc == 1)
   {
   _cprintf("Total Bytes %d\n", earth->BytesUsedTotal());
   return SML0001P;
   }
else
   {
   _cprintf("Invalid parameter\n");
   return SML0003P;
   }
}