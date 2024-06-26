#include "global.hpp"
#include "header.hpp"
#include "split.hpp"
#include "utility.hpp"
#include "body_size.hpp"
#include "header_word.hpp"
#include <climits>

Header::Header() : _not_find("")
{
}

Header::~Header()
{
}

Header::Header(Header const &header) : _not_find("")
{
    *this = header;
}

Header& Header::operator=(Header const &header)
{
    if (this == &header){
        return (*this);
    }
    this->_headers.clear();
    std::map<std::string, std::string >::const_iterator ite = header._headers.begin();
    std::map<std::string, std::string >::const_iterator end = header._headers.end();
    while(ite != end){
        std::string key = ite->first;
        std::transform(key.begin(), key.end(), key.begin(), Utility::tolower);
        this->_headers.insert(std::make_pair(key, ite->second));
        ite++;
    }
    return (*this);
}

Header Header::from_splited_data(Split &sp, size_t offset)
{
    DEBUG("Header::from_splited_data:" + Utility::to_string(offset));
    Header header;
    for(size_t i=offset;i<sp.size();i++){
        std::string const &line = sp[i];
        Split splited_line(line, ":", false, true);

        if (splited_line.size() == 1){
            ERROR("not find ':' in header line:" + line);
            throw HttpException("400");
        }else if (splited_line.size() == 2){
            header._headers.insert(std::make_pair(
            Utility::to_lower(Utility::trim_white_space(splited_line[0])) ,
            Utility::trim_white_space(splited_line[1]))
            );
        }else{
            //"abc:def:ghi:jkl"のように:が複数あるとき
            // 最初のものを区切り文字として、残りは連結する
            std::string cat;
            for(size_t j=1;j<splited_line.size();j++){
                cat += splited_line[j];
                cat += ":";
            }
            cat = cat.substr(0, cat.size()-1);
            header._headers.insert(std::make_pair(
            Utility::to_lower(Utility::trim_white_space(splited_line[0])) ,
            Utility::trim_white_space(cat)
            ));
        }
    }
    return (header);
}

void Header::print_info() const
{
    map<string, string>::const_iterator ite = _headers.begin();
    map<string, string>::const_iterator end = _headers.end();
    cout << endl  << "Print Headers" << endl;
    for (; ite != end; ite++)
        cout << " [" << (*ite).first << "]: [" << (*ite).second << "]" << endl;

}

bool Header::is_chunked() const
{
    string const &trans_enc = this->find(TRANSFER_ENCODING);
    if (trans_enc == TRANSFER_ENCODING_CHUNKED){
        return (true);
    }else{
        return (false);
    }
}

std::string const &Header::find(const string &name) const
{
     map<string, string>::const_iterator ite = _headers.find(name);
     if (ite == _headers.end()){
         return (this->_not_find);
     }
     return (ite->second);
}

string const &Header::get_content_length_str() const
{
    std::string name = "content-length";
    string const &value = find(name);
    if (value == this->_not_find){
        return (this->_not_find);
    }
    return (value);

}

std::string Header::get_content_type() const
{
    string const &type = this->find(CONTENT_TYPE);
    if(type == this->not_find()){
        return (type);
    }
    size_t pos = type.find(";");
    if(pos != std::string::npos){
        return (type.substr(0,pos));
    }
    return (type);
}

ssize_t Header::get_content_length() const
{
    std::string name = CONTENT_LENGTH;
    string const &value = find(name);
    if (value == this->_not_find){
        return (-1);
    }

    BodySize body_size = BodySize::from_string(value);
    return (ssize_t)(body_size.to_number());
}

std::string const &Header::get_host() const
{
    std::string const name = "host";
    std::string const &value = find(name);
    if (value == this->_not_find){
        return (this->_not_find);
    }
    return (value);
}

string const &Header::cookie() const
{
    std::string name = "cookie";
    string const &value = find(name);
    if (value == this->_not_find){
        return (this->_not_find);
    }
    return (value);
}


size_t Header::size() const
{
    return (this->_headers.size());
}
void Header::insert(std::string const &key, std::string const &value)
{
    this->_headers.insert(std::make_pair(Utility::to_lower(key), value));
}

void Header::erase(std::string const &key)
{
    this->_headers.erase(key);
}

std::map<std::string, std::string >::iterator Header::begin()
{
    return (_headers.begin());
}

std::map<std::string, std::string >::iterator Header::end()
{
    return (_headers.end());
}

std::string const &Header::not_find() const
{
    return (this->_not_find);
}

std::map<std::string, std::string >::const_iterator Header::cbegin() const
{
    return (_headers.begin());
}

std::map<std::string, std::string >::const_iterator Header::cend() const
{
    return (_headers.end());
}
