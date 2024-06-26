#include "webserv_file_factory.hpp"
#include "global.hpp"
#include "http_exception.hpp"
#include "multi_normal_file.hpp"
#include "socket_chunk_file.hpp"
#include "pipe_file.hpp"
#include "vector_read_cgi_file.hpp"

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
    DEBUG("WebservFileFactory::make_normal_file file:" + filepath);
    NormalFile *normal_file = NormalFile::from_filepath(filepath, mode);
    return (this->make_webserv_file(fd, normal_file, DefaultFunc::open, DefaultFunc::read, DefaultFunc::write, DefaultFunc::close, DefaultFunc::remove, DefaultFunc::can_read, DefaultFunc::can_write, DefaultFunc::path, DefaultFunc::size, DummyFunc::is_chunk, DummyFunc::set_chunk, DummyFunc::completed));
}

WebservFile *WebservFileFactory::make_multi_normal_file(std::string const &directory_path, std::string const &boundary, FileDiscriptor const &fd)
{
    DEBUG("WebservFileFactory::make_multi_normal_file()");
    MultiNormalFile *multi_normal_file = MultiNormalFile::from_directory_path(directory_path, boundary, fd);
    return (this->make_webserv_file(fd, multi_normal_file, DummyFunc::open, MultiFileFunc::read_result, MultiFileFunc::write, DefaultFunc::close, DummyFunc::remove, DummyFunc::can_read, DummyFunc::can_write, DefaultFunc::path, DummyFunc::size, DefaultFunc::is_chunk, DefaultFunc::set_chunk, DefaultFunc::completed));
}

WebservFile *WebservFileFactory::make_pipe_file(FileDiscriptor const &fd, FileDiscriptor const &pipe_fd, IReader *reader)
{

    DEBUG("WebservFileFactory::make_pipe_file:" + fd.to_string());
    PipeFile *pipe_file = PipeFile::from_fd(pipe_fd, NULL, reader);
    return (this->make_webserv_file(fd, pipe_file, DummyFunc::open, DefaultFunc::read, DummyFunc::write, DefaultFunc::close, DummyFunc::remove, DummyFunc::can_read, DummyFunc::can_write, DummyFunc::path, DummyFunc::size, DummyFunc::is_chunk, DummyFunc::set_chunk, DummyFunc::completed));
}

WebservFile *WebservFileFactory::make_pipe_file(FileDiscriptor const &fd, WebservFile *file, IWriter* iwriter, IReader* ireader)
{
    DEBUG("WebservFileFactory::make_pipe_file with file:" + fd.to_string());
    PipeFile *socket_file = PipeFile::from_file(fd, file, iwriter, ireader);
    return (this->make_webserv_file(fd, socket_file, CommonFunc::open, BufferFunc::read, BufferFunc::write, CommonFunc::close, DummyFunc::remove,DummyFunc::can_read, DummyFunc::can_write, DummyFunc::path, DefaultFunc::size, DummyFunc::is_chunk, DummyFunc::set_chunk, DummyFunc::completed));
}


WebservFile *WebservFileFactory::make_socket_file(FileDiscriptor const &registered_fd, FileDiscriptor const &fd, IWriter* iwriter, IReader* ireader)
{
    DEBUG("WebservFileFactory::make_socket_file_as_read:" + fd.to_string());
    SocketFile *socket_file = SocketFile::from_fd(fd, iwriter, ireader);
    return (this->make_webserv_file(registered_fd, socket_file, CommonFunc::open, CommonFunc::read, CommonFunc::write, CommonFunc::close, DummyFunc::remove, DummyFunc::can_read, DummyFunc::can_write, DummyFunc::path, DefaultFunc::size, DefaultFunc::is_chunk, DummyFunc::set_chunk, DummyFunc::completed));
}

WebservFile *WebservFileFactory::make_socket_file(FileDiscriptor const &fd, WebservFile *file, IWriter* iwriter, IReader* ireader)
{
    DEBUG("WebservFileFactory::make_socket_file with file:" + fd.to_string());
    SocketFile *socket_file = SocketFile::from_file(fd, file, iwriter, ireader);
    return (this->make_webserv_file(fd, socket_file, CommonFunc::open, BufferFunc::read, BufferFunc::write, CommonFunc::close, DummyFunc::remove, DummyFunc::can_read, DummyFunc::can_write, DummyFunc::path, DefaultFunc::size, DummyFunc::is_chunk, DummyFunc::set_chunk, DummyFunc::completed));
}

WebservFile *WebservFileFactory::make_socket_file_for_post_cgi(FileDiscriptor const &fd, WebservFile *file, IWriter* iwriter, IReader* ireader)
{
    DEBUG("WebservFileFactory::make_socket_file_for_post_cgi with file:" + fd.to_string());
    SocketFile *socket_file = SocketFile::from_file(fd, file, iwriter, ireader);
    return (this->make_webserv_file(fd, socket_file, CommonFunc::open, BufferFunc::read, BufferFunc::write, CommonFunc::close, DummyFunc::remove, BufferFunc::can_read, DummyFunc::can_write, DummyFunc::path, DefaultFunc::size, DummyFunc::is_chunk, DummyFunc::set_chunk, DummyFunc::completed));
}

WebservFile *WebservFileFactory::make_socket_chunk_file_for_post_cgi(FileDiscriptor const &fd, WebservFile *file)
{
    DEBUG("WebservFileFactory::make_socket_chunk_file:" + fd.to_string());
    SocketChunkFile *socket_file = SocketChunkFile::from_file(fd, file);
    return (this->make_webserv_file(fd, socket_file, DefaultFunc::open, ChunkedFunc::read, DummyFunc::write, CommonFunc::close, DummyFunc::remove, BufferFunc::can_read, DummyFunc::can_write, DummyFunc::path, DefaultFunc::size, DummyFunc::is_chunk, DummyFunc::set_chunk, DummyFunc::completed));
}

WebservFile *WebservFileFactory::make_socket_chunk_file_for_autoindex(FileDiscriptor const &fd, WebservFile *file)
{
    DEBUG("WebservFileFactory::make_socket_chunk_file:" + fd.to_string());
    SocketChunkFile *socket_file = SocketChunkFile::from_file(fd, file);
    return (this->make_webserv_file(fd, socket_file, DefaultFunc::open, ChunkedFunc::read_for_autoindex, DummyFunc::write, CommonFunc::close, DummyFunc::remove, DummyFunc::can_read, DummyFunc::can_write, DummyFunc::path, DefaultFunc::size, ChunkedFunc::is_chunk, DummyFunc::set_chunk, DummyFunc::completed));
}

WebservFile *WebservFileFactory::make_socket_chunk_file(FileDiscriptor const &fd, WebservFile *file)
{
    DEBUG("WebservFileFactory::make_socket_chunk_file:" + fd.to_string());
    SocketChunkFile *socket_file = SocketChunkFile::from_file(fd, file);
    return (this->make_webserv_file(fd, socket_file, DefaultFunc::open, ChunkedFunc::read_for_write, DummyFunc::write, CommonFunc::close, DummyFunc::remove, DummyFunc::can_read, DummyFunc::can_write, DummyFunc::path, DefaultFunc::size, ChunkedFunc::is_chunk, DummyFunc::set_chunk, DummyFunc::completed));
}

WebservFile *WebservFileFactory::make_socket_chunk_file_for_write(FileDiscriptor const &fd, WebservFile *file)
{
    DEBUG("WebservFileFactory::make_socket_chunk_file:" + fd.to_string());
    SocketChunkFile *socket_file = SocketChunkFile::from_file(fd, file);
    return (this->make_webserv_file(fd, socket_file, DefaultFunc::open, ChunkedFunc::read_for_write, DummyFunc::write, CommonFunc::close, DummyFunc::remove, DummyFunc::can_read, DummyFunc::can_write, DummyFunc::path, DefaultFunc::size, ChunkedFunc::is_chunk, DummyFunc::set_chunk, DummyFunc::completed));
}

WebservFile *WebservFileFactory::make_socket_chunk_file_for_write(FileDiscriptor const &fd, WebservFile *file, std::vector<char> &buffer)
{
    DEBUG("WebservFileFactory::make_socket_chunk_file:" + fd.to_string());
    SocketChunkFile *socket_file = SocketChunkFile::from_file(fd, file);
    socket_file->save(buffer);
    return (this->make_webserv_file(fd, socket_file, DefaultFunc::open, ChunkedFunc::read_for_write, DummyFunc::write, CommonFunc::close, DummyFunc::remove, DummyFunc::can_read, DummyFunc::can_write, DummyFunc::path, DefaultFunc::size, ChunkedFunc::is_chunk, DummyFunc::set_chunk, DummyFunc::completed));
}


WebservFile *WebservFileFactory::make_result_file_for_cgi(FileDiscriptor const &fd, ApplicationResult *file)
{
    (void)fd;
    return (make_webserv_file(fd, file, DefaultFunc::open<ApplicationResult>, DefaultFunc::read<ApplicationResult>, DefaultFunc::write<ApplicationResult>, DefaultFunc::close<ApplicationResult>, DummyFunc::remove<ApplicationResult>, DummyFunc::can_read<ApplicationResult>, DummyFunc::can_write<ApplicationResult>, DummyFunc::path<ApplicationResult>, DummyFunc::size<ApplicationResult>, DefaultFunc::is_chunk<ApplicationResult>, DummyFunc::set_chunk<ApplicationResult>, DefaultFunc::completed<ApplicationResult>));
}

WebservFile *WebservFileFactory::make_vector_file_for_cgi(FileDiscriptor const &fd, size_t buf_size, ProcessID const &pid)
{
    VectorReadCGIFile *vector_file = VectorReadCGIFile::from_buf_size(buf_size, pid);
    return (this->make_webserv_file(fd, vector_file, CommonFunc::open, DefaultFunc::read, DefaultFunc::write, CommonFunc::close, DummyFunc::remove, DummyFunc::can_read, DummyFunc::can_write, DummyFunc::path, DefaultFunc::size, DummyFunc::is_chunk, DummyFunc::set_chunk, CheckSocketReadEndForCGIFunc::completed ));
}

WebservFile *WebservFileFactory::make_vector_file_for_socket(FileDiscriptor const &fd, size_t buf_size)
{
    VectorFile *vector_file = VectorFile::from_buf_size(buf_size);
    return (this->make_webserv_file(fd, vector_file, CommonFunc::open, DefaultFunc::read, DefaultFunc::write, CommonFunc::close, DummyFunc::remove, DummyFunc::can_read, DummyFunc::can_write, DummyFunc::path, DefaultFunc::size, DummyFunc::is_chunk, DummyFunc::set_chunk, CheckSocketReadEndFunc::completed ));
}

WebservFile *WebservFileFactory::make_vector_file(FileDiscriptor const &fd, std::string const& buf_ref)
{
    VectorFile *vector_file = VectorFile::from_ref(buf_ref);
    return (this->make_webserv_file(fd, vector_file, CommonFunc::open, DefaultFunc::read, DefaultFunc::write, CommonFunc::close, DummyFunc::remove, DummyFunc::can_read, DummyFunc::can_write, DummyFunc::path, DefaultFunc::size, DummyFunc::is_chunk, DummyFunc::set_chunk, DummyFunc::completed));
}

WebservFile *WebservFileFactory::make_vector_file(FileDiscriptor const &fd, char *buf, size_t size)
{
    VectorFile *vector_file = VectorFile::from_buf(buf, size);
    return (this->make_webserv_file(fd, vector_file, CommonFunc::open, DefaultFunc::read, DefaultFunc::write, CommonFunc::close, DummyFunc::remove, DummyFunc::can_read, DummyFunc::can_write, DummyFunc::path, DefaultFunc::size, DummyFunc::is_chunk, DummyFunc::set_chunk, DummyFunc::completed));
}

WebservFile *WebservFileFactory::make_error_file(FileDiscriptor const &fd, StatusCode const &status_code)
{
    ErrorFile *error_file = ErrorFile::from_status_code(status_code);
    return (this->make_webserv_file(fd, error_file, CommonFunc::open, CommonFunc::read, DummyFunc::write, CommonFunc::close, DummyFunc::remove, DummyFunc::can_read, DummyFunc::can_write, DummyFunc::path, DefaultFunc::size, DummyFunc::is_chunk, DummyFunc::set_chunk, DummyFunc::completed));
}

WebservFile *WebservFileFactory::make_directory_file(FileDiscriptor const &fd, std::string const &path, std::string const &relative_path, std::string const &domain)
{
    DirectoryFile *directory_file = DirectoryFile::from_path(path, relative_path, domain);
    return (this->make_webserv_file(fd, directory_file));
}

WebservFile *WebservFileFactory::make_request_file(FileDiscriptor const &fd, Request *req)
{
    return (this->make_webserv_file(fd, req, DummyFunc::open, DefaultFunc::read, DummyFunc::write, DummyFunc::close, DummyFunc::remove, DummyFunc::can_read, DummyFunc::can_write, DummyFunc::path, DefaultFunc::size, DummyFunc::is_chunk, DummyFunc::set_chunk, DummyFunc::completed));
}


WebservFile *WebservFileFactory::make_request_file_read_buf(FileDiscriptor const &fd, Request *req)
{
    return (this->make_webserv_file(fd, req, DummyFunc::open, RequestBufferFunc::read, DummyFunc::write, DummyFunc::close, DummyFunc::remove, DefaultFunc::can_read, DummyFunc::can_write, DummyFunc::path, DefaultFunc::size, DummyFunc::is_chunk, DummyFunc::set_chunk, DummyFunc::completed));
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

