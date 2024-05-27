#ifndef APPLICATION_RESULT_HPP
#define APPLICATION_RESULT_HPP
//#include "application.hpp"
#include "process_id.hpp"
#include "config_location.hpp"
#include "config.hpp"
#include "request.hpp"
#include "webserv_file.hpp"
#include "status_code.hpp"
#include "buffer_controller.hpp"
#include "ireader.hpp"
#include "iwriter.hpp"

//class Application;
class ApplicationResult
{
    public:
        ~ApplicationResult();

        static ApplicationResult* from_status_code(StatusCode &code, Method const &method);
        static ApplicationResult* from_status_code(StatusCode &code, string method);
        static ApplicationResult* from_fd(int in, int out, int pid);

        int open();
        int read(char **data, size_t size);
        int write(char **data, size_t size);
        int close();

        //bool can_read();
        //int remove();
        //std::string const &path();

        Method const &method() const;
        StatusCode const &status_code() const;
        Header const &header() const;
        WebservFile *file();
        //void set_app(Application *app);
        void set_file(WebservFile *file);

        bool completed();
        //bool is_completed();
        void set_status_code(int status_code);
        void set_completed(bool flag);
        void add_header(std::string const &key, std::string const &value) ;
        void set_is_cgi(bool flag);
        bool is_cgi();

        //Application    *app();
        FileDiscriptor &cgi_in();
        FileDiscriptor &cgi_out();
        ProcessID      &pid();
        bool            is_chunk();
    private:
        ApplicationResult();
        ApplicationResult(StatusCode &code, Method const &method);
        ApplicationResult(StatusCode &code, string &method);

        char *app_;
        StatusCode code_;
        //std::map<std::string, std::string> header_;
        Header         header_;
        Method method_;
        std::string text;
        WebservFile *file_;
        bool completed_;
        FileState   state;
        BufferController buffer;
        bool           is_cgi_;
        IReader *reader;
        IWriter *writer;
        FileDiscriptor cgi_in_;
        FileDiscriptor cgi_out_;
        ProcessID      pid_;

};
#endif
