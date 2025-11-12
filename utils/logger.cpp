#include "logger.h"
#include <iomanip>

using namespace std; 

namespace utils{
    Logger :: Logger() : level(LogLevel :: INFO),to_file(false){}
    Logger :: ~Logger (){
        if (file.is_open()) 
        file.close(); 
    }

    Logger &Logger :: instance()
    {
        static Logger inst;
        return inst; 
    }

    void Logger :: set_logfile(const string &path)
    {
        lock_guard<mutex> lock(mtx); 
        file.open(path, ios :: app);
        to_file = file.is_open(); 
    }

    void Logger :: set_level(LogLevel lvl)
    {
        lock_guard<mutex> lock(mtx); 
        level = lvl; 
    }

    string Logger :: timestamp()
    {
        using namespace chrono; 
        auto now = system_clock :: now();
        time_t t = system_clock :: to_time_t(now); 
        tm tm{}; 

        localtime_s (&tm, &t); 
        
        ostringstream oss; 
        oss << put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str(); 
    }

    string Logger :: level_str(LogLevel lvl)
    {
        switch(lvl)
        {
            case LogLevel ::INFO : return "INFO"; 
            case LogLevel::WARN: return "WARN";
            case LogLevel::ERROR: return "ERROR";
            case LogLevel::DEBUG: return "DEBUG";
        }
        return "UNK"; 
    }

    void Logger::log(LogLevel lvl, const string& msg) 
    {
    lock_guard<mutex> lock(mtx);
    if (lvl > level) 
    return;
    string line = "[" + timestamp() + "][" + level_str(lvl) + "] " + msg;
    cout << line << endl;
    if (to_file && file.is_open()) file << line << endl;
    }
}