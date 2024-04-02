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
        //char*   buf();
        void    clear_raw_buf();
        //size_t  buf_size();
        //char*   get_raw_buf_pointer();
        //char*   get_raw_buf_begin();
        //int     raw_buf_space();
        int      get_buf_body_size();
        char    *get_buf_body(size_t *size);
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

        //void    set_buf_pos(size_t pos);
        //
        //

        void        set_path_info(std::string const &path);
        std::string const &requested_filepath() const;
        std::string const &requested_path() const;
        std::string const &tmp_path_info() const;
        std::string const &path_info() const;
        std::string const &parent_dir_path() const;
        //File    get_target_file(const ConfigLocation *location);

        void    print_info() const;
        RequestLine const &req_line() const;
        Header const &header() const;

        FileDiscriptor fd() const;
        //FileDiscriptor sockfd() const;

        // File Interface
        //int open();
        //int close();
        int read(char **buf, size_t size);
        //int write(char **buf, size_t size);
        //int save(char *data, size_t size);
        //bool can_read();
        size_t size();
        //bool is_chunk();
        //int remove();
        //std::string const &path();

        void set_file(WebservFile *file);
        void add_read_body_size(size_t size);

        //void set_source_file(File *source);
        //File *get_source_file();
        //int open_source_file();
        //int close_source_file();
        //ssize_t get_data(char** data);
        //
    private:
        Request();
        Request(FileDiscriptor const &fd);

        WebservFile *file;
        FileDiscriptor const &fd_;
        //FileDiscriptor sockfd_;
        //char    raw_buf[MAX_BUF];
        //char    raw_buf[200];
        //int     raw_buf_point;
        //size_t  raw_buf_pos_;
        char    *buf_body;
        //vector<string> buf_body;
        int     buf_body_size;
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
        std::vector<char> tmp_buf;
        bool            is_cgi_;
        size_t          read_body_size_;
        //File *source;

};

namespace RequestBufferFunc{
    template <class FileT>
    int read(FileT *file, char **data, size_t size)
    {
        *data= file->get_buf_body(&size);
        return (size);
    }

}

#endif
