
#ifndef WEBSERV_FILE_FACTORY_HPP
#define WEBSERV_FILE_FACTORY_HPP

#include "webserv_file.hpp"
#include "file_manager.hpp"
#include "global.hpp"
#include "iwriter.hpp"
#include "ireader.hpp"
#include "status_code.hpp"
#include "normal_file.hpp"
#include "vector_file.hpp"
#include "error_file.hpp"
#include "directory_file.hpp"
#include "socket_file.hpp"
#include "http_exception.hpp"





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
        WebservFile *make_webserv_file_regular(FileDiscriptor const &fd, FileT *file);
        template <class FileT>
        WebservFile *make_webserv_file(FileDiscriptor const &fd, FileT *file, int(* open)(FileT *));
        template <class FileT>
        //WebservFile *make_webserv_file(FileDiscriptor const &fd, FileT *file, FUNC open, FUNC close, IO_FUNC read, IO_FUNC write, FUNC remove, BOOL_FUNC can_read, STRING_FUNC path);
        WebservFile *make_webserv_file(FileDiscriptor const &fd, FileT *file, int(* open)(FileT *), int(*  read)(FileT *, char **data, size_t size), int(*  write)(FileT *, char **data, size_t size), int(* close)(FileT *),  int(*  remove)(FileT *), bool(*  can_read)(FileT *), string const&(*  path)(FileT *), size_t(size)(FileT *), bool(is_chunk)(FileT *), void(set_chunk)(FileT *, bool flag));
        //
        WebservFile *make_normal_file(FileDiscriptor const &fd, std::string const &filepath, std::ios_base::openmode mode);
        WebservFile *make_multi_normal_file(FileDiscriptor const &fd, std::string const &filepath, std::ios_base::openmode mode);
        WebservFile *make_socket_file(FileDiscriptor const &fd, IWriter* iwriter, IReader* ireader);
        //WebservFile *make_socket_file_as_write(FileDiscriptor const &fd, IWriter* iwriter, IReader* ireader, bool is_chunked);
        WebservFile *make_error_file(FileDiscriptor const &fd, StatusCode const &status_code);
        //WebservFile *make_socket_file(FileDiscriptor &fd, IWriter* iwriter, IReader* ireader);
        //WebservFile *make_socket_file(FileDiscriptor &fd, IWriter* iwriter, IReader* ireader);
        WebservFile *make_vector_file(FileDiscriptor const &fd, size_t buf_size);
        WebservFile *make_vector_file(FileDiscriptor const &fd, std::string const& buf_ref);
        WebservFile *make_directory_file(FileDiscriptor const &fd, std::string const &path, std::string const &relative_path, std::string const &domain);

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
        (file->close());
        DEBUG("Default close() No.1");
        return 0;
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
namespace ChunkedFunc{


    template <class FileT>
    size_t get_chunked_size(FileT *file, char **buf)
    {
        char tmp_buf[21] = {0};
        char *tmp_pbuf = tmp_buf;
        ssize_t read_size = file->read(&tmp_pbuf, 20);
        //ssize_t read_size = ::recv(fd.to_int(), tmp_buf, size, 20);
        char *pos = Utility::strnstr(tmp_buf, CRLF, read_size);
        int exceed_str_size = 20 - (pos - &(tmp_buf[0])+2);
        if(pos){
            pos[0] = '\0';
        }else{
            ERROR(" not chunked format");
            throw HttpException("400");
        }
        pos += 2;
        Utility::memcpy(*buf, pos, exceed_str_size);
        *buf += exceed_str_size;

        return (Utility::hex_string_to_number(pos));
        //if(chunked_size == 0){
            //return (0);
        //}
    }

    template <class FileT>
    int read(FileT *file, char **data, size_t size)
    {
        DEBUG("Chunked read()");

        if(file->is_chunked() == false){
            return (DefaultFunc::read(file, data, size));
        }

        if (file->state != FILE_OPEN){
            return (0);
        }

        size_t chunked_size = file->chunked_size();
        if(chunked_size == 0){
            chunked_size = get_chunked_size(file, data);
        }
        MYINFO("SocketReaderChunked::chunked_size = " + Utility::to_string(chunked_size));
        ssize_t read_size;
        if(chunked_size > size){
            read_size = file->read(data, size);
        }else{
            read_size = file->read(data, chunked_size);
        }
        if(read_size > 0){
            chunked_size -= read_size;
        }
        file->set_chunked_size(chunked_size);

        return (read_size);
    }
    template <class FileT>
    int write(FileT *file, char **data, size_t size)
    {
        DEBUG("Chunked write()");
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
}


template <class FileT>
WebservFile *WebservFileFactory::make_webserv_file(FileDiscriptor const &fd, FileT *file)
{
    WebservFile *new_file = new WebservFile(file, DefaultFunc::open<FileT>, DefaultFunc::read<FileT>, DefaultFunc::write<FileT>, DefaultFunc::close<FileT>, DefaultFunc::remove<FileT>, DefaultFunc::can_read<FileT>, DefaultFunc::path<FileT>, DefaultFunc::size<FileT>, DummyFunc::is_chunk<FileT>, DummyFunc::set_chunk<FileT> );
    this->file_manager->insert(fd, new_file);
    return (new_file);
}

template <class FileT>
WebservFile *WebservFileFactory::make_webserv_directory_file(FileDiscriptor const &fd, FileT *file)
{
    WebservFile *new_file = new WebservFile(file, DefaultFunc::open<FileT>, DefaultFunc::read<FileT>, DefaultFunc::write<FileT>, DefaultFunc::close<FileT>, DefaultFunc::remove<FileT>, DefaultFunc::can_read<FileT>, DefaultFunc::path<FileT>, DefaultFunc::size<FileT>, DummyFunc::is_chunk<FileT>, DummyFunc::set_chunk<FileT>);
    this->file_manager->insert(fd, new_file);
    return (new_file);
}

template <class FileT>
WebservFile *WebservFileFactory::make_webserv_file_regular(FileDiscriptor const &fd, FileT *file)
{
    //(void)open;
    WebservFile *new_file = new WebservFile(file, DefaultFunc::open<FileT>, DefaultFunc::read<FileT>, DefaultFunc::write<FileT>, DefaultFunc::close<FileT>, DummyFunc::remove<FileT>, DummyFunc::can_read<FileT>, DummyFunc::path<FileT>, DummyFunc::size<FileT>, DummyFunc::is_chunk<FileT>, DummyFunc::set_chunk<FileT>);
    this->file_manager->insert(fd, new_file);
    return (new_file);
}

//WebservFile *WebservFileFactory::make_webserv_file(FileDiscriptor const &fd, FileT *file, int(* open)(FileT *), int(*  read)(FileT *, char **data, size_t size), int(*  write)(FileT *, char **data, size_t size), int(* close)(FileT *),  int(*  remove)(FileT *), bool(*  can_read)(FileT *), string const&(*  path)(FileT *), size_t(size)(FileT *), bool(is_chunk)(FileT *))


template <class FileT>
//WebservFile *WebservFileFactory::make_webserv_file(FileDiscriptor const &fd, FileT *file, int(* open)(FileT *), int(*  read)(FileT *, char **data, size_t size), int(*  write)(FileT *, char **data, size_t size), int(* close)(FileT *),  int(*  remove)(FileT *), bool(*  can_read)(FileT *), string const&(*  path)(FileT *), size_t(size)(FileT *), bool(is_chunk)(FileT *), void(set_chunk)(FileT *, bool flag))
WebservFile *WebservFileFactory::make_webserv_file(FileDiscriptor const &fd, FileT *file, int(* open)(FileT *), int(*  read)(FileT *, char **data, size_t size), int(*  write)(FileT *, char **data, size_t size), int(* close)(FileT *),  int(*  remove)(FileT *), bool(*  can_read)(FileT *), string const&(*  path)(FileT *), size_t(size)(FileT *), bool(is_chunk)(FileT *), void(set_chunk)(FileT *, bool flag) )
{
    WebservFile *new_file = new WebservFile(file, open, read, write, close, remove, can_read, path, size, is_chunk, set_chunk);
    this->file_manager->insert(fd, new_file);
    return (new_file);
}

#endif

