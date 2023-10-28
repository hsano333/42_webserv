
#include "error_file.hpp"
#include "global.hpp"

ErrorFile::ErrorFile()
{
    ;
}

ErrorFile::~ErrorFile()
{
}

ErrorFile* ErrorFile::from_status_code(StatusCode const &code)
{
    ErrorFile *file = new ErrorFile();
    //file.code = code;

    const std::string str1 = "<div>\
  <div style=\"text-align:center;\">\
	<h1 >Webserv Error Page</h1>\
	<h1 >";
    const std::string str2 = "</h1>\
	<h1 >";

    const std::string str3 = "</h1>\
  </div>\
</div>";

    file->text = str1 + code.to_string() + str2 + string(code.message()) + str3;
    //j.file->text = str1;
    //std::cout << code.to_string() << std::endl;
    //cout << "str:" << page.text << endl;

    return (file);
}

int ErrorFile::open()
{
    this->state = FILE_OPEN;
    return 0;
}

int ErrorFile::read(char **buf, size_t size)
{
    std::cout << "ErrorFile::read No.1" << std::endl;
    (void)size;
    if (this->state != FILE_OPEN){
        return (0);
        //std::cout << "ErrorFile::read No.2" << std::endl;
        //ERROR("ErrorFile::read() state is  not FILE_OPEN");
        //throw std::runtime_error("ErrorFile::read() state is  not FILE_OPEN");
    }
    std::cout << "ErrorFile::read No.3" << std::endl;
    *buf = const_cast<char*>(&(this->text[0]));
    std::cout << "ErrorFile::read No.4:" << this->text << std::endl;
    //Utility::memcpy(*buf, dirr->d_name, Utility::strlen(dirr->d_name));

    this->state = FILE_CLOSE;
    return this->text.size();
}

int ErrorFile::write(char **buf, size_t size)
{
    (void)buf;
    (void)size;
    return (0);
}

int ErrorFile::close()
{
    this->state = FILE_CLOSE;
    return (0);
}

bool ErrorFile::can_read()
{
    return (true);
}

size_t ErrorFile::size()
{
    return (this->text.size());
}

bool ErrorFile::is_chunk()
{
    return (false);
}


int ErrorFile::remove()
{
    // error file is not exist
    return (-1);
}

std::string const &ErrorFile::path()
{
    return (this->text);
}
