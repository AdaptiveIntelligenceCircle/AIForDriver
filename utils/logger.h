#pragma once 
#include <string> 
#include <mutex> 
#include <fstream> 

#include <iostream> 
#include <chrono>
using namespace std;
#include <vector> 

namespace utils
{
    enum class LogLevel
    {
        INFO, WARN, ERROR, DEBUG
    };

    class Logger
    {
        public: 
        static Logger &instance(); 

        void set_logfile(const string &path); 
        void set_level(LogLevel lvl); 

        void log(LogLevel lvl, const string &msg); 

        inline void info(const string &msg){
            log(LogLevel :: INFO, msg);
        }

        inline void warn(const string &msg)
        {
            log(LogLevel :: WARN, msg); 
        }

        inline void error(const string &msg)
        {
            log(LogLevel :: ERROR, msg); 
        }

        inline void debug(const string &msg)
        {
            log(LogLevel :: DEBUG, msg); 
        }

        private: 
        Logger(); 
        ~Logger(); 

        mutex mtx; 
        ofstream file; 
        LogLevel level; 
        bool to_file; 

        string timestamp(); 
        string level_str(LogLevel lvl); 
    }; 
} // namespace utils.. 