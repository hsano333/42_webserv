#ifndef WEBSERV_ENTITY_HPP
#define WEBSERV_ENTITY_HPP

#include "file_discriptor.hpp"
//#include "application.hpp"
#include "application_result.hpp"
#include "webserv_io.hpp"
#include "body.hpp"

class Application;
class WebservEntity
{
    public:
        WebservEntity();
        ~WebservEntity();
        WebservEntity(FileDiscriptor &fd, FileDiscriptor &socket_fd, Config *cfg);
        WebservEntity(WebservEntity const &entity);
        WebservEntity& operator=(WebservEntity const &entity);

        void clean();

        void set_app(Application *app);
        void set_result(ApplicationResult *result);
        void set_request(Request *req);
        void set_response(Response *res);
        void set_completed(bool flag);
        void set_force_close(bool flag);

        FileDiscriptor const &fd();
        FileDiscriptor const &socket_fd();
        Application          *app();
        ApplicationResult    *app_result();
        Request     *request();
        Response    *response();
        Body        &body();
        WebservIO   &io();

        Config const *config();
        bool completed(){return this->completed_;};
        bool force_close();
        //Body const &body() const;
        void set_body(Body &body);
    private:
        FileDiscriptor fd_;
        FileDiscriptor sock_fd_;
        Request *req_;
        Response *res_;
        Config *cfg_;
        Application       *app_;
        ApplicationResult *app_result_;
        WebservIO io_;

        bool completed_;
        bool force_close_;
        Body body_;

};
#endif
