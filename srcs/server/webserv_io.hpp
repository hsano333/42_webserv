#ifndef WEBSERV_IO
#define WEBSERV_IO

#include "epoll.hpp"
#include "file.hpp"
#include "global.hpp"

class WebservIO
{
    public:
        WebservIO();
        ~WebservIO();
        File *source();
        File *destination();
        File *source_for_read();
        File *destination_for_read();
        File *source_for_write();
        File *destination_for_write();

        void set_source(File *file);
        void set_destination(File *file);
        /*
        void set_source_for_read(File *file);
        void set_destination_for_read(File *file);
        void set_source_for_write(File *file);
        void set_destination_for_write(File *file);
        */

        //for cgi
        void            set_write_io(File *src, File *dst);
        void            set_read_io(File *src, File *dst);
        void            set_write_fd(FileDiscriptor const &write_fd);
        void            set_read_fd(FileDiscriptor const &read_fd);
        void            set_write_fd(FileDiscriptor &write_fd);
        void            set_read_fd(FileDiscriptor &read_fd);
        FileDiscriptor  &get_write_fd();
        FileDiscriptor  &get_read_fd();
        
        void            set_in_out(uint32_t inout);
        uint32_t        in_out();


        void switching_io(uint32_t epoll_event);

    private:
        File *source_;
        File *destination_;

        File *read_source_;
        File *read_destination_;
        File *write_source_;
        File *write_destination_;

        FileDiscriptor  write_fd_;
        FileDiscriptor  read_fd_;

        uint32_t in_out_;
};

#endif
