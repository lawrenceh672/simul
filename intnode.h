#include "simaster.h"
#include "utils.h"
#include <stdio.h>

typedef struct NodeHeader
{
int children;
int val;
}NodeHeader;

class IntNode
{
friend class StatGuide;
public:
IntNode(unsigned int chil, IntNode *parent);
IntNode(unsigned int members, IntNode *parent, int notice);
~IntNode();
IntNode *Level(int lev);
IntNode *add(unsigned int adde);
void enumAll(int num);
void set(unsigned int adde);
unsigned int BytesUsed();
IntNode *nextSib(int *path); //returns the next sibling in sequence
int findIndex(IntNode *inp); //Finds the index for an arbitary node, lower down
int GetIndex(); //Finds the index of this node
int TotalChildren();
int Level();
int TotalLevels();
void PrintTree(FILE *fp);
void SetPath(int *path);
IntNode *Find(int *path);
int Count(); //Counts everything in its level
int Offset();
void LineTree(int members); //Makes a straight line out of the tree
void SaveTree(FILE *fp);
void LoadTree(FILE *fp);
IntNode *GetNode(int serial);
int GetSerial();
int Count_All_Children();
int Count_Level(int lev);
void Merge(IntNode *tree);
void DeleteNode(unsigned int node);
void DeleteAllChildren();
void Squabbling_Children(); //Separates each child so that it has a parent all to itself :-)
IntNode *Incorporate(IntNode *root);
IntNode *AddNode(IntNode *nde);
void SetFlagArrays(); //allocates the correct amount of memory for the flags

unsigned int children;
IntNode **chilptr;
IntNode *parent;
int val; //the actual node

static unsigned char *SelectFLG;

private:
unsigned int count;
IntNode *Root();
};


