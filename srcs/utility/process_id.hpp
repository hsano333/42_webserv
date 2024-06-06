
#ifndef PROCESS_ID
#define PROCESS_ID
#include "positive_number.hpp"
#include <string>

class ProcessID : PositiveNumber
{
    public:
        ProcessID();
        ~ProcessID();
        static ProcessID from_int(int pid);
        int to_int() const;
        std::string const to_string() const;
        void kill();
    private:
        ProcessID(int pid);
        int pid;

};

#endif
