#ifndef URI_HPP
#define URI_HPP
#include <string>
#include <vector>
#include <map>

class Request;

class URI
{
  public:
    URI();
    //URI(std::string& uri);
    //URI(Request* req);
    URI(const URI& uri);
    URI& operator=(const URI& uri);
    //void set_raw(std::string& raw);
    void parse(Request* req);
    ~URI();
    void print_uri() const;
    bool is_cgi() const;
    bool is_file() const;
    const std::string& get_uri() const;
    const std::string& get_filename() const;
    const std::string& get_root() const;
    const std::string& get_relative_filepath() const;
    const std::string& get_filepath() const;
    const std::string& get_location_path() const;
    const std::vector<std::string>& get_query() const;
    const std::string& get_path_info() const;
    void uri_encode(std::string& raw_uri);
    void ignore_fragment(std::string& uri);
    void retrieve_query(std::string& uri);

  private:
    std::string _raw_uri;
    std::string _endorded_uri;
    //std::string _raw_uri;
    std::string _filename;
    std::string _root;
    std::string _relative_filepath;
    std::string _filepath;
    std::string _location_path;
    std::vector<std::string> _querys;
    std::string _path_info;
    bool _is_cgi;
    bool _is_file;
    bool _is_directory;

    void remove_file_info(std::string tmp_file_path);
    std::string get_root_dir(std::map<std::string, std::vector<std::string> >& props);
    std::string concat_path(std::string root, std::string uri);
    void check_invalid_word(std::string &str);
};
#endif
