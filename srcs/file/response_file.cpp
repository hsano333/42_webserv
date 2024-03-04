
//#include "object_file.hpp"
#include "global.hpp"

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::map;

ResponseFile::ResponseFile()
{
    ;
}

ResponseFile::~ResponseFile()
{
}

ResponseFile* ResponseFile::from_status_code(StatusCode const &code)
{
    ResponseFile *file = new ResponseFile();
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

int ResponseFile::open()
{
    this->state = FILE_OPEN;
    return 0;
}

int ResponseFile::read(char **buf, size_t size)
{
    (void)size;
    if (this->state != FILE_OPEN){
        return (0);
    }
    std::cout << "ResponseFile::read No.3" << std::endl;
    *buf = const_cast<char*>(&(this->text[0]));
    std::cout << "ResponseFile::read No.4:" << this->text << std::endl;

    this->state = FILE_CLOSE;
    return this->text.size();
}

int ResponseFile::write(char **buf, size_t size)
{
    (void)buf;
    (void)size;
    return (0);
}

int ResponseFile::save(char *buf, size_t size)
{
    for(size_t i=0;i<size;i++){
        this->tmp_buf.push_back(buf[i]);
    }
    return (this->tmp_buf.size());
}

int ResponseFile::close()
{
    DEBUG("ResponseFile::close()");
    this->state = FILE_CLOSE;
    return (0);
}

bool ResponseFile::can_read()
{
    return (true);
}

size_t ResponseFile::size()
{
    return (this->text.size());
}

bool ResponseFile::is_chunk()
{
    return (false);
}


int ResponseFile::remove()
{
    // error file is not exist
    return (-1);
}

std::string const &ResponseFile::path()
{
    return (this->text);
}
