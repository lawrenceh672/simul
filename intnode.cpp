#include "intnode.h"
#include "simaster.h"
#include <stdlib.h>

unsigned char *IntNode::SelectFLG = NULL;

IntNode::IntNode(unsigned int chil, IntNode *par)
{
unsigned int loop = 0;

count = 0;
chilptr = NULL;
children = 0;
for(loop = 0;loop < chil;loop++)
   add(loop);
parent = par;
val = 0;
}

IntNode::~IntNode()
{
unsigned int loop;

if(chilptr != NULL)
   {
   for(loop = 0;loop < children;loop++)
      delete chilptr[loop];
   free(chilptr);
   }
free(SelectFLG);
}


IntNode *IntNode::add(unsigned int adde)
{
if(children < count)
   {
   chilptr[children] = new IntNode(0, this);
   chilptr[children]->val = adde;
   children++;
   }
else
   {
   chilptr = (IntNode **)realloc(chilptr, sizeof(IntNode *) * (children + 15));
   chilptr[children] = new IntNode(0, this);
   chilptr[children]->val = adde;
   count += 15;
   children++;
   }
return chilptr[children - 1];
}

void IntNode::set(unsigned int adde)
{
val = adde;
}

void IntNode::enumAll(int num)
{
int loop;

for(loop = 0;loop < num; loop++)
   chilptr[loop]->set(loop);
}

unsigned int IntNode::BytesUsed()
{
unsigned int loop, count = 0, retval = sizeof(IntNode);

if(chilptr == NULL)
   return retval;
else
   {
   for(loop = 0; loop < children;loop++)
      count += chilptr[loop]->BytesUsed();
   }
return count;
}

IntNode *IntNode::nextSib(int *path)
{
IntNode *tp;
unsigned int index;
if(parent == NULL) //its the world node
   {
   //set the path to it
   SetPath(path);
   return this; //It has no sibling, cept itself
   }

index = findIndex(this);
if(parent->children <= index + 1)
{
 tp = parent->nextSib(NULL); //get the next sib of the parent pointer to get its first child
 tp->chilptr[0]->SetPath(path);
 return tp->chilptr[0]; //Start at the beginning of the next array
}
else
   {
   parent->chilptr[index + 1]->SetPath(path);
   return parent->chilptr[index + 1]; //Easy!
   }
}

int IntNode::findIndex(IntNode *inp)
{
int ret = 0;
if(inp->parent != NULL) //That is, if its not the world node
   {
   while(inp->parent->chilptr[ret] != inp)
   ret++;
   }
else
   return 0; //Index of the world node, ie there is only one world
return ret;
}

int IntNode::TotalChildren()
{
//start from the root node, and do a breadth width search thru every node
IntNode *curr, *nlstart, *par;
int retval = 0;

curr = Root();             //get the root node
retval += curr->children; //get the root node's children counted
curr = curr->chilptr[0]; //Start on the level after the root node
par = curr; //prepare to start counting from the next level

while(curr->chilptr != NULL)
   {
   nlstart = curr->chilptr[0]; //set a path to the next level to start counting
   retval += curr->children;
   while((curr = curr->nextSib(NULL)) != par) //count every last one of its siblings
      {
      retval += curr->children; //add in it's siblings children as well
      }
   curr = nlstart;
   par = curr;
   }
return retval;
}

IntNode *IntNode::Root()
{
IntNode *curr;

curr = this;

while(curr->parent != NULL)
   curr = curr->parent;
return curr;
}

int IntNode::Level()
{
int level = 0;
IntNode *loc;

loc = this;

while(loc->parent != NULL)
   {
   loc = loc->parent;
   level++;
   }
return level;
}

int IntNode::TotalLevels()
{
IntNode *root, *chil;
int levels = 1;

root = Root();
chil = root;
while(chil->chilptr != NULL)
   {
   levels++;
   chil = chil->chilptr[0];
   }
return levels;
}

void IntNode::PrintTree(FILE *fp)
{
IntNode *tree, *par, *sib;
int levels, loop, iloop, chils, *lvlcnt = NULL;

tree = Root();
levels = TotalLevels();
chils = TotalChildren();
fprintf(fp, "The Root Node(%d) has %d children\n", tree->val, tree->children);
fprintf(fp, "Total Children %d\n", chils );
chils = 0;
for(loop = 0;loop < levels;loop++)
   {
   par = tree; //now add in stuff for all its siblings
   sib = par;
   lvlcnt = (int *)realloc(lvlcnt, (loop + 1) * sizeof(int));
   lvlcnt[loop] = 0;
   iloop = 0;
   do
      {
      if(sib->parent != NULL)
         fprintf(fp, "Node %d: Level %d. val = %d.  Child of Node %d: Level %d(%d)\n", iloop, loop, sib->val, sib->parent->val, loop - 1, sib->parent->val);
      chils += sib->children;
      lvlcnt[loop] += sib->children;
      iloop++;
      }
   while((sib = sib->nextSib(NULL) ) != par);
   if(sib->chilptr != NULL)
      tree = sib->chilptr[0]; //go to the next level
   else
      break;
   }
fprintf(fp, "Total Children %d\n", chils );
for(loop = 0;loop < levels;loop++)
   fprintf(fp, "Level %d has %d children.\n", loop, lvlcnt[loop]);
}

void IntNode::SetPath(int *path)
{
int lev, loop;
IntNode *nnode;
if(path == NULL)
   return;
lev = Level();

nnode = this;
for(loop = lev;loop > 0;loop--)
   {
   path[loop] = nnode->val;
   nnode = nnode->parent;
   }
path[loop] = nnode->val; //the root node
}

IntNode *IntNode::Find(int *path)
{
IntNode *root, *nnode;
unsigned int loop, iloop;

root = Root();
nnode = root;

while(path[loop] != NOTSELECTED)
   {
   while(iloop < children && nnode->chilptr[iloop]->val != path[loop])
      {
      iloop++;
      }
   if(iloop == children) //it wasn't found
      return NULL;
   else
      nnode = nnode->chilptr[iloop];
   loop++;
   }
return nnode;
}

int IntNode::Count()
{
int count = 0;
IntNode *par, *sib;

par = this; //now add in stuff for all its siblings
sib = par;

do
   {
   count++;
   }
while((sib = sib->nextSib(NULL) ) != par);
return count;
}

void IntNode::LineTree(int members)
{
int loop;
IntNode *curr;

curr = this;
for(loop = 0;loop < members;loop++)
   {
   curr->add(0);
   curr = curr->chilptr[0];
   }
}

int IntNode::Offset()
{
int offset = 0,level = 0, loop = 0, thislevel;
int path[NUMCOMPONENTS];
IntNode *oscalc, *orig;

//get the leftmost node on the parent's level
//get the next sib until its equal to this nodes parent
//add up all the children for each node

if(parent == NULL) //If this is the top of a tree, its children has an offset of zero
   return 0;
else
   {
   thislevel = Level();
   orig = parent;
   oscalc = orig;
   while(oscalc->parent != NULL)  //this gets the left most node on this level
      {
      oscalc = oscalc->parent;
      level++;
      }
   for(loop = 0;loop < level;loop++)
      {
      oscalc = oscalc->chilptr[0];
      }
   }
SetPath(path); //get the numbers tracing out this node
for(loop = 0;loop < path[thislevel];loop++) //get all its immediate sibling counted in first
   {
   offset += orig->chilptr[loop]->children;
   }
while(oscalc != orig && oscalc->chilptr != NULL)  //now count in the extended siblings in
   {
   offset += oscalc->children;
   oscalc = oscalc->nextSib(path);
   }
return offset;
}

void IntNode::SaveTree(FILE *fp)
{
NodeHeader info;
unsigned int loop;

info.children = children;
info.val = val;

fwrite(&info, sizeof(NodeHeader), 1, fp);

for(loop = 0;loop < children;loop++)
   chilptr[loop]->SaveTree(fp);
}

void IntNode::LoadTree(FILE *fp)
{
NodeHeader info;
int loop;

fread(&info, sizeof(NodeHeader), 1, fp);
val = info.val;
children = info.children;
chilptr = (IntNode **)malloc(info.children * sizeof(IntNode *));
count = (((info.children / 15) + 1) * 15);
for(loop = 0;loop < info.children;loop++)
   {
   chilptr[loop] = new IntNode(0, this);
   chilptr[loop]->LoadTree(fp);
   }
}

IntNode *IntNode::GetNode(int serial)
{
int loop, thisser, start, finish, lbnd, ubnd, mbnd;
IntNode *retv;

if((thisser = GetSerial()) == serial) //Serial number are awarded in a stack like fashion
   return this;

if(children == 0)
   return NULL;
else
   {
   lbnd = chilptr[0]->GetSerial();  //make sure its possible its in this range
   if(serial < lbnd)
      return NULL;
   ubnd = chilptr[children - 1]->GetSerial();
   if(serial > ubnd) //It has to be in the last node's tree, if anywhere
      return chilptr[children - 1]->GetNode(serial);
   else
      {
      mbnd = chilptr[children / 2]->GetSerial();
      if(serial < mbnd)
         {
         start = 0;
		 if(children / 2 > 1) finish = children / 2; else finish = 1;
         }
      else
         {
         if(serial > mbnd)
            {
            start = children / 2;
            finish = children;
            }
         else //this is it
            return chilptr[children / 2];
         }
      }
   for(loop = start;loop < finish;loop++)
      {
      if((retv = chilptr[loop]->GetNode(serial)) != NULL)
         return retv;
      }
   }
return NULL;
}

int IntNode::GetSerial()
{
int cnt = 0, ind = 0, loop = 0;

if(parent == NULL)  //The root node has a serial number of zero
   return 0;
while(parent->chilptr[ind] != this) //Get its index
   ind++;
for(loop = 0;loop < ind;loop++)
   cnt += parent->chilptr[loop]->Count_All_Children();
return parent->GetSerial() + ind + cnt + 1;
}

int IntNode::Count_All_Children()
{
unsigned int loop, cnt = 0;

cnt = children; //first its immediate children

for(loop = 0;loop < children;loop++)
   {
   cnt += chilptr[loop]->Count_All_Children(); //then its children children
   }
return cnt;
}

int IntNode::Count_Level(int lev)
{
unsigned int loop;
int count = 0, level;

level = Level();

if((lev - 1) == level) //the number of members in the level below is simply the members variable
   return children;
if(lev <= level || lev == NOTSELECTED) //trying to count an unselected level, or a greater or equal than level is nonsense
   return 0;

//otherwise get the count from everymember
for(loop = 0;loop < children;loop++)
   count += chilptr[loop]->Count_Level(lev);
return count;

}

void IntNode::Merge(IntNode *tree)
{
unsigned int loop;

for(loop = 0;loop < tree->children;loop++) //put its children onto the tree
   {

   IntNode *newn;

   newn = AddNode(tree->chilptr[loop]);
   newn->Merge(tree->chilptr[loop]);
   }
}

void IntNode::DeleteNode(unsigned int node)
{
if(node > children || node < 0 || children == 0)
   return;
delete chilptr[node];
chilptr[node] = NULL;

/* This is the conserve memory approach
children--;
count = (((children / 15) + 1) * 15);
chil = (IntNode **)malloc(count * sizeof(IntNode *));
//Move the memory around
memcpy(chil, chilptr, node * sizeof(IntNode *));
if(node < children - 1) //it wasn't the end node
   memcpy(&chil[node], &chilptr[node + 1], children - 1 - node);
free(chilptr);
cchilptr = chil;
*/
}

void IntNode::DeleteAllChildren()
{
unsigned int loop;

for(loop = 0;loop < children;loop++)
   delete chilptr[loop];
free(chilptr);
chilptr = NULL;
children = 0;
count = 0;
}

int IntNode::GetIndex()
{
unsigned int loop;

if(parent == NULL)
   return 0;

for(loop = 0;loop < parent->children;loop++)
   {
   if(parent->chilptr[loop] == this)
      return loop;
   }
return -1;
}

void IntNode::Squabbling_Children()
{
unsigned int loop;
IntNode *par;

for(loop = 0;loop < children;loop++)
   {
   par = new IntNode(0, NULL);
   par->Incorporate(chilptr[loop]);
   }
}

IntNode *IntNode::Incorporate(IntNode *chil)
{
int loop = 0;

//put chil into the this's children array
if(children < count)
   {
   chilptr[children] = chil->chilptr[loop];
   children++;
   }
else
   {
   chilptr = (IntNode **)realloc(chilptr, sizeof(IntNode *) * (children + 15));
   chilptr[children] = chil;
   count += 15;
   children++;
   }
chil->parent = this;
return chilptr[children - 1];
}

IntNode *IntNode::AddNode(IntNode *nde)
{
IntNode *nnde;
nnde = new IntNode(0, this);
nnde->val = nde->val; //the only info that needs to be copied over

if(children < count)
   {
   chilptr[children] = nnde;
   children++;
   }
else
   {
   chilptr = (IntNode **)realloc(chilptr, sizeof(IntNode *) * (children + 15));
   chilptr[children] = nnde;
   count += 15;
   children++;
   }
return nnde;
}

void IntNode::SetFlagArrays()
{
IntNode *root;
int nds;

free(SelectFLG);
root = Root();
nds = root->Count_All_Children();

SelectFLG = (unsigned char *)calloc(1, (nds >> 3) + 1);
}
