//Error Code Section - Do not Change this line.  That means YOU!
#define SML0000P 0 //Command cancelled and aborted by user
#define SML0000N -9999 //Exit code
#define SML0001N -1 //Unitegibble Command.  Complete Syntax error.
#define SML0002N -2 //Syntax error in Object ID identifier.
#define SML0003N -3 //Invalid Object ID(But correct syntax)
#define SML0004N -4 //File open error
#define SML0005N -5 //Keyword as parameter(possible missing or off balanced paranthesis)
#define SML0006N -6 //Invalid directory
#define SML0001P 1 //A-Ok. No problems man.
#define SML0002P 2 //Excess Object Identifiers specified and excess ones ignored
#define SML0003P 3 //Invalid object type identified in the bu function

//MassStorage error code section.  Dont' change THIS line
#define MSS0003P 10003 //no more record changes in memory
#define MSS0002P 10002 //Last record change undone
#define MSS0001P 10001 //record type name was longer than the maximum size and was truncated
#define MSS0000P 10000 //Perfect, no warnings
#define MSS0000N -10000 //worked, but warnings generated
#define MSS0001N -10001 //Invalid File Format
#define MSS0002N -10002 //could not reopen file after save
#define MSS0003N -10003 //could not open file to save to
#define MSS0004N -10004 //Mismatch between rows and file size
