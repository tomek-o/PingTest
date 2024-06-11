//---------------------------------------------------------------------------


#pragma hdrstop

#include "TestUtils.h"
#include <System.hpp>
#include <Forms.hpp>
#include <time.h>

//---------------------------------------------------------------------------

#pragma package(smart_init)

namespace
{

#define LOG_PROMPT "TestUtils"

long timediff(clock_t t1, clock_t t2) {
	long elapsed;
	elapsed = static_cast<long>(((double)t2 - t1) / CLOCKS_PER_SEC * 1000);
	return elapsed;
}

}


void SleepWithMsgProcessing(int miliseconds, volatile bool &breakRequest)
{
	if (miliseconds <= 0)
	{
		Application->ProcessMessages();
		return;
	}

	clock_t t1 = clock();
	long elapsed = 0;
	while (elapsed < miliseconds)
	{
		if (breakRequest)
		{
			break;
		}
		Application->ProcessMessages();
		Sleep(2);
		elapsed = timediff(t1, clock());
	}
}

