#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include "status_code.hpp"
#include "header.hpp"
//#include "error_page.hpp"
#include "file.hpp"
#include "http_data.hpp"
#include "config_server.hpp"
#include "config_location.hpp"
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

class Response : public File
{
    public:
        Response();
        ~Response();
        Response(Response const &res);
        Response& operator=(Response const &res);
        static Response* from_success_status_code(StatusCode &code, File *file);
        static Response* from_error_status_code(StatusCode &code);
        static Response* from_error_file(File *file, StatusCode &code);
        //static Response* from_cgi_header_line(Split &header_line, File *file, ConfigServer const *server, ConfigLocation const *location);
        static Response* from_cgi_header_line(Split &header_line, File *file);
        void   set_header(Split &sp, size_t offset);
        //static Response* from_error_page(ErrorPage &page);

        static Response* from_redirect(StatusCode &code, std::string const &filepath);
        static Response* from_file(File *file);
        void add_header(std::string const &key, std::string const &value);
        //static Response* from_file(std::string const &filepath);
        //static Response* from_directory(std::string const &filepath);
        /*
        File *get_source_file();
        int open_source_file();
        int close_source_file();
        */
        File *get_file();
        const Header &header();
        //ssize_t get_data(char** data);
        void print_info();
        void set_exist_body(bool flag);
        bool check_body_size(ConfigServer const *server);
        char*   get_buf_body(int *size);
        void    set_buf_body(char *body_p, int size);

        // File Interface
        int open();
        int close();
        int read(char **buf, size_t size);
        int write(char **buf, size_t size);
        int save(char *data, size_t size);
        bool can_read();
        size_t size();
        bool is_chunk();
        int remove();
        std::string const &path();
        void check_body_and_chunk();

    private:
        StatusCode status_code;
        Header headers;
        char    *buf_body;
        int     buf_body_size;
        File *file;

        bool is_redirect;
        SEND_STATE send_state;
        std::string status_line;
        std::string header_line;

        void make_status_line();
        void make_header_line();
        int read_body_and_copy_chunk(char** dst, size_t size);
        int read_body_and_copy(char** dst, size_t size);
        //bool exist_body_;
        bool has_body;
        bool is_chunked;
        std::vector<char> tmp_buf;
};


#endif /* RESPONSE_HPP */
