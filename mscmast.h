#include <stdio.h>
#include "simaster.h"
#include "utils.h"

#define RecFlagDSC 0 //record is on disk
#define RecFlagUPD 1 //record was changed
#define RecFlagDEL 2 //record was deleted
#define RecFlagMEM 3 //record is in memory
#define RecFlagADD 4 //record was added and not saved
#define RecFlagSOR 5 //records in memory have been sorted if set

class RecordData;
class ChangeInfo;