#include "log.hpp"
#include "global.hpp"
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include "utility.hpp"


using std::string;
using std::cout;
using std::endl;

std::time_t Log::boot_time = std::time(NULL);

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
    ofs.close();
}

Log *Log::get_instance()
{
    if (_singleton == NULL){
        _singleton = new Log();
    }
    return (_singleton);
}

void Log::delete_instance()
{
    if (_singleton != NULL){
        delete (_singleton);
        _singleton = NULL;
    }
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
    std::time_t now = std::time(NULL);
    ofs << std::setw(5) << now - Log::boot_time << " ";
    ofs << file << " " << line << ", ";
    ofs << "[" << str << "]" << endl << std::flush;
}



void Log::write(const char *file,int line, const char *level,  const string &str, int log_level)
{
    static int cnt = 0;
    if (log_level >= LOG_LEVEL){
        Log* log = get_instance();
        std::string tmp = string(level);
        tmp += string(str);
        log->write_log(file, line, tmp);
    }
    cnt++;
}
