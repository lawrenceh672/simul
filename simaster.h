#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <locale.h>
#include <iostream.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys\types.h>
#include <sys\stat.h>

#define GENESPERPERSON 7
#define MaxPopSize 10000
#define InitialFam 5
#define InitialFamSize 20
#define InitialNations 2
#define DOMINANT 1
#define RECESSIVE 2
#define PEOPLENAME 0
#define FAMILYNAME 0
#define NATIONNAME 1
#define LINKSYMMETRY 1 //a gene linked to one, links it to the other automatically
#define getrandom( min, max ) ((rand() % (int)(((max)+1) - (min))) + (min))
#define MAXLINKS 2
#define NUMCOMPONENTS 5//W,N,F,P,G to allow for future inclusion or exclusion of different human schemas
#define DIFFS //A random number of ways to differentiate the components(NUMCOMPONENTS*DIFFS to be exact)
#define LOGMESSAGE 1    //Program messages
#define ERRORMESSAGE 2
#define GENERALMESSAGE 3
#define STATMESSAGE 4
#define WOR 0
#define NAT 1 //Order of the components for array use, instead of naming them explicitly
#define FAM 2 //in a structure of some sort
#define PER 3
#define GEN 4
#define DEST 4 //This is the destination (messages)
#define ERRORMESSAGELENGTH 8 //The length of SMLXXXXY messages
#define MAX_NODE_INFO_SIZE 512 //The absolute maximum size of the data used to store the vitals about any type of node
#define MAX_RECTYPE_NAME_SIZE 64 //For saving, the max length of a name
#define TRUE 1
#define FALSE 0
#define MAXFILENAMELENGTH 512
#define GENDER 0 //Person Flags
#define MALE 0 //The GENDER flag returns zero for male
#define FEMALE 1 //one for female

//These defines pertain to codes for Object Identification routines
#define WILDCARD -3
#define IGNORESTATS -4
#define NOTSELECTED -5

//Empty Class definitions
class Gene;
class Person;
class Family;
class Nation;
class World;
class IntNode;
class Geography; //Maintains the physical world simulation
class Interface; //Link between simul and user
class StatGuide; //For selecting all or part of the schema hierarchy

#ifndef STAT_STRUCTURES
#define STAT_STRUCTURES
typedef struct GenericStats
{
int mof; //the sequence number of its parent
int members; //how many members it controls
int offset; //how many other children exist in the left side of the tree before its own children
int seqno; //its own sequence number
int serial;
}GenericStats;

typedef struct GStats
{
int Gene_Index;
int Gene_Links;
int Gene_Link_Index[MAXLINKS];
unsigned char flags;
int enviroID;
int PartNumber;
GenericStats gs;
/*
flag 1: Dominant/Recessive 0/1
*/
}GStats;

typedef struct PStats
{
unsigned char flags;
GenericStats gs;
}PStats;

typedef struct FStats
{
GenericStats gs;
}FStats;

typedef struct NStats
{
GenericStats gs;
}NStats;

typedef struct WStats
{
GenericStats gs;
char name[20];
}WStats;

typedef struct LStats
{
int counts[NUMCOMPONENTS];
}LStats;
#endif
