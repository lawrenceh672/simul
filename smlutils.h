#include "simaster.h"
#include "utils.h"

class StatGuide
{
public:
StatGuide(World *earthi);
~StatGuide();

FILE *dest; //Where's it goin?
unsigned char output; //Which form is the output Gene - World 0...GG PP...WW 2 bits per output flag
int level; //Usually the level that information is requested of

IntNode *selTree; //the selection tree;
unsigned char flags; //Gene, Person, Family, Nation, World. 0 don't output 1 000X XXXX
int OutputLevel(int level) {return GetFlag(level, &flags); }
private:
World *earth;
};

//Simul specific Utilities
extern IntNode *BuildTree(int *list, World *earth);
