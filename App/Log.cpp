#include "Log.hpp"

Log::LogBuffer::LogBuffer():
	std::basic_streambuf<char, std::char_traits<char>>()
{
}

Log::LogBuffer::LogBuffer(OutputBuffer buff):
	std::basic_streambuf<char, std::char_traits<char>>()
{
	if (buff.IsAlive()) {
		m_outputBuffers.insert({buff.record.name, buff});
	}
}

Log::LogBuffer::LogBuffer(std::initializer_list<OutputBuffer> buffers):
	std::basic_streambuf<char, std::char_traits<char>>()
{
	for (auto buff : buffers)
	{
		if (buff.IsAlive()) 
		{
			m_outputBuffers.insert({ buff.record.name, buff });
		}
	}
}

Log::LogBuffer::int_type Log::LogBuffer::overflow(int_type type)
{
	int_type const eof = traits::eof();
	bool streamReachedEOF = false;

	if (traits::eq_int_type(type, eof))
		return traits::not_eof(type);

	for (auto buffer : m_outputBuffers)
	{
		if (!buffer.second.isActive) 
			continue;

		int_type result = buffer.second.streamPtr->sputc(type);

		if (result == eof)
			streamReachedEOF = true;
	}

	return streamReachedEOF ? eof : type;
}

Log::LogBuffer::int_type Log::LogBuffer::sync()
{
	constexpr int_type SYNC_OK	=  1;
	constexpr int_type SYNC_ERR	= -1;

	int_type syncStatus = SYNC_OK;

	for (auto buffer : m_outputBuffers)
	{
		if (!buffer.second.isActive)
			continue;

		int_type res = buffer.second.streamPtr->pubsync();
		
		if (!res) 
			syncStatus = SYNC_ERR;
	}

	return syncStatus;
}

void Log::Logger::FilterBuffers(Message msg)
{
	switch (m_filterUsage)
	{
		case FilterState::LOCAL	:
		{
			for (auto & subBuffer : m_buffer.m_outputBuffers)
			{
				subBuffer.second.isActive = 
					subBuffer.second.record.filter.MatchesMessage(msg);
			}
			return;
		}

		case FilterState::ATTACH :
		{
			for (auto & subBuffer : m_buffer.m_outputBuffers)
			{
				subBuffer.second.isActive =
					subBuffer.second.record.filter.MatchesMessage(msg) &&
					m_globalFilter.MatchesMessage(msg);
			}
			return;
		}

		case FilterState::OVERWRITE :
		{
			for (auto & subBuffer : m_buffer.m_outputBuffers)
			{
				subBuffer.second.isActive = m_globalFilter.MatchesMessage(msg);
			}
			return;
		}

		case FilterState::NONE :
		default :
		{
			for (auto & subBuffer : m_buffer.m_outputBuffers)
			{
				subBuffer.second.isActive = true;
			}
			return;
		}
	}
}

void Log::Logger::FlushAll()
{
	for (auto buff : m_buffer.m_outputBuffers)
		if(buff.second.isActive)
			buff.second.streamPtr->pubsync();
}

Log::Logger::Logger():
	std::basic_ostream<char, std::char_traits<char>>(&m_buffer)
{ 
	namespace filesystem = std::experimental::filesystem;

	if(!filesystem::exists(m_logFolder))
		filesystem::create_directory(m_logFolder);
}

Log::Logger::Logger(std::initializer_list<stream*> streams, std::string const & logFilePath):
	std::basic_ostream<char, std::char_traits<char>>(&m_buffer),
	m_logFolder(logFilePath)
{
	namespace filesystem = std::experimental::filesystem;

	if (!filesystem::exists(m_logFolder))
		filesystem::create_directory(m_logFolder);

	for (auto & outStream : streams)
	{
		LogBuffer::OutputBuffer currentBuffer{ 0 };

		LogRecord buffRecord{ 0 };

		buffRecord.name = m_sDefaultBufferName + std::to_string(m_buffer.m_bufferCounter++);
		buffRecord.isFile = false;
		buffRecord.isTemp = false;
		buffRecord.filter = { 0 };

		buffRecord.filter.filterLevel = false;
		buffRecord.filter.filterTags  = false;

		currentBuffer.streamPtr = outStream->rdbuf();
		currentBuffer.record	= buffRecord;

		m_buffer.m_outputBuffers.insert({buffRecord.name, currentBuffer});
	}
}

Log::Logger::Logger(std::initializer_list<LogStream> streams, std::string const & logFilePath) :
	std::basic_ostream<char, std::char_traits<char>>(&m_buffer),
	m_logFolder(logFilePath)
{
	namespace filesystem = std::experimental::filesystem;

	if (!filesystem::exists(m_logFolder))
		filesystem::create_directory(m_logFolder);

	auto & buffers = m_buffer.m_outputBuffers;

	for (auto & outStream : streams)
	{
		LogBuffer::OutputBuffer currentBuffer{ 0 };
		currentBuffer.streamPtr = outStream.first->rdbuf();
		currentBuffer.record	= outStream.second;

		if (!currentBuffer.IsAlive())
			continue;

		if (buffers.find(outStream.second.name) != buffers.end())
			currentBuffer.record.name = m_sDefaultBufferName + std::to_string(m_buffer.m_bufferCounter++);

		m_buffer.m_outputBuffers.insert({ currentBuffer.record.name, currentBuffer });
	}
}

void Log::Logger::AddStream(stream * strm, std::string const & name, bool isFile, bool isTemp)
{
	LogRecord record{ 0 };
	LogFilter filter{ 0 };

	filter.filterLevel	= false;
	filter.filterTags	= false;

	record.isFile	= isFile;
	record.isTemp	= isTemp;
	record.filter	= filter;

	record.name		= m_buffer.m_outputBuffers.find(name) == m_buffer.m_outputBuffers.end() ?
					  name : m_sDefaultBufferName + std::to_string(m_buffer.m_bufferCounter++);

	LogBuffer::OutputBuffer outBuff;
	outBuff.streamPtr	= strm->rdbuf();
	outBuff.record		= record;

	m_buffer.m_outputBuffers.insert({ record.name, outBuff });
}

void Log::Logger::AddStream(stream * strm, std::string const & name, LogFilter filter, bool isFile, bool isTemp)
{
	LogRecord record{ 0 };

	record.isFile = isFile;
	record.isTemp = isTemp;
	record.filter = filter;
	record.name = m_buffer.m_outputBuffers.find(name) == m_buffer.m_outputBuffers.end() ?
					name : m_sDefaultBufferName + std::to_string(m_buffer.m_bufferCounter++);

	LogBuffer::OutputBuffer outBuff;

	outBuff.streamPtr = strm->rdbuf();
	outBuff.record = record;

	m_buffer.m_outputBuffers.insert({ record.name, outBuff });
}

void Log::Logger::AddStream(stream * strm, LogRecord record)
{
	LogBuffer::OutputBuffer outBuff;

	if (m_buffer.m_outputBuffers.find(record.name) != m_buffer.m_outputBuffers.end())
		record.name = m_sDefaultBufferName + std::to_string(m_buffer.m_bufferCounter++);

	outBuff.streamPtr	= strm->rdbuf();
	outBuff.record		= record;

	m_buffer.m_outputBuffers.insert({ record.name, outBuff });
}

bool Log::Logger::CreateLogFile(std::string name, LogFilter filter, bool isPathAbsolute)
{
	std::string filename = isPathAbsolute ? name : m_logFolder + '/' + name;

	if (m_logFiles.find(name) == m_logFiles.end()) 
	{
		try
		{
			m_logFiles.insert({filename, std::ofstream()});

			std::ofstream& fileStream = m_logFiles[filename];
			fileStream.open(filename, std::ios::trunc);

			if (!fileStream.good() ||
				!fileStream.is_open())
			{
				return false;
			}

			LogRecord record{0};

			record.isFile	= true;
			record.isTemp	= false;
			record.name		= filename;
			record.filter	= filter;

			AddStream(&fileStream, record);

		}
		catch (std::ios::failure ioException) 
		{	
			if(m_logFiles.find(filename) != m_logFiles.end())
				m_logFiles.erase(filename);

			return false;
		}
		catch (std::exception e) 
		{
			if (m_logFiles.find(filename) != m_logFiles.end())
				m_logFiles.erase(filename);

			return false;
		}

		return true;
	}
	else
		return false;
}

void Log::Logger::AddStream(stream * strm, bool isFile)
{
	LogRecord record{0};
	LogFilter filter{0};

	filter.filterLevel = false;
	filter.filterTags  = false;

	record.isFile	= isFile;
	record.isTemp	= false;
	record.filter	= filter;
	record.name		= m_sDefaultBufferName + std::to_string(m_buffer.m_bufferCounter++);

	LogBuffer::OutputBuffer outBuff;
	outBuff.streamPtr	= strm->rdbuf();
	outBuff.record		= record;
	
	m_buffer.m_outputBuffers.insert({record.name, outBuff});
}

void Log::Logger::AddStream(stream * strm, LogFilter filter, bool isFile)
{
	LogRecord record{ 0 };

	record.isFile	= isFile;
	record.isTemp	= false;
	record.filter	= filter;
	record.name		= m_sDefaultBufferName + std::to_string(m_buffer.m_bufferCounter++);

	LogBuffer::OutputBuffer outBuff;
	outBuff.streamPtr = strm->rdbuf();
	outBuff.record = record;

	m_buffer.m_outputBuffers.insert({ record.name, outBuff });
}

bool Log::LogFilter::MatchesMessage(Message msg)
{
	bool checkTag	{false};
	bool checkLevel {false};

	if (filterTags)
	{
		for (auto tag : tags)
		{
			if (tag == msg.tag)
				checkTag = true;
		}
	}
	else
		checkTag = true;

	checkLevel = 
		filterLevel ? 
		bool(msg.level & level) :
		true;

	return checkTag && checkLevel;
}

void Log::Logger::ClearAllManaged()
{
	for (auto buff = m_buffer.m_outputBuffers.begin();
		buff != m_buffer.m_outputBuffers.end();
		++buff) 
	{
		if (buff->second.isManaged)
		{
			m_buffer.m_outputBuffers.erase(buff);
		}
	}
}

void Log::Logger::DeleteLogFiles(bool removeFileHandles)
{
	if (removeFileHandles)
	{
		for (auto buff = m_buffer.m_outputBuffers.begin();
			buff != m_buffer.m_outputBuffers.end(); ++buff)
		{
			if(buff->second.record.isFile)
				m_buffer.m_outputBuffers.erase(buff);
		}
	}

	std::remove(m_logFolder.c_str());
}

void Log::Logger::RemoveAllDead()
{
	for (auto buff = m_buffer.m_outputBuffers.begin();
			buff != m_buffer.m_outputBuffers.end();
			++ buff)
	{
		if(!buff->second.IsAlive())
			m_buffer.m_outputBuffers.erase(buff);
	}
}

void Log::Logger::RemoveTemp()
{
	for (auto buff = m_buffer.m_outputBuffers.begin();
		buff != m_buffer.m_outputBuffers.end();
		++buff)
	{
		if (buff->second.record.isTemp)
			m_buffer.m_outputBuffers.erase(buff);
	}
}

void Log::Logger::RemoveStream(const char * streamName)
{
	std::map<std::string, LogBuffer::OutputBuffer>::iterator toRemove;

	if ((toRemove = m_buffer.m_outputBuffers.find(streamName))
		!= m_buffer.m_outputBuffers.end())
	{
		if (toRemove->second.record.isFile)
		{
			std::map<std::string, std::ofstream>::iterator logFile;
			if ((logFile = m_logFiles.find(streamName)) != m_logFiles.end())
			{
				m_logFiles.erase(logFile);
			}
		}

		m_buffer.m_outputBuffers.erase(streamName);
	}
}

void Log::Logger::RemoveAllStreams()
{
	m_buffer.m_outputBuffers.clear();
}

void Log::Logger::PrintMessage(Message msg)
{
	FilterBuffers(msg);

	std::string log = "";
	
	if (bool(m_printFlags & PrintFlags::USE_TIMESTAMP))
	{
		std::stringstream formatter;

		std::time_t	currentTime = std::time(nullptr);
		std::tm timestamp;
		localtime_s(&timestamp, &currentTime);

		formatter << std::put_time(&timestamp, "%c") << char(0x20);

		log += formatter.str();
	}

	if (bool(m_printFlags & PrintFlags::USE_TAG))
		log += "[" + msg.tag + "]" + char(0x20);

	if (bool(m_printFlags & PrintFlags::USE_LEVEL))
	{
		switch (msg.level)
		{
		case LogLevel::LDEBUG:
			log = SetConsoleColor(log, LogColor::GREEN);
			log += STR_DBG;
			break;

		case LogLevel::LWARNING:
			log += STR_WRN;
			log = SetConsoleColor(log, LogColor::YELLOW);
			SetConsoleColor(log, LogColor::YELLOW, true);
			break;

		case LogLevel::LERROR:
			log += STR_ERR;
			log = SetConsoleColor(log, LogColor::RED);  
			SetConsoleColor(log, LogColor::RED, true);  
			break;

		case LogLevel::LFATAL:
			log += STR_FTL;
			log = SetConsoleColor(log, LogColor::DARK_RED);
			SetConsoleColor(log, LogColor::DARK_RED, true);
			break;

		case LogLevel::LINFO:
			log += STR_INF;
			log = SetConsoleColor(log, LogColor::CYAN);
			break;

		case LogLevel::LVERBOSE:
		default:
			log += STR_VBS;
			log = SetConsoleColor(log, LogColor::WHITE);
			break;
		}

		log += std::string(SPACING, char(0x20));
	}

	log += msg.text;
	log += '\n';

	write(log.c_str(), log.size());
	FlushAll();
}

void Log::Logger::SetFilterState(FilterState state)
{
	m_filterUsage = state;
}

void Log::Logger::ConfigureFilter(LogFilter filter)
{
	m_globalFilter = filter;
}

void Log::Logger::SetLogFilePath(std::string newPath, bool removeOld)
{
	//TO DO => make new directory if it doesn't exist
	namespace fs = std::experimental::filesystem;

	if (!fs::exists(newPath))
		return;

	if (removeOld)
		DeleteLogFiles(true);

	m_logFolder = newPath;
}

void Log::Logger::AddFilterTag(const char * tag)
{
	bool exists = false;
	for (auto currentTag : m_globalFilter.tags)
	{
		if (currentTag == tag)
			exists = true;
	}

	if(!exists)
		m_globalFilter.tags.push_back(tag);
}

void Log::Logger::AddFilterLevel(LogLevel level)
{
	m_globalFilter.level |= level;
}

void Log::Logger::RemoveFilter(const char * tag)
{
	std::remove_if(m_globalFilter.tags.begin(), m_globalFilter.tags.end(), 
		[=](std::string tg) -> bool
		{
			return tg == tag;
		});
}

void Log::Logger::RemoveFilter(LogLevel level)
{
	m_globalFilter.level &= ~level;
}

void Log::Logger::ModifyStreamFilter(const char * streamName, LogFilter filter)
{
	std::map<std::string, LogBuffer::OutputBuffer>::iterator buffToModify;

	if ((buffToModify = m_buffer.m_outputBuffers.find(streamName)) 
			!= m_buffer.m_outputBuffers.end())
	{
		buffToModify->second.record.filter = filter;
	}
	
}

void Log::Logger::ModifyStreamFilter(const char * streamName, const char * tag, bool remove)
{
	std::map<std::string, LogBuffer::OutputBuffer>::iterator buffToModify;

	if ((buffToModify = m_buffer.m_outputBuffers.find(streamName))
		!= m_buffer.m_outputBuffers.end())
	{
		if (remove)
			std::remove_if(buffToModify->second.record.filter.tags.begin(),
						   buffToModify->second.record.filter.tags.end(),
							[=](std::string tg) -> bool
							{
								return tg == tag;
							});
		else
			buffToModify->second.record.filter.tags.push_back(tag);
	}
}

void Log::Logger::ModifyStreamFilter(const char * streamName, LogLevel level, bool remove)
{
	std::map<std::string, LogBuffer::OutputBuffer>::iterator buffToModify;

	if ((buffToModify = m_buffer.m_outputBuffers.find(streamName))
		!= m_buffer.m_outputBuffers.end())
	{
		if (remove)
			buffToModify->second.record.filter.level &= ~level;
		else
			buffToModify->second.record.filter.level |= level;
	}
}

Log::LogFilter Log::Logger::GetFilter() const
{
	return m_globalFilter;
}

std::string Log::Logger::GetLogFilePath() const
{
	return m_logFolder;
}
