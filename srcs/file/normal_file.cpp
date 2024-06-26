#include <sys/stat.h>
#include "global.hpp"
#include "normal_file.hpp"
#include "normal_reader.hpp"
#include "stream_reader.hpp"
#include "stream_writer.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

using std::cout;
using std::endl;

NormalFile::NormalFile(IReader* ireader, std::string const &filepath, std::ios_base::openmode option)
    :
        ireader(ireader),
        iwriter(NULL),
        filepath(filepath),
        option(option)
{
    this->state = FILE_NOT_OPEN;
}

NormalFile::NormalFile(IWriter* iwriter, std::string const &filepath, std::ios_base::openmode option)
    :
        ireader(NULL),
        iwriter(iwriter),
        filepath(filepath),
        option(option)
{
    this->state = FILE_NOT_OPEN;
}

NormalFile::NormalFile()
{
    this->state = FILE_NOT_OPEN;
}

NormalFile::~NormalFile()
{
    DEBUG("NormalFile Destructor");
    if (this->state == FILE_OPEN || this->state == FILE_READING){
        ::close(fd.to_int());
        this->state = FILE_NOT_OPEN;
    }
}


NormalFile* NormalFile::from_filepath(std::string const &filepath, std::ios_base::openmode option)
{
    DEBUG("NormalFile filepath=" + filepath);
    StreamReader *ireader = StreamReader::get_instance();
    StreamWriter *iwriter = StreamWriter::get_instance();
    if((option & std::ios::in) == std::ios::in){
        MYINFO("NormalFile::from_filepath() Reader");
        return (new NormalFile(ireader, filepath, option));
    }else if((option & std::ios::out) == std::ios::out){
        MYINFO("NormalFile::from_filepath() Writer");
        return (new NormalFile(iwriter, filepath, option));
    }
    ERROR("NormalFile::from_filepath() can't open file:" + filepath);
    throw std::invalid_argument("NormalFile::from_filepath() can't open file");
}

int NormalFile::open()
{
    DEBUG("NormalFile::open()");
    if (this->state != FILE_NOT_OPEN){
        WARNING("NormalFile::open() state is  not FILE_NOT_OPEN");
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
}

int NormalFile::close()
{
    DEBUG("NormalFile::close() ");
    if (this->state != FILE_NOT_OPEN){
        (this->iofs.close());
        this->state = FILE_NOT_OPEN;
        return (0);
    }
    return (-1);
}

int NormalFile::read(char **buf, size_t size)
{
    DEBUG("NormalFile::read");
    return (this->ireader->read(this->fd, *buf, size, &(this->iofs)));
}

int NormalFile::write(char **buf, size_t size)
{
    DEBUG("NormalFile::write size=" + Utility::to_string(size));
    if(this->state == FILE_OPEN){
        return (this->iwriter->write(this->fd, *buf, size, &(this->iofs)));
    }
    return (-1);
}

int NormalFile::save(char *buf, size_t size)
{
    for(size_t i=0;i<size;i++){
        this->tmp_buf.push_back(buf[i]);
    }
    return (this->tmp_buf.size());
}


bool NormalFile::can_write()
{
    return (Utility::is_writable_file(this->filepath));
}

bool NormalFile::can_read()
{
    DEBUG("NormalFile::can_read() filepath:" + this->filepath);
    return (Utility::is_readable_file(this->filepath));
}

size_t NormalFile::size()
{
    return (Utility::get_file_size(this->filepath));
}

bool NormalFile::is_chunk()
{
    return (false);
}


int NormalFile::remove()
{
    return (std::remove(this->filepath.c_str()));
}

std::string const &NormalFile::path()
{
    return (this->filepath);
}

