#ifndef STATE_PROCESS_HPP
#define STATE_PROCESS_HPP
#include "iread.hpp"
#include "split.hpp"

class IStateProcess
{
    public:
        virtual int iread(int fd, char* buf, size_t size) = 0;
        virtual void split(std::string const& str, std::string delimiter) = 0;
};

class StateProcessReqeust : public IStateProcess
{
    public:
        StateProcessReqeust();
        ~StateProcessReqeust();
        int iread(int fd, char* buf, size_t size);
        void split(std::string const& str, std::string delimiter);
};

class StateProcessCGI : public IStateProcess
{
    public:
        StateProcessCGI();
        ~StateProcessCGI();
        int iread(int fd, char* buf, size_t size);
        void split(std::string const& str, std::string delimiter);
};

#endif
