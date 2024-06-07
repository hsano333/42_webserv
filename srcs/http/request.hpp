#ifndef REQUEST_H
#define REQUEST_H

#include "http_data.hpp"
//#include "config.hpp"
#include "config_location.hpp"
#include "global.hpp"
#include "ireader.hpp"
#include "socket_data.hpp"
#include "split.hpp"
#include "uri.hpp"
#include "header.hpp"
#include "method.hpp"
#include "webserv_file.hpp"
#include <map>
#include <string>
#include <vector>
#include "file_discriptor.hpp"
#include "request_line.hpp"
#include "buffer_controller.hpp"

using std::map;
using std::string;
using std::vector;

typedef enum E_STATE
{
    STILL_NOT_LOAD,
    LOADED_STATUS_LINE,
    LOADED_STATUS_HEADER,
    LOADED_STATUS_BODY,
    END_LOAD,

} REQUEST_STATE;

class Request
{
    public:
        ~Request();

        static Request *from_fd(FileDiscriptor const &fd);
        void    clear_raw_buf();
        int      get_buf_body_size();
        char    *get_buf_body(size_t *size);
        void    add_buf_body_p(size_t size);
        void    set_buf_body(char *body_p, size_t size);
        void    clear_buf_body();
        void    set_request_line(std::string const &str);
        void    set_header(Split &sp, size_t offset);
        void    set_read_completed(bool flag);
        void    set_requested_filepath(const ConfigLocation *location);
        bool    read_completed();

        void    check_parent_permittion(std::string &path);

        bool    is_file() const;
        bool    is_directory() const;
        bool    is_not_executable_parent_dir() const;
        bool    is_deletable_parent_dir() const;

        void    set_is_file(bool flag);
        void    set_is_directory(bool flag);

        bool    is_cgi();
        void    set_cgi(bool flag);

        void set_has_body(bool flag);
        bool has_body();


        void        set_path_info(std::string const &path);
        std::string const &requested_filepath() const;
        std::string const &requested_path() const;
        std::string const &tmp_path_info() const;
        std::string const &path_info() const;
        std::string const &parent_dir_path() const;

        void    print_info() const;
        RequestLine const &req_line() const;
        Header const &header() const;

        FileDiscriptor fd() const;

        // File Interface
        int read(char **buf, size_t size);
        bool can_read();
        size_t size();

        void set_file(WebservFile *file);
        void add_read_body_size(size_t size);

        void set_auth(std::string const &auth);
        std::string const &auth();
        WebservFile *file();

    private:
        Request();
        Request(FileDiscriptor const &fd);

        WebservFile *file_;
        FileDiscriptor const &fd_;
        std::vector<char> buf_body;
        int     buf_body_size;
        size_t  buf_body_p;
        bool    is_file_;
        bool    is_directory_;
        bool    is_not_executable_parent_dir_;
        bool    is_deletable_parent_dir_;
        std::string requested_filepath_;
        std::string requested_path_;
        std::string parent_dir_path_;

        RequestLine    req_line_;
        Header         header_;
        std::string    tmp_path_info_;
        std::string    path_info_;
        bool           read_completed_;
        bool            is_cgi_;
        size_t          read_body_size_;
        bool            has_body_;
        std::string     auth_;

};

namespace RequestBufferFunc{
    template <class FileT>
    int read(FileT *file, char **data, size_t size)
    {
        size_t buf_size;
        char *tmp = file->get_buf_body(&buf_size);
        if(buf_size <= 0 || tmp == NULL){
            return (buf_size);
        }
        if(size > buf_size){
            size = buf_size;
        }

        for(size_t i=0;i<size;i++){
            (*data)[i] = tmp[i];
        }
        file->add_buf_body_p(size);
        return (size);
    }
}

#endif
