#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include "Object.h"

#include <PCH/pch.h>
#include <Defines/Defines.h>

#include <sstream>
#include <fstream>
#include <time.h>
#include <iomanip>


#ifndef THROW_EXCEPTION
    /**
     * @brief THROW_EXCEPTION \n 
     * An Example:
     * 
     *     try { \n 
     *         THROW_EXCEPTION(1, "This is my error"); \n
     *     } \n 
     *         catch(VException& e) \n 
     *     { \n 
     *         e.what(); \n 
     *     } \n 
     */
    #define THROW_EXCEPTION(ErrorDesc, ...) throw Clunk::clException(ErrorDesc, __FILE__, __LINE__, ##__VA_ARGS__)

    #ifdef VK_GFX
        #ifndef VK_CHECK
            #define VK_CHECK(x)                                     \
                    {                                               \
                        VkResult err = x;                           \
                        if(err)                                     \
                        {                                           \
                            THROW_EXCEPTION("Vulkan Error: %s", err);\
                        }                                           \
                    }                                               
        #endif
    #endif

#endif


namespace Clunk
{
    class clException : public std::exception
    {
    public:

        String m_ErrorDesc;

        String m_SrcFileName;
        
        String m_LineNumber;

        String m_ErrText;

        /**
         * @brief Overrides std::exception::what()
         * @return const char* (C string) featuring: Error number, Error Desc, Src File, Line Number
         */
        const char* what();

        clException(String ErrorDesc, String SrcFileName, i32 LineNumber, ...);

    protected:

    private:

    };


    // class LogManager : public clObject
    // {

    // public:

    //     // Log File buffer
    //     std::stringstream logBuffer;

    //     /**
    //      * @brief Get the LogManager Singleton object.
    //      * @return LogManager* 
    //      */
    //     static LogManager* GetLogManager();

    //     /**
    //      * @brief Creates a log file.
    //      *      Should be called at application startup before entering
    //      *      a try-block
    //      * @param FileName Name of the file.
    //      */
    //     void Create(String FileName = "log.txt");

    //     /**
    //      * @brief Commits the contents to the log file and clears the text buffer.
    //      */
    //     void Flush();

    //     /**
    //      * @brief Closes the file
    //      */
    //     void Close();

    //     /**
    //      * @brief Writes an exception to the log file.
    //      * @param e VException to log.
    //      */
    //     void LogException(clException e);

    //     /**
    //      * @brief Gets the time as string. 
    //      * Used for recording the time of an error.
    //      * Format: hours:mins:secs
    //      * @return String
    //      */
    //     String GetTimeString();

    // protected:


    // private:

    //     static LogManager m_LogManager;

    //     // Log file handle
    //     std::ofstream m_LogFile;

    //     LogManager() {};
    //     virtual ~LogManager() {};

    // };

}


#endif