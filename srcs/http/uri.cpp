#include "uri.hpp"
#include "global.hpp"
#include "utility.hpp"
#include <iostream>
#include <string>

using std::cout;
using std::endl;

URI::URI()
{
    ;
}

URI::~URI()
{
    ;
}

URI::URI(URI const &uri)
{
    *this = uri;
}

URI& URI::operator=(URI const &uri)
{
    if (this == &uri)
        return (*this);
    this->raw_ = uri.raw_;
    this->query_ = uri.query_;
    this->path_sp = uri.path_sp;
    this->path_ = uri.path_;
    return (*this);
}

URI URI::from_string(std::string const &uri_raw)
{
    DEBUG("URI::from_string uri_raw:" + uri_raw);
    if(uri_raw == ""){
        ERROR("URI::from_string() Error: uri is blank ");
        throw std::invalid_argument("URI::from_string() Error: uri is blank ");
    }

    URI uri;
    uri.raw_ = Utility::delete_duplicated_slash(Utility::trim_white_space(uri_raw));
    size_t pos = uri.raw_.find("#");
    string tmp_uri;
    if(pos != std::string::npos){
        tmp_uri = uri.raw_.substr(0, pos);
    }else{
        tmp_uri = uri.raw_;
    }

    pos = tmp_uri.find('?');
    if(pos != std::string::npos){
        uri.query_ = tmp_uri.substr(pos+1);
        tmp_uri = tmp_uri.substr(0, pos);
    }else{
        uri.query_ = "";
    }

    //std::string &uri_last = *(uri.path_sp.end() -1);
    /*
    size_t pos = uri.raw_.find("?");
    std::string tmp_uri;
    if (pos != std::string::npos){
        uri.query = Split(uri.raw_.substr(pos+1), "&");

        std::string &query_last = *(uri.query.end() -1);
        Split tmp_fragment(query_last, "#");
        if (query_last[0] != '#'){
            query_last = tmp_fragment[0];
        }else{
            query_last = "";
        }
        tmp_uri = uri.raw_.substr(0,pos);
    }else{
        pos = uri.raw_.find("#");
        if (pos != std::string::npos){
            tmp_uri = uri.raw_.substr(0,pos);
        }else{
            tmp_uri = uri.raw_;
        }
    }
    */


    uri.path_sp = Split(tmp_uri , "/");
    uri.encoded_path_sp = uri.path_sp;
    //uri.path_ = tmp_uri;

    for(size_t i=0;i<uri.encoded_path_sp.size();i++){
        uri.uri_encode(uri.encoded_path_sp[i]);
        uri.path_ = uri.path_ + "/" + uri.encoded_path_sp[i];
    }
    /*
    for(size_t i=0;i<uri.query.size();i++){
        uri.uri_encode(uri.query[i]);
    }
    */

    return (uri);
}

std::string const &URI::query() const
{
    return (this->query_);
}

std::string const &URI::raw() const
{
    return (this->raw_);
}

std::string const &URI::path() const
{
    return (this->path_);
}

const Split &URI::splited_path() const
{
    return (this->path_sp);
}

void URI::uri_encode(std::string const &raw_uri_)
{
    std::string raw_uri = raw_uri_;
    char* pos = &(raw_uri[0]);
    size_t cnt = 0;
    char tmp_hex[2];
    size_t size = raw_uri.size();
    char *dst = new char[raw_uri.size()+1];
    char *bk = dst;
    while(pos && size > cnt){
        if(*pos == '%'){
            if (size > cnt+2){
                tmp_hex[0] = pos[1];
                tmp_hex[1] = pos[2];
                const std::string tmp = std::string(tmp_hex);
                char value = Utility::hex_string_to_int(tmp);
                *dst = value;
                dst++;
                pos+=3;
                cnt+=3;
                continue;
            }else{
                ERROR("Uri encorded error:" + raw_uri);
                std::runtime_error("Uri encorded error");
                //cout << ("INFO Uri parse start loop No.3") << endl;
                //todo error
            }
        }
        *dst = *pos;
        dst++;
        pos++;
        cnt++;
    }
    cout << endl;
    cout << endl;
    pos[0] = '\0';
    dst[0] = '\0';
    //_endorded_uri = string(bk);
    raw_uri = std::string(bk);
    delete[] (bk) ;
}

std::string const &URI::get_path_info() const
{
    return (this->path_info_);
}

void URI::set_path_info(std::string &path)
{
    this->path_info_ = path;
}


void URI::print_info() const
{
    cout << "|-- Print URI --|" << endl;
    cout << "raw=" << this->raw() << endl;
    for(size_t i=0;i<path_sp.size();i++){
        cout << "path_sp[" << i << "]=" << path_sp[i] << endl;
    }
    cout << "path=" << this->path() << endl;
    cout << "query=" << this->query() << endl;
    /*
    for(size_t i=0;i<query.size();i++){
        cout << "query[" << i << "]=" << query[i] << endl;
    }
    */


    cout << "|--------------------------|" << endl;
}

std::ostream& operator<<(std::ostream& os, const  URI&uri)
{
    std::string out = uri.raw();
    os << out;
    return (os);
}
