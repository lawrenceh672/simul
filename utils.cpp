#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "utils.h"
#include "error.h"

/*
+-------------------+
|StopWatch functions|
+-------------------+
*/
double StopWatch::CurrTime()
{
double s,f;
f = (double)clock();
s = start;
return (f - s)/CLOCKS_PER_SEC;
}

void StopWatch::Start()
{
start = (double)clock();

SetFlagOn(0, &flags);
SetFlagOff(1, &flags);
}

double StopWatch::Stop()
{
double retval;
finish = (double)clock(); //Finish counting up the last stretch

SetFlagOff(0, &flags);
retval = ((finish - start)/CLOCKS_PER_SEC) + runtime;
Reset(); //Prepare to start again
return retval;
}

double StopWatch::Pause()
{
double ret;
finish = (double)clock(); //Finish this stretch

SetFlagOn(1, &flags);
ret = (finish - start)/CLOCKS_PER_SEC; //Calculate the time spent
runtime += ret; //Add it to the total
pauses++;
return runtime;
}

/*
+----------------+
|Helper Functions|
+----------------+
*/

int AllNumbers(char *inp)
{
int loop = 0;
unsigned char lookup[61] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0};

while(inp[loop] != 0)
   {
   if(!lookup[inp[loop]])
      return 0; //Fail on the first non number character
   loop++;
   }
return 1;
}


void StringBetween(char *inp, char stchar, char endchar, int *start, int *length)
{
int loop = 0;

while(inp[loop] != 0 && inp[loop] != stchar)
   loop++;

if(inp[loop] == 0)
   {
   *start = 0;
   *length = 0;
   return;
   }
else
   *start = loop;

while(inp[loop] != 0 && inp[loop] != endchar)
   loop++;

if(inp[loop] == 0)
   {
   *start = 0;
   *length = 0;
   return; //no use if theres no end character
   }
else
   *length = loop - *start;
}

void FreeCharPP(char **inp, int count)
{
int loop;
if(inp == NULL)
   return;

for(loop = 0; loop < count; loop++)
   {
   free(inp[loop]);
   }
free(inp);
}

int StringSize(char *string)
{
int loop = 0;
if(string == NULL)
   return 0;

while(string[loop] != 0)
   loop++;
return loop+1; //For the null char
}

int SetFlag(unsigned int index, unsigned char *arr, unsigned char inp) //compatibility and flexibility reasons
{
if(inp == 1)
   return SetFlagOn(index, arr);
else
   return SetFlagOff(index, arr);
}

int SetFlagOn(unsigned int index, unsigned char *arr)
{
unsigned char val, arri;
val = 1 << index % 8; //put a one in the correct position
arri = arr[index >> 3];

arr[index >> 3] = arri | val;
return 1;
}

int SetFlagOff(unsigned int index, unsigned char *arr)
{
unsigned char val, arri;
val = 255 - (1 << index % 8); //put a zero in the correct position
arri = arr[index >> 3];

arr[index >> 3] = arri & val;
return 1;
}

unsigned char GetFlag(int index, unsigned char *arr)
{
unsigned char tp;
tp = index % 8;
return (arr[index >> 3] & (1 << tp)) >> tp;
}

int strlenBytes(const char *string)
{
int loop = 0;
if(string == NULL)
   return 0;

return strlen(string)+1; //For the null char
}

char *GetString(const char *nme, char *dst)
{
_cprintf("%s", nme);
return gets(dst);
}

int GetInteger(const char *prompt)
{
char inp[33];

if(prompt != NULL)
   _cprintf("%s", prompt);
gets(inp);
if(!AllNumbers(inp))
   return GetInteger("Please Enter a number\n");
else
   return atoi(inp);
}

