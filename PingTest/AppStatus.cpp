//---------------------------------------------------------------------------


#pragma hdrstop

#include "AppStatus.h"
#include "FormMain.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

void SetAppStatus(AnsiString text)
{
	frmMain->StatusBar->Panels->Items[1]->Text = text;
}
