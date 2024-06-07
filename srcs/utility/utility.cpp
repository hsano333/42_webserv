
#include "utility.hpp"
#include "split.hpp"
#include "global.hpp"
#include <cmath>
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
    size_t i = 0;
    size_t k = 0;
    size_t len_n;

    i = -1;
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

string Utility::get_extension(string const &str)
{
    size_t pos = str.rfind(".");
    if(pos == std::string::npos || pos == str.size()-1){
        return ("");
    }
    pos++;
    return (str.substr(pos));
}

int Utility::tolower(int c){
    if('A' <= c && c <= 'Z'){
        c = c - 'A' + 'a';
    }
    return (c);
}

string Utility::to_lower(string str)
{
    string result = "";
    for (size_t i = 0; i < str.length(); i++) {
        result += Utility::tolower(str[i]);
    }
    return (result);
}

string Utility::remove_obstruction_in_uri(string const &str)
{
    return Utility::delete_duplicated_slash(Utility::trim_white_space(str));
}


string Utility::trim_char(string const &str, char c)
{
    std::string::size_type left = str.find_first_not_of(c);
    if (left != std::string::npos) {
        std::string::size_type right = str.find_last_not_of(c);
        return str.substr(left, right - left + 1);
    } else
        return "";
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
    if(str.size() == 1 && str == "/"){
        return ("/");
    }
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

// less than 255(FF)
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


std::string Utility::to_hexstr(size_t i)
{
    stringstream ss;
    ss << std::hex << i;
    return (ss.str());
}

std::string Utility::uchar_to_hexstr(unsigned char c)
{
    unsigned int i = c;
    stringstream ss;
    ss << std::hex << i;
    return (ss.str());
}


size_t Utility::hex_string_to_number(const std::string &str)
{
    DEBUG("Utility::hex_string_to_number str=" + str);
    size_t size = str.size();
    if(size > 16){
        throw std::invalid_argument("Utility::hex_string_to_number:too large str=" + str);
    }
    size_t sum = 0;
    for(size_t i=0;i<str.size();i++){
        unsigned char tmp = char_to_hex(str[i]);
        size_t pow = std::pow(16, size-i-1);
        sum += tmp * pow;
    }
    return (sum);
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

size_t Utility::to_size_t(string const &str)
{
    std::string::const_iterator ite = std::find_if(str.begin(), str.end(), is_notdigit);
    if(ite != str.end()){
        WARNING("Utility::to_size_t() error: invalid argument:" + str);
        throw std::invalid_argument("Utility::to_size_t() error: invalid argument");
    }

    size_t cvt;
    stringstream ss;
    ss << str;
    ss >> cvt;
    if(str.size() < 20){
        return (cvt);
    }
    if(Utility::to_string(cvt) != str){
        throw std::invalid_argument("Utility::to_size_t() error: overflow:" + str);
    }
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

bool Utility::is_readable_file(std::string const &path)
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

bool Utility::is_deletable_file(std::string const &path)
{
    struct stat fileInfo;
    if (stat(path.c_str(), &fileInfo) != 0){
        return false;
    }
    return (fileInfo.st_mode & S_IFREG) && (fileInfo.st_mode & S_IWUSR) && (fileInfo.st_mode & S_IXUSR);
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

bool Utility::is_executable_directory(std::string const &path)
{
    struct stat fileInfo;
    if (stat(path.c_str(), &fileInfo) != 0){
        return false;
    }
    return (fileInfo.st_mode & S_IFDIR) && (fileInfo.st_mode & S_IXUSR);
}

bool Utility::is_not_redable_directory(std::string const &path)
{
    struct stat fileInfo;
    if (stat(path.c_str(), &fileInfo) != 0){
        return false;
    }
    return (fileInfo.st_mode & S_IFDIR) && (fileInfo.st_mode & S_IRUSR);
}

bool Utility::is_deletable_directory(std::string const &path)
{
    struct stat fileInfo;
    if (stat(path.c_str(), &fileInfo) != 0){
        return false;
    }
    return (fileInfo.st_mode & S_IFDIR) && (fileInfo.st_mode & S_IWUSR) && (fileInfo.st_mode & S_IXUSR);
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
}

std::string Utility::get_file_updated_date(std::string const &filepath)
{
    struct stat fileInfo;
    if (stat(filepath.c_str(), &fileInfo) != 0){
        return (0);
    }

    char datetime[64];
    Utility::memset(datetime, 0, 64);
    std::strftime(datetime, sizeof(datetime), "%d-%b-%Y %T", std::gmtime(&fileInfo.st_ctime));
    return (datetime);
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


bool Utility::is_redable_directory_not_stat(std::string const &filepath)
{
    if(opendir(filepath.c_str()))
    {
        return (true);
    }


    return (false);
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

TEST_CASE("hex_string_to_size_t")
{
    CHECK(Utility::to_size_t("0") == 0);
    CHECK(Utility::to_size_t("18446744073709551615") == SIZE_MAX);
    CHECK_THROWS_AS(Utility::to_size_t("18446744073709551616") ,std::invalid_argument);
    CHECK_THROWS_AS(Utility::to_size_t("-1") ,std::invalid_argument);
    CHECK_THROWS_AS(Utility::to_size_t("1111a11111") ,std::invalid_argument);
}

TEST_CASE("hex_string_to_number")
{
    CHECK(Utility::hex_string_to_number("0") == 0);
    CHECK(Utility::hex_string_to_number("FF") == 255);
    CHECK(Utility::hex_string_to_number("7FFFFFFFFFFFFFFF") == 9223372036854775807ul);
    CHECK(Utility::hex_string_to_number("FFFFFFFFFFFFFFFF") == 18446744073709551615ul);
    CHECK_THROWS_AS(Utility::to_size_t("FFFFFFFFFFFFFFFF") ,std::invalid_argument);
    CHECK_THROWS_AS(Utility::to_size_t("g11") ,std::invalid_argument);
    CHECK_THROWS_AS(Utility::to_size_t("1g11") ,std::invalid_argument);
}

#endif
