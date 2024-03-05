/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv_file_factory.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsano <hsano@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 20:21:21 by hsano             #+#    #+#             */
/*   Updated: 2024/03/05 21:33:27 by sano             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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


namespace DummyFunc{
int open_dummy();
int close_dummy();
int remove_dummy();
bool can_read_dummy();
int read_dummy(char **data, size_t size);
int write_dummy(char **data, size_t size);
//int remove_dummy();
bool can_read_dummy();
string &path_dummy();
};

typedef int(* FUNC)(void);
typedef int(* IO_FUNC)(char **data, size_t size);
typedef bool(* BOOL_FUNC)();
typedef string&(* STRING_FUNC)();

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
        WebservFile *make_webserv_file(FileDiscriptor const &fd, FileT *file, FUNC open, FUNC close, IO_FUNC read, IO_FUNC write, FUNC remove, BOOL_FUNC can_read, STRING_FUNC path);
        //
        WebservFile *make_normal_file(FileDiscriptor const &fd, std::string const &filepath, std::ios_base::openmode mode);
        WebservFile *make_socket_file(FileDiscriptor const &fd, IWriter* iwriter, IReader* ireader);
        WebservFile *make_error_file(FileDiscriptor const &fd, StatusCode const &status_code);
        //WebservFile *make_socket_file(FileDiscriptor &fd, IWriter* iwriter, IReader* ireader);
        //WebservFile *make_socket_file(FileDiscriptor &fd, IWriter* iwriter, IReader* ireader);
        WebservFile *make_vector_file(FileDiscriptor const &fd, size_t buf_size);
        WebservFile *make_vector_file(FileDiscriptor const &fd, std::string const& buf_ref);
        WebservFile *make_directory_file(FileDiscriptor const &fd, std::string const &path, std::string const &relative_path, std::string const &domain);
    private:
        WebservFileFactory();
        //WebservFileFactory(FileManager *file_manager);
        static WebservFileFactory *singleton;
        FileManager *file_manager;
};


template <class FileT>
WebservFile *WebservFileFactory::make_webserv_file(FileDiscriptor const &fd, FileT *file, FUNC open, FUNC close, IO_FUNC read, IO_FUNC write, FUNC remove, BOOL_FUNC can_read, STRING_FUNC path)
{
    WebservFile *new_file = new WebservFile(file, open, read, write, close, remove, can_read);
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


template <class FileT>
WebservFile *WebservFileFactory::make_webserv_file(FileDiscriptor const &fd, FileT *file)
{
    WebservFile *new_file = new WebservFile(file, DefaultFunc::open<FileT>, DefaultFunc::read<FileT>, DefaultFunc::write<FileT>, DefaultFunc::close<FileT>, DefaultFunc::remove<FileT>, DefaultFunc::can_read<FileT>, DefaultFunc::path<FileT>);
    this->file_manager->insert(fd, new_file);
    return (new_file);
}


#endif

