
#include "object_file.hpp"
#include "global.hpp"

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::map;

ObjectFile::ObjectFile()
{
    ;
}

ObjectFile::~ObjectFile()
{
}

ObjectFile* ObjectFile::from_status_code(StatusCode const &code)
{
    ObjectFile *file = new ObjectFile();
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

int ObjectFile::open()
{
    this->state = FILE_OPEN;
    return 0;
}

int ObjectFile::read(char **buf, size_t size)
{
    (void)size;
    if (this->state != FILE_OPEN){
        return (0);
    }
    std::cout << "ObjectFile::read No.3" << std::endl;
    *buf = const_cast<char*>(&(this->text[0]));
    std::cout << "ObjectFile::read No.4:" << this->text << std::endl;
    //Utility::memcpy(*buf, dirr->d_name, Utility::strlen(dirr->d_name));

    this->state = FILE_CLOSE;
    return this->text.size();
}

int ObjectFile::write(char **buf, size_t size)
{
    (void)buf;
    (void)size;
    return (0);
}

int ObjectFile::save(char *buf, size_t size)
{
    for(size_t i=0;i<size;i++){
        this->tmp_buf.push_back(buf[i]);
    }
    return (this->tmp_buf.size());
}

int ObjectFile::close()
{
    this->state = FILE_CLOSE;
    return (0);
}

bool ObjectFile::can_read()
{
    return (true);
}

size_t ObjectFile::size()
{
    return (this->text.size());
}

bool ObjectFile::is_chunk()
{
    return (false);
}


int ObjectFile::remove()
{
    // error file is not exist
    return (-1);
}

std::string const &ObjectFile::path()
{
    return (this->text);
}
