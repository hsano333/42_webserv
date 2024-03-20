#include "webserv_file_factory.hpp"
#include "global.hpp"
#include "http_exception.hpp"
#include "multi_normal_file.hpp"
//#include "socket_reader.hpp"
//#include "socket_writer.hpp"

WebservFileFactory::WebservFileFactory() : file_manager(NULL)
{
    ;
}

WebservFileFactory::~WebservFileFactory()
{
    ;
}

WebservFile *WebservFileFactory::make_normal_file(FileDiscriptor const &fd, std::string const &filepath, std::ios_base::openmode mode)
{
    NormalFile *normal_file = NormalFile::from_filepath(filepath, mode);
    return (this->make_webserv_file(fd, normal_file, DefaultFunc::open, DefaultFunc::read, DefaultFunc::write, DefaultFunc::close, DefaultFunc::remove, DefaultFunc::can_read, DefaultFunc::path, DefaultFunc::size, DummyFunc::is_chunk, DummyFunc::set_chunk));
}

WebservFile *WebservFileFactory::make_multi_normal_file(FileDiscriptor const &fd, std::string const &path, std::ios_base::openmode mode)
{
    MultiNormalFile *multi_normal_file = MultiNormalFile::from_directory_path(path, mode);
    return (this->make_webserv_file(fd, multi_normal_file, DefaultFunc::open, DefaultFunc::read, DefaultFunc::write, DefaultFunc::close, DummyFunc::remove, DummyFunc::can_read, DefaultFunc::path, DummyFunc::size, DefaultFunc::is_chunk, DefaultFunc::set_chunk));
}



WebservFile *WebservFileFactory::make_socket_file(FileDiscriptor const &fd, IWriter* iwriter, IReader* ireader)
{
    DEBUG("WebservFileFactory::make_socket_file_as_read:" + fd.to_string());
    SocketFile *socket_file = SocketFile::from_fd(fd, iwriter, ireader);
    return (this->make_webserv_file(fd, socket_file, CommonFunc::open, ChunkedFunc::read, CommonFunc::write, CommonFunc::close, DummyFunc::remove, DefaultFunc::can_read, DummyFunc::path, DummyFunc::size, DummyFunc::is_chunk, DummyFunc::set_chunk));
}


/*
WebservFile *WebservFileFactory::make_socket_file_as_read(FileDiscriptor const &fd, IWriter* iwriter, IReader* ireader, bool is_chunked)
{
    DEBUG("WebservFileFactory::make_socket_file_as_read:" + fd.to_string());
    SocketFile *socket_file = SocketFile::from_fd(fd, iwriter, ireader);
    if(is_chunked){
        return (this->make_webserv_file(fd, socket_file, CommonFunc::open, CommonFunc::read, CommonFunc::write, CommonFunc::close, DummyFunc::remove, DefaultFunc::can_read, DummyFunc::path, DummyFunc::size, DummyFunc::is_chunk));
    }else{
        return (this->make_webserv_file(fd, socket_file, CommonFunc::open, ChunkedFunc::read, CommonFunc::write, CommonFunc::close, DummyFunc::remove, DefaultFunc::can_read, DummyFunc::path, DummyFunc::size, DummyFunc::is_chunk));
    }
}

WebservFile *WebservFileFactory::make_socket_file_as_write(FileDiscriptor const &fd, IWriter* iwriter, IReader* ireader, bool is_chunked)
{
    DEBUG("WebservFileFactory::make_socket_file_as_write:" + fd.to_string());
    SocketFile *socket_file = SocketFile::from_fd(fd, iwriter, ireader);
    if(is_chunked){
        return (this->make_webserv_file(fd, socket_file, CommonFunc::open, CommonFunc::read, CommonFunc::write, CommonFunc::close, DummyFunc::remove, DefaultFunc::can_read, DummyFunc::path, DummyFunc::size, DummyFunc::is_chunk));
    }else{
        return (this->make_webserv_file(fd, socket_file, CommonFunc::open, CommonFunc::read, ChunkedFunc::write, CommonFunc::close, DummyFunc::remove, DefaultFunc::can_read, DummyFunc::path, DummyFunc::size, DummyFunc::is_chunk));
    }
}
*/

WebservFile *WebservFileFactory::make_vector_file(FileDiscriptor const &fd, size_t buf_size)
{
    VectorFile *vector_file = VectorFile::from_buf_size(buf_size);
    return (this->make_webserv_file(fd, vector_file, CommonFunc::open, DefaultFunc::read, DefaultFunc::write, CommonFunc::close, DummyFunc::remove, DummyFunc::can_read, DummyFunc::path, DefaultFunc::size, DummyFunc::is_chunk, DummyFunc::set_chunk));
}

WebservFile *WebservFileFactory::make_vector_file(FileDiscriptor const &fd, std::string const& buf_ref)
{
    VectorFile *vector_file = VectorFile::from_ref(buf_ref);
    return (this->make_webserv_file(fd, vector_file, CommonFunc::open, DefaultFunc::read, DefaultFunc::write, CommonFunc::close, DummyFunc::remove, DummyFunc::can_read, DummyFunc::path, DefaultFunc::size, DummyFunc::is_chunk, DummyFunc::set_chunk));
}

WebservFile *WebservFileFactory::make_error_file(FileDiscriptor const &fd, StatusCode const &status_code)
{
    ErrorFile *error_file = ErrorFile::from_status_code(status_code);
    return (this->make_webserv_file(fd, error_file, CommonFunc::open, CommonFunc::read, DummyFunc::write, CommonFunc::close, DummyFunc::remove, DummyFunc::can_read, DummyFunc::path, DefaultFunc::size, DummyFunc::is_chunk, DummyFunc::set_chunk));
}

WebservFile *WebservFileFactory::make_directory_file(FileDiscriptor const &fd, std::string const &path, std::string const &relative_path, std::string const &domain)
{
    DirectoryFile *directory_file = DirectoryFile::from_path(path, relative_path, domain);
    return (this->make_webserv_file(fd, directory_file));
}


WebservFileFactory *WebservFileFactory::singleton = NULL;
string WebservFileFactory::string_ref = "";
WebservFileFactory *WebservFileFactory::get_instance()
{
    if (WebservFileFactory::singleton == NULL){
        ERROR("singleton is NULL");
        throw std::runtime_error("singleton is NULL");
    }
    return (singleton);
}

WebservFileFactory *WebservFileFactory::get_instance(FileManager *file_manager)
{
    if (WebservFileFactory::singleton == NULL){
        singleton = new WebservFileFactory();
        singleton->file_manager = file_manager;
    }
    return (singleton);
}

