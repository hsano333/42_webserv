/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsano <hsano@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 08:44:30 by hsano             #+#    #+#             */
/*   Updated: 2023/10/22 02:11:58 by sano             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include "status_code.hpp"
#include "error_page.hpp"
#include "file.hpp"
#include <unistd.h>
#include <map>

enum SEND_STATE{
    ERROR,
    STILL_NOT_SEND,
    SENT_STATUS_LINE,
    SENT_HEADER,
    OPEND_FILE,
    SENT_BODY,
    CLOSE,
};

class Response
{
    public:
        Response();
        ~Response();
        Response(Response const &res);
        Response& operator=(Response const &res);
        static Response* from_status_code(StatusCode &code);
        static Response* from_error_page(ErrorPage &page);

        static Response* from_redirect(StatusCode &code, std::string const &filepath);
        static Response* from_file(File *file);
        void add_header(std::string const &key, std::string const &value);
        //static Response* from_file(std::string const &filepath);
        //static Response* from_directory(std::string const &filepath);
        int open_file();
        int close_file();
        ssize_t get_data(char** data);
        void print_info();
        void set_exist_body(bool flag);

    private:
        StatusCode status_code;
        std::map<std::string, std::string> headers;
        File *file;

        bool is_redirect;
        SEND_STATE send_state;
        std::string status_line;
        std::string header_line;

        void make_status_line();
        void make_header_line();
        int read_body_and_copy_chunk(char** dst, size_t size);
        int read_body_and_copy(char** dst, size_t size);
        bool exist_body_;
};


#endif /* RESPONSE_HPP */
