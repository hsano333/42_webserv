#ifdef UNIT_TEST
#include "doctest.h"
#endif
#include "content_type.hpp"
#include "utility.hpp"

// http://www.t-net.ne.jp/~cyfis/rfc/mime/rfc2046_ja-1.html#INDEX_4_1_2
// ↑charsetはデフォルトでUS-ASCIIらしい
ContentType::ContentType(map<string, string> const& headers) : _charset(""), _boundary(""), me("ContentType::")
{
    string value;
    try {
        value = headers.at("content-type");
    } catch (std::out_of_range& e) {
        _media_type = "application/octet-stream";
        return;
    }

    vector<string> directives = parse_directives(value);
    if (directives.size() == 0)
        throw runtime_error(me + "ContentType() : Content-Type's value is empty");

    _media_type = directives[0];

    set_directive(directives);

    // validate
    if (_media_type == "")
        throw runtime_error(me + "ContentType() : invalid media_type value: " + _media_type);
    if (_charset == "")
        _charset = get_default_mime_type(_media_type);
    if (is_multipart() && _boundary == "")
        throw runtime_error(me + "ContentType() : multipart Content-Type( " + _media_type + " ) must have boundary");
}

ContentType::ContentType() {}

ContentType& ContentType::operator=(ContentType const& other)
{
    if (this != &other) {
        this->_media_type = other._media_type;
        this->_charset = other._charset;
        this->_boundary = other._boundary;
    }
    return *this;
}

void ContentType::set_directive(vector<string> directives)
{
    for (size_t i = 1; i < directives.size(); i++) {
        string directive = directives[i];
        size_t pos = directive.find('=');
        if (pos == string::npos)
            throw runtime_error(me + "ContentType() : invalid Content-Type syntax");
        string key = Utility::trim_white_space(directive.substr(0, pos));
        string value = Utility::trim_white_space(directive.substr(pos + 1));
        if (key == "charset")
            _charset = Utility::to_lower(value);
        else if (key == "boundary")
            _boundary = value;
        else
            throw runtime_error(me + "ContentType() : invalid Content-Type directive: " + key + "=" + value);
    }
}

bool ContentType::is_multipart() const
{
    return _media_type.find("multipart/") == 0;
}

#include <iostream>
vector<string> ContentType::parse_directives(string value)
{
    size_t i = 0;

    vector<string> directives;
    string buf;
    while (value[i] != *(value.end())) {
        if (value[i] == ';') {
            directives.push_back(buf);
            buf.clear();
        } else
            buf += value[i];
        i++;
    }
    directives.push_back(buf);
    return directives;
}

ContentType::~ContentType() {}

// http://www.t-net.ne.jp/~cyfis/rfc/mime/rfc2046_ja-1.html#INDEX_4_1_2
// RFC2046で定義されている
string ContentType::get_default_mime_type(string media_type)
{
    if (media_type.find("text/") == 0)
        return "US-ASCII";
    return "";
}

string const& ContentType::get_media_type() const
{
    return _media_type;
}
string const& ContentType::get_charset() const
{
    return _charset;
}
string const& ContentType::get_boundary() const
{
    return _boundary;
}

std::ostream& operator<<(std::ostream& os, ContentType const& ct)
{
    os << "media_type: '" << ct.get_media_type() << "', ";
    os << "charset: '" << ct.get_charset() << "', ";
    os << "boundary: '" << ct.get_boundary() << "'";
    return os;
}

#ifdef UNIT_TEST
TEST_CASE("ContentType::ContentType()")
{
    map<string, string> headers;
    headers["content-type"] = "text/html; charset=ISO-8859-4";
    ContentType ct = ContentType(headers);
    CHECK(ct.get_media_type() == "text/html");
    CHECK(ct.get_charset() == "iso-8859-4");
    CHECK(ct.get_boundary() == "");
}

TEST_CASE("ContentType::ContentType()")
{
    map<string, string> headers;
    headers["content-type"] = "; charset=ISO-8859-4";
    CHECK_THROWS_AS((ContentType(headers)), std::runtime_error);
}

TEST_CASE("ContentType::ContentType()")
{
    map<string, string> headers;
    headers["content-type"] = "text/html; charset=";
    ContentType ct = ContentType(headers);
    CHECK(ct.get_media_type() == "text/html");
    CHECK(ct.get_charset() == "US-ASCII");
    CHECK(ct.get_boundary() == "");
}

TEST_CASE("ContentType::ContentType()")
{
    map<string, string> headers;
    headers["content-type"] = "text/html";
    ContentType ct = ContentType(headers);
    CHECK(ct.get_media_type() == "text/html");
    CHECK(ct.get_charset() == "US-ASCII");
    CHECK(ct.get_boundary() == "");
}

TEST_CASE("ContentType::ContentType()")
{
    map<string, string> headers;
    headers["content-type"] = "multipart/form-data; charset=ISO-8859-4; boundary=1234567890";
    ContentType ct(headers);
    CHECK(ct.get_media_type() == "multipart/form-data");
    CHECK(ct.get_charset() == "iso-8859-4");
    CHECK(ct.get_boundary() == "1234567890");
}

TEST_CASE("ContentType::ContentType()")
{
    map<string, string> headers;
    headers["content-type"] = "multipart/form-data";
    CHECK_THROWS_AS((ContentType(headers)), std::runtime_error);
}

TEST_CASE("ContentType::ContentType()")
{
    map<string, string> headers;
    headers["content-type"] = "";
    CHECK_THROWS_AS((ContentType(headers)), std::runtime_error);
}

TEST_CASE("ContentType::ContentType()")
{
    map<string, string> headers;
    ContentType ct(headers);
    CHECK(ct.get_media_type() == "application/octet-stream");
    CHECK(ct.get_charset() == "");
    CHECK(ct.get_boundary() == "");
}

#endif
