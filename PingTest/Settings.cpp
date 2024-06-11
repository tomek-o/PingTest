//---------------------------------------------------------------------------


#pragma hdrstop

#include "Settings.h"
#include <algorithm>
#include <fstream>
#include <json/json.h>

//---------------------------------------------------------------------------

#pragma package(smart_init)

Settings appSettings;

inline void strncpyz(char* dst, const char* src, int dstsize) {
	strncpy(dst, src, dstsize);
	dst[dstsize-1] = '\0';
}

int Settings::Read(AnsiString asFileName)
{
	Json::Value root;   // will contains the root value after parsing.
	Json::Reader reader;

	try
	{
		std::ifstream ifs(asFileName.c_str());
		std::string strConfig((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
		ifs.close();
		bool parsingSuccessful = reader.parse( strConfig, root );
		if ( !parsingSuccessful )
		{
			return 2;
		}
	}
	catch(...)
	{
		return 1;
	}

	{
		int maxX = GetSystemMetrics(SM_CXSCREEN);
		/** \todo Ugly fixed taskbar margin */
		int maxY = GetSystemMetrics(SM_CYSCREEN) - 32;

		const Json::Value &jv = root["frmMain"];
		jv.getIntInRange("width", frmMain.width, 250, maxX + 20);
		jv.getIntInRange("height", frmMain.height, 200, maxY + 20);
		jv.getIntInRange("positionX", frmMain.posX, 0, maxX - 100);
		jv.getIntInRange("positionY", frmMain.posY, 0, maxY - 100);
		jv.getBool("maximized", frmMain.windowMaximized);
		jv.get("alwaysOnTop", frmMain.alwaysOnTop);
	}

	{
		const Json::Value &jv = root["logging"];
		jv.getBool("logToFile", logging.logToFile);
		jv.getBool("flush", logging.flush);
		jv.getIntInRange("maxFileSize", logging.maxFileSize, Logging::MIN_MAX_FILE_SIZE, Logging::MAX_MAX_FILE_SIZE);
		jv.getUInt("maxUiLogLines", logging.maxUiLogLines);
	}

	return 0;
}

int Settings::Write(AnsiString asFileName)
{
	Json::Value root;
	Json::StyledWriter writer;

	{
		Json::Value &jv = root["frmMain"];
		jv["width"] = frmMain.width;
		jv["height"] = frmMain.height;
		jv["positionX"] = frmMain.posX;
		jv["positionY"] = frmMain.posY;
		jv["maximized"] = frmMain.windowMaximized;
		jv["alwaysOnTop"] = frmMain.alwaysOnTop;
	}

	{
		Json::Value &jv = root["logging"];
		jv["logToFile"] = logging.logToFile;
		jv["flush"] = logging.flush;
		jv["maxFileSize"] = logging.maxFileSize;
		jv["maxUiLogLines"] = logging.maxUiLogLines;
	}


	std::string outputConfig = writer.write( root );

	try
	{
		std::ofstream ofs(asFileName.c_str());
		ofs << outputConfig;
		ofs.close();
	}
	catch(...)
	{
    	return 1;
	}

	return 0;
}


