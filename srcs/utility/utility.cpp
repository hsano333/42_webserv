
#include "utility.hpp"
#include "split.hpp"
#include "global.hpp"
#include <algorithm>
#include <cstdlib>
#include <dirent.h>
#include <iostream>
#include <istream>
#include <sstream>
#include <vector>
#include <sys/stat.h>


using std::cout;
using std::endl;
using std::make_pair;
using std::string;

#include <stdio.h>

size_t Utility::strlen(const char* str)
{
    size_t cnt = 0;
    while (str[cnt]) {
        cnt++;
    }
    return (cnt);
}

void	*Utility::memset(void *b, int c, size_t len)
{
    size_t i = 0;
    while (i < len)
        ((unsigned char *)b)[i++] = c;
    return (b);
}

char* Utility::memcpy(char* dst, const char* src, size_t n)
{
    size_t i;
    unsigned char* dst_p;
    unsigned char* src_p;

    if (n == 0)
        return (dst);
    else if (dst == NULL && src == NULL)
        return (NULL);
    i = n;
    dst_p = (unsigned char*)dst;
    src_p = (unsigned char*)src;
    while (i--) {
        // cout << "i=" << i << endl;
        *(dst_p++) = *(src_p++);
    }
    return (dst);
}


char* Utility::strdup(const char* str)
{
    size_t len = Utility::strlen(str);
    char *tmp = new char[len+1];
    tmp[len] = '\0';
    Utility::memcpy(tmp, str, len);
    return (tmp);
}

static char* process(const char* haystack, const char* needle, size_t len)
{
    size_t i;
    size_t k;
    size_t len_n;

    i = -1;
    k = 0;
    len_n = Utility::strlen(needle);
    if (len_n == 0)
        return ((char*)haystack);
    if (len_n > Utility::strlen(haystack) || len_n > len)
        return (NULL);
    while (++i < len) {
        if (haystack[i] == needle[k])
            k++;
        else {
            i -= k;
            k = 0;
        }
        if (len_n == k)
            return (&(((char*)haystack)[i - len_n + 1]));
    }
    return (NULL);
}

char* Utility::strnstr(const char* haystack, const char* needle, size_t len)
{
    if (len == 0) {
        if (needle == NULL || needle[0] == '\0')
            return ((char*)haystack);
        return (NULL);
    }
    return (process(haystack, needle, len));
}

string Utility::to_lower(string str)
{
    string result = "";
    for (size_t i = 0; i < str.length(); i++) {
        result += tolower(str[i]);
    }
    return (result);
}

string Utility::trim_white_space(string const &str)
{
    std::string::size_type left = str.find_first_not_of("\t\n\v\f\r ");
    if (left != std::string::npos) {
        std::string::size_type right = str.find_last_not_of("\t\n\v\f\r ");
        return str.substr(left, right - left + 1);
    } else
        return "";
}

std::string Utility::delete_duplicated_slash(std::string str)
{
    string delimiter = "/";
    Split sp(str, delimiter);
    std::string only_one_slash;
    if (str[0] == '/') {
        only_one_slash = delimiter;
    }
    for (size_t i = 0; i < sp.size(); i++) {
        only_one_slash += sp[i];
        only_one_slash += "/";
    }
    if (str[str.size() - 1] != '/') {
        only_one_slash.erase(only_one_slash.end() - 1, only_one_slash.end());
    }
    return (only_one_slash);
}

static bool orderby_string_len(string a, string b)
{
    return (a.size() >= b.size());
}

void Utility::sort_orderby_len(std::vector<std::string>& str)
{
    (sort(str.begin(), str.end(), orderby_string_len));
}

/*
int char_to_hex(char c)
{
    if ('0' <= c && c <='9'){
        return (c-'0');
    }else if ('a' <= c && c <= 'f'){
        return (c-'a' + 10);
    }else if ('A' <= c && c <= 'F'){
        return (c-'A' + 10);
    }
    return (-1);
}
*/

unsigned char char_to_hex(char c)
{
    if ('0' <= c && c <='9'){
        return (c-'0');
    }else if ('a' <= c && c <= 'f'){
        return (c-'a' + 10);
    }else if ('A' <= c && c <= 'F'){
        return (c-'A' + 10);
    }
    WARNING("Utility::char_to_hex() error: invalid argument:");
    throw std::invalid_argument("Utility::char_to_hex() error: invalid argument");
    return (0);
}

char Utility::hex_string_to_int(const std::string& hex_string)
{
    if (hex_string.size () > 2){
        return (0);
    }
    if (hex_string.size () == 1){
        return (char_to_hex(hex_string[0]));
    }
    if (hex_string.size () == 2){
        int tmp = char_to_hex(hex_string[0]);
        if(tmp< 0){
            return (tmp);
        }
        int val = (tmp) << 4;
        tmp =  char_to_hex(hex_string[1]);
        if(tmp < 0){
            return (tmp);
        }
        val += (tmp);
        return (val);
    }
    return (0);
}


unsigned char Utility::hex_string_to_uchar(const std::string& hex_string)
{
    //sstream ss;
    //unsigned char tmp;
    if (hex_string.size () > 2){
        WARNING("Utility::hex_string_to_int() error: invalid argument:" + hex_string);
        throw std::invalid_argument("Utility::hex_string_to_int() error: invalid argument");
    }
    if (hex_string.size () == 1){
        return (char_to_hex(hex_string[0]));
    }
    if (hex_string.size () == 2){
        unsigned char tmp = char_to_hex(hex_string[0]);
        unsigned char val = (tmp) << 4;
        tmp =  char_to_hex(hex_string[1]);
        val += (tmp);
        return (val);
    }
    return (0);
}

char** Utility::cp_env(char** environ, std::vector<string> vec)
{
    size_t i=0;
    while(environ[i]){
        i++;
    }
    char **env = (char **)malloc(sizeof(char*) * (i+vec.size()+1));
    i = 0;
    while(environ[i])
    {
        env[i] = Utility::strdup(environ[i]);
        i++;
    }
    for(size_t j=0; j<vec.size();j++){
        env[i+j] = Utility::strdup(vec[j].c_str());
    }
    env[i+vec.size()] = NULL;
    return (env);
}

std::string Utility::to_hexstr(size_t i)
{
    stringstream ss;
    ss << std::hex << i;
    return (ss.str());
}


static bool is_notdigit(const char &c)
{
    if('0' <= c && c <= '9'){
        return false;
    }
    return true;
}

unsigned int Utility::to_uint(string const &str)
{

    std::string::const_iterator ite = std::find_if(str.begin(), str.end(), is_notdigit);
    if(ite != str.end()){
        WARNING("Utility::to_uint() error: invalid argument:" + str);
        throw std::invalid_argument("Utility::to_uint() error: invalid argument");
    }

    unsigned int cvt;
    stringstream ss;
    ss << str;
    ss >> cvt;

    return (cvt);
}


int Utility::to_int(string &str)
{
    std::string const &s = str;
    return (Utility::to_int(s));
}

int Utility::to_int(string const &str)
{
    if (str == ""){
        WARNING("Utility::to_int() error: invalid argument:" + str);
        throw std::invalid_argument("Utility::to_int() error: invalid argument");
    }

    std::string::const_iterator begin = str.begin();
    if(str[0] == '-'){
        begin++;
    }

    std::string::const_iterator ite = std::find_if(begin, str.end(), is_notdigit);
    if(ite != str.end()){
        WARNING("Utility::to_int() error: invalid argument:" + str);
        throw std::invalid_argument("Utility::to_int() error: invalid argument");
    }


    int cvt;
    stringstream ss;
    ss << str;
    ss >> cvt;

    return (cvt);
}

int Utility::to_int(char const *str)
{
    std::string tmp = str;
    return (Utility::to_int(tmp));
}

ssize_t Utility::to_ssize_t(string &str)
{
    ssize_t cvt;
    stringstream ss;
    ss << str;
    ss >> cvt;

    return (cvt);
}

ssize_t Utility::to_ssize_t(string const &str)
{
    return (Utility::to_ssize_t(str));
}

size_t Utility::to_size_t(string &str)
{
    size_t cvt;
    stringstream ss;
    ss << str;
    ss >> cvt;

    return (cvt);
}

size_t Utility::to_size_t(string const &str)
{
    return Utility::to_size_t(str);
}

/*
int Utility::read_body_and_copy(char *src, char** dst, size_t size)
{
    char *tmp = *dst;
    int chunk_size = 20;

    // chunkサイズは16進数
    string size_str = Utility::to_hexstr(size);
    size_str += "\r\n";
    size_t len = size_str.size();
    Utility::memcpy(&(tmp[chunk_size]), src, size);
    Utility::memcpy(&(tmp[chunk_size-len]), size_str.c_str(), len);
    *dst= &(tmp[chunk_size-len]);
    return (size+len);
}
*/

size_t Utility::get_map_str_size(map<string, string> &data)
{
    map<string,string>::iterator ite = data.begin();
    map<string,string>::iterator end = data.end();
    size_t size = 0;
    for(; ite != end ; ite++){
        size += ite->second.size();
    }
    return (size);
}

std::string Utility::extract_json_object(const std::string &target, std::string &str)
{

    cout << target << endl;
    cout << str << endl;
    std::string tmp = "test";
    return tmp;
}

bool Utility::is_regular_file(std::string const &path)
{
    struct stat fileInfo;
    if (stat(path.c_str(), &fileInfo) != 0){
        return false;
    }
    return (fileInfo.st_mode & S_IFREG);
}


bool Utility::is_executable_file(std::string const &path)
{
    struct stat fileInfo;
    if (stat(path.c_str(), &fileInfo) != 0){
        return false;
    }
    return (fileInfo.st_mode & S_IFREG) && (fileInfo.st_mode & S_IXOTH);
}

bool Utility::is_redable_file(std::string const &path)
{
    struct stat fileInfo;
    if (stat(path.c_str(), &fileInfo) != 0){
        return false;
    }
    return (fileInfo.st_mode & S_IFREG) && (fileInfo.st_mode & S_IRUSR);
}

bool Utility::is_writable_file(std::string const &path)
{
    struct stat fileInfo;
    if (stat(path.c_str(), &fileInfo) != 0){
        return false;
    }
    return (fileInfo.st_mode & S_IFREG) && (fileInfo.st_mode & S_IWUSR);
}

bool Utility::is_directory(std::string const &path)
{
    struct stat fileInfo;
    if (stat(path.c_str(), &fileInfo) != 0){
        return false;
    }
    return (fileInfo.st_mode & S_IFDIR);
}

bool Utility::is_redable_directory(std::string const &path)
{
    struct stat fileInfo;
    if (stat(path.c_str(), &fileInfo) != 0){
        return false;
    }
    return (fileInfo.st_mode & S_IFDIR) && (fileInfo.st_mode & S_IRUSR);
}


std::string Utility::time_to_string()
{
    char datetime[64];
    std::time_t now = std::time(NULL);
    std::strftime(datetime, sizeof(datetime), "%a, %d %b %Y %T GMT", std::gmtime(&now));
    return (datetime);
}

size_t Utility::get_file_size(std::string const &filepath)
{
    struct stat fileInfo;
    if (stat(filepath.c_str(), &fileInfo) != 0){
        return (0);
    }
    return (fileInfo.st_size);
    //return (fileInfo.st_blocks);
}

std::string Utility::get_file_updated_date(std::string const &filepath)
{
    struct stat fileInfo;
    if (stat(filepath.c_str(), &fileInfo) != 0){
        return (0);
    }

    char datetime[64];
    Utility::memset(datetime, 0, 64);
    //std::time_t now = std::time(NULL);
    //std::time_t now = fileInfo.st_ctime;
    //std::strftime(datetime, sizeof(datetime), " %T", std::gmtime(&now));
    std::strftime(datetime, sizeof(datetime), "%d-%b-%Y %T", std::gmtime(&fileInfo.st_ctime));
    return (datetime);
    //return std::ctime(&fileInfo.st_ctime);
}

std::string Utility::adjust_filesize(size_t filesize)
{
    int cnt = 0;
    size_t filesize_cp = filesize;
    while(filesize_cp){
        filesize_cp/= 1024;
        cnt++;
    }
    std::string size_str = "";
    if (cnt <= 1){
        size_str = Utility::to_string(filesize);
    }else if(cnt == 2){
        size_str = Utility::to_string(filesize/1024) + "k";
    }else if(cnt == 3){
        size_str = Utility::to_string(filesize/1024/1024) + "M";
    }else if(cnt == 4){
        size_str = Utility::to_string(filesize/1024/1024/1024) + "G";
    }else if(cnt >= 5){
        size_str = Utility::to_string(filesize/1024/1024/1024/1024) + "T";
    }
    return (size_str);
}


// 存在しないステータスコードを指定すると、空文字列を返す
string Utility::get_http_status_message(string status_code)
{
    map<string, string> status_codes;
    // 1xx
    status_codes.insert(make_pair("100", "Continue"));
    status_codes.insert(make_pair("101", "Switching Protocols"));
    status_codes.insert(make_pair("102", "Processing"));
    // 2xx
    status_codes.insert(make_pair("200", "OK"));
    status_codes.insert(make_pair("201", "Created"));
    status_codes.insert(make_pair("202", "Accepted"));
    status_codes.insert(make_pair("203", "Non-Authoritative Information"));
    status_codes.insert(make_pair("204", "No Content"));
    status_codes.insert(make_pair("205", "Reset Content"));
    status_codes.insert(make_pair("206", "Partial Content"));
    status_codes.insert(make_pair("207", "Multi-Status"));
    status_codes.insert(make_pair("208", "Already Reported"));
    status_codes.insert(make_pair("226", "IM Used"));
    // 3xx
    status_codes.insert(make_pair("300", "Multiple Choices"));
    status_codes.insert(make_pair("301", "Moved Permanently"));
    status_codes.insert(make_pair("302", "Found"));
    status_codes.insert(make_pair("303", "See Other"));
    status_codes.insert(make_pair("304", "Not Modified"));
    status_codes.insert(make_pair("305", "Use Proxy"));
    status_codes.insert(make_pair("307", "Temporary Redirect"));
    status_codes.insert(make_pair("308", "Permanent Redirect"));
    // 4xx
    status_codes.insert(make_pair("400", "Bad Request"));
    status_codes.insert(make_pair("401", "Unauthorized"));
    status_codes.insert(make_pair("402", "Payment Required"));
    status_codes.insert(make_pair("403", "Forbidden"));
    status_codes.insert(make_pair("404", "Not Found"));
    status_codes.insert(make_pair("405", "Method Not Allowed"));
    status_codes.insert(make_pair("406", "Not Acceptable"));
    status_codes.insert(make_pair("407", "Proxy Authentication Required"));
    status_codes.insert(make_pair("408", "Request Timeout"));
    status_codes.insert(make_pair("409", "Conflict"));
    status_codes.insert(make_pair("410", "Gone"));
    status_codes.insert(make_pair("411", "Length Required"));
    status_codes.insert(make_pair("412", "Precondition Failed"));
    status_codes.insert(make_pair("413", "Payload Too Large"));
    status_codes.insert(make_pair("414", "URI Too Long"));
    status_codes.insert(make_pair("415", "Unsupported Media Type"));
    status_codes.insert(make_pair("416", "Range Not Satisfiable"));
    status_codes.insert(make_pair("417", "Expectation Failed"));
    status_codes.insert(make_pair("418", "I'm a teapot"));
    status_codes.insert(make_pair("421", "Misdirected Request"));
    status_codes.insert(make_pair("422", "Unprocessable Entity"));
    status_codes.insert(make_pair("423", "Locked"));
    status_codes.insert(make_pair("424", "Failed Dependency"));
    status_codes.insert(make_pair("425", "Too Early"));
    status_codes.insert(make_pair("426", "Upgrade Required"));
    status_codes.insert(make_pair("428", "Precondition Required"));
    status_codes.insert(make_pair("429", "Too Many Requests"));
    status_codes.insert(make_pair("431", "Request Header Fields Too Large"));
    status_codes.insert(make_pair("451", "Unavailable For Legal Reasons"));
    // 5xx
    status_codes.insert(make_pair("500", "Internal Server Error"));
    status_codes.insert(make_pair("501", "Not Implemented"));
    status_codes.insert(make_pair("502", "Bad Gateway"));
    status_codes.insert(make_pair("503", "Service Unavailable"));
    status_codes.insert(make_pair("504", "Gateway Timeout"));
    status_codes.insert(make_pair("505", "HTTP Version Not Supported"));
    status_codes.insert(make_pair("506", "Variant Also Negotiates"));
    status_codes.insert(make_pair("507", "Insufficient Storage"));
    status_codes.insert(make_pair("508", "Loop Detected"));
    status_codes.insert(make_pair("510", "Not Extended"));
    status_codes.insert(make_pair("511", "Network Authentication Required"));
    return status_codes[status_code];
}

#ifdef UNIT_TEST
#include "doctest.h"
TEST_CASE("trim_white_space")
{
    CHECK(Utility::trim_white_space("  \t\n\v\f\r  ") == "");
    CHECK(Utility::trim_white_space("  \t\n\v\f\r  a") == "a");
    CHECK(Utility::trim_white_space("a  \t\n\v\f\r  ") == "a");
    CHECK(Utility::trim_white_space("  \t\n\v\f\r  a  \t\n\v\f\r  ") == "a");
    CHECK(Utility::trim_white_space("  \t\n\v\f\r  a  \t\n\v\f\r  b  \t\n\v\f\r  ") == "a  \t\n\v\f\r  b");
}

TEST_CASE("hex_string_to_uchar")
{
    CHECK(Utility::hex_string_to_uchar("11") == 17);
    CHECK(Utility::hex_string_to_uchar("0") == 0);
    CHECK(Utility::hex_string_to_uchar("ff") == 255);
    CHECK(Utility::hex_string_to_uchar("F0") == 240);
    CHECK_THROWS_AS(Utility::hex_string_to_uchar("FF0") ,std::invalid_argument);
    CHECK_THROWS_AS(Utility::hex_string_to_uchar("fg") ,std::invalid_argument);
    CHECK_THROWS_AS(Utility::hex_string_to_uchar("g3") ,std::invalid_argument);
}

#endif
