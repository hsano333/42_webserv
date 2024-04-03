#include "split.hpp"
#include <algorithm>
#include <iostream>
#include <ostream>
#include <climits>
#include <map>
#include <stdio.h>
#include "global.hpp"

using std::cout;
using std::endl;
using std::string;
using std::vector;

Split::Split()
{
}

Split::~Split()
{
}

Split::Split(Split const &sp)
{
    this->_splitted_string = sp._splitted_string;
}

Split& Split::operator=(Split const &sp)
{
    if(this == &sp)
        return (*this);
    this->_splitted_string = sp._splitted_string;
    return (*this);
}


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
    //cout << "splitted_strs" << splitted_strs.size() << endl;
}

static void split(std::string const &str, std::string const &delimiter, vector<string> &vec)
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
    //cout << "splitted_strs" << splitted_strs.size() << endl;
}

void Split::set_word(std::string &str, std::string const &delimiter)
{
    //string tmp = string(str);
    size_t del_len = delimiter.size();
    if (del_len == 0) {
        this->_splitted_string.push_back(str);
        return;
    }
    split(str, delimiter, this->_splitted_string);
}

void Split::split_not_delete_delimiter(std::string const &str, std::string const &delimiter)
{
    size_t del_len = delimiter.size();
    std::string::size_type offset = std::string::size_type(0);
    std::string::size_type pos;
    while (1) {
        pos = str.find(delimiter, offset);
        if (pos == string::npos) {
            string last = str.substr(offset);
            //cout << "last:" << last << endl;
            if (last != delimiter && last.size() != 0) {
                this->_splitted_string.push_back(last);
            }
            break;
        }
        this->_splitted_string.push_back(str.substr(offset, pos - offset+1));
        offset = pos + del_len;
    }
}


Split::Split(char *str, std::string const &delimiter)
{
    string tmp = string(str);
    size_t del_len = delimiter.size();
    if (del_len == 0) {
        this->_splitted_string.push_back(tmp);
        return;
    }
    split(tmp, delimiter, this->_splitted_string);
}

Split::Split(std::string const& str, std::string const &delimiter)
{
    size_t del_len = delimiter.size();
    if (del_len == 0) {
        this->_splitted_string.push_back(str);
        return;
    }
    split(str, delimiter, this->_splitted_string);
}

Split::Split(std::string const& str, std::string const &delimiter, bool multi_delimiter)
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

//bool compare_lower(std::pair<char, int> &a, std::pair<char, int> &b)
bool compare_lower(std::map<char, int>::iterator &a, std::map<char, int>::iterator &b)
{
    return (a->second > b->second);
}

static std::string find_used_delimiter(std::string const &str, std::string const &delimiter)
{
    vector<int>ascii(128,0);
    std::string replace_delimiter = "";

    for(size_t i=0;i<delimiter.size();i++){
        char c = delimiter[i];
        ascii[c] = 100000;
    }
    for(size_t i=0;i<str.size();i++){
        char c = str[i];
        ascii[c]++;
    }

    std::sort(ascii.begin(), ascii.end());
    std::multimap<int,char> ascii_count;
    for(size_t i=42;i<ascii.size()-1;i++){
        ascii_count.insert(std::make_pair(ascii[i],i));
    }
    //cout << "asci size:" << ascii.size() << endl;
    std::multimap<int,char>::iterator ite = ascii_count.begin();
    std::multimap<int,char>::iterator end = ascii_count.end();
    int cnt = 0;
    while(ite != end){
        replace_delimiter.push_back(ite->second);
        cnt++;
        ite++;
        if(cnt > 15){
            break;
        }
    }
    return replace_delimiter;
}

Split::Split(std::string const& raw_str, std::string const &delimiter, bool multi_delimiter, bool d_quote)
{
    std::vector<std::string> backup;
    size_t replace_cnt = 0;
    std::string str = raw_str;
    size_t del_len = delimiter.size();
    std::string random_word;
    if (del_len == 0) {
        this->_splitted_string.push_back(str);
        return;
    }

    if(d_quote){
        random_word = find_used_delimiter(str, delimiter);
        size_t pos = str.find('\"');
        while(pos != std::string::npos){
            size_t end = str.find('\"', pos+1);
            if(end != std::string::npos){
                if(multi_delimiter){
                    while(pos != end){
                        if(delimiter.find(str[pos]) != std::string::npos){
                            replace_cnt++;
                            std::string tmp;
                            tmp.push_back((str[pos]));
                            backup.push_back(tmp);
                            str.replace(pos, 1, random_word);
                            pos += (random_word.size()-1);
                            end = str.find('\"', pos+1);
                        }
                        pos++;
                    }
                }else{
                    pos = str.find(delimiter, pos);
                    while(pos < end){
                        replace_cnt++;
                        str.replace(pos, delimiter.size(), random_word);
                        backup.push_back(delimiter);
                        //pos += (random_word.size());
                        pos += (random_word.size());
                        end = str.find('\"', pos);
                        pos = str.find(delimiter, pos);
                    }
                }
            }
            pos = str.find('\"', end+1);
        }
    }
    if(multi_delimiter){
        split_delimiters(str, delimiter, this->_splitted_string);
    }else{
        split(str, delimiter, this->_splitted_string);
    }

    if(d_quote && replace_cnt > 0){
        size_t backup_i = 0;
        for(size_t i=0;i<_splitted_string.size();i++){
            std::string &word = _splitted_string[i];
            //cout << word << endl;
            size_t pos = word.find(random_word);
            while(pos != std::string::npos){
                std::string backup_word = backup[backup_i];
                word.replace(pos, random_word.size(), backup_word);
                pos = word.find(random_word, pos);
                backup_i++;
            }
        }
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

std::string const &Split::operator[](size_t i) const
{
    return (this->_splitted_string[i]);
}

size_t Split::size() const
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

void Split::print()
{
    for(size_t i=0;i<this->size();i++){
        cout << "split[" << i << "]=" << this->_splitted_string[i] << endl;
    }
}
