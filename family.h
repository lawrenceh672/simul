#include "simaster.h"
#include "hier.h"

#ifndef FAMILY
class Family : public level <Person, Family, FStats>
{
public:
char *name;

Family::Family(IntNode *root);
Family() {name = NULL;};
~Family();
void AutoName();
unsigned int RetMem();
unsigned int BytesUsedLocal(); //How many bytes were allocated by this family
unsigned int MallocsUsedLocal(); //How many bytes were allocated by this family
MapBlock *Location(int x, int y);
void StatMember(StatGuide *sg);
FStats *UnPackageNodeInfo(FStats *in);
FStats *PackageNodeInfo(FStats *out);
};
#define FAMILY
#endif
