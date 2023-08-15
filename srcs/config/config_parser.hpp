#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include "split.hpp"
#include "global.hpp"
#include "utility.hpp"
#include "config_parsered_data.hpp"

using std::endl;
using std::cout;

template <class T, class U>
class ConfigParser
{
    public:
        ConfigParser(const std::string &target);
        ~ConfigParser();
        //std::vector<std::string> parser(std::string const &str, T * object);
        ConfigParseredData parser(std::string const &str, T * object);
        const std::string target;
    private:
        //bool find_target(Split &sp, size_t &i);
        bool find_target(std::string const &str, size_t *i_p, std::vector<std::string> &out_words);
        std::string retrieve_string(std::string const &str, size_t *i);
        void retrieve_properties(std::string const &str, std::vector<std::vector<std::string> > &properties);
};

template <class T, class U>
ConfigParser<T,U>::ConfigParser(const std::string &target) : target(target)
{
    ;
}

template <class T, class U>
ConfigParser<T,U>::~ConfigParser()
{
    ;
}

template <class T, class U>
std::string ConfigParser<T, U>::retrieve_string(std::string const &str, size_t *i_pointer)
{
    size_t i = *i_pointer;
    std::string parenthesis_begin = "{";
    std::string parenthesis_end = "}";
    int parenthesis_cnt = 0;

    size_t start = 1;
    size_t end = 1;

    while(str[i]){
        if(str[i] == '{'){
            parenthesis_cnt++;
            if(parenthesis_cnt == 1){
                start = i+1;
            }
        }else if(str[i] == '}'){
            parenthesis_cnt--;
            if(parenthesis_cnt == 0){
                end= i-1;
                break;
            }
        }
        i++;
    }
    *i_pointer = i;
    return (str.substr(start, end-start+1));
}

/*
template <class T, class U>
bool ConfigParser<T,U>::find_target(Split &sp, size_t &i_p)
{
    size_t i = *i_p;
    std::string line = sp[i];
    Split words(line, "\r ", true, true);
    for(size_t j=0;j<words.size();j++){
        if(words[j].find(target) != std::string::npos){
            if(words[j] == target + "{"){
                return (true);
            }else if(j != words.size()-1 && words[j+1] == "{"){
                return (true);
            }else if(i != sp.size()-1){
                Split tmp(sp[i+1], "\r ", true);
                if((tmp.size() > 0) && (tmp[0].size() > 0) && (tmp[0][0] == '{')){
                    (*i_p)++;
                    return (true);
                }else{
                    ERROR("Invalid Config Error:");
                    throw std::runtime_error("config parser error");
                }
            }else{
                    ERROR("Invalid Config Error:");
                    throw std::runtime_error("config parser error");
            }
            return (true);
        }
    }
    return (false);
}
*/
template <class T, class U>
bool ConfigParser<T,U>::find_target(std::string const &str, size_t *i_pointer, std::vector<std::string> &out_words)
{
    size_t i = *i_pointer;
    size_t pos = str.find(target, i);
    if(pos != std::string::npos)
    {
        size_t end = str.find("{", pos+target.size());
        if(end != std::string::npos){
            std::string between_string = str.substr(pos+target.size(), end-(pos+target.size()) );
            //size_t crlf_pos = between_string.find("\n");
            *i_pointer = end;
            Split sp(between_string, " ", false, true);
            for(size_t j=0;j<sp.size();j++){
                if(sp[j] != "" && sp[j] != "\n" && sp[j] != "\r\n" && sp[j] != " "){
                    out_words.push_back(sp[j]);
                }
            }
            return true;
        }else{
                //cout << "failse? No.2" << endl;
            return false;
        }


        /*
        pos += target.size();
        std::string tmp;
        while(str[pos]){
            if(str[pos] == '\n' || str[pos] == '\r' || str[pos] == ' '){
                if(tmp != ""){
                    out_words.push_back(tmp);
                }
                tmp = "";
            }else if(str[pos] == '{'){
                if(tmp != ""){
                    out_words.push_back(tmp);
                }
                return true;
            }else{
                tmp.push_back(str[pos]);
            }
            pos++;
        }
        return false;
        */
    }
    return (false);
}



template <class T, class U>
void ConfigParser<T, U>::retrieve_properties(std::string const &str_original, std::vector<std::vector<std::string> > &properties)
{


    /*
    size_t i = 0;
    while(str_original[i]){
        if(str_original[i] != '\r' && str_original[i]!='\n' && str_original[i]!=' '){
            break;
        }
            i++;
    }
    size_t begin = i;
    i = str_original.size()-1;
    while(i != 0){
        if(str_original[i] != '\r' && str_original[i]!='\n' && str_original[i]!=' '){
            break;
        }
        i--;
    }
    size_t end = i+1;


    std::string str = str_original.substr(begin, end);
    */


    std::string str = Utility::trim_white_space(str_original);
    if(str.size() == 0){
        return ;
    }

    /*
    cout << "target:" << target << endl;
    cout << "str.size():" <<  str.size()<< endl;
    cout << "str:[[" << str << "]]" << endl;
    */

    Split sp(str, ";", false, true);
    std::vector<std::string> line_properties;
    for(size_t i=0;i<sp.size();i++){
        //cout << "line:[" << sp[i] << "]" <<  endl;

        Split word(sp[i], "\r\n ", true, true);
        //cout << "line:[" << sp[i] << "]" <<  endl;
        line_properties.clear();
        for(size_t j=0;j<word.size();j++){
            if(word[j] != "\n" && word[j] != "\r\n" && word[j] != " "){
                //cout << "pushed word:[" << word[j] << "]" << endl;
                line_properties.push_back(word[j]);
            }
        }
        //cout << "push sisze:" << line_properties.size() << endl;
        properties.push_back(line_properties);
    }
    //cout << "properties test target:" << target << endl;
    /*
    for(size_t i=0;i<properties.size();i++){
        for(size_t j=0;j<properties[i].size();j++){
            cout << "i:" << i << ", j:" << j << ", properties:" << properties[i][j] << endl;
        }
        cout << endl;
    }
    */
}


template <class T, class U>
ConfigParseredData ConfigParser<T, U>::parser(std::string const &str, T * object)
{
    ConfigParseredData parsered_data;
    std::vector<std::string> out_strings;
    std::vector<std::vector<std::string> > properties;

    size_t i = 0;
    size_t begin= 0;
    size_t end = 0;


    /*
    cout << "target:" << target << endl;
    cout << "str:" << str << endl;
    cout << "target:" << target << endl;
    */

    std::vector<std::string> target_string;

    while(str[i]){
        begin= i;
        end = i;
        if(find_target(str, &end, out_strings)){
            size_t pos = str.find(target, begin);
            //parsered_data.target_strings.push_back(this->retrieve_string(str,&end));
            parsered_data.push_back_next_string(this->retrieve_string(str,&end));
            parsered_data.push_back_next_properties(out_strings);
            out_strings.clear();

            //cout << "target No.0:" << target << endl;
            if(pos != std::string::npos && pos >= begin){
                //cout << "target No.1: i=" << i << ", pos=" << pos  << endl;
                std::string string_before_target = str.substr(i,pos-i);
                //cout << "string_before_target:[" << string_before_target << "]" << endl;
                if(string_before_target.size() > 0){
                    retrieve_properties(string_before_target, properties);
                }
            }
            i = end+1;
            continue;
        }else{
            break;
        }
        i = end+1;
    }
    std::string string_rest = str.substr(end+1);
    if(string_rest.size() > 0){
        retrieve_properties(string_rest, properties);
    }
    //object->assign_out_properties(out_strings);
    object->assign_properties(properties);
    //parsered_data.assign_next_target_properties(out_strings);
    //parsered_data.set_next_string(target_string);

    return parsered_data;
}

#endif
