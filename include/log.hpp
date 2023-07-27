#ifndef LOG_HPP
#define LOG_HPP
#include <string>
#include <fstream>

class Log
{
    public:
        static Log *get_instance();
        static void write(const char *file,int line, const char *level, std::string str, int log_level);
        static void check_writable();
    private:
        bool is_writable;
        static Log *_singleton;
        std::ofstream ofs;
        void write_log(const char *file,int line,std::string& str);
        Log();
        ~Log();

};

#endif
