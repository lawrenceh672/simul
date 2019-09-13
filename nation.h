#include "simaster.h"
#include "hier.h"

#ifndef nation
class Nation : public level <Family, Nation, NStats>
{

public:
char *name;
NStats *stats(int dest); //which nation to gather stats on
void StatMember(StatGuide *sg);
void AutoName();
unsigned int BytesUsedLocal();
unsigned int MallocsUsedLocal();
NStats *UnPackageNodeInfo(NStats *in);
NStats *PackageNodeInfo(NStats *out);
IntNode *Initialize_Local(IntNode *root);

Nation::Nation(IntNode *root);
Nation() {name = NULL;};
~Nation();
int population;
};
#define nation
#endif
