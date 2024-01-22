
#include <sys/stat.h>
#include "global.hpp"
#include "cgi_file.hpp"
#include "normal_reader.hpp"
#include "normal_writer.hpp"
//#include "stream_reader.hpp"
//#include "stream_writer.hpp"
//
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

using std::cout;
using std::endl;

CGIFile::CGIFile(IReader* ireader, FileDiscriptor fd)
    :
        ireader(ireader),
        iwriter(NULL),
        fd(fd)
{
    this->state = FILE_OPEN;
}

CGIFile::CGIFile(IWriter* iwriter, FileDiscriptor fd)
    :
        ireader(NULL),
        iwriter(iwriter),
        fd(fd)
        //ifs(std::ifstream(filepath.c_str(), std::ios::in))
{
    this->state = FILE_OPEN;
}

CGIFile::CGIFile()
{
    this->state = FILE_NOT_OPEN;
}

CGIFile::~CGIFile()
{
    //delete StreamReader::get_instance();
    if (this->state == FILE_OPEN || this->state == FILE_READING){
        if(this->fd.to_int() > 0){
            //std::cout << "fd:" << this->fd << std::endl;
            ::close(fd.to_int());
        }
    }
}


CGIFile* CGIFile::from_fd(FileDiscriptor fd, RWOption option)
{
    NormalReader *ireader = NormalReader::get_instance();
    NormalWriter *iwriter = NormalWriter::get_instance();
    //if (Utility::is_regular_file(filepath)){
    if(option == READ_ONLY){
        MYINFO("CGIFile::from_filepath() Reader");
        return (new CGIFile(ireader, fd));
    }else if(option == WRITE_ONLY){
        MYINFO("CGIFile::from_filepath() Writer");
        return (new CGIFile(iwriter, fd));
    }
    ERROR("CGIFile::from_filepath() can't open file");
    throw std::invalid_argument("CGIFile::from_fd() can't open file");
}

int CGIFile::open()
{
    DEBUG("CGIFile::open()");
    return (true);
    /*
    if (this->state != FILE_NOT_OPEN){
        WARNING("CGIFile::open() state is  not FILE_NOT_OPEN");
        throw std::runtime_error("CGIFile::open() state is  not FILE_NOT_OPEN");
    }

    this->iofs.open(this->filepath.c_str(), this->option);
    if (this->iofs.is_open()){
        this->state = FILE_OPEN;
        return (0);
    }else{
        ERROR("can't open file:" + this->filepath);
        this->state = FILE_ERROR;
        throw std::runtime_error("can't open file");
    }
    return (-1);
    */
}

int CGIFile::close()
{
    DEBUG("CGIFile::close() fd:" + Utility::to_string(fd.to_int()));
    if (this->state == FILE_OPEN){
        if(this->fd.to_int() > 0){
            return ::close(fd.to_int());
        }
    }
    return (-1);
}

int CGIFile::read(char **buf, size_t size)
{
    if(tmp_buf.size() > 0){
        for(size_t i=0;i<tmp_buf.size();i++){
            (*buf)[i] = tmp_buf[i];
        }

        this->tmp_buf.clear();
        return (tmp_buf.size());
    }
    //return (0);
    //FileDiscriptor tmp_fd = FileDiscriptor::from_int(0);
    return (this->ireader->read(this->fd, *buf, size, NULL));
}

int CGIFile::write(char **buf, size_t size)
{
    DEBUG("CGIFile::write() fd=" + Utility::to_string(this->fd.to_int()));
    return (this->iwriter->write(this->fd, *buf, size, NULL));
}

int CGIFile::save(char *buf, size_t size)
{
    for(size_t i=0;i<size;i++){
        this->tmp_buf.push_back(buf[i]);
    }
    return (this->tmp_buf.size());
}


bool CGIFile::can_write()
{
    return (true);
}

bool CGIFile::can_read()
{
    return (true);
}

size_t CGIFile::size()
{
    throw std::runtime_error("can't get size()");
}

bool CGIFile::is_chunk()
{
    return (true);
}


int CGIFile::remove()
{
    throw std::runtime_error("can't remove()");
}

std::string const &CGIFile::path()
{
    throw std::runtime_error("can't path()");
}

