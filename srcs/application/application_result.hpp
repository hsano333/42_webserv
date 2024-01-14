#ifndef APPLICATION_RESULT_HPP
#define APPLICATION_RESULT_HPP
#include "application.hpp"
#include "config_location.hpp"
#include "config.hpp"
#include "request.hpp"
#include "file.hpp"
#include "status_code.hpp"
#include "buffer_controller.hpp"
#include "ireader.hpp"
#include "iwriter.hpp"

class ApplicationResult : public File
{
    public:
        ~ApplicationResult();

        static ApplicationResult* from_status_code(StatusCode &code);

        int open();
        int read(char **data, size_t size);
        int write(char **data, size_t size);
        int save(char *data, size_t size);
        int close();

        bool can_read();
        int remove();

        Method const &method() const;
        StatusCode const &status_code() const;
        std::map<std::string, std::string> const &header() const;
        File *file();
        void set_file(File *file);

        bool is_completed();
        //void set_status_code(StatusCode &code);
        void set_completed(bool flag);
        void add_header(std::string const &key, std::string &value) ;

    private:
        ApplicationResult();
        ApplicationResult(StatusCode code);

        StatusCode code_;
        std::map<std::string, std::string> header_;
        Method method_;
        std::string text;
        File *file_;
        bool completed;
        FileState   state;
        BufferController buffer;
        IReader *reader;
        IWriter *writer;

};
#endif
