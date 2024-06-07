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
        ConfigParseredData parser(std::string const &str, T * object);
        const std::string target;
    private:
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
            *i_pointer = end;
            Split sp(between_string, " ", false, true);
            for(size_t j=0;j<sp.size();j++){
                if(sp[j] != "" && sp[j] != "\n" && sp[j] != "\r\n" && sp[j] != " "){
                    out_words.push_back(sp[j]);
                }
            }
            return true;
        }else{
            return false;
        }
    }
    return (false);
}



template <class T, class U>
void ConfigParser<T, U>::retrieve_properties(std::string const &str_original, std::vector<std::vector<std::string> > &properties)
{
    std::string str = Utility::trim_white_space(str_original);
    if(str.size() == 0){
        return ;
    }

    Split sp(str, ";", false, true);
    std::vector<std::string> line_properties;
    for(size_t i=0;i<sp.size();i++){

        Split word(sp[i], "\r\n ", true, true);
        line_properties.clear();
        for(size_t j=0;j<word.size();j++){
            if(word[j] != "\n" && word[j] != "\r\n" && word[j] != " "){
                line_properties.push_back(word[j]);
            }
        }
        properties.push_back(line_properties);
    }
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

    std::vector<std::string> target_string;

    while(str[i]){
        begin= i;
        end = i;
        if(find_target(str, &end, out_strings)){
            size_t pos = str.find(target, begin);
            parsered_data.push_back_next_string(this->retrieve_string(str,&end));
            parsered_data.push_back_next_properties(out_strings);
            out_strings.clear();

            if(pos != std::string::npos && pos >= begin){
                std::string string_before_target = str.substr(i,pos-i);
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
    if(end >= str.size()){
        throw std::runtime_error("Config Parser Error: {} is not close? or find not [;]");
    }
    std::string string_rest = str.substr(end+1);
    if(string_rest.size() > 0){
        retrieve_properties(string_rest, properties);
    }
    object->assign_properties(properties);
    return parsered_data;
}

#endif
