#include "simaster.h"
#include "hier.h"

#ifndef GENE
class Gene : public level<Lowest_Level, Gene, GStats>
{
public:
unsigned short type;
void StatMember(StatGuide *sg);
unsigned int BytesUsedLocal();  //Returns the amount of allocated memory
unsigned int MallocsUsedLocal();  //Returns the amount of allocated memory
GStats *PackageNodeInfo(GStats *out);
GStats *UnPackageNodeInfo(GStats *in);

Gene(IntNode *root);
Gene() {linked = NULL; enviroID = NULL; partNumber = 0; flags = 0; linked = 0; links = 0; index = 0;};
~Gene();

int enviroID;
int partNumber;
unsigned char flags;
Gene **linked; //a pointer to an array of Gene pointers
unsigned char links;
unsigned char index;
};
#define GENE
#endif
