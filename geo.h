#include <stdlib.h>

#ifndef GEOGRAPHY
typedef struct tag_MapBlock
{
int resource;
int x;
int y;
}MapBlock;


class Geography
{
public:
MapBlock *location(int x, int y); //returns a pointer to the location of the mapblock in question
Geography(int x, int y);
unsigned int BytesUsedLocal();
unsigned int MallocsUsedLocal();

private:
MapBlock *area;
int DimX;
int DimY;
};
#define GEOGRAPHY
#endif
