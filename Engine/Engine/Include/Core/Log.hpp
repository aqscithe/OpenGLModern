#pragma once

#include <fstream>

#include <Core/Core.hpp>


namespace Core
{
    namespace Debug
    {
        class Log
        {
        public:
            Log() = delete;
            Log(const Log& log) = delete;
            void operator=(const Log& log) = delete;

            static void             print(std::string statement, int logType);
            static void             configureLogFiles();
            static void             redirectLogs(int outputType);
            static void             endRedirect(int outputType);
            
            static bool             enabled;
        private:
            static void             printTitle(int logFile);
            static void             logHeader(int logFile);
            static void             logDivider(int logFile);
            static void             printDateTime(int logFile);

            static std::ofstream    outMessage;
            static std::ofstream    outError;
            static std::streambuf*  coutbuf;
            static std::streambuf*  cerrbuf; 

        };
    }
}
