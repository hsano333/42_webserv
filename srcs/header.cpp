/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsano <hsano@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 23:24:19 by hsano             #+#    #+#             */
/*   Updated: 2023/07/09 01:13:36 by sano             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.hpp"
#include "split.hpp"
#include "utility.hpp"
#include <climits>

Header::Header() : _err(false), _err_code(0), _chunked(false)
{
}

Header::~Header()
{
}

void Header::print() const
{
    map<string, string>::const_iterator ite = _headers.begin();
    map<string, string>::const_iterator end = _headers.end();
    cout << endl  << "Print Headers" << endl;
    for (; ite != end; ite++)
        cout << " [" << (*ite).first << "]: [" << (*ite).second << "]" << endl;

}

void Header::load_data(char *buf)
{
        Split sp_line(buf, "\r\n");
        for(size_t i=0;i<sp_line.size();i++){
            string tmp = sp_line[i];
            size_t dquote_pos1 = sp_line[i].find('\"');
            size_t dquote_pos2 = 0;
            if (dquote_pos1 != std::string::npos){
                // find DQUOT, it is error in my rule.
                this->_err = true;
                this->_err_code = 400;
                cout << "dquote_pos1:" << dquote_pos1 << ", dquote_pos2:" << dquote_pos2 << endl;
            }else{
                size_t colon_pos = sp_line[i].find(':');
                if (colon_pos == 0 || colon_pos == sp_line[i].size() - 1 || colon_pos == std::string::npos){
                    this->_err = true;
                    this->_err_code = 400;
                }
                string field_name = sp_line[i].substr(0, colon_pos);
                std::transform(field_name.begin(), field_name.end(), field_name.begin(), static_cast<int (*)(int)>(std::toupper));
                string field_value = sp_line[i].substr(colon_pos+1);

                if (field_name.find(" ", field_name.size()-1) == field_name.size()-1){
                        //As specified in RFC7230, it return an error code 400 if there is a space vefore the colon(:).
                        this->_err = true;
                        this->_err_code = 400;
                        return ;
                }

                map<string, string>::iterator ite = _headers.find(field_name);
                map<string, string>::iterator end = _headers.end();
                if (ite == end){
                    _headers.insert(make_pair(field_name, field_value));
                }else{
                    _headers[field_name] = ite->second + "," + field_value;
                }
            }
        }
}

void Header::check_header_related_body()
{
    const string trans_enc_str =  "transfer-encoding";
    string &trans_enc = this->find(trans_enc_str);
    if (trans_enc == "chunked"){
        this->_chunked = true;
    }else{
        this->_chunked = false;
    }
}

std::string &Header::find(const string &name)
{
     map<string, string>::iterator ite = _headers.find(name);
     if (ite == _headers.end()){
         return (this->_not_find);
     }
     return (ite->second);
}

size_t Header::get_content_length()
{
    if (_content_length != 0){
        return (_content_length);
    }
    std::string name = "content-length";
    string &value = find(name);
    if (value == this->_not_find){
        this->_content_length = INT_MAX;
        return (_content_length);
    }
    int value_size = Utility::to_int(value);
    if (value_size <= 0){
        this->_content_length = INT_MAX;
        return (_content_length);
    }
    this->_content_length = value_size;
    return (_content_length);
}
