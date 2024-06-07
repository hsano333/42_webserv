
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
    (void)size;
    *buf = const_cast<char*>(&(this->text[0]));

    return (this->text.size());
}

size_t ErrorFile::size()
{
    return (this->text.size());
}

