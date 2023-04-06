#pragma once

// #define VK_NO_PROTOTYPES

#include "Application.h"
#include "Logger.h"
#include "Exception.h"
// #include <unistd.h>


extern Clunk::Application* Clunk::CreateApplication();

int main()
{
    auto app = Clunk::CreateApplication();
    
    Clunk::LogFileManager* LogFile = Clunk::LogFileManager::GetLogFileManager();
    // LOG_INFO("The current directory is %s", get_current_dir_name());

    LogFile->Create("../Assets/log.txt");

    try
    {
        if(!app->Initialize())
        {
            THROW_EXCEPTION("Application failed to initialize.");
            
        }
        if(!app->Run())
        {
            THROW_EXCEPTION("Application did not shutdown gracefully.");
        }
    }
    catch(Clunk::clException& e)
    {
        LogFile->LogException(e);
    }

    LogFile->Flush();
    LogFile->Close();
    delete(app);
}