#ifndef WEBSERV_IO
#define WEBSERV_IO

#include "epoll.hpp"
#include "webserv_file.hpp"
#include "global.hpp"

class WebservIO
{
    public:
        WebservIO();
        ~WebservIO();
        WebservFile *source();
        WebservFile *destination();
        WebservFile *source_for_read();
        WebservFile *destination_for_read();
        WebservFile *source_for_write();
        WebservFile *destination_for_write();

        void set_source(WebservFile *file);
        void set_destination(WebservFile *file);

        //for cgi
        void            set_write_io(WebservFile *src, WebservFile *dst);
        void            set_read_io(WebservFile *src, WebservFile *dst);
        void            set_write_fd(FileDiscriptor const &write_fd);
        void            set_read_fd(FileDiscriptor const &read_fd);
        void            set_write_fd(FileDiscriptor &write_fd);
        void            set_read_fd(FileDiscriptor &read_fd);
        FileDiscriptor  &get_write_fd();
        FileDiscriptor  &get_read_fd();
        
        void            set_in_out(uint32_t inout);
        uint32_t        in_out();
        int             save(char *data, size_t offset, size_t size);
        int             save(char const *data, size_t offset, size_t size);
        size_t          load(char **data);
        void            clear_tmp_data();

        bool            is_read_completed();
        void            set_read_completed(bool flag);
        bool            is_write_completed();
        void            set_write_completed(bool flag);

        void switching_io(uint32_t epoll_event);
        size_t          total_write_size();
        void            set_total_write_size(size_t size);
        void            add_total_write_size(size_t size);

    private:
        WebservFile *source_;
        WebservFile *destination_;

        WebservFile *read_source_;
        WebservFile *read_destination_;
        WebservFile *write_source_;
        WebservFile *write_destination_;

        FileDiscriptor  write_fd_;
        FileDiscriptor  read_fd_;

        uint32_t in_out_;
        std::vector<char> tmp_buf;

        bool is_read_completed_;
        bool is_write_completed_;

        size_t total_write_size_;
};

#endif
