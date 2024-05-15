#ifndef CGI_IO_HPP
#define CGI_IO_HPP
#include "file_discriptor.hpp"

class CGI_IO
{
    public:
        FileDiscriptor const &pid();
        FileDiscriptor const &fd_in();
        FileDiscriptor const &fd_out();
    private:
        FileDiscriptor pid_;
        FileDiscriptor fd_in_;
        FileDiscriptor fd_out_;
};


#endif
