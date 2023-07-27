#include "split.hpp"
#include <algorithm>
#include <iostream>
#include <ostream>

using std::cout;
using std::endl;
using std::string;
using std::vector;


void split_delimiters(std::string const& str, std::string delimiters, vector<string> &vec)
{
    vector<string> &splitted_strs = vec;

    //size_t del_len = delimiter.size();
    //std::vector<char> = 
    size_t del_size = delimiters.size();
    size_t del_len = 1;
    std::string::size_type offset = std::string::size_type(0);
    std::string::size_type pos;
    std::string::size_type tmp_pos;
    while (1) {
        pos = string::npos;
        for(size_t i=0;i<del_size;i++){
            tmp_pos = str.find(delimiters[i], offset);
            if (tmp_pos != std::string::npos && ((tmp_pos < pos) || pos == string::npos)){
                pos = tmp_pos;
            }
        }
        if (pos == string::npos) {
            string last = str.substr(offset);
            if (last.size() != 0){
                bool tmp = true;
                for(size_t i=0;i<del_size;i++){
                    if (last[0] == delimiters[i]) {
                        tmp = false;
                    }
                }
                if (tmp){
                    splitted_strs.push_back(last);
                }
            }
            //if (last != delimiter && last.size() != 0) {
                //splitted_strs.push_back(last);
            //}
            break;
        }
        if (pos - offset > 0) {
            splitted_strs.push_back(str.substr(offset, pos - offset));
            //this->_splitted_string.push_back(str.substr(offset, pos - offset));
        }
        offset = pos + del_len;
    }
    cout << "splitted_strs" << splitted_strs.size() << endl;
}

static void split(std::string const& str, std::string delimiter, vector<string> &vec)
{
    vector<string> &splitted_strs = vec;

    size_t del_len = delimiter.size();
    std::string::size_type offset = std::string::size_type(0);
    std::string::size_type pos;
    while (1) {
        pos = str.find(delimiter, offset);
        if (pos == string::npos) {
            string last = str.substr(offset);
            if (last != delimiter && last.size() != 0) {
                //this->_splitted_string.push_back(last);
                splitted_strs.push_back(last);
            }
            break;
        }
        if (pos - offset > 0) {
            splitted_strs.push_back(str.substr(offset, pos - offset));
            //this->_splitted_string.push_back(str.substr(offset, pos - offset));
        }
        offset = pos + del_len;
    }
    cout << "splitted_strs" << splitted_strs.size() << endl;
}

void Split::split_not_delete_delimiter(std::string const& str, std::string delimiter)
{
    size_t del_len = delimiter.size();
    std::string::size_type offset = std::string::size_type(0);
    std::string::size_type pos;
    while (1) {
        pos = str.find(delimiter, offset);
        if (pos == string::npos) {
            string last = str.substr(offset);
            cout << "last:" << last << endl;
            if (last != delimiter && last.size() != 0) {
                this->_splitted_string.push_back(last);
            }
            break;
        }
        this->_splitted_string.push_back(str.substr(offset, pos - offset+1));
        offset = pos + del_len;
    }
}


Split::Split(char *str, std::string delimiter)
{
    string tmp = string(str);
    size_t del_len = delimiter.size();
    if (del_len == 0) {
        this->_splitted_string.push_back(tmp);
        return;
    }
    split(tmp, delimiter, this->_splitted_string);
}

Split::Split(std::string const& str, std::string delimiter)
{
    size_t del_len = delimiter.size();
    if (del_len == 0) {
        this->_splitted_string.push_back(str);
        return;
    }
    split(str, delimiter, this->_splitted_string);
}

Split::Split(std::string const& str, std::string delimiter, bool multi_delimiter)
{
    size_t del_len = delimiter.size();
    if (del_len == 0) {
        this->_splitted_string.push_back(str);
        return;
    }
    if(multi_delimiter){
        split_delimiters(str, delimiter, this->_splitted_string);
    }else{
        split(str, delimiter, this->_splitted_string);
    }
}


/*
Split::Split(std::string const& str, std::string delimiter, bool rest_delimiter)
{
    size_t del_len = delimiter.size();
    if (del_len == 0) {
        this->_splitted_string.push_back(str);
        return;
    }

    if (rest_delimiter){
        split(str, delimiter,_splitted_string);
    }else{
        split(str, delimiter,_splitted_string);
        //this->split_not_delete_delimiter(str, delimiter);
    }
}
*/

void Split::concat(std::string& str, std::string delimiter)
{
    if (this->_splitted_string.size() == 0){
        split(str, delimiter, _splitted_string);
        return;
    }
    string concat = *(this->_splitted_string.end()-1) + str;
    this->_splitted_string.pop_back();
    split(concat, delimiter,_splitted_string);
}

Split::iterator Split::begin()
{
    return (this->_splitted_string.begin());
}

Split::iterator Split::end()
{
    return (this->_splitted_string.end());
}

std::string& Split::operator[](size_t i)
{
    return (this->_splitted_string[i]);
}

size_t Split::size()
{
    return (this->_splitted_string.size());
}

std::ostream& operator<<(std::ostream& os, Split& sp)
{
    std::vector<std::string>::iterator ite = sp.begin();
    std::vector<std::string>::iterator end = sp.end();
    for (; ite != end; ite++) {
        os << __FILE__ << " " << __LINE__ << "line";
        os << "[" << *ite << "]";
    }
    return (os);
}


void Split::split_cp(std::string const& str, std::string delimiter, std::vector<std::string>& dst, bool multi_delimiter)
{
    if (multi_delimiter){
        split_delimiters(str, delimiter, dst);
    }else{
        split(str, delimiter, dst);
    }
    //cout << "dst size:" << dst.size() << endl;
}

