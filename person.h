#include "hier.h"

#ifndef PERSON
class Person : public level <Gene, Person, PStats>
{
public:
char *name;
Person::Person(IntNode *root);
Person() {name = NULL;};
~Person();

void SetName(char *NewName);
void AutoName(); //Gives an automatic name for the person
int Disperse(int *arr, int max, int choicesize);  //Returns how many links were generated
int Gender();
unsigned int BytesUsedLocal(); //Bytes allocated
unsigned int MallocsUsedLocal(); //Bytes allocated
void StatMember(StatGuide *sg); //the actual routine to format/output the stuff
PStats *UnPackageNodeInfo(PStats *in);
PStats *PackageNodeInfo(PStats *out);

private:
unsigned char flags;
Person *father; //parents.
Person *mother;
MapBlock *where;
/*
Flag zero sex- m/f - 0/1
*/
};
#define PERSON
#endif
