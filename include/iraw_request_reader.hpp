#ifndef IRAW_REQUEST_READER_H
#define IRAW_REQUEST_READER_H
#include "iread.hpp"
#include "split.hpp"
#include <string>
#include <unistd.h>
#include <vector>

class IRawRequestReader
{
    public:
        std::string& getline() = 0;
};
#endif
