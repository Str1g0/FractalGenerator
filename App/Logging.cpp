#include "Logging.h"


namespace Log
{
	struct
	{
		Logger& operator()()
		{
			static std::once_flag init_flag;
			std::call_once(init_flag, 
				[this]() 
				{
					Init();
				});

			return m_logger;
		}

	private:

		std::string GetTimeStamp()
		{
			std::time_t time = std::time(0);
			std::tm		now;

			localtime_s(&now, &time);

			std::stringstream formatter;
			
			formatter << now.tm_mday << '.' << now.tm_mon << '.' << now.tm_year + 1900;
			formatter << "_";
			formatter << now.tm_hour << ';' << now.tm_min << ';' << now.tm_sec;

			return formatter.str() + ".log";
		}

		void Init()
		{
			freopen_s(((FILE**)stdout), "CONOUT$", "w", stdout);
			freopen_s(((FILE**)stderr), "CONOUT$", "w", stderr);

			std::ios::sync_with_stdio(false);

			std::clog.rdbuf()->pubsetbuf(m_stdoutBuffer, Log::LOG_SIZE);
			std::cerr.rdbuf()->pubsetbuf(m_stderrBuffer, Log::LOG_SIZE);

			Log::LogFilter outFilter;
			outFilter.filterTags = false;
			outFilter.filterLevel = true;
			outFilter.level = LogLevel::LDEBUG | LogLevel::LINFO | LogLevel::LVERBOSE;

			Log::LogFilter errFilter;
			errFilter.filterTags = false;
			errFilter.filterLevel = true;
			errFilter.level = LogLevel::LWARNING | LogLevel::LERROR | LogLevel::LFATAL;

			m_logger.AddStream(&std::clog, "stdout", outFilter);
			m_logger.AddStream(&std::cerr, "stderr", errFilter);

			Log::LogFilter fileFilter;
			fileFilter.filterLevel = false;
			fileFilter.filterTags  = false;

			m_logger.CreateLogFile("logFile_" + GetTimeStamp(), fileFilter);
		}

		Logger m_logger;

		char m_stdoutBuffer[Log::LOG_SIZE];
		char m_stderrBuffer[Log::LOG_SIZE];
		char m_stdlogBuffer[Log::LOG_SIZE];

	} __globalLog;

	Logger& GetEngineLogger()
	{
		return __globalLog();
	}
}