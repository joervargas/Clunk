#include "Logger.h"
#include "Exception.h"

#include <iostream>
#include <stdarg.h>
#include <cstring>
#include <sstream>

// #if defined PLATFORM_WINDOWS
// #include <windows.h>
// #else
// #include <stdlib.h>
// #endif

namespace Clunk
{
    
    b8 initialize_logging()
    {
        // TODO: implement logic
        return true;
    }

    void shutdown_logging(void* state) {
        // TODO: implement logic
    }

    void Log_Output(ELog_Level level, String message, String fileName, i32 lineNumber, ...)
    {
        const char* level_strings[6] = {"{FATAL}: ", "{ERROR}: ", "{WARN}: ", "{INFO}: ", "{DEBUG}: ", "{TRACE}: "};
        const char* color_strings[6] = {"0;41", "1;31", "1;33", "1;32", "1;34", "1;30"};
        b8 is_error = level < ELog_Level::WARN_LEVEL;


        // String msg = String(level_strings[level]) + message;

        const u32 msg_length = 3200;
        char out_msg[msg_length] {};
        // set zeros
        memset(out_msg, 0, sizeof(out_msg));

        // manage variable arguments
        // __builtin_va_list args_ptr;
        VA_LIST args_ptr;
        // va_start(args_ptr, message);
        va_start(args_ptr, lineNumber);
        VSN_PRINTF(out_msg, msg_length, message.c_str(), args_ptr);
        va_end(args_ptr);

        std::stringstream msg;

        if(is_error)
        {
            msg << "\n********************************* ERROR *********************************\n";
        }
        msg << level_strings[level] << 
            "\n\t" << out_msg << 
            "\n\tFile: " << fileName <<
            " Line: " << lineNumber;
        if(is_error)
        {
            msg << "\n************************************************************************\n";
        }

        // printf("\033[%sm%s\033[0m", colour_strings[color], message);
        // std::cout << "\033[" << color_strings[level] << "m" << out_msg << "\033[m" << std::endl;
        std::cout << "\033[" << color_strings[level] << "m" << msg.str() << "\033[m" << std::endl;

        LogFileManager* LogFile = LogFileManager::GetLogFileManager();
        LogFile->logBuffer << msg.str() << std::endl;
    }

    LogFileManager LogFileManager::m_LogFileManager;

    LogFileManager* LogFileManager::GetLogFileManager()
    {
        return &m_LogFileManager;
    }

    void LogFileManager::Create(String FileName)
    {
        m_LogFile.open(FileName.c_str());
    }

    void LogFileManager::Flush()
    {
        m_LogFile << logBuffer.str(); // stream buffer data into file
        m_LogFile.flush(); // commit to file
        logBuffer.str(""); // Empty the buffer
    }

    void LogFileManager::Close()
    {
        m_LogFile.close();
    }

    void LogFileManager::LogException(clException e)
    {
        // logBuffer << "*********************************ERROR*********************************\n";
        logBuffer << GetTimeString() << "\n" << e.what();
        // logBuffer << "***********************************************************************\n";
        Flush();
    }

    String LogFileManager::GetTimeString()
    {
        std::stringstream TimeStr;

        struct tm *pTime;
        time_t ctTime; 
        time(&ctTime);
        pTime = localtime(&ctTime);

        TimeStr << std::setw(2) << std::setfill('0') << pTime->tm_hour << ":";
        TimeStr << std::setw(2) << std::setfill('0') << pTime->tm_min << ":";
        TimeStr << std::setw(2) << std::setfill('0') << pTime->tm_sec;

        return TimeStr.str();
    }

} // namespace Clunk
