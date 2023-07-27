#include "log.hpp"
#include "global.hpp"
#include <stdio.h>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

Log::Log()
{
    std::string filename = "LOG_FILE";
    ofs.open(LOG_FILE, std::ios::app);
    if (ofs){
        is_writable = true;
    }else{
        is_writable = false;
    }
}

Log *Log::_singleton = NULL;
Log::~Log()
{
    cout << "test" << endl;
    ofs.close();
    delete (_singleton);
}

Log *Log::get_instance()
{
    if (_singleton == NULL){
        _singleton = new Log();
    }
    return (_singleton);
}

void Log::check_writable()
{
    Log* log = get_instance();
    if (!log->is_writable){
        cout << "Warning: Log file is not writable" << endl;
    }
}

void Log::write_log(const char *file,int line, string& str)
{
    ofs << file << " " << line << ", ";
    ofs << "[" << str << "]" << endl << std::flush;
}

void Log::write(const char *file,int line, const char *level,  string str, int log_level)
{
    if (log_level >= LOG_LEVEL){
        Log* log = get_instance();

        std::string tmp = string(level);
        tmp += string(str);
        log->write_log(file, line, tmp);
    }
}
