
#ifndef WEBSERV_FILE_FACTORY_HPP
#define WEBSERV_FILE_FACTORY_HPP

#include "webserv_file.hpp"
#include "file_manager.hpp"
#include "global.hpp"
#include "header_word.hpp"
#include "iwriter.hpp"
#include "ireader.hpp"
#include "status_code.hpp"
#include "normal_file.hpp"
#include "vector_file.hpp"
#include "error_file.hpp"
#include "directory_file.hpp"
#include "socket_file.hpp"
#include "http_exception.hpp"
#include "utility.hpp"
#include "split.hpp"
#include "header.hpp"
#include "request.hpp"
#include "application_result.hpp"
#include <cstdio>
#include <stdlib.h>





/*
namespace DummyFunc{
int open_dummy();
int close_dummy();
int remove_dummy();
bool can_read_dummy();
int read_dummy(char **data, size_t size);
int write_dummy(char **data, size_t size);
//int remove_dummy();
bool can_read_dummy();
string const &path_dummy();
};
*/

typedef int(* FUNC)(void);
typedef int(* IO_FUNC)(char **data, size_t size);
typedef bool(* BOOL_FUNC)();
typedef const string&(* STRING_FUNC)();

class WebservFileFactory
{
    public:
        static WebservFileFactory *get_instance(FileManager *file_manager);
        static WebservFileFactory *get_instance();
        ~WebservFileFactory();
        //File *make();
        //
        template <class FileT>
        WebservFile *make_webserv_directory_file(FileDiscriptor const &fd, FileT *file);
        template <class FileT>
        WebservFile *make_webserv_file(FileDiscriptor const &fd, FileT *file);

        template <class FileT>
        WebservFile *make_result_file_for_cgi(FileDiscriptor const &fd,  FileT *file);
        template <class FileT>
        WebservFile *make_webserv_file_regular(FileDiscriptor const &fd, FileT *file);
        template <class FileT>
        WebservFile *make_webserv_file(FileDiscriptor const &fd, FileT *file, int(* open)(FileT *));
        template <class FileT>
        //WebservFile *make_webserv_file(FileDiscriptor const &fd, FileT *file, FUNC open, FUNC close, IO_FUNC read, IO_FUNC write, FUNC remove, BOOL_FUNC can_read, STRING_FUNC path);
        WebservFile *make_webserv_file(FileDiscriptor const &fd, FileT *file, int(* open)(FileT *), int(*  read)(FileT *, char **data, size_t size), int(*  write)(FileT *, char **data, size_t size), int(* close)(FileT *),  int(*  remove)(FileT *), bool(*  can_read)(FileT *), bool(*  can_write)(FileT *), string const&(*  path)(FileT *), size_t(size)(FileT *), bool(is_chunk)(FileT *), void(set_chunk)(FileT *, bool flag), bool(completed)(FileT *));
        //
        WebservFile *make_normal_file(FileDiscriptor const &fd, std::string const &filepath, std::ios_base::openmode mode);
        WebservFile *make_pipe_file(FileDiscriptor const &fd, FileDiscriptor const &pipe_fd , IReader *reader);
        WebservFile *make_multi_normal_file(std::string const &directory_path, std::string const &boundary, FileDiscriptor const &fd);
        WebservFile *make_pipe_file(FileDiscriptor const &fd, WebservFile *file, IWriter* iwriter, IReader* ireader);
        WebservFile *make_socket_file(FileDiscriptor const &registrered_fd, FileDiscriptor const &fd, IWriter* iwriter, IReader* ireader);
        WebservFile *make_socket_file(FileDiscriptor const &fd, IWriter* iwriter, IReader* ireader);
        WebservFile *make_socket_file(FileDiscriptor const &fd, WebservFile *file, IWriter* iwriter, IReader* ireader);
        WebservFile *make_socket_file_for_post_cgi(FileDiscriptor const &fd, WebservFile *file, IWriter* iwriter, IReader* ireader);
        WebservFile *make_socket_chunk_file(FileDiscriptor const &fd, WebservFile *file);
        WebservFile *make_socket_chunk_file_for_post_cgi(FileDiscriptor const &fd, WebservFile *file);
        WebservFile *make_socket_chunk_file_for_autoindex(FileDiscriptor const &fd, WebservFile *file);
        WebservFile *make_socket_chunk_file_for_write(FileDiscriptor const &fd, WebservFile *file);
        WebservFile *make_socket_chunk_file_for_write(FileDiscriptor const &fd, WebservFile *file, std::vector<char> &buffer );
        WebservFile *make_error_file(FileDiscriptor const &fd, StatusCode const &status_code);
        WebservFile *make_result_file_for_cgi(FileDiscriptor const &fd, ApplicationResult *file);
        WebservFile *make_vector_file_for_cgi(FileDiscriptor const &fd, size_t buf_size);
        WebservFile *make_vector_file_for_socket(FileDiscriptor const &fd, size_t buf_size);
        WebservFile *make_vector_file(FileDiscriptor const &fd, std::string const& buf_ref);
        WebservFile *make_vector_file(FileDiscriptor const &fd, char *buf, size_t size);
        WebservFile *make_directory_file(FileDiscriptor const &fd, std::string const &path, std::string const &relative_path, std::string const &domain);
        WebservFile *make_request_file(FileDiscriptor const &fd, Request *req);
        WebservFile *make_request_file_read_buf(FileDiscriptor const &fd, Request *req);

        // for dummy_func; not use;
        static string string_ref;
    private:
        WebservFileFactory();
        static WebservFileFactory *singleton;
        FileManager *file_manager;
};

namespace DefaultFunc{
    template <class FileT>
    int open(FileT *file){
        DEBUG("(Default open()");
        return (file->open());
    }

    template <class FileT>
    int close(FileT *file){
        DEBUG("Default close()");
        return (file->close());
    }

    template <class FileT>
    int remove(FileT *file){
        DEBUG("Default remove()");
        return (file->remove());
    }

    template <class FileT>
    bool can_read(FileT *file){
        DEBUG("Default can_read()");
        return (file->can_read());
    }
    template <class FileT>
    bool can_write(FileT *file){
        DEBUG("Default can_write()");
        return (file->can_write());
    }

    template <class FileT>
    int read(FileT *file, char **data, size_t size)
    {
        DEBUG("Default read()");
        (void)data;
        (void)size;
        return (file->read(data, size));
    }

    template <class FileT>
    int write(FileT *file, char **data, size_t size){
        DEBUG("Default write()");
        (void)data;
        (void)size;
        return (file->write(data, size));
    }

    template <class FileT>
    std::string const &path(FileT *file){
        DEBUG("Default path()");
        return (file->path());
    }

    template <class FileT>
    size_t size(FileT *file){
        DEBUG("Default size()");
        return (file->size());
    }

    template <class FileT>
    bool is_chunk(FileT *file){
        DEBUG("Default is_chunk()");
        return (file->is_chunk());
    }
    template <class FileT>
    void set_chunk(FileT *file, bool flag){
        DEBUG("Default is_chunk()");
        return (file->set_chunk(flag));
    }
    template <class FileT>
    bool completed(FileT *file){
        DEBUG("Default completed()");
        return (file->completed());
    }
}

namespace CommonFunc{
    template <class FileT>
    int open(FileT *file){
        DEBUG("Common open()");
        file->state = FILE_OPEN;
        return 0;
    }
    template <class FileT>
    int close(FileT *file){
        DEBUG("Common close()");
        file->state = FILE_CLOSE;
        return 0;
    }
    template <class FileT>
    int read(FileT *file, char **data, size_t size)
    {
        DEBUG("Common read()");
        if (file->state != FILE_OPEN){
            return (0);
        }

        file->state = FILE_COMPLETED_READ;
        return (file->read(data, size));
    }
    template <class FileT>
    int write(FileT *file, char **data, size_t size)
    {
        DEBUG("Common write()");
        if (!(file->state == FILE_OPEN || file->state == FILE_WRITING)){
            return (0);
        }
        file->state = FILE_WRITING;
        return (file->write(data, size));
    }
}


namespace DummyFunc{
    template <class FileT>
    int open(FileT *file){
        (void)file;
        DEBUG("open_dummy()");
        return 0;
    }
    template <class FileT>
    int close(FileT *file){
        (void)file;
        DEBUG("close_dummy()");
        return 0;
    }
    template <class FileT>
    int remove(FileT *file){
        (void)file;
        DEBUG("remove_dummy()");
        return 0;
    }
    template <class FileT>
    bool can_read(FileT *file){
        (void)file;
        DEBUG("can_read_dummy()");
        return true;
    }
    template <class FileT>
    bool can_write(FileT *file){
        (void)file;
        DEBUG("can_write_dummy()");
        return true;
    }

    template <class FileT>
    int read(FileT *file, char **data, size_t size){
        (void)file;
        DEBUG("read_dummy()");
        (void)data;
        (void)size;
        return 0;
    }
    template <class FileT>
    int write(FileT *file, char **data, size_t size){
        (void)file;
        DEBUG("write_dummy()");
        (void)data;
        (void)size;
        return 0;
    }

    template <class FileT>
    std::string const &path(FileT *file){
        (void)file;
        DEBUG("path_dummy()");
        return (WebservFileFactory::string_ref);
    }
    template <class FileT>
    size_t size(FileT *file){
        (void)file;
        DEBUG("size_dummy()");
        return (0);
    }
    template <class FileT>
    bool is_chunk(FileT *file){
        (void)file;
        DEBUG("is_chunk_dummy()");
        return (false);
    }
    template <class FileT>
    void set_chunk(FileT *file, bool flag){
        DEBUG("is_chunk_dummy()");
        (void)file;
        (void)flag;
    }
    template <class FileT>
    bool completed(FileT *file){
        (void)file;
        DEBUG("Dummy completed()");
        return (false);
    }
}


template <class FileT>
WebservFile *WebservFileFactory::make_webserv_file(FileDiscriptor const &fd, FileT *file)
{
    WebservFile *new_file = new WebservFile(file, DefaultFunc::open<FileT>, DefaultFunc::read<FileT>, DefaultFunc::write<FileT>, DefaultFunc::close<FileT>, DefaultFunc::remove<FileT>, DefaultFunc::can_read<FileT>, DummyFunc::can_write<FileT>, DefaultFunc::path<FileT>, DefaultFunc::size<FileT>, DummyFunc::is_chunk<FileT>, DummyFunc::set_chunk<FileT>, DummyFunc::completed<FileT> );
    this->file_manager->insert(fd, new_file);
    return (new_file);
}

template <class FileT>
WebservFile *WebservFileFactory::make_webserv_directory_file(FileDiscriptor const &fd, FileT *file)
{
    WebservFile *new_file = new WebservFile(file, DefaultFunc::open<FileT>, DefaultFunc::read<FileT>, DefaultFunc::write<FileT>, DefaultFunc::close<FileT>, DefaultFunc::remove<FileT>, DefaultFunc::can_read<FileT>, DummyFunc::can_write<FileT>, DefaultFunc::path<FileT>, DefaultFunc::size<FileT>, DummyFunc::is_chunk<FileT>, DummyFunc::set_chunk<FileT>);
    this->file_manager->insert(fd, new_file);
    return (new_file);
}

template <class FileT>
WebservFile *WebservFileFactory::make_result_file_for_cgi(FileDiscriptor const &fd,  FileT *file)
{
    WebservFile *new_file = new WebservFile(file, DefaultFunc::open<FileT>, DefaultFunc::read<FileT>, DefaultFunc::write<FileT>, DefaultFunc::close<FileT>, DummyFunc::remove<FileT>, DummyFunc::can_read<FileT>, DummyFunc::can_write<FileT>, DummyFunc::path<FileT>, DummyFunc::size<FileT>, DefaultFunc::is_chunk<FileT>, DummyFunc::set_chunk<FileT>, CheckSocketReadEndFunc::completed<FileT>);
    this->file_manager->insert(fd, new_file);
    return (new_file);
}

template <class FileT>
WebservFile *WebservFileFactory::make_webserv_file_regular(FileDiscriptor const &fd, FileT *file)
{
    //(void)open;
    WebservFile *new_file = new WebservFile(file, DefaultFunc::open<FileT>, DefaultFunc::read<FileT>, DefaultFunc::write<FileT>, DefaultFunc::close<FileT>, DummyFunc::remove<FileT>, DummyFunc::can_read<FileT>, DummyFunc::can_write<FileT>, DummyFunc::path<FileT>, DummyFunc::size<FileT>, DefaultFunc::is_chunk<FileT>, DummyFunc::set_chunk<FileT>, DummyFunc::completed<FileT>);
    this->file_manager->insert(fd, new_file);
    return (new_file);
}

//WebservFile *WebservFileFactory::make_webserv_file(FileDiscriptor const &fd, FileT *file, int(* open)(FileT *), int(*  read)(FileT *, char **data, size_t size), int(*  write)(FileT *, char **data, size_t size), int(* close)(FileT *),  int(*  remove)(FileT *), bool(*  can_read)(FileT *), string const&(*  path)(FileT *), size_t(size)(FileT *), bool(is_chunk)(FileT *))


template <class FileT>
//WebservFile *WebservFileFactory::make_webserv_file(FileDiscriptor const &fd, FileT *file, int(* open)(FileT *), int(*  read)(FileT *, char **data, size_t size), int(*  write)(FileT *, char **data, size_t size), int(* close)(FileT *),  int(*  remove)(FileT *), bool(*  can_read)(FileT *), string const&(*  path)(FileT *), size_t(size)(FileT *), bool(is_chunk)(FileT *), void(set_chunk)(FileT *, bool flag))
WebservFile *WebservFileFactory::make_webserv_file(FileDiscriptor const &fd, FileT *file, int(* open)(FileT *), int(*  read)(FileT *, char **data, size_t size), int(*  write)(FileT *, char **data, size_t size), int(* close)(FileT *),  int(*  remove)(FileT *), bool(*  can_read)(FileT *), bool(*  can_write)(FileT *), string const&(*  path)(FileT *), size_t(size)(FileT *), bool(is_chunk)(FileT *), void(set_chunk)(FileT *, bool flag), bool(completed)(FileT *) )
{
    WebservFile *new_file = new WebservFile(file, open, read, write, close, remove, can_read, can_write, path, size, is_chunk, set_chunk, completed);
    this->file_manager->insert(fd, new_file);
    return (new_file);
}

#endif

