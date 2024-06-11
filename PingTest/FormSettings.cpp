//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormSettings.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmSettings *frmSettings;
//---------------------------------------------------------------------------
__fastcall TfrmSettings::TfrmSettings(TComponent* Owner)
	: TForm(Owner)
{
	this->appSettings = NULL;
	pages->ActivePage = tsGeneral;
}
//---------------------------------------------------------------------------
void __fastcall TfrmSettings::FormShow(TObject *Sender)
{
    assert(appSettings);
	tmpSettings = *appSettings;
	chbAlwaysOnTop->Checked = tmpSettings.frmMain.alwaysOnTop;

	chbLogToFile->Checked = tmpSettings.logging.logToFile;
	cmbMaxUiLogLines->ItemIndex = -1;
	for (int i=0; i<cmbMaxUiLogLines->Items->Count; i++)
	{
		if ((unsigned int)StrToInt(cmbMaxUiLogLines->Items->Strings[i]) >= tmpSettings.logging.maxUiLogLines)
		{
			cmbMaxUiLogLines->ItemIndex = i;
			break;
		}
	}
	if (cmbMaxUiLogLines->ItemIndex == -1)
	{
		cmbMaxUiLogLines->ItemHeight = cmbMaxUiLogLines->Items->Count - 1;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmSettings::btnCancelClick(TObject *Sender)
{
	this->Close();	
}
//---------------------------------------------------------------------------
void __fastcall TfrmSettings::btnApplyClick(TObject *Sender)
{
	tmpSettings.logging.logToFile = chbLogToFile->Checked;

	*appSettings = tmpSettings;
	this->Close();	
}
//---------------------------------------------------------------------------

void __fastcall TfrmSettings::chbAlwaysOnTopClick(TObject *Sender)
{
	tmpSettings.frmMain.alwaysOnTop = chbAlwaysOnTop->Checked;	
}
//---------------------------------------------------------------------------

void __fastcall TfrmSettings::cmbMaxUiLogLinesChange(TObject *Sender)
{
	tmpSettings.logging.maxUiLogLines = StrToInt(cmbMaxUiLogLines->Text);	
}
//---------------------------------------------------------------------------

void __fastcall TfrmSettings::FormKeyPress(TObject *Sender, char &Key)
{
	if (Key == VK_ESCAPE)
	{
		Close();
	}
}
//---------------------------------------------------------------------------

