/** \file
*/
//---------------------------------------------------------------------------

#ifndef SettingsH
#define SettingsH
//---------------------------------------------------------------------------
#include <System.hpp>

class Settings
{
public:
	int Read(AnsiString asFileName);
	int Write(AnsiString asFileName);
	struct Gui
	{
		enum { SCALING_MIN = 50 };
		enum { SCALING_MAX = 200 };
		int scalingPct;					///< scaling (percentage)
		Gui(void):
			scalingPct(100)
		{}
	} gui;
	struct FrmMain
	{
		int posX, posY;				///< main window coordinates
		int height, width;			///< main window size
		bool windowMaximized;			///< is main window maximized?
		bool alwaysOnTop;
		FrmMain(void):
			width(600), height(400),
			posX(30), posY(30),
			windowMaximized(false),
			alwaysOnTop(false)
		{}
	} frmMain;
	struct Logging
	{
		bool logToFile;
		bool flush;
		enum {
			MIN_MAX_FILE_SIZE = 0,
			MAX_MAX_FILE_SIZE = 1000*1024*1024
		};
		enum {
			DEF_MAX_FILE_SIZE = 10*1024*1024
		};
		int maxFileSize;
		unsigned int maxUiLogLines;
		Logging(void):
			logToFile(false),
			flush(false),
			maxFileSize(DEF_MAX_FILE_SIZE),
			maxUiLogLines(5000)
		{}
	} logging;
};

extern Settings appSettings;

#endif
