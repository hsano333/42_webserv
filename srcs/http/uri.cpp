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
    this->query = uri.query;
    this->path_sp = uri.path_sp;
    this->path_sp = uri.path_sp;
    return (*this);
}

URI URI::from_string(std::string &uri_raw)
{
    DEBUG("URI::from_string uri_raw:" + uri_raw);
    if(uri_raw == ""){
        ERROR("URI::from_string() Error: uri is blank ");
        throw std::invalid_argument("URI::from_string() Error: uri is blank ");
    }

    URI uri;

    uri.raw_ = Utility::delete_duplicated_slash(Utility::trim_white_space(uri_raw));
    uri.path_sp = Split(uri.raw_, "/");

    std::string &uri_last = *(uri.path_sp.end() -1);
    Split tmp_query(uri_last, "?");
    uri_last = tmp_query[0];

    if (tmp_query.size() > 1){
        uri.query = Split(tmp_query[1], "&");
        std::string &query_last = *(uri.query.end() -1);
        Split tmp_fragment(query_last, "#");
        query_last = tmp_fragment[0];
    }else{
        Split tmp_fragment(uri_last, "#");
        uri_last = tmp_fragment[0];
    }

    for(size_t i=0;i<uri.path_sp.size();i++){
        uri.uri_encode(uri.path_sp[i]);
    }
    for(size_t i=0;i<uri.query.size();i++){
        uri.uri_encode(uri.path_sp[i]);
    }

    return (uri);
}

std::string URI::raw() const
{
    return (this->raw_);
}

void URI::uri_encode(std::string& raw_uri)
{
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

std::ostream& operator<<(std::ostream& os, const  URI&uri)
{
    std::string out = uri.raw();
    os << out;
    return (os);
}
