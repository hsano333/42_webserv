#include "webserv_file_factory.hpp"
#include "global.hpp"
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
    return (this->make_webserv_file(fd, normal_file, DefaultFunc::open, DefaultFunc::read, DefaultFunc::write, DefaultFunc::close, DefaultFunc::remove, DefaultFunc::can_read, DefaultFunc::path));
}

WebservFile *WebservFileFactory::make_socket_file(FileDiscriptor const &fd, IWriter* iwriter, IReader* ireader)
{
    DEBUG("WebservFileFactory::make_socket_file:" + fd.to_string());
    //SocketReader *socket_reader = SocketReader::get_instance();
    //SocketWriter *socket_writer = SocketWriter::get_instance();
    SocketFile *socket_file = SocketFile::from_fd(fd, iwriter, ireader);
    return (this->make_webserv_file(fd, socket_file, DefaultFunc::open, DefaultFunc::read, DefaultFunc::write, DefaultFunc::close, DefaultFunc::remove, DefaultFunc::can_read, DefaultFunc::path));
}

WebservFile *WebservFileFactory::make_vector_file(FileDiscriptor const &fd, size_t buf_size)
{
    VectorFile *vector_file = VectorFile::from_buf_size(buf_size);
    return (this->make_webserv_file(fd, vector_file));
}

WebservFile *WebservFileFactory::make_vector_file(FileDiscriptor const &fd, std::string const& buf_ref)
{
    VectorFile *vector_file = VectorFile::from_ref(buf_ref);
    return (this->make_webserv_file(fd, vector_file));
}

WebservFile *WebservFileFactory::make_error_file(FileDiscriptor const &fd, StatusCode const &status_code)
{
    ErrorFile *error_file = ErrorFile::from_status_code(status_code);
    return (this->make_webserv_file(fd, error_file));
}

WebservFile *WebservFileFactory::make_directory_file(FileDiscriptor const &fd, std::string const &path, std::string const &relative_path, std::string const &domain)
{
    DirectoryFile *directory_file = DirectoryFile::from_path(path, relative_path, domain);
    return (this->make_webserv_file(fd, directory_file));
}


WebservFileFactory *WebservFileFactory::singleton = NULL;
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



