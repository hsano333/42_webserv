#include "process_id.hpp"
#include "utility.hpp"
#include "global.hpp"
#include <sys/types.h>
#include <signal.h>

ProcessID::ProcessID() : pid(-1)
{
;
}

ProcessID::ProcessID(int pid) : pid(pid)
{
;
}

ProcessID::~ProcessID()
{
;
}

ProcessID ProcessID::from_int(int pid)
{
    if(pid < 0){
        ERROR("pid is must > 0, but pid:" + Utility::to_string(pid));
        throw std::invalid_argument("pid is must > 0");
    }
    return ProcessID(pid);
}


int ProcessID::to_int() const
{
    DEBUG("to_int():");
    return (this->pid);
}

std::string const ProcessID::to_string() const
{
    return (Utility::to_string(this->pid));
}

void ProcessID::kill()
{
    if(this->pid >= 0){
        DEBUG("Process close:pid=" + this->to_string());
        ::kill(this->pid, SIGINT);
        this->pid = -1;
    }
}
