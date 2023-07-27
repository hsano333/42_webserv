#ifndef CONTENT_TYPE_H
#define CONTENT_TYPE_H
#include "utility.hpp"
#include <algorithm>
#include <cctype>
#include <map>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

using std::map;
using std::runtime_error;
using std::string;
using std::vector;

class ContentType
{
  private:
    string _media_type;
    string _charset;
    string _boundary;

    // ログ出力用変数
    const string me;

    vector<string> parse_directives(string value);
    string get_default_mime_type(string media_type);

    void set_directive(vector<string> directives);

  public:
    ContentType(map<string, string> const& headers);
    ContentType();
    ~ContentType();
    ContentType& operator=(ContentType const& other);
    string const& get_media_type() const;
    string const& get_charset() const;
    string const& get_boundary() const;

    bool is_multipart() const;
    friend std::ostream& operator<<(std::ostream& os, ContentType const& ct);
};

#endif /* CONTENT_TYPE_H */
