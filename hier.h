#include "simaster.h"
#include "datapool.h"
#include "geo.h"

#ifndef HIER
class Lowest_Level
{
public:
Lowest_Level() {};
Lowest_Level(IntNode *root) {if(root != NULL) fprintf(OutputMessage(ERRORMESSAGE), "Lowest Level was created!!!!\n"); };
int isLowest() { return TRUE; }; //every other def will return 0
unsigned int BytesUsed(int level) {return 0; };
unsigned int MallocsUsed(int level) {return 0; };
int ArchiveMember() {return 0;};
void *GetSchema(int *path) {return NULL;};
unsigned int Counter(int level) {return 0;};
void DeleteMembers() {};
void Statter(StatGuide *sg, IntNode *root) {};
unsigned int Counts(StatGuide *sg, IntNode *root) {return 0;};
void SaveTree(IntNode *root);
void Initialize_Member(int ser) {};
int AddMembers(IntNode *root, IntNode *tree, int new_serial) {return 0;};
void Clear_DataPool() {};

FILE *OutputMessage(char dest);

static DataPool<Lowest_Level, LStats> DP;
LStats *UnPackageNodeInfo(LStats *sts) {return NULL;};
LStats *PackageNodeInfo(LStats *out) {return NULL;};
LStats *UnPackageNodeInfo_General(LStats *sts) {return NULL;};
LStats *PackageNodeInfo_General(LStats *out) {return NULL;};
static FILE *error, *logf, *statf, *screen, *treef; //Output Streams
static Geography Map; //An instance of a class that holds the structure of the map in memory
static IntNode *tree; //an in memory representation of the tree
void Initialize_DataPool(IntNode *root) {};

int members;
int mof;

protected:
int seqno;
int offset; //a number to add to get the datapool sequence number for its members
int serial; //its serial number
IntNode *here; //a pointer to its node in the hierarchy tree
};

template <class L, class M, class T> //Lower level, this level, Package data
class level : public Lowest_Level //this composes the base class for all hierarchy levels
{
public:
level();
~level();
int isLowest() { return FALSE; };
int ArchiveMember();
virtual void StatMember(StatGuide *sg);
void Statter(StatGuide *sg, IntNode *root);
void *GetSchema(int *path);
unsigned int Counts(StatGuide *sg, IntNode *root);
unsigned int Counter(int leveli);
unsigned int BytesUsed(int leveli);
virtual unsigned int BytesUsedLocal(); //every level does it differently
T *PackageNodeInfo_General(T *out); //Based on the whatever stat structure
T *UnPackageNodeInfo_General(T *in); //Converts the data from the archive to the dynamic memory system
virtual T *PackageNodeInfo(T *out) {return out;}; //Based on the whatever stat structure
virtual T *UnPackageNodeInfo(T *in) {return in;}; //Converts the data from the archive to the dynamic memory system
unsigned int MallocsUsed(int leveli);
virtual unsigned int MallocsUsedLocal(); //every level does it differently
L *GetData(int ID);
void DeleteMembers(); //just like the destructor except it doesn't do any of the automatic stuff(like removing the ability to call functions)
int AddMembers(IntNode *root, IntNode *tree, int new_ser); //returns how many were successfully created
MapBlock *Location(int x, int y);
virtual IntNode *Initialize_Local(IntNode *root) {return root;};
void Initialize_DataPool(IntNode *root);
void ChangeStructure(IntNode *root);
void Clear_DataPool();

static unsigned char LevelID; //ID's the level for recursion purposes
static DataPool<M, T> DP; //the hard storage modules
static L LowerInfo; //A captured member of the lower level to get at its static variables
};
#define HIER
#endif
