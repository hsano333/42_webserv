#ifndef UTILITY_H
#define UTILITY_H
//#include "config.hpp"
#include "utility.hpp"
#include "ip_address.hpp"
#include <sstream>
#include <string>
#include <unistd.h>
#include <map>
#include <vector>
#include <iostream>
#include <ctime>

using std::map;
using std::pair;
using std::string;
using std::stringstream;
using std::vector;

class Utility
{
  public:
    static char* memcpy(char* dst, const char* src, size_t n);
    static void   *memset(void *b, int c, size_t len);
    static size_t strlen(const char* str);
    static string delete_space(string& str);
    static char* strnstr(const char* haystack, const char* needle, size_t len);
    static string to_lower(string str);
    static string trim_white_space(string const &str);

    static std::string delete_duplicated_slash(std::string str);
    static void sort_orderby_len(std::vector<std::string>& str);
    static unsigned char hex_string_to_uchar(const string& hex_string);
    static char** cp_env(char** environ, std::vector<string> vec);
    static char* strdup(const char* str);

    template <typename T>
    static string to_string(T value);
    static string to_string2(int value);
    static int to_int(string &str);
    static unsigned int to_uint(string const &str);
    static ssize_t to_ssize_t(string &str);
    static size_t to_size_t(string &str);
    static int to_int(string const &str);
    static int to_int(char const *str);
    static ssize_t to_ssize_t(string const &str);
    static size_t to_size_t(string const &str);
    static std::string to_hexstr(size_t i);
    static IP_Address name_to_address(std::string name);

    static char hex_string_to_int(const string& hex_string);
    static string get_http_status_message(string status_code);
    template <typename T>
    static void print_vector(vector<T> v);
    //static int read_body_and_copy(int fd, char** dst, size_t size);
    //static int read_body_and_copy(int fd, char** buf, int size, std::string str);
    //static int read_body_and_copy(ByteVector str, char** buf, int size);
    //static int read_body_and_copy(char *src, char** dst, size_t size);
    static size_t get_map_str_size(map<std::string, std::string> &data);

    static std::string extract_json_object(const std::string &target, std::string &str);
    static bool is_regular_file(std::string const &path);
    static bool is_executable_file(std::string const &path);
    static bool is_redable_file(std::string const &path);
    static bool is_writable_file(std::string const &path);
    static bool is_deletable_file(std::string const &path);

    static bool is_directory(std::string const &path);
    static bool is_redable_directory(std::string const &path);
    static bool is_executable_directory(std::string const &path);
    static bool is_writable_directory(std::string const &path);
    static bool is_deletable_directory(std::string const &path);
    static bool is_not_redable_directory(std::string const &path);

    static std::string time_to_string();
    static size_t get_file_size(std::string const &filepath);
    static std::string get_file_updated_date(std::string const &filepath);
    static std::string adjust_filesize(size_t filesize);

    static bool is_redable_directory_not_stat(std::string const &filepath);
};

template <typename T>
string Utility::to_string(T value)
{
    stringstream sss;
    string str;

    sss << value;
    str = sss.str();

    return (str);
}

template <typename T>
void Utility::print_vector(vector<T> v)
{
    for (size_t i = 0; i < v.size(); i++)
        if (v.size() - 1 != i)
            std::cout << v[i] << ",";
        else
            std::cout << v[i] << std::endl;
}

#endif /* UTILITY_H */
