#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include "status_code.hpp"
#include "header.hpp"
//#include "error_page.hpp"
#include "webserv_file.hpp"
#include "http_data.hpp"
#include "config_server.hpp"
#include "config_location.hpp"
#include "file_discriptor.hpp"
#include <unistd.h>
#include <map>
#include <vector>

enum SEND_STATE{
    ERROR,
    STILL_NOT_SEND,
    SENT_STATUS_LINE,
    SENT_HEADER,
    OPEND_FILE,
    SENT_BODY,
    CLOSE,
};

class Response
{
    public:
        Response();
        ~Response();
        Response(Response const &res);
        Response& operator=(Response const &res);
        static Response* from_success_status_code(FileDiscriptor const &fd, StatusCode &code, WebservFile *file, ConfigLocation const *location);
        static Response* from_error_status_code(FileDiscriptor const &fd, StatusCode &code, ConfigLocation const *location);
        static Response* from_cgi_header_line(FileDiscriptor const &fd, Split &header_line, WebservFile *file);
        void   set_header(Split &sp, size_t offset);

        static Response* from_redirect(StatusCode &code, std::string const &filepath);
        static Response* change_error_code(Response* res, FileDiscriptor const &fd, StatusCode &code, ConfigLocation const *location);
        void add_header(std::string const &key, std::string const &value);
        WebservFile *get_file();
        const Header &header();
        void print_info();
        void set_exist_body(bool flag);
        bool check_body_size(ConfigServer const *server);
        char*   get_buf_body(int *size);
        void    set_buf_body(char *body_p, int size);

        // File Interface
        int open();
        int close();
        int read_data(char **data, char **ref, size_t size, bool &ref_flag);
        int read(char **buf, size_t size);
        int write(char **buf, size_t size);
        bool is_chunk();
        bool has_body();
        bool read_completed();
        bool check_body_and_chunk();
        void add_written_body_size(size_t size);
        void switching_file(WebservFile *file);
        std::vector<char> &buffer();
        void clear_buffer();
        void set_satus_code(StatusCode &code);

    private:
        StatusCode status_code;
        Header headers;
        std::vector<char> buf_body;
        int     buf_body_size;
        WebservFile *file;

        bool is_redirect;
        SEND_STATE send_state;
        std::string status_line;
        std::string header_line;

        void make_status_line();
        void make_header_line();
        int read_body_and_copy_chunk(char** dst, size_t size);
        int read_body_and_copy(char** dst, size_t size);
        bool has_body_;
        bool is_chunked;
        std::vector<char> tmp_buf;

        size_t written_body_size;
        size_t header_index;

};


#endif /* RESPONSE_HPP */
