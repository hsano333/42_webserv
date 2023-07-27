#include "response_cgi.hpp"
#include "request_cgi.hpp"
#include "cgi.hpp"
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

ResponseCGI::ResponseCGI(CGI* cgi) : Response((cgi->get_req())), _cgi(cgi), _header_chunked("Content-Type: text/plain\r\nTransfer-Encoding: chunked\r\n")
{
    cout << "Response CGI Instance!!!!!" << endl;
    cout << "Response CGI Instance!!!!!" << endl;
    cout << "Response CGI Instance!!!!!" << endl;
    cout << "Response CGI Instance!!!!!" << endl;
    cout << "Response CGI Instance!!!!!" << endl;
    ;
}

ResponseCGI::~ResponseCGI()
{
    delete _cgi;
}

void ResponseCGI::load_and_set_status_code(string &str)
{
    int code = 0;
    if (str.size() > 0){
        Split status(str, " ");
        if (status.size() == 3){
            code = Utility::to_int(status[1]);
            if (get_status_message(code) != get_status_message(UNKNOWN_CODE)){
                set_status(code);
            }else{
                code = 0;
            }
        }
    }
    if (code == 0){
        set_status(200);
    }
}
int ResponseCGI::determine_status_code(RequestCGI *result)
{
    if (this->_cgi->get_exit_status() != 0){
        cout << "500 NG" << endl;
        return (500);
    }else if (result->exist_ContentTypeHeader() && !result->exist_LocationHeader()){
        cout << "200 OK" << endl;
        return (200);
    }else if (result->exist_ContentTypeHeader() && result->exist_LocationHeader()){
        cout << "302 OK" << endl;
        return (302);
    }else if (!result->exist_ContentTypeHeader() && result->exist_LocationHeader()){
        cout << "302 OK" << endl;
        return (302);
    }else if (!result->exist_ContentTypeHeader() && result->exist_LocationHeader()){
        cout << "302 OK" << endl;
        return (302);
    }
    return (0);
}

ssize_t ResponseCGI::get_data(char** data)
{
    //char *bk = *data;

    string res_str;
    RequestCGI* result = _cgi->get_result();
    _cgi->check_wait_process();

    if (_send_state == STILL_NOT_SEND) {
        if (!result->exist_StatusHeader()){
            int status_code = determine_status_code(result);
            result->set_status_code(status_code);
            _status_line = this->get_status_line();
            *data = const_cast<char*>(_status_line.c_str());
            return (_status_line.size());
        }else{

        }

        result->get_concat_header(this->_header_concat);
        //_header_sent = true;
    } else if (_send_state == SENT_HEADER){

    } else if (_send_state == SENT_BODY){

    }
    return (result->read_result(data, BUF_MAX));
}

bool ResponseCGI::open_responsed_file()
{
    std::cout << "open CGI" << std::endl;
    return (true);
}

void ResponseCGI::close_responsed_file()
{
    std::cout << "close CGI" << std::endl;
}

/*
void ResponseCGI::copy_header(string &header)
{
    RequestCGI* result = _cgi->get_result();
    header = result->get_header_line();
    while(1)
    {
        string &tmp = result->get_header_line();
        if (tmp == ""){
            break;
        }
        header += tmp;
    }
}
*/

/*
ssize_t ResponseCGI::get_data(char** data)
{
    char *bk = *data;
    RequestCGI* result = _cgi->get_result();
    //result->print_request();
    //return 0;
    if (!this->_header_sent) {
        _cgi->check_wait_process();

        string res_str;
        if (this->_cgi->get_end_process()) {
            if (result->get_status_code() != 0){
                _header_sent = true;
                result->get_concat_header(this->_header_concat);
                if (this->_header_concat == ""){
                    cout << "header last??????" << endl;
                    cout << "header last??????" << endl;
                    cout << "header last??????" << endl;
                    cout << "header last??????" << endl;
                    //const std::string last_header = "Transfer-Encoding:chunked\r\n\r\n";
                    *data = const_cast<char*>(this->_header_concat.c_str());
                    //this->_header_sent = true;
                    return (this->_header_concat.size());
                }else{
                    if (!result->exist_ContentLengthHeader()){
                        this->_header_concat += "Transfer-Encoding: chunked\r\n";
                    }
                    *data = const_cast<char*>(this->_header_concat.c_str());
                    return (this->_header_concat.size());
                }
            }else{
                int status_code = determine_status_code(result);
                result->set_status_code(status_code);
                _status_line = this->get_status_line();
                *data = const_cast<char*>(_status_line.c_str());
                return (_status_line.size());
            }
        } else {
            result->set_status_code(500);
            res_str = this->get_status_line();
            *data = const_cast<char*>(res_str.c_str());
            this->_all_sent = true;
            return (res_str.size());
        }
    }else if (result->has_extra_buf()){
        char ttmp;
        char *tmp;
        tmp = &ttmp;
        int tmp_size = result->get_extra_buf(&tmp);
        if (tmp_size <= 0){
            return (0);
        }
        int cp_size = Utility::read_body_and_copy(tmp, data, tmp_size);
        if (cp_size > 0){
            return (cp_size);
        }
    }
    *data = bk;
    return (result->read_result(data, BUF_MAX));
}

*/
