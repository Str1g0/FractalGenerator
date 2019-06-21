#pragma once

#include <Util.h>


namespace Log
{
	enum class LogColor :
		byte
	{
		WHITE	,
		RED		,
		GREEN	,
		BLUE	,
		YELLOW	,
		CYAN	,
		PURPLE	,
		DARK_RED
	};

	#ifdef _WIN32
		enum class WinConsoleColor :
			byte
		{
			WHITE	= FOREGROUND_INTENSITY,
			RED		= WHITE | FOREGROUND_RED,
			GREEN	= WHITE | FOREGROUND_GREEN,
			BLUE	= WHITE | FOREGROUND_BLUE,
			CYAN	= WHITE | FOREGROUND_BLUE | FOREGROUND_GREEN,
			PURPLE	= WHITE | FOREGROUND_BLUE | FOREGROUND_RED,
			YELLOW  = FOREGROUND_RED | FOREGROUND_GREEN,
			D_RED	= FOREGROUND_RED,
			D_GREEN	= FOREGROUND_GREEN,
			D_BLUE	= FOREGROUND_BLUE,
		};


	#elif __linux__
		
		constexpr const char* CONSOLE_RED	  =  "\x1B[31m";
		constexpr const char* CONSOLE_GREEN	  =  "\x1B[32m";
		constexpr const char* CONSOLE_YELLOW  =  "\x1B[33m";
		constexpr const char* CONSOLE_BLUE	  =  "\x1B[34m";
		constexpr const char* CONSOLE_PURPLE  =  "\x1B[35m";
		constexpr const char* CONSOLE_CYAN	  =  "\x1B[36m";
		constexpr const char* CONSOLE_WHITE	  =  "\x1B[37m";

	#endif

		std::string SetConsoleColor(std::string msg, LogColor color, bool isStdErr = false);
}
