#include "geo.h"
#include <malloc.h>

MapBlock *Geography::location(int x, int y)
{
return (MapBlock *)(&area[x*DimX + y]);
}

Geography::Geography(int x, int y)
{
int xl, yl;

area = (MapBlock *)malloc(x*y*sizeof(MapBlock));
DimX = x;
DimY = y;

for(xl = 0; xl < DimX; xl++)
   {
   for(yl = 0; yl < DimY; yl++)
      {
      area[xl*DimX + yl].x = xl;
      area[xl*DimX + yl].y = yl;
      }
   }
}

unsigned int Geography::BytesUsedLocal()
{
return _msize(area);
}

unsigned int Geography::MallocsUsedLocal()
{
return 1;
}
