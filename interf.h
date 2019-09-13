#include "world.h"
#include "error.h"

#ifndef INTERFACE
class Param
{
public:
Param();
~Param();

char **parms; //an array of string that are the parameters
char **switches; //switches
int parmc;// counts
int switchc;
};

typedef struct Func
{
int (*funcptr)(Param *pm, World *earth);  //the pointer to the function
void *out; //A pointer to its output (whatever its type - possibly UDF)
char delim;  //what constitutes the delimiter between its arguments
char keyword[3]; //What the text of the keyword is 2 letters plus null terminated
int FuncID; //A unique integer for this function
char *syntax; //A string that shows the syntax in case of error
}Func;


class Interface
{
public:
int Prompt(); //Sets up the prompt for instructions
int ReadInput();
Interface(World *mem);
~Interface();
int Lookup(char *inp, char *delim, int *retval);
int Execute(Func *fPtr, Param *pm, World *earth);
char *Parameterizer(char *inp, Param *pm);  //Parses up a correctly specified SML command string into its components
int RegisterFunc( int (*funcptr)(Param *pm, World *earth), char delim, char *syn, char *key);

private:
char input[512];
char PromptString[20];
Param *pm;
Func **Library; //My own functions
int FuncsRegd; //Number of registered Functions
World *memberOf;
};
#define INTERFACE
#endif

//Simul Keywords
int Stat(Param *pm, World *earth);
int Empty(Param *pm, World *earth);
int GlobalVar(Param *pm, World *earth);
int Help(Param *pm, World *earth);
int Save(Param *pm, World *earth);
int Load(Param *pm, World *earth);
int Init(Param *pm, World *earth);
IntNode *WorldSpecs();
int Count(Param *pm, World *earth);
int BytesUsed(Param *pm, World *earth);
