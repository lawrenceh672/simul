#include <malloc.h>
#include <stdlib.h>
#include <time.h>

extern int StringSize(char *string); //Size in bytes of string(including null termination)
extern void StringBetween(char *inp, char stchar, char endchar, int *start, int *length);  //Gets the start and length of chars between two chars
extern int AllNumbers(char *inp); //0 if has a non numeric character, 1 otherwise
extern void FreeCharPP(char **inp, int count); //Frees an array of pointers allocated with malloc
extern int SetFlag(unsigned int index, unsigned char *arr, unsigned char inp);
extern int SetFlagOff(unsigned int index, unsigned char *arr);
extern int SetFlagOn(unsigned int index, unsigned char *arr);
extern unsigned char GetFlag(int index, unsigned char *arr);
extern int strlenBytes(const char *inp);
extern char *GetString(const char *nme, char *dst);
extern int GetInteger(const char *prompt);

#ifndef STOPWATCH
#define STOPWATCH
class StopWatch
{
public:
StopWatch() {start = 0; finish = 0; runtime = 0; pauses = 0; SetFlagOff(0, &flags); SetFlagOff(1, &flags); };
void Start();
double Stop();
double Pause();
int Pauses() {return pauses;};
double CurrTime();
int Started() {return GetFlag(0, &flags); };
int Paused() {return GetFlag(1, &flags); };
void Reset() {start = 0; finish = 0; runtime = 0; pauses = 0; flags = 0;};

private:
double start;
double finish;
double runtime;
int pauses;
unsigned char flags;
/*
0 - Started 0/1 - n/y
1 - Paused  0/1 - n/y
*/
};
#endif
