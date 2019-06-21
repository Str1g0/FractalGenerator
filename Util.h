#pragma once

#include "StdAfx.h"

typedef unsigned char		byte;
typedef	char				sbyte;

typedef __int8				int8;
typedef __int16				int16;
typedef __int32				int32;
typedef __int64				int64;

typedef unsigned __int8		uint8;
typedef unsigned __int16	uint16;
typedef unsigned __int32	uint32;
typedef unsigned __int64	uint64;

typedef const char*			cstring;

template<typename T>
using hash_map = typename std::unordered_map<std::string, T>;

#define STR(x) (std::to_string(x))

constexpr auto LOG_SIZE = 1024u;

FORCEINLINE uint32 constexpr ENUM(uint16 val)
{
	return 1 << val;
}

template<typename T>
T inline constexpr Square(T x)
{
	return x * x;
}

namespace Misc
{

	inline std::string ExtractFilename(std::string absPath)
	{
		uint32 beginIndex = absPath.size();
		for (; beginIndex != 0 && absPath[beginIndex] != '\\' && absPath[beginIndex] != '/'; --beginIndex);

		return absPath.substr(beginIndex + 1);
	}

	inline std::string GetFileContent(std::string src)
	{
		std::ofstream file(src);
		std::stringstream  convertor;

		convertor << file.rdbuf();
		return convertor.str();
	}

	struct Noncopyable
	{
		Noncopyable() = default;
		Noncopyable(Noncopyable const&) = delete;
		Noncopyable operator=(Noncopyable const&) = delete;
	};

	struct Immovable
	{
		Immovable() = default;
		Immovable(Immovable&&) = delete;
		Immovable operator=(Immovable const&) = delete;
	};

	struct Unique : virtual public Noncopyable, Immovable {};

	template<typename ...args>
	UINT inline ShowMessageBox(HWND hwnd, UINT type, const char* title, const char * fmt, args... vargs)
	{
		char buffer[LOG_SIZE] = "";
		sprintf_s(buffer, LOG_SIZE, fmt, vargs...);

		 UINT res = MessageBoxA(hwnd, buffer, title, type);

		 return res;
	}

	static void inline ToggleConsole()
	{
		static bool bIsConsoleVisible = false;

		bIsConsoleVisible ? FreeConsole() : AllocConsole();
		bIsConsoleVisible = !bIsConsoleVisible;
	}

}