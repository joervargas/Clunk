#pragma once

// #include <Defines/defines.h>
#include "Object.h"
#include "Exception.h"
#include <Defines/Defines.h>
#include <sstream>
#include <fstream>


namespace Clunk
{

    /** @brief Indicates if warning level logging is enabled. */
    #define LOG_WARN_ENABLED 1
    /** @brief Indicates if info level logging is enabled. */
    #define LOG_INFO_ENABLED 1
    /** @brief Indicates if debug level logging is enabled. */
    #define LOG_DEBUG_ENABLED 1
    /** @brief Indicates if trace level logging is enabled. */
    #define LOG_TRACE_ENABLED 1

    /** @brief Represents levels of logging */
    typedef enum ELog_Level {
        /** @brief Fatal log level, should be used to stop the application when hit. */
        FATAL_LEVEL = 0,
        /** @brief Error log level, should be used to indicate critical runtime problems that cause the application to run improperly or not at all. */
        ERROR_LEVEL = 1,
        /** @brief Warning log level, should be used to indicate non-critial problems with the application that cause it to run suboptimally. */
        WARN_LEVEL = 2,
        /** @brief Info log level, should be used for non-erronuous informational purposes. */
        INFO_LEVEL = 3,
        /** @brief Debug log level, should be used for debugging purposes. */
        DEBUG_LEVEL = 4,
        /** @brief Trace log level, should be used for verbose debugging purposes. */
        TRACE_LEVEL = 5
    } ELog_Level;

    /**
     * @brief Initializes logging system. Call twice; once with state = 0 to get required memory size,
     * then a second time passing allocated memory to state.
     * 
     * @param memory_requirement A pointer to hold the required memory size of internal state.
     * @param state 0 if just requesting memory requirement, otherwise allocated block of memory.
     * @return b8 True on success; otherwise false.
     */
    b8 initialize_logging();
    // b8 initialize_logging(u64* memory_requirement, void* state);

    /**
     * @brief Shuts down the logging system.
     * @param state A pointer to the system state.
     */
    void shutdown_logging(void* state);

    /**
     * @brief Outputs console logging at the given level.
     * @param level The log level to use.
     * @param message The message to be logged.
     * @param ... Any formatted data that should be included in the log entry.
     */
    void Log_Output(ELog_Level level, String message, String fileName, i32 lineNumber, ...);

    /**
     * @brief Outputs logging at the given level.
     * @param level The log level to use.
     * @param message The message to be logged.
     * @param ... Any formatted data that should be included in the log entry.
     */
    #define CLOG_PRINT(level, message, ...) Log_Output(level, message, __FILE__, __LINE__, ##__VA_ARGS__);

    /** 
     * @brief Console Logs a fatal-level message. Should be used to stop the application when hit.
     * @param message The message to be logged. Can be a format string for additional parameters.
     * @param ... Additional parameters to be logged.
     */
    #define CLOG_FATAL(message, ...) Log_Output(Clunk::ELog_Level::FATAL_LEVEL, __FILE__, __LINE__, ##__VA_ARGS__);

    /** 
     * @brief Console Logs an error-level message. Should be used to indicate critical runtime problems 
     * that cause the application to run improperly or not at all.
     * @param message The message to be logged.
     * @param ... Any formatted data that should be included in the log entry.
     */
    #define CLOG_ERROR(message, ...) Log_Output(Clunk::ELog_Level::ERROR_LEVEL, message, __FILE__, __LINE__, ##__VA_ARGS__);

    /** 
     * @brief Console Logs a warning-level message. Should be used to indicate non-critial problems with 
     * the application that cause it to run suboptimally.
     * @param message The message to be logged.
     * @param ... Any formatted data that should be included in the log entry.
     */
    #define CLOG_WARN(message, ...) Log_Output(Clunk::ELog_Level::WARN_LEVEL, message, __FILE__, __LINE__, ##__VA_ARGS__);

    /** 
     * @brief Console Logs a warning-level message. Should be used to indicate non-critial problems with 
     * the application that cause it to run suboptimally. Does nothing when LOG_WARN_ENABLED != 1
     * @param message The message to be logged.
     * @param ... Any formatted data that should be included in the log entry.
     */
    #define CLOG_INFO(message, ...) Log_Output(Clunk::ELog_Level::INFO_LEVEL, message, __FILE__, __LINE__, ##__VA_ARGS__);

    /** 
     * @brief Console Logs an info-level message. Should be used for non-erronuous informational purposes.
     * @param message The message to be logged.
     * @param ... Any formatted data that should be included in the log entry.
     */
    #define CLOG_DEBUG(message, ...) Log_Output(Clunk::ELog_Level::DEBUG_LEVEL, message, __FILE__, __LINE__, ##__VA_ARGS__);

    /** 
     * @brief Console Logs a trace-level message. Should be used for verbose debugging purposes.
     * @param message The message to be logged.
     * @param ... Any formatted data that should be included in the log entry.
     */
    #define CLOG_TRACE(message, ...) Log_Output(Clunk::ELog_Level::TRACE_LEVEL, message, __FILE__, __LINE__, ##__VA_ARGS__);


    class LogFileManager : public ClObject
    {
   public:

        // Log File buffer
        std::stringstream logBuffer;

        /**
         * @brief Get the LogManager Singleton object.
         * @return LogManager* 
         */
        static LogFileManager* GetLogFileManager();

        /**
         * @brief Creates a log file.
         *      Should be called at application startup before entering
         *      a try-block
         * @param FileName Name of the file.
         */
        void Create(String FileName = "log.txt");

        /**
         * @brief Commits the contents to the log file and clears the text buffer.
         */
        void Flush();

        /**
         * @brief Closes the file
         */
        void Close();

        /**
         * @brief Writes an exception to the log file.
         * @param e VException to log.
         */
        void LogException(clException e);

        /**
         * @brief Gets the time as string. 
         * Used for recording the time of an error.
         * Format: hours:mins:secs
         * @return String
         */
        String GetTimeString();

    protected:


    private:

        static LogFileManager m_LogFileManager;

        // Log file handle
        std::ofstream m_LogFile;

        LogFileManager() {};
        virtual ~LogFileManager() {};

    };

}
