/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv_write_event.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsano <hsano@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 01:24:28 by hsano             #+#    #+#             */
/*   Updated: 2024/02/13 16:27:05 by sano             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef WEBSERV_EVENT_WRITE_HPP
#define WEBSERV_EVENT_WRITE_HPP
#include "webserv_event.hpp"
#include "file_discriptor.hpp"
#include "iwriter.hpp"
#include "response.hpp"
#include "file.hpp"
#include "webserv_cgi_event.hpp"
#include "http_data.hpp"
#include "webserv_entity.hpp"

// source_fileからデータを読み出し、fdに対して書き込む
class WebservWriteEvent
{
    public:
        ~WebservWriteEvent();
        EWebservEvent which();

        FileDiscriptor  &fd();
        Request         *req();
        Response        *res();
        File            *src();
        File            *dst();
        void            set_src(File *file);
        void            set_dst(File *file);
        //HttpData        *source();
        //File            *source();
        bool is_completed();
        void set_completed(bool flag);
        void increase_timeout_count(int count);
        int  timeout_count();
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        E_EpollEvent get_next_epoll_event(WebservEvent *event);
        int write(char *buf, size_t size);
        //static WebservWriteEvent *from_error_status_code(WebservEvent *event, StatusCode &code, File *file);
        //static WebservWriteEvent *from_event_for_cgi(WebservEvent *event, Response *res);
        static WebservEvent *from_event(WebservEvent *event, File *src, File *dst);
        //static WebservWriteEvent *from_cgi_fd(FileDiscriptor fd, Request *req, IReader *reader);
        //void set_cgi_event(WebservCgiEvent *cgi_event);
        //WebservCgiEvent *cgi_event();
        //WebservEntity *entity();

    private:
        WebservWriteEvent();
        WebservWriteEvent(File *src,  File *dst);

        FileDiscriptor  fd_;
        Request         *req_;
        Response        *res_;
        //HttpData        *source_;
        File            *source_file;
        File            *destination_file;
        int             timeout_count_;
        //IWriter *writer;
        //IReader *reader;
        bool is_completed_;
        WebservCgiEvent *cgi_event_;
        //WebservEntity   *entity_;
};
#endif
