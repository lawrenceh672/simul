#include "msc.h"
#include <ctype.h>
#include <conio.h>
#include <stdlib.h>

int Title(MassStorage *ms);
int RTMenu(MassStorage *ms);
int RMMenu(MassStorage *ms);
int UNMenu(MassStorage *ms);

main(int argc, char *argv[])
{
char fname[512];
int done = 0, choice;
MassStorage *ms;
if(argc == 2)
   ms = new MassStorage(argv[1]);
else
   ms = new MassStorage("msc.dat");

do
   {
   choice = Title(ms);
   switch(choice)
      {
      case 'a':
         do
            {
            choice = RTMenu(ms);
            } while(choice != 666);
         break;
      case 'b':
         do
            {
            choice = RMMenu(ms);
            } while(choice != 666);
         break;
      case 'c':
         do
            {
            choice = UNMenu(ms);
            } while(choice != 666);
         break;
      case 'd':
         GetString("Enter File name\n", fname);
         if(fname[0] == 0)
            {
            printf("Okay, nevermind\n");
            }
         else
            {
            delete ms;
            ms = new MassStorage(fname);
            }
         break;
      case 'z':
         done = 1;
         break;
      }
   } while(!done);
delete ms;
}

int Title(MassStorage *ms)
{
int choice;

printf("\nA. Record type menu\n");
printf("B. Record manipulation menu\n");
printf("C. Undo System\n");
printf("D. Load another File\n");
printf("Z. Quit\n");
choice = _getch();
choice = tolower(choice);
return choice;
}

int RTMenu(MassStorage *ms)
{
int choice;
MassStorage_type *mst, *tmst;
char inp[512], buffer[33];
int iloop, loop, randrec, randcol;

inp[0] = 0;
printf("\nA. Add record\n");
printf("B. Add a randomly generated record\n");
printf("C. View all record types\n");
printf("D. View specfic record type\n");
printf("E. Show stats\n");
printf("Z. Leave this menu\n");

choice = _getche();

choice = tolower(choice);
switch(choice)
   {
   case 'a':
      GetString("Enter record type name\n", inp);
      mst = new MassStorage_type();
      mst->SetName(inp);
      randcol = GetInteger("Enter Columns\n");
      for(loop = 0;loop < randcol;loop++)
         mst->AddColumn(GetInteger("Enter Column length in bytes\n"));
      ms->AddRecordType(mst);
      break;
   case 'b':
      randrec = getrandom(1, 3);
      for(loop = 0;loop < randrec;loop++)
         {
         strcat(strcat(inp, "Random Record "), _itoa(loop, buffer, 10));
         mst = new MassStorage_type();
         mst->SetName(inp);
         randcol = getrandom(1, 5);
         for(iloop = 0;iloop < randcol;iloop++)
            {
            mst->AddColumn(getrandom(1, 50));
            }
         ms->AddRecordType(mst);
         }
      break;
   case 'c':
      for(loop = 0;loop < ms->rectypes;loop++)
         {
         tmst = &ms->typeindex;
         printf("Record %d(%d columns - %d record length):\n", loop, tmst->cols, tmst->hi.RecordSize);
         for(iloop = 0;iloop < tmst->cols;iloop++)
            printf("Column %d size: %d\n",iloop, tmst->colsize[iloop]);
         }
      break;
   case 'e':
      printf("Bytes Used %d\n", ms->BytesUsed());
      ms->PrintStats();
      break;
   case 'z':
      return 666;
   }
}

int RMMenu(MassStorage *ms)
{
int choice, rows, adds, upds, dels, maxops;
MassStorage_type *mst, *tmst;
int loop, iloop, rlp;
static int rowsd = 0;
char *tp, inp[512];
RecordData *rd;
StopWatch s1; //For timing purposesr

printf("\nA. Show Record types\n");
printf("B. Add Random data to record\n");
printf("C. Delete a record\n");
printf("D. Update a record\n");
printf("E. View data\n");
printf("F. Force save\n");
printf("G. Save as\n");
printf("H. Do random actions\n");
printf("Z. Leave this menu\n");
choice = _getch();
choice = tolower(choice);
_cprintf("%c\n", choice);
switch(choice)
   {
   case 'a':
      for(loop = 0;loop < ms->rectypes;loop++)
         {
         tmst = &ms->typeindex;
         printf("Record %d(%d columns - %d record length):\n", loop, tmst->cols, tmst->hi.RecordSize);
         for(iloop = 0;iloop < tmst->cols;iloop++)
            printf("Column %d size: %d\n",iloop, tmst->colsize[iloop]);
         }
      break;
   case 'b':
      rows = GetInteger("Enter rows\n");
      mst = &ms->typeindex;
      tp = (char *)malloc(mst->hi.RecordSize);
      for(rlp = 0;rlp < rows;rlp++)
         {
         for(loop = 0;loop < mst->hi.RecordSize;loop++)
            {
            tp[loop] = getrandom(49, 59);
            }
         ms->AddRecord((void *)tp);
         }
      free(tp);
      break;
   case 'c':
      choice = GetInteger("Enter Row\n");
      rowsd = ms->DeleteRecord(choice);
      break;
   case 'd':
      rlp = GetInteger("Enter Row\n");
      printf("Enter Data\n");
      mst = &ms->typeindex;;
      tp = (char *)calloc(1, mst->hi.RecordSize);
      if(_kbhit())
         _getch();
      for(loop = 0;loop < mst->hi.RecordSize;loop++)
         {
         choice = _getche();
         if(choice == 13)
            break;
         else
            tp[loop] = choice;
         }
      ms->UpdateRecord(rlp, (void *)tp);
      free(tp);
      break;
   case 'e':
      mst = &ms->typeindex;
      rows = ms->RowsLoaded + ms->RowsAdded;
      tp = (char *)malloc(mst->hi.RecordSize);
      loop = 0;
      for(loop = 0;loop < rows;loop++)
         {
         rd = ms->GetRecord(loop);
         if(rd->IsDeleted())
            printf("Row %d has been deleted.\n", loop);
         else
            {
            printf("Row %d: ", loop);
            for(rlp = 0;rlp < mst->hi.RecordSize;rlp++)
               printf("%c", ((char *)rd->data)[rlp]);
            printf("\n");
            }
         }
      break;
   case 'f':
      ms->Save();
      break;
   case 'g':
      ms->SaveAs(GetString("Enter File name\n", inp));
      break;
   case 'h':
      //Get all the variables set up
      adds = GetInteger("Number of adds\n");
      upds = GetInteger("Number of updates\n");
      dels = GetInteger("Number of deletes\n");
      rows = ms->RowsLoaded + ms->RowsAdded;
      maxops = __max(__max(adds,dels),upds);
      mst = &ms->typeindex;
      tp = (char *)calloc(1, mst->hi.RecordSize);
      s1.Start();
      for(loop = 0;loop < maxops;loop++)
         {
         if(loop < adds)
            {
            for(iloop = 0;iloop < mst->hi.RecordSize;iloop++)
               {
               tp[iloop] = getrandom(49, 59);
               }
            ms->AddRecord((void *)tp);
            }
         if(loop < dels)
            {
            rowsd = ms->DeleteRecord(getrandom(0,rows));
            }
         if(loop < upds)
            {
            strcpy(tp, "u");
            strcat(tp, _itoa(loop,inp,10));
            ms->UpdateRecord(getrandom(0,rows),(void *)tp);
            }
         }
      free(tp);
      printf("%f seconds\n", s1.Stop());
      break;
   case 'z':
      return 666;
   }
}

int UNMenu(MassStorage *ms)
{
int choice, loop, rec, rlp, rows, rowloop;
ChangeInfo *ci;
RecordData *rd;
MassStorage_type *mst;
char *tp;

mst = &ms->typeindex;
printf("\nA. List record history\n");
printf("B. List all records history\n");
printf("C. List all records changed\n");
printf("D. Undo last change for a record\n");
printf("E. Undo last change for all records\n");
printf("F. Clear undo system\n");
printf("G. Clear undo for a record\n");
printf("H. View Data\n");
printf("Z. Leave this menu\n");
choice = _getch();
choice = tolower(choice);
_cprintf("%c\n", choice);
switch(choice)
   {
   case 'a':
      rec = GetInteger("Enter record to view\n");
      ci = ms->GetRecordHistory(rec);
      if(ci == NULL)
         printf("Record %d has not been changed\n", rec);
      else
         {
         printf("Record %d has been changed %d times.\n", rec, ci->changes);
         for(loop = 0;loop < ci->changes;loop++)
            {
            printf("Change %d: ", loop);
            rd = ci->GetChange(loop);
            if(rd->IsDeleted())
               printf("row was deleted.", loop);
            else
               {
               if(rd->IsAdded())
                  printf("Row was added with ");
               else
                  printf("Row was updated with ");
               for(rlp = 0;rlp < mst->hi.RecordSize;rlp++)
                  printf("%c", ((char *)rd->data)[rlp]);
               }
            printf("\n");
            }
         }
      break;
   case 'b':
      for(rowloop = 0;rowloop < ms->RowsAdded + ms->RowsLoaded;rowloop++)
         {
         ci = ms->GetRecordHistory(rowloop);
         if(ci == NULL)
            printf("Record %d has not been changed\n", rowloop);
         else
            {
            printf("Record %d has been changed %d times.\n", rowloop, ci->changes);
            for(loop = 0;loop < ci->changes;loop++)
               {
               printf("Change %d: ", loop);
               rd = ci->GetChange(loop);
               if(rd->IsDeleted())
                  printf(" row %d was deleted.", loop);
               else
                  {
                  if(rd->IsUpdated())
                     printf(" row %d was updated to ", loop);
                  else
                     {
                     if(rd->IsAdded())
                        printf(" row %d was added with ", loop);
                     }
                  for(rlp = 0;rlp < mst->hi.RecordSize;rlp++)
                     printf("%c", ((char *)rd->data)[rlp]);
                  }
               printf("\n");
               }
            }
         }
      break;
   case 'c':
      for(rowloop = 0;rowloop < ms->RowsAdded + ms->RowsLoaded;rowloop++)
         {
         ci = ms->GetRecordHistory(rowloop);
         if(ci != NULL)
            printf("Row %d(%d changes)\n", ci->row, ci->changes);
         }
      break;
   case 'd':
      rec = GetInteger("Enter record to undo\n");
      ms->UndoLastChange(rec);
      break;
   case 'e':
      for(loop = 0;loop < ms->RowsAdded + ms->RowsLoaded;loop++)
         {
         ms->UndoLastChange(0);
         }
      break;
   case 'f':
      ms->ClearAllUndoStacks();
      break;
   case 'g':
      rec = GetInteger("Enter record to clear\n");
      ms->ClearUndoStack(rec);
      break;
   case 'h':
      rows = ms->RowsLoaded + ms->RowsAdded;
      tp = (char *)malloc(mst->hi.RecordSize);
      loop = 0;
      for(loop = 0;loop < rows;loop++)
         {
         rd = ms->GetRecord(loop);
         if(rd->IsDeleted())
            printf("Row %d has been deleted.\n", loop);
         else
            {
            printf("Row %d: ", loop);
            for(rlp = 0;rlp < mst->hi.RecordSize;rlp++)
               printf("%c", ((char *)rd->data)[rlp]);
            printf("\n");
            }
         }
      break;
   case 'z':
      return 666;
      break;
   }
}
