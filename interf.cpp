#include "interf.h"
#include <conio.h>
//#include <iostream.h>
#include <ctype.h>
#include "error.h"
#include "utils.h"
#include "st_hl.h"
#include "intnode.h"
#include "smlutils.h"

Interface::Interface(World *mem)
{
memberOf = mem; //Set it up
pm = new Param;
FuncsRegd = 0;

PromptString[0] = 0;
strcat(PromptString, "SIMUL> ");
memset(((void *)input), 0, 512);

Library = (Func **)malloc(sizeof(Func *)*10);//Number of functions
RegisterFunc(&Empty, ' ', "ex", "ex");
RegisterFunc(&Stat, ' ',"st object nat.fam.per.gen [form = (H)uman (B)inary (I)xf (D)atabase] Defaults Human","st");
RegisterFunc(&GlobalVar, ' ', "va Variable [NewValue]", "va");
RegisterFunc(&Help, ' ', "hl keyword", "hl");
RegisterFunc(&Save, ' ', "sv filename", "sv");
RegisterFunc(&Load, ' ', "ld filename", "ld");
RegisterFunc(&Init, ' ', "iz worldname", "iz");
RegisterFunc(&Count, ' ', "ct objid", "ct");
RegisterFunc(&BytesUsed, ' ', "bu type", "bu");
_cprintf("Registered function library(%d functions)\n", FuncsRegd);
}

Interface::~Interface()
{
int loop;

for(loop = 0;loop < FuncsRegd;loop++) //Unload the functions
   delete Library[loop];
free(Library);

delete pm;
}

int Interface::Prompt()
{
int ch, rc, counter = 0, frc = 0;
unsigned char Done = 0;

_cprintf("%s", PromptString);

while(kbhit == 0) {};
do
   {
   ch = _getch();
   switch (ch)
      {
      case 13:
         Done = 1;
         break;
      case 8:
         if(counter != 0)
            {
            counter--;
            input[counter] = 0;
            _putch(8);
            _putch(32);
            _putch(8);
            }
         break;
      case '(':
         if(counter == 0) //determine where to autospace everything
            {
            _putch(ch);
            break;
            }
         if(input[counter - 1] == 32)
            {
            _putch(ch);
            input[counter] = ch;
            counter++;
            break;
            } //otherwise space it out properly
         _putch(32);
         input[counter] = 32;
         _putch(ch);
         input[counter + 1] = ch;
         counter = counter + 2;
         break;
      default:
         input[counter] = ((char)tolower(ch));
         counter++;
         _putch(ch);
         break;
      }
   }
while(!Done);
_cprintf("\n");
frc = ReadInput(); //Reads and parses the input for further subprocessing

switch(frc)
   {
   case -1:
      _cprintf("Unitegibble Command.\n");
      return 666;
   case -2:
      return 666;
   default:
      rc = Execute(Library[frc], pm, memberOf);
      if(rc == SML0001N) //General Syntax error
         _cprintf("%s\n", Library[frc]->syntax );
      if(rc != SML0000N)
         {
         memset(((void *)input), 0, 512);
         delete pm;
         pm = new Param;
         cprintf("%d ", rc);
         return 666;
         }
      return 0;
   }
}

int Interface::ReadInput()
{
int loop = 0, iloop = 0, tkfix = 0, out = 0;
char delim[2] = " "; //just a space
int retval = -1;

if(input[0] == 0) //just the enter key was pushed
   return -2;
Parameterizer(input, pm);
Lookup(pm->parms[0], &delim[0], &retval); //Find the function being done
return retval;  //Return the supposedly good stuff
}

int Interface::Lookup(char *inp, char *delim, int *retval)
{
int loop;

for(loop = 0; loop < FuncsRegd; loop++)
   {
   if((strcmp(inp, Library[loop]->keyword)) == 0) //Check the first keyword
      {
      *delim = Library[loop]->delim;
      *retval = Library[loop]->FuncID;
      return 1; //Yep there is a function found
      }
   }
return 0; //no function found
}

char *Interface::Parameterizer(char *inp, Param *pm)
{
char *buffer, *toke, *tp[128], *ts[128];
int paranc = 0, tkfix = 0, iloop = 0, loop = 0;

//Get the first token in.  It should be a valid keyword
toke = strtok(inp, " ");
tp[0] = (char *)malloc(sizeof(char) * StringSize(toke));
strcpy(tp[0], toke);
pm->parmc++;
buffer = NULL;

while((toke = strtok(buffer, " ")) != NULL)
   {
   if(toke[0] == '-')
      {
      loop = StringSize(toke);
      ts[pm->switchc] = (char *)malloc(sizeof(char)*loop);
      strcpy(ts[pm->switchc], toke);
      pm->switchc++;
      buffer = NULL;
      }
   else
      {
      if(toke[0] == '(') //Start of a recursion
         {
         //Go through until the ending ) can be found
         paranc = 1;
         tkfix = 0; //strtok puts NULL where the " " is.  Want to ignore that the first time
         iloop = 1;
         while(paranc > 0 && tkfix < 2)
            {
            if(toke[iloop] == '(')
               paranc++;
            if(toke[iloop] == ')')
               paranc--;
            if(toke[iloop] == 0)
               {
               tkfix++;
               if(tkfix == 2)
                  return NULL;
               toke[iloop] = ' ';
               }
            iloop++;
            }
         buffer = &toke[iloop + 1];
         tp[pm->parmc] = (char *)calloc(1, sizeof(char)*iloop + 1);
         strncpy(tp[pm->parmc], toke, iloop-1);
         toke[iloop] = 0;
         pm->parmc++;
         }
      else  //Its not a switch or a paranthese.  So just plug in the parameter
         {
         loop = StringSize(toke);
         tp[pm->parmc] = (char *)calloc(1, sizeof(char)*loop + 1);
         strcpy(tp[pm->parmc], toke);
         pm->parmc++;
         buffer = NULL;
         }
      }
   }
//Allocate the pointer arrays
pm->parms = (char **)malloc(sizeof(char *)* pm->parmc);
pm->switches  = (char **)malloc(sizeof(char *)* pm->switchc);

//Lock the words in
for(loop = 0; loop < pm->parmc; loop++)
   pm->parms[loop] = tp[loop];

for(loop = 0; loop < pm->switchc; loop++)
   pm->switches[loop] = ts[loop];
return tp[0];  //Let the calling function know the primary keyword
}

int Interface::Execute(Func *fPtr, Param *pm, World *earth)
{
int loop = 0, parmcTP = 0, switchcTP = 0, retval = 0, rc =0;
char delim;
char **parmsTP = NULL, **switchesTP = NULL;
Param *PM;

//Do the recursive functions
//printf("KeyWord: %s\n", pm->parms[0]);
for(loop = 0; loop < pm->parmc; loop++)
   {
   if(pm->parms[loop][0] == '(')
      {
      pm->parms[loop][0] = ' ';
      pm->parms[loop][StringSize(pm->parms[loop]) - 1] = 0;

      PM = new Param;
      Parameterizer(pm->parms[loop], PM);
      Lookup(PM->parms[0], &delim, &rc); //Find the function being done
      retval = Execute(Library[rc], PM, earth);
      delete PM;

      if(retval == -1)
         return -1;  //Something was off balanced
      }
   }
rc = fPtr->funcptr(pm, earth);
return rc; //Whatever Error/Success code
}

int Interface::RegisterFunc( int (*funcptr)(Param *pm, World *earth), char delim, char *syn, char *key)
{
char *temp;

Library[FuncsRegd] = new Func;
Library[FuncsRegd]->funcptr = funcptr;
Library[FuncsRegd]->out = NULL;
Library[FuncsRegd]->delim = delim;
Library[FuncsRegd]->FuncID = FuncsRegd;
temp = (char *)malloc(StringSize(syn));
strcpy(temp, syn);
Library[FuncsRegd]->syntax = temp;
strcpy(Library[FuncsRegd]->keyword, key);
return ++FuncsRegd;
}

/********************************************************************************************/
/*Main KeyWord Functions                                                                    */
/*                                                                                          */
/*                                                                                          */
/*                                                                                          */
/*                                                                                          */
/*                                                                                          */
/*                                                                                          */
/*                                                                                          */
/*                                                                                          */
/*                                                                                          */
/*                                                                                          */
/********************************************************************************************/

int Empty(Param *pm, World *earth)
{
return SML0000N; //the exit code
}

int GlobalVar(Param *pm, World *earth)
{
return -1;
}

int Load(Param *pm, World *earth)
{
if(pm->parmc > 2) //ld path (other stuff)
   return SML0001N;
earth->KillEveryone(); //Clear out the world
if(pm->parmc == 2)
   {
   earth->LoadData(pm->parms[1]);
   }
else
   {
   earth->LoadData("./"); //current directory
   }
return SML0001P;
}

Param::Param()
{
switchc = 0;
parmc = 0;
switches = NULL;
parms = NULL;
}

Param::~Param()
{
FreeCharPP(parms, parmc);
FreeCharPP(switches, switchc);
}

