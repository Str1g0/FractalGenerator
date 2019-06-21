#pragma once

#include <type_traits>
#include <map>
#include <filesystem>

#include <Util.h>
#include "Console.hpp"

	namespace Log
	{
		constexpr auto LOG_SIZE = 1024u;

		enum class LogLevel : 
			byte
		{
			LDEBUG		= ENUM(0),
			LINFO		= ENUM(1),
			LWARNING	= ENUM(2),
			LERROR		= ENUM(3),
			LFATAL		= ENUM(4),
			LVERBOSE	= ENUM(5),
			ALL			= LDEBUG | LINFO | LWARNING | LERROR | LFATAL | LVERBOSE
		};

		enum class FilterState : 
			byte
		{
			NONE,
			LOCAL,
			ATTACH,
			OVERWRITE
		};

		struct LogFilter;
		struct LogRecord;
		struct Message;

		struct LogFilter
		{
			bool filterTags;
			bool filterLevel;
			LogLevel					level;
			std::vector<std::string>	tags;

			bool MatchesMessage(Message msg);
		};

		enum class PrintFlags :
			byte
		{
			USE_TAG			= ENUM(0),
			USE_TIMESTAMP	= ENUM(1),
			BUFFER_STREAMS	= ENUM(2),
			USE_LEVEL		= ENUM(3),
			USE_COLOR		= ENUM(4),
			ALL				= USE_TAG | USE_TIMESTAMP | BUFFER_STREAMS | USE_LEVEL | USE_COLOR
		};

		struct LogRecord
		{
			bool			isTemp;
			bool			isFile;
			std::string		name;
			LogFilter		filter;
		};

		struct Message
		{
			LogLevel		level;
			std::tm			timestamp;
			std::string		tag;
			std::string		text;
		};

		class Logger;
		class LogBuffer;

		LogLevel inline operator |= (LogLevel & l1, LogLevel l2) {
			return l1 = static_cast<LogLevel>(static_cast<byte>(l1) | static_cast<byte>(l2));
		}

		LogLevel inline operator | (LogLevel l1, LogLevel l2) {
			return l1 |= l2;
		}

		LogLevel inline operator &= (LogLevel & l1, LogLevel l2) {
			return l1 = static_cast<LogLevel>(static_cast<byte>(l1) & static_cast<byte>(l2));
		}

		LogLevel inline operator & (LogLevel l1, LogLevel l2) {
			return l1 &= l2;
		}

		LogLevel inline operator ~ (LogLevel & l1) {
			return l1 = static_cast<LogLevel>(~static_cast<byte>(l1));
		}

		PrintFlags inline operator & (PrintFlags l1, PrintFlags l2) {
			return static_cast<PrintFlags>(static_cast<byte>(l1) & static_cast<byte>(l2));
		}

		class LogBuffer :
			public std::basic_streambuf<char, std::char_traits<char>>
		{
			protected:

				friend class Logger;

				typedef std::basic_streambuf<char> BaseBuff;
				typedef std::basic_ostream<char>   BaseStream;
				typedef std::char_traits<char>	   traits;
				
				struct OutputBuffer
				{
					inline void SetBuffer() {

						if (IsAlive()) {
							streamPtr->pubsetbuf(charBuffer, LOG_SIZE);
						}
					}
					
					char charBuffer[LOG_SIZE];

					BaseBuff*  streamPtr; 
					LogRecord  record;

					bool isActive	{true};
					bool isManaged	{false};

					bool inline IsAlive() {
						return streamPtr != nullptr;
					}
				};

				typedef typename std::char_traits<char>::int_type int_type;
			
				std::map<std::string, OutputBuffer> m_outputBuffers;
				uint16							    m_bufferCounter	{0u};

			public:

				explicit LogBuffer();
				explicit LogBuffer(OutputBuffer buff);
				LogBuffer(std::initializer_list<OutputBuffer> buffers);

				int_type overflow(int_type type) override;
				int_type sync()					 override;

		};

		class Logger :
			public std::basic_ostream<char, std::char_traits<char>>
		{
			protected:

				constexpr static const char* STR_DBG = "<DEBUG>:";
				constexpr static const char* STR_WRN = "<WARNING>:";
				constexpr static const char* STR_ERR = "<ERROR>:";
				constexpr static const char* STR_INF = "<INFO>:";
				constexpr static const char* STR_FTL = "<FATAL>:";
				constexpr static const char* STR_VBS = "<VERBOSE>:";
				constexpr static uint8		 SPACING = 1u;

				enum class LogErrCode : 
					uint16
				{
					NONE,
					NAME_CONFLICT = ENUM(0),
					NULL_STREAM	  = ENUM(1)
				};

				typedef std::basic_ostream<char>	  stream;
				typedef std::pair<stream*, LogRecord> LogStream;

				static constexpr const char * m_sDefaultBufferName = "OutputStream";

				LogFilter	m_globalFilter;
				FilterState	m_filterUsage	{ FilterState::LOCAL };
				PrintFlags  m_printFlags	{ PrintFlags::ALL };

				LogBuffer								m_buffer;
				std::string								m_logFolder{ "./logs" };
				std::map<std::string, std::ofstream>	m_logFiles;

				void FilterBuffers(Message msg);
				void FlushAll();

			public:

				Logger();
				Logger(std::initializer_list<stream*>    streams, std::string const & logFilePath = "./logs");
				Logger(std::initializer_list<LogStream>  streams, std::string const & logFilePath = "./logs");

				void AddStream(stream* strm, bool isFile = false);
				void AddStream(stream* strm, LogFilter filter, bool isFile = false);
				void AddStream(stream* strm, std::string const & name, bool isFile = false, bool isTemp = false);
				void AddStream(stream* strm, std::string const & name, LogFilter filter, bool isFile = false, bool isTemp = false);
				void AddStream(stream* strm, LogRecord record);

				bool CreateLogFile(std::string name, LogFilter filter, bool isPathAbsolute = false);
				
				void ClearAllManaged();
				void DeleteLogFiles(bool removeFileHandles = true);
				void RemoveAllDead();

				void RemoveTemp();
				void RemoveStream(const char * streamName);
				void RemoveAllStreams();

				template<typename ...args>
				static Message CreateMessage(LogLevel level, const char* tag, const char* fmt, args... vargs);

				void PrintMessage(Message msg);

				template<typename ...args>
				void PrintM(LogLevel level, const char* tag, const char* fmt, args... vargs);

				template<typename T, typename ...args>
				void Print(T current, args... vargs);

				template<typename T>
				void Print(T current);

				template<typename ...args>
				void Printf(const char * fmt, args... vargs);

				void SetFilterState(FilterState state);
				void ConfigureFilter(LogFilter filter);
				void SetLogFilePath(std::string newPath, bool removeOld = false);
				
				void AddFilterTag(const char * tag);
				void AddFilterLevel(LogLevel level);

				void RemoveFilter(const char * tag);
				void RemoveFilter(LogLevel level);

				void ModifyStreamFilter(const char * streamName, LogFilter filter);
				void ModifyStreamFilter(const char * streamName, const char * tag, bool remove = false);
				void ModifyStreamFilter(const char * streamName, LogLevel level,   bool remove = false);

				LogFilter	GetFilter()			const;
				std::string GetLogFilePath()	const;
		};

		template<typename ...args>
		inline Message Logger::CreateMessage(LogLevel level, const char * tag, const char * fmt, args ...vargs)
		{
			Message msg;
			msg.level	= level;
			msg.tag		= tag;

			char buffer[LOG_SIZE] = "";
			sprintf_s(buffer, LOG_SIZE, fmt, vargs...);

			msg.text = buffer;

			return msg;
		}

		template<typename ...args>
		inline void Logger::PrintM(LogLevel level, const char * tag, const char * fmt, args ...vargs)
		{
			return PrintMessage(CreateMessage(level, tag, fmt, vargs...));
		}

		template<typename T, typename ...args>
		inline void Logger::Print(T current, args ...vargs)
		{
			(*this) << current;
			return Print(vargs...);
		}

		template<typename T>
		inline void Logger::Print(T current)
		{
			(*this) << current << '\n';
			FlushAll();
		}

		template<typename ...args>
		inline void Logger::Printf(const char * fmt, args ...vargs)
		{
			char buffer[LOG_SIZE] = "";
			sprintf_s(buffer, LOG_SIZE, fmt, vargs...);
			
			write(buffer, std::strlen(buffer));
			FlushAll();
		}

}