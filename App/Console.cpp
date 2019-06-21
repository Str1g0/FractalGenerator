#include "Console.hpp"

std::string Log::SetConsoleColor(std::string msg, LogColor color, bool isStdErr)
{
	#ifdef _WIN32
		WinConsoleColor winColor;
	#endif

	switch (color)
	{
		case LogColor::WHITE:
		{
			#ifdef _WIN32
				winColor = WinConsoleColor::WHITE;
			#elif __linux__
				msg = (CONSOLE_WHITE + msg);
			#endif
		}
		break;

		case LogColor::RED:
		{
			#ifdef _WIN32
				winColor = WinConsoleColor::RED;
			#elif __linux__
				msg = (CONSOLE_RED + msg);
			#endif
		}
		break;

		case LogColor::DARK_RED:
		{
			#ifdef _WIN32
				winColor = WinConsoleColor::D_RED;
			#elif __linux__
				msg = (CONSOLE_RED + msg);
			#endif
		}
		break;

		case LogColor::GREEN:
		{
			#ifdef _WIN32
				winColor = WinConsoleColor::GREEN;
			#elif __linux__
				msg = (CONSOLE_GREEN + msg);
			#endif
		}
		break;

		case LogColor::BLUE:
		{
			#ifdef _WIN32
				winColor = WinConsoleColor::BLUE;
			#elif __linux__
				msg = (CONSOLE_BLUE + msg);
			#endif
		}
		break;

		case LogColor::YELLOW:
		{
			#ifdef _WIN32
				winColor = WinConsoleColor::YELLOW;
			#elif __linux__
				msg = (CONSOLE_YELLOW + msg);
			#endif
		}
		break;

		case LogColor::CYAN:
		{
			#ifdef _WIN32
				winColor = WinConsoleColor::CYAN;
			#elif __linux__
				msg = (CONSOLE_CYAN + msg);
			#endif
		}
		break;

		case LogColor::PURPLE:
		{
			#ifdef _WIN32
				winColor = WinConsoleColor::PURPLE;
			#elif __linux__
				msg = (CONSOLE_PURPLE + msg);
			#endif
		}
		break;
	}

	#ifdef _WIN32
		HANDLE consoleHandle = GetStdHandle(!isStdErr ? STD_OUTPUT_HANDLE : STD_ERROR_HANDLE);
		SetConsoleTextAttribute(consoleHandle, (WORD)winColor);
	#endif

		return msg;
}
