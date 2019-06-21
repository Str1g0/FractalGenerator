#pragma once

#include <App\Log.hpp>

#define LOG_DBG(tag, fmt, ...)	Log::Print(LogLevel::LDEBUG,	  tag, fmt, __VA_ARGS__)
#define LOG_INFO(tag, fmt, ...)	Log::Print(LogLevel::LINFO,	  tag, fmt, __VA_ARGS__)
#define LOG_WARN(tag, fmt, ...)	Log::Print(LogLevel::LWARNING,  tag, fmt, __VA_ARGS__)
#define LOG_ERR(tag, fmt, ...)	Log::Print(LogLevel::LERROR,	  tag, fmt, __VA_ARGS__)
#define LOG_FTL(tag, fmt, ...)	Log::Print(LogLevel::LFATAL,	  tag, fmt, __VA_ARGS__)
#define LOG_VRB(tag, fmt, ...)	Log::Print(LogLevel::LVERBOSE,  tag, fmt, __VA_ARGS__)

//for convenience
typedef Log::LogLevel LogLevel;

#define ASSERT(exp, fmt, ...)																			\
	if(!exp)																							\
	{																									\
		Log::Print(LogLevel::LFATAL, "ASSERT","LINE: %u, FUNCTION: %s, FILE: %s",						\
									__LINE__, __FUNCTION__, Misc::ExtractFilename(__FILE__).c_str());	\
		Log::Print(LogLevel::LFATAL, "ASSERT", fmt, __VA_ARGS__);										\
		std::abort();																					\
	}	

	namespace Log
	{
		Logger& GetEngineLogger();

		static std::mutex g_print_mutex;

		template<typename...args>
		void Print(LogLevel level, const char* tag, const char* fmt, args... vargs);

		template<typename...args>
		void Print(LogLevel level, const char* tag, std::string fmt, args... vargs);
		
		template<typename ...args>
		void Print(LogLevel level, const char * tag, const char * fmt, args ...vargs)
		{
			std::lock_guard<std::mutex> guard{ g_print_mutex };
			GetEngineLogger().PrintM(level, tag, fmt, vargs...);
		}

		template<typename ...args>
		void Print(LogLevel level, const char * tag, std::string fmt, args ...vargs)
		{
			std::lock_guard<std::mutex> guard{ g_print_mutex };
			GetEngineLogger().PrintM(level, tag, fmt.c_str(), vargs...);
		}
	}

