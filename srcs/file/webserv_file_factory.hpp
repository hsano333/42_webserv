
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
        WebservFile *make_webserv_file(FileDiscriptor const &fd, FileT *file);
        template <class FileT>
        WebservFile *make_webserv_file(FileDiscriptor const &fd, FileT *file, int(* open)(FileT *));
        template <class FileT>
        //WebservFile *make_webserv_file(FileDiscriptor const &fd, FileT *file, FUNC open, FUNC close, IO_FUNC read, IO_FUNC write, FUNC remove, BOOL_FUNC can_read, STRING_FUNC path);
        WebservFile *make_webserv_file(FileDiscriptor const &fd, FileT *file, int(* open)(FileT *), int(*  read)(FileT *, char **data, size_t size), int(*  write)(FileT *, char **data, size_t size), int(* close)(FileT *),  int(*  remove)(FileT *), bool(*  can_read)(FileT *), string const&(*  path)(FileT *));
        //
        WebservFile *make_normal_file(FileDiscriptor const &fd, std::string const &filepath, std::ios_base::openmode mode);
        WebservFile *make_socket_file(FileDiscriptor const &fd, IWriter* iwriter, IReader* ireader);
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


template <class FileT>
WebservFile *WebservFileFactory::make_webserv_file(FileDiscriptor const &fd, FileT *file, int(* open)(FileT *), int(*  read)(FileT *, char **data, size_t size), int(*  write)(FileT *, char **data, size_t size), int(* close)(FileT *),  int(*  remove)(FileT *), bool(*  can_read)(FileT *), string const&(*  path)(FileT *))
{
    WebservFile *new_file = new WebservFile(file, open, read, write, close, remove, can_read, path);
    this->file_manager->insert(fd, new_file);
    return (new_file);
}


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
}


template <class FileT>
WebservFile *WebservFileFactory::make_webserv_file(FileDiscriptor const &fd, FileT *file)
{
    WebservFile *new_file = new WebservFile(file, DefaultFunc::open<FileT>, DefaultFunc::read<FileT>, DefaultFunc::write<FileT>, DefaultFunc::close<FileT>, DefaultFunc::remove<FileT>, DefaultFunc::can_read<FileT>, DefaultFunc::path<FileT>);
    this->file_manager->insert(fd, new_file);
    return (new_file);
}

template <class FileT>
WebservFile *WebservFileFactory::make_webserv_file(FileDiscriptor const &fd, FileT *file, int(* open)(FileT *))
{
    (void)open;
    WebservFile *new_file = new WebservFile(file, DefaultFunc::open<FileT>, DefaultFunc::read<FileT>, DefaultFunc::write<FileT>, DefaultFunc::close<FileT>, DefaultFunc::remove<FileT>, DefaultFunc::can_read<FileT>, DefaultFunc::path<FileT>);
    this->file_manager->insert(fd, new_file);
    return (new_file);

}

#endif

