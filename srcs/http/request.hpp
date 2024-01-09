#ifndef REQUEST_H
#define REQUEST_H

#include "http_data.hpp"
//#include "config.hpp"
#include "config_location.hpp"
#include "global.hpp"
#include "ireader.hpp"
#include "socket_data.hpp"
#include "split.hpp"
#include "uri.hpp"
#include "header.hpp"
#include "method.hpp"
#include "file.hpp"
#include <map>
#include <string>
#include <vector>
#include "file_discriptor.hpp"
#include "request_line.hpp"

using std::map;
using std::string;
using std::vector;

typedef enum E_STATE
{
    STILL_NOT_LOAD,
    LOADED_STATUS_LINE,
    LOADED_STATUS_HEADER,
    LOADED_STATUS_BODY,
    END_LOAD,

} REQUEST_STATE;

class Request : public File
{
    public:
        Request();
        ~Request();

        static Request *from_fd(FileDiscriptor fd);
        //char*   buf();
        void    clear_raw_buf();
        //size_t  buf_size();
        //char*   get_raw_buf_pointer();
        //char*   get_raw_buf_begin();
        //int     raw_buf_space();
        char*   get_buf_body(int *size);
        void    set_buf_body(char *body_p, int size);
        void    clear_buf_body();
        void    set_request_line(std::string const &str);
        void    set_header(Split &sp, size_t offset);
        void    set_read_completed(bool flag);
        void    set_requested_filepath(const ConfigLocation *location);
        bool    read_completed();

        void    check_parent_permittion(std::string &path);

        bool    is_file() const;
        bool    is_directory() const;
        bool    is_not_executable_parent_dir() const;
        bool    is_deletable_parent_dir() const;

        void    set_is_file(bool flag);
        void    set_is_directory(bool flag);
        //void    set_buf_pos(size_t pos);

        std::string const &requested_filepath() const;
        std::string const &requested_path() const;
        std::string const &tmp_path_info() const;
        std::string const &parent_dir_path() const;
        //File    get_target_file(const ConfigLocation *location);

        void    print_info() const;
        RequestLine const &req_line() const;
        Header const &header() const;

        FileDiscriptor fd() const;
        //FileDiscriptor sockfd() const;

        // File Interface
        int open();
        int close();
        int read(char **buf, size_t size);
        int write(char **buf, size_t size);
        bool can_read();
        size_t size();
        bool is_chunk();
        int remove();
        std::string const &path();

        //void set_source_file(File *source);
        //File *get_source_file();
        //int open_source_file();
        //int close_source_file();
        //ssize_t get_data(char** data);
        //
    private:
        File *file;
        FileDiscriptor fd_;
        //FileDiscriptor sockfd_;
        //char    raw_buf[MAX_BUF];
        //char    raw_buf[200];
        //int     raw_buf_point;
        //size_t  raw_buf_pos_;
        char    *buf_body;
        int     buf_body_size;
        bool    is_file_;
        bool    is_directory_;
        bool    is_not_executable_parent_dir_;
        bool    is_deletable_parent_dir_;
        std::string requested_filepath_;
        std::string requested_path_;
        std::string parent_dir_path_;

        RequestLine    req_line_;
        Header         header_;
        std::string    tmp_path_info_;
        bool           read_completed_;
        //File *source;

};

/*
class Request : public SocketData
{
    public:
        Request();
        virtual ~Request();
        Request(FileDiscriptor fd);
        Request(FileDiscriptor fd, IReader* iread);
        void read_up_to_header();
        REQUEST_STATE get_state();
        void set_port(Port &port);
        const Port& get_port() const;
        const std::string& get_host() const;
        const string& get_path() const;
        //const Method& get_method() const;
        const Method& get_method() const;
        const URI& get_uri() const;
        void print_request() const;
        bool is_cgi();
        virtual void parse();
    protected:
        int _fd;
        Method method_;
        char _buf[MAX_BUF+1];
        bool _buf_max;
        int _buf_pos;
        int _end_status_line;
        char* body_pos;
        int  _body_size;
        REQUEST_STATE _state;
        //string _status_line;
        IReader* _iread;
        //std::map<std::string, std::string > _headers;
        Header _headers;
        //URI* _uri;
        //int _tmp_read_size;
        //string status_code;
        //URI* _uri;
        bool _err;
        int  _err_code;
        bool _is_status_read_complete;
        bool _is_header_read_complete;

        void parse_status_line(char *crlf, int read_size);
        void parse_header();
        void load_body();
        int read_fd();

        string _host;
        URI _uri;
        string _path;
        Port port;
        //string _method;
        string _version;

        std::string& find_headers(string &name);
        //ContentType _content_type;
        //ssize_t _content_length;
};
*/


//class Request : public SocketData
//{
//  public:
//    Request(FileDiscriptor fd);
//    Request(FileDiscriptor fd, std::string& port);
//    Request(FileDiscriptor fd, IRead* iread);
//    virtual ~Request();
//
//    void print_request() const;
//
//    string _except_body;
//    void read_up_to_header();
//
//    // -- request line --
//    const string& get_version() const;
//    HttpMethod get_method() const;
//    const string& get_path() const;
//    vector<string> get_path_list() const;
//    const URI& get_uri();
//
//    // -- request header --
//    const map<string, string>& get_headers() const;
//    string get_domain() const;
//    string get_ip_address() const;
//    ssize_t get_content_length() const;
//    string get_transfer_encoding() const;
//    bool is_cgi() const;
//    ContentType const& get_content_type() const;
//    size_t get_header_sum_size();
//
//    // -- config --
//    //Config const* get_config() const; // これいらなくない？
//    Server const* get_server_config() const;
//    Location const* get_location_config() const;
//    void parse_server_config();
//    std::string& get_port();
//    std::string& get_host();
//    // std::string& get_host() const;
//    // static string identify_method(METHOD method);
//    // static METHOD identify_method(string method);
//    // bool increment_timeout(int time);
//    // void clear_timeout();
//
//    // -- body --
//    // bodyはメモリに格納できないレベルで大きい場合があるので、（一旦ファイルに書き込む（予定））
//    //ByteVector read_header(); // will move to private
//    ByteVector read_body(); // will move to private
//    ByteVector get_body_text();
//    ByteVector get_body() const;
//    vector<ByteVector> get_body_splitted() const;
//    virtual bool is_full_body_loaded() const;
//    string _path;
//
//  protected:
//    const static int BUF_MAX = 1600;
//    // -- init --
//    virtual void parse();
//    virtual void parse_request_line();
//    virtual void parse_header_field();
//    //virtual bool read_rest();
//    void parse_content_length();
//    void read_to_header();
//
//    // -- util --
//    bool is_contents_upload_file(string const& content_type);
//
//    // -- 内部変数 --
//    const int _fd;
//    ByteVector _loaded_packet_body;
//    RawRequestReader _buf;
//    char buf[2048];
//    int buf_pos;
//    vector<string> _tmp_body_file_list;
//
//    // -- request line --
//    // string _path;
//    HttpMethod _method;
//    string _version;
//    URI _uri;
//
//    // -- request header --
//    bool _header_read_complete;
//    ssize_t _content_length;
//    TransferEncoding _transfer_encoding;
//    map<string, string> _headers;
//    ByteVector _body;
//    string _err_line;
//    string _domain;
//    string _ip;
//    string _port;
//    string _host;
//    ContentType _content_type;
//};
//
#endif
