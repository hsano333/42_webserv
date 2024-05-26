
#include "error_file.hpp"
#include "global.hpp"

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::map;

ErrorFile::ErrorFile()
{
    ;
}

ErrorFile::~ErrorFile()
{
}

ErrorFile* ErrorFile::from_status_code(StatusCode const &code)
{
    DEBUG("ErrorFile::from_status_code");

    ErrorFile *file = new ErrorFile();
    //file.code = code;

    const std::string str1 = "<div>\
  <div style=\"text-align:center;\">\
	<h1 >Webserv Error Page</h1>\
	<h2 >";
    const std::string str2 = "</h2>\
	<h2 >";

    const std::string str3 = "</h2>\
  </div>\
</div>";

    file->text = str1 + code.to_string() + str2 + string(code.message()) + str3;
    return (file);
}


int ErrorFile::read(char **buf, size_t size)
{
    /*
    std::cout << "ErrorFile::read No.1" << std::endl;
    if (this->state != FILE_OPEN){
        return (0);
        //std::cout << "ErrorFile::read No.2" << std::endl;
        //ERROR("ErrorFile::read() state is  not FILE_OPEN");
        //throw std::runtime_error("ErrorFile::read() state is  not FILE_OPEN");
    }
    std::cout << "ErrorFile::read No.3" << std::endl;
    */
    (void)size;
    std::cout << "ErrorFile::read :" << this->text << std::endl;
    *buf = const_cast<char*>(&(this->text[0]));

    //this->state = FILE_CLOSE;
    return (this->text.size());
}

size_t ErrorFile::size()
{
    return (this->text.size());
}


/*
int ErrorFile::save(char *buf, size_t size)
{
    for(size_t i=0;i<size;i++){
        this->tmp_buf.push_back(buf[i]);
    }
    return (this->tmp_buf.size());
}


bool ErrorFile::can_read()
{
    return (true);
}

*/
/*
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
*/

/*
std::string const &ErrorFile::path()
{
    return (this->text);
}
*/
