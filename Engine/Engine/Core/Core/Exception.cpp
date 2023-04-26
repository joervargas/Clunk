#include "Exception.h"
#include "Logger.h"

#ifdef PLATFORM_LINUX
    #include <stdarg.h>
#endif

namespace Clunk
{

    const char* clException::what()
    {
        return m_ErrText.c_str();
    }

    clException::clException(
            String ErrorDesc, 
            String SrcFileName, 
            i32 LineNumber,
            ...
        )
    {

        const u32 msg_length = 3200;
        char out_msg[msg_length] {};
        // set zeros
        memset(out_msg, 0, sizeof(out_msg));

        // manage variable arguments
        __builtin_va_list args_ptr;
        va_start(args_ptr, LineNumber);
        vsnprintf(out_msg, msg_length, ErrorDesc.c_str(), args_ptr);
        va_end(args_ptr);

        // Set properties
        m_ErrorDesc = out_msg;
        m_SrcFileName = SrcFileName;
        m_LineNumber = LineNumber;

        // Write properties to humand readable string
        std::stringstream ErrStr;

        // ErrStr << "Error: " << 
            // m_ErrorDesc << "\nSrc File: " <<
            // m_SrcFileName << "\nLine Number: " << 
            // m_LineNumber << "\n";

        // m_ErrText = ErrStr.str();
        Log_Output(ELog_Level::ERROR_LEVEL, m_ErrorDesc, SrcFileName, LineNumber );
        // CLOG_ERROR(m_ErrText.c_str())
    }


    // LogManager LogManager::m_LogManager;

    // LogManager* LogManager::GetLogManager()
    // {
    //     return &m_LogManager;
    // }

    // void LogManager::Create(String FileName)
    // {
    //     m_LogFile.open(FileName.c_str());
    // }

    // void LogManager::Flush()
    // {
    //     m_LogFile << logBuffer.str(); // stream buffer data into file
    //     m_LogFile.flush(); // commit to file
    //     logBuffer.str(""); // Empty the buffer
    // }

    // void LogManager::Close()
    // {
    //     m_LogFile.close();
    // }

    // void LogManager::LogException(clException e)
    // {
    //     logBuffer << "*********************************ERROR*********************************\n";
    //     logBuffer << GetTimeString() << "\n" << e.what();
    //     logBuffer << "***********************************************************************\n";
    //     Flush();
    //     // CLOG_ERROR(e.what());
    // }

    // String LogManager::GetTimeString()
    // {
    //     std::stringstream TimeStr;

    //     struct tm *pTime;
    //     time_t ctTime; 
    //     time(&ctTime);
    //     pTime = localtime(&ctTime);

    //     TimeStr << std::setw(2) << std::setfill('0') << pTime->tm_hour << ":";
    //     TimeStr << std::setw(2) << std::setfill('0') << pTime->tm_min << ":";
    //     TimeStr << std::setw(2) << std::setfill('0') << pTime->tm_sec;

    //     return TimeStr.str();
    // }

}

