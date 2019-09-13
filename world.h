#include "hier.h"
#include "simaster.h"

#ifndef WORLD
class World : public level<Nation, World, WStats>
{
public:
char name[20];
void InitializeNation(Nation *nat);
unsigned int BytesUsedLocal();
unsigned int MallocsUsedLocal();
unsigned int BytesUsedTotal();
int count(int typeof, int *inds); //census function
void KillEveryone(); //Removes all people from the world
WStats *UnPackageNodeInfo(WStats *in);
WStats *PackageNodeInfo(WStats *out);
int LoadData(char *path);
IntNode *Initialize_Local(IntNode *root);
void StatMember(StatGuide *sg);

int fs, ps, gs, ns, ws; //Size of the SIMUL human organizers

World::World(IntNode *root);
World() {};
~World();

//These are the communication pointers for the world and all its subparts.
struct data
{
char **INT8;
unsigned char **UINT8;
short **INT16;
unsigned short **UINT16;
int **INT32;
unsigned int **UINT32;
};

unsigned int population;
unsigned int families;
unsigned int nations;
int DimX, DimY;

private:
//FILE *error, *logf, *out, *statf, *screen; //Output Streams
};
#define WORLD
#endif
