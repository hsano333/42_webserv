#ifndef CGI_APPLICATION_HPP
#define CGI_APPLICATION_HPP
#include "application.hpp"

typedef enum E_CGIApplication
{
    GET_CGI,
    POST_CGI,
    DELETE_CGI,
} ECGIApplication;

class CGIApplication : public Application
{
    bool is_cgi();

};

#endif
