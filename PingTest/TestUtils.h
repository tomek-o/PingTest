//---------------------------------------------------------------------------

#ifndef TestUtilsH
#define TestUtilsH
//---------------------------------------------------------------------------


#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) ((sizeof(a))/(sizeof((a)[0])))
#endif

void SleepWithMsgProcessing(int miliseconds, volatile bool &breakRequest);


#endif
