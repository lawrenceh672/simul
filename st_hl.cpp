#include "simaster.h"
#include "error.h"
#include "st_hl.h"
#include <stdlib.h>
#include "utils.h"
#include <string.h>


/*
Stat Keyword Helper Functions



*/
int ParseObjectID(char *inp, int *out)
{
//#.#.#.# or (n)ation[NationID].(f)amily[FamID].(p)erson[PersonID].(g)ene[GeneID];
char *toke;
int temp = 0, loop = 1; //1 for the highest level below world(0)

toke = strtok(inp, "."); //Object Id's are always separated by .'s
if(toke == NULL)
   {
   out = NULL;
   return SML0001N; //Nothing here a general syntax error
   }
temp = VerifyObjectIDSegment(toke);
if(temp == -1)
   {
   out = NULL;
   return SML0002N; //These are merely an incorrectly specified ObjectID
   }
else
   out[loop] = temp;
loop++;

while( (toke = strtok(NULL, ".")) != NULL && loop < NUMCOMPONENTS)
   {
   temp = VerifyObjectIDSegment(toke);
   if(temp == -1)
      {
      out = NULL;
      return SML0002N;
      }
   else
      out[loop] = temp;
   loop++;
   }

if(toke != NULL)
   return SML0002P; //Ignoring excess object Identifiers

return SML0001P;
}


int VerifyObjectIDSegment(char *inp)
{
int start = 0, length = 0;
//n
//nation
//f
//family
//p
//person
//g
//gene
if(inp == NULL)
   return -2; //-2 is not bad syntax, just undefined
if(AllNumbers(inp))
   return atoi(inp); //If its just numbers pass it through automatically
if(inp[0] == '*' && inp[1] == 0) //Just the *
   return -3;
if(inp[0] == 'n')
   {
   if((inp[1] != '[') && (strstr(inp, "nation") == NULL))
      return -1; //Incorrect syntax
   else
      {
      StringBetween(inp, '[', ']', &start, &length);
      inp[start+length] = 0;
      if(AllNumbers(&inp[start+1]))
         return atoi(&inp[start+1]); //Start looking after the start character
      else
         {
         if(inp[start+1] == '*')
            return -3;  //-3 is hencforth the select everything character
         else
            return -1;
         }
      }
   }
if(inp[0] == 'f')
   {
   if((inp[1] != '[') && (strstr(inp, "family") == NULL))
      return -1; //Incorrect syntax
   else
      {
      StringBetween(inp, '[', ']', &start, &length);
      inp[start+length] = 0;
      if(AllNumbers(&inp[start+1]))
         return atoi(&inp[start+1]); //Start looking after the start character
      else
      	  {
      		 if(inp[start+1] == '*')
            return -3;  //-3 is hencforth the select everything character
         else
            return -1;
         }
      }
   }
if(inp[0] == 'p')
   {
   if((inp[1] != '[') && (strstr(inp, "person") == NULL))
      return -1; //Incorrect syntax
   else
      {
      StringBetween(inp, '[', ']', &start, &length);
      inp[start+length] = 0;
	  if(AllNumbers(&inp[start+1]))
	     return atoi(&inp[start+1]); //Start looking after the start character
	  else
	  {
		 if(inp[start+1] == '*')
		    return -3;  //-3 is hencforth the select everything character
		 else
			return -1;
	  }
      }
   }
if(inp[0] == 'g')
   {
   if((inp[1] != '[') && (strstr(inp, "gene") == NULL))
      return -1; //Incorrect syntax
   else
      {
      StringBetween(inp, '[', ']', &start, &length);
      inp[start+length] = 0;
	  if(AllNumbers(&inp[start+1]))
	     return atoi(&inp[start+1]); //Start looking after the start character
	  else
	  {
		 if(inp[start+1] == '*')
		    return -3;  //-3 is hencforth the select everything character
		 else
			return -1;
	  }
      }
   }
return -1; //if nothing can be matched, something is screwed
}

