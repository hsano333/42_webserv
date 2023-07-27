
#include "uri.hpp"
#include "request.hpp"
#include <map>
#include <string.h>
#include <sys/stat.h>
#include <vector>
#include "split.hpp"
#include "utility.hpp"
#include "global.hpp"
#include <sstream>

using std::string;

URI::URI() {}
//URI::URI(string &uri) {
    ////raw_uri = Utility::delete_duplicated_slash(Utility::trim_white_space(uri));
//}

URI::~URI() {}

URI::URI(const URI& uri)
{
    *this = uri;
}

URI& URI::operator=(URI const& uri)
{
    if (this == &uri) {
        return (*this);
    }
    this->_raw_uri = uri._raw_uri;
    this->_filename = uri._filename;
    this->_root = uri._root;
    this->_is_cgi = uri._is_cgi;
    this->_filepath = uri._filepath;
    this->_relative_filepath = uri._relative_filepath;
    this->_location_path = uri._location_path;
    this->_querys = uri._querys;
    this->_path_info = uri._path_info;
    return (*this);
}

void URI::uri_encode(string& raw_uri)
{
    char* pos = &(raw_uri[0]);
    size_t cnt = 0;
    char tmp_hex[2];
    size_t size = raw_uri.size();
    char *dst = new char[raw_uri.size()+1];
    char *bk = dst;
    while(pos && size > cnt){
        if(*pos == '%'){
            if (size > cnt+2){
                tmp_hex[0] = pos[1];
                tmp_hex[1] = pos[2];
                string tmp = string(tmp_hex);
                char value = Utility::hex_string_to_int(tmp);
                *dst = value;
                dst++;
                pos+=3;
                cnt+=3;
                continue;
            }else{
                INFO("INFO Uri parse error");
            //cout << ("INFO Uri parse start loop No.3") << endl;
                //todo error
            }
        }
        *dst = *pos;
        dst++;
        pos++;
        cnt++;
    }
    cout << endl;
    cout << endl;
    pos[0] = '\0';
    dst[0] = '\0';
    //_endorded_uri = string(bk);
    raw_uri = string(bk);
    delete[] (bk) ;
}

std::string URI::concat_path(std::string root, std::string uri)
{
    Split root_sp(root, "/");
    Split uri_sp(uri, "/");
    string path = root;

    size_t j = 0;
    for(size_t i=root_sp.size()-1; i != 0;i--){
    //for(size_t i=0; i < root_sp.size();i++){
        //path += root_sp[i] + "/";
        if (j < uri_sp.size())
        {
            if(root_sp[i] != uri_sp[j]){
                for(size_t k=j;k<uri_sp.size() ; k++)
                {
                    path += uri_sp[k] + "/";
                }
                break;
            }
            j++;
        }
    }
    cout << "concat:" << path << endl;

    return (path);
}

static char find_invalid_char(char c)
{
    string invalid_words=":[]@!$&'()*+,;\"";
    int i=0;
    while(invalid_words[i]){
        if (invalid_words[i] == c){
            return (c);
        }
        i++;
    }
    return (0);
}

void URI::check_invalid_word(std::string &str)
{
    string::iterator result = std::find_if(str.begin(), str.end(), find_invalid_char);
    if (result == str.end()){
        return ;
    }
    WARNING("There is a invalid Word in URI:" + Utility::to_string(*result) + ", invalid word is as follows :\n[]@!$&'()*+,;\"");
}

void URI::parse(Request* req)
{
    //INFO("INFO parse URI raw_uri=" + raw_uri);
    cout << "start URI class" << endl;
    string path = req->get_path();
    check_invalid_word(path);

    INFO("INFO parse No.0 path=" + path);
    if (path == ""){
    INFO("INFO parse No.1 path=" + path);
        return;
    }
    INFO("INFO parse No.2 path=" + path);
    //this->uri_encode(path);
    INFO("INFO parse raw_uri=" + path);
    INFO("INFO parse encoded uri=" + _endorded_uri);
    this->_raw_uri = path;
    this->ignore_fragment(path);
    this->retrieve_query(path);

    uri_encode(path);
    this->_location_path = path;
    this->uri_encode(_location_path);
    this->_location_path = Utility::delete_duplicated_slash(_location_path);

    const Config* cfg = Config::get_instance();
    if (cfg == NULL){
        return ;
    }
    std::map<std::string, std::vector<std::string> > props =
        cfg->get_locations_properties(req->get_port(), req->get_host(), this->_location_path);

    this->print_uri();
    this->_root = get_root_dir(props);

    cout << "_root:" << _root << endl;
    cout << "_location_path:" << _location_path << endl;
    INFO("INFO parse root=" + _root);
    INFO("INFO parse _location_path=" + _location_path);

    this->remove_file_info(this->concat_path(_root ,this->_location_path));
    this->print_uri();
}

void URI::ignore_fragment(string& uri)
{
    size_t pos = uri.find("#");
    if (pos == std::string::npos){
        return;
    }
    uri = uri.substr(0, pos);
    //*uri = "";
}

void URI::retrieve_query(string& uri)
{
    cout << "query uri:" << uri << endl;

    size_t pos = uri.find("?");
    if (pos != std::string::npos) {
        string query = uri.substr(pos + 1);
        Split::split_cp(query, "?/", this->_querys, true);
        for(size_t i=0; i < _querys.size(); i++){
            uri_encode(_querys[i]);
        }
        uri = uri.substr(0, pos);
    }
    for(size_t i=0;i< _querys.size(); i++){
        cout << "_query[" << i << "]:[" << _querys[i] << "]" << endl;
    }
}

std::string URI::get_root_dir(std::map<std::string, std::vector<std::string> >& props)
{
    std::string target = "root";

    std::map<std::string, std::vector<std::string> >::iterator test = props.begin();
    std::map<std::string, std::vector<std::string> >::iterator test_end = props.end();
    for(; test != test_end;test++){
        cout << "test->first:" << test->first << endl;
        for(size_t i=0; i< test->second.size();i++){
            cout << "test->second[" << i << "]:"  << test->second[i] << endl;
        }

    }



    std::map<std::string, std::vector<std::string> >::iterator ite = props.find(target);
    this->_is_cgi = false;
    if (ite == props.end()){
        target = "cgi_pass";
        cout << "cgi_pass No.1:" << endl;
        ite = props.find(target);
        this->_is_cgi = true;
    }
    const char* tmp_path;
    struct stat tmp_stat;
    if (ite != props.end()) {
        cout << "cgi_pass No.2:" << endl;
        for (size_t i = 0; i < props[target].size(); i++) {
        cout << "cgi_pass No.3:" << endl;
            tmp_path = (props[target][i]).c_str();
            stat(tmp_path, &tmp_stat);
            if (S_ISDIR(tmp_stat.st_mode)) {
        cout << "cgi_pass No.4:" << endl;
                return (Utility::delete_duplicated_slash(tmp_path) + "/");
            } else {
        cout << "cgi_pass No.5:" << endl;
                continue;
            }
        cout << "cgi_pass No.6:" << endl;
        }
    }
    throw std::invalid_argument("Invalid URI No.1");
}

void URI::print_uri() const
{
    std::stringstream ss;
    
    ss << endl << "print URI" << endl;
    //ss <<  "raw_uri:" << raw_uri << endl;
    ss <<  "_endorded_uri:" << _endorded_uri << endl;
    ss << "_filename:[" << _filename << "]" << endl;
    ss << "_filename:[" << _filename << "]" << endl;
    ss << "_root:[" << _root << "]" << endl;
    ss << "_relative_filepath:[" << _relative_filepath << "]" << endl;
    ss << "_filepath:[" << _filepath << "]" << endl;
    ss << "_location_path:[" << _location_path << "]" << endl;
    for(size_t i=0;i< _querys.size(); i++){
        ss << "_query[" << i << "]:[" << _querys[i] << "]" << endl;
    }
    ss << "_path_info:[" << _path_info << "]" << endl << endl;
    INFO(ss.str());

}

void URI::remove_file_info(std::string tmp_file_path)
{
    cout << "tmp file path:" << tmp_file_path << endl;
    std::string path = Utility::delete_duplicated_slash(tmp_file_path);
    size_t pos = path.rfind("/");
    string path_info;
    struct stat tmp_stat;
    bool exist_flag = false;
    while (pos != std::string::npos && pos != 0) {
        int rval = stat(path.c_str(), &tmp_stat);
        cout << " path:" << path << endl;
        cout << "rval:" << rval << endl;
        cout << "st_mode:" << tmp_stat.st_mode << endl;
        cout << "path:" << path << endl;
        if (rval == 0 && (S_ISREG(tmp_stat.st_mode) || S_ISDIR(tmp_stat.st_mode) )) {
            cout << "st_mode OK" << endl;
            cout << "pos:" << pos << endl;
            //path_info = path.substr(path.size()) + "/" + path_info;
            cout << "path_info No.1:" << path_info << endl;
            //path = path.substr(0, pos);
            exist_flag = true;
            this->_is_file = S_ISREG(tmp_stat.st_mode);
            this->_is_directory = S_ISDIR(tmp_stat.st_mode);
            this->_path_info = Utility::delete_duplicated_slash(path_info);
            if (this->_path_info == "/"){
                this->_path_info = "";
            }
            break;
        }
        path_info = path.substr(pos) + "/" + path_info;
            cout << "path_info No.2:" << path_info << endl;
        path = path.substr(0, pos);

        pos = path.rfind("/", pos - 1);
    }
    if (!exist_flag) {
        throw std::invalid_argument("Invalid URI No.2");
    }
    //this->_path_info = Utility::delete_duplicated_slash(path_info);
    cout << "this->_path_info:" << this->_path_info << endl;
    cout << "this->_root:" << this->_root << endl;
    cout << "this->path:" << path << endl;
    if (this->_root.size() < path.size()) {
        this->_filepath = _root + path.substr(this->_root.size());
        this->_relative_filepath = path.substr(this->_root.size());
        this->_is_file = true;
    } else {
        this->_is_file = false;
    }

    pos = this->_filepath.rfind('/');
    if (pos != std::string::npos) {
        this->_filename = this->_filepath.substr(pos + 1);
    } else if(this->_is_file) {
        throw std::invalid_argument("Invalid URI No.4");
    }
}

const std::string& URI::get_uri() const
{
    return (this->_raw_uri);
}

const std::string& URI::get_filename() const
{
    return (this->_filename);
}

const std::string& URI::get_root() const
{
    return (this->_root);
}

const std::string& URI::get_relative_filepath() const
{
    return (this->_relative_filepath);
}

const std::string& URI::get_filepath() const
{
    return (this->_filepath);
}

const std::string& URI::get_location_path() const
{
    return (this->_location_path);
}

const vector<string>& URI::get_query() const
{
    return (this->_querys);
}

const std::string& URI::get_path_info() const
{
    return (this->_path_info);
}

bool URI::is_cgi() const
{
    return (this->_is_cgi);
}
