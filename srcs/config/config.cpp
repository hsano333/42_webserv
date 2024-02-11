#include "config.hpp"
#include "config_http.hpp"
#include "config_server.hpp"
#include "utility.hpp"
#include "split.hpp"
#include <iostream>
#include <string>
#include "log.hpp"
#include "global.hpp"
#include "http_exception.hpp"
#include "normal_file.hpp"
#include <sys/socket.h>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::map;
using std::pair;
using std::make_pair;

using std::cout;
using std::endl;

Config *Config::_singleton = NULL;
std::string Config::_filepath = "webserv.conf";
//std::ifstream Config::_fin;
//
//

/*
void Config::make_repository(std::string &filepath)
{
     repository.set_data(factory.make(filepath));
}
*/

void make_http(char *str)
{
    //Config *cfg = Config::get_instance();
    //cfg->http = new ConfigHttp();
    cout << str << endl;
}

Config::Config(
        FDManager *fd_manager
        )
    : fd_manager_(fd_manager)
//Config::Config() : fd_manager(fd_manager)
{
    //std::string tmp = "tmp";
    //this->http = new ConfigHttp(tmp);
    /*
    _fin.open(_filepath.c_str());
    if (!_fin){
        throw std::runtime_error("can not open file");
    }
    std::map<string, string> tmp;
    retrieve(content_word,tmp, make_http);

    this->http.parse();
    _fin.close();
    */
}

Config::~Config()
{
    DEBUG("Config::~Config Destructor");
    delete http;
    //delete Config::_singleton;
}

void Config::load()
{
    //this->repository.load_file();
}

void Config::retrieve(const string &word, map<string, string> &map, void(*f)(char *str))
{
    std::string line;
    while(this->getline(line)){
        size_t pos = line.find(word);
        if(pos != std::string::npos){
            pos += word.size();
            while(line[pos] != '{'){
                if (line[pos] == '\0' || line[pos] == '\n'){
                    if(!this->getline(line))
                    {
                        ERROR("fialure to parse config file: \'{\' is not find, " + word);
                        throw std::exception();
                    }
                }else if(line[pos] != ' '){
                    ERROR("fialure to parse config file: Invalid word [" + line + "]");
                    throw std::exception();
                }
                pos++;
            }
            f(&line[pos]);
            continue;
        }
        Split sp(line, " ");
        if (line.size() == 2){
            map.insert(make_pair(sp[0],sp[1]));
        }else if (line.size() == 1){
            if (sp[0][0] != '\n'){
                map.insert(make_pair(sp[1],sp[1]));
            }
        }
    }

}

bool Config::getline(std::string &line)
{
    if(std::getline(_fin, line)){
        return (true);
    }
    return (false);
}


void Config::set_filepath(const char *filepath)
{
    Config::_filepath = string(filepath);
}

/*
Config* Config::get_instance()
{
    //if (Config::_singleton == NULL){
        //Config::_singleton =  new Config();
    //}
    //return (Config::_singleton);
    return (NULL);
}
*/

map<pair<Port, string>, const ConfigServer*> Config::servers_cache;
//map<pair<Port, string>, vector<string> > Config::locations_cache;
map<std::pair<ConfigServer const *, std::string>, ConfigLocation const * > Config::locations_cache;
map<pair<pair<Port, string>, string>, map<string, vector<string> > > Config::_locations_content_cache;
map<pair<pair<Port, string>, string>, map<string, vector<string> > > Config::_locations_properties_cache;


/*
const ConfigServer* Config::get_server(Request const *req) const
{
    if(req == NULL){
        return NULL;
    }
    Header const &header = req->header();

    std::string const &header_hostname = header.get_host();
    Split sp(header_hostname, ":");
    if(sp.size() != 2){
        ERROR("There is no Host in Headers");
        throw HttpException("400");
    }
    std::string name = sp[0];
    std::string port_str = sp[1];
    Port port = Port::from_string(port_str);

    const ConfigServer *server = this->get_server(port, name);
    return (server);

}
*/



const ConfigServer* Config::get_server(Request const *req) const
{
    //(void)sockfd;
    if(req == NULL){
        return NULL;
    }
    Header const &header = req->header();

    std::string const &header_hostname = header.get_host();
    Split sp(header_hostname, ":");
    if(sp.size() != 2){
    //if(true){
        WARNING("There is no Host in Headers");
        struct sockaddr_in addr;
        socklen_t len = sizeof(addr);

        try{
            //FileDiscriptor sockfd = req->sockfd();
            FileDiscriptor sockfd = this->fd_manager_->socket_fd_from_epoll_fd(req->fd());
            if(getsockname(sockfd.to_int(), (struct sockaddr*)&addr, &len) < 0){
                WARNING("failure to get port number");
                throw HttpException("500");
            }
            Port port = Port::from_int(ntohs(addr.sin_port));

            const ConfigServer *server = this->get_server(port, "");
            return (server);


        }catch(std::invalid_argument &e){
            throw HttpException("500");
        }
    }
    std::string name = sp[0];
    //std::string port_str = sp[1];
    Port port = Port::from_string(sp[1]);

    const ConfigServer *server = this->get_server(port, name);
    return (server);

}

const ConfigServer *Config::get_server(Port const& port, string const& host) const
{
    DEBUG("Config::get_server");

    DEBUG("Config::get_server No.1");
    map<pair<Port, string>, ConfigServer const*>::iterator cash_ite = servers_cache.find(make_pair(port, host));
    if (cash_ite != servers_cache.end()){
        return (cash_ite->second);
    }
    DEBUG("Config::get_server No.2");
    if(servers_cache.size() > 100){
        servers_cache.clear();
    }

    DEBUG("Config::get_server No.3");
    vector<ConfigServer const*> servers;

    /*
    for (size_t i = 0; i < http->get_server_size(); i++) {
        if (http->server(i)->listen() == port && http->server(i)->server_name() == host && http->server(i)->is_default_server()) {
            servers_cache.insert(make_pair(make_pair(port, host), http->server(i)));
            return (http->server(i));
        }
    }
    */

    IP_Address host_address = IP_Address::from_string_or_name(host);

    this->check();

    DEBUG("Config::get_server No.4");
    //std::vector<Port>
    cout << "http test NULL?" << endl;
    printf("http=%p\n", http);
    cout << "http test NULL No.2?" << endl;
    ConfigServer const *default_server = NULL;
    for (size_t i = 0; i < http->get_server_size(); i++) {
    DEBUG("Config::get_server No.5");
        if (http->server(i)->listen() == port){
    DEBUG("Config::get_server No.6");
            if(default_server == NULL){
    DEBUG("Config::get_server No.7");
                default_server = http->server(i);
            }
    DEBUG("Config::get_server No.8");

            IP_Address config_address = IP_Address::from_string_or_name(http->server(i)->server_name());
            MYINFO("config_address:" + config_address.to_string());
            MYINFO("host_address:" + host_address.to_string());
            if (config_address == host_address){
                MYINFO("Get Server OK");
                servers_cache.insert(make_pair(make_pair(port, host), http->server(i)));
                return (http->server(i));
            }
        }
    }
    if (default_server){
        return (default_server);
    }

    DEBUG("Config::get_server No.6");
    return (http->server(0));
    //return (http->server(i));
    //return (NULL);
}

ConfigLocation const *Config::get_location(ConfigServer const *server, const Request *req) const
{
    string const &path = req->req_line().uri().path();
    const Split &path_sp = req->req_line().uri().splited_path();
    DEBUG("Config::get_location path_sp size:" + Utility::to_string(path_sp.size()));

    map<std::pair<ConfigServer const *, std::string>, ConfigLocation const * >::iterator cash_ite = Config::locations_cache.find(make_pair(server, path));
    if (cash_ite != Config::locations_cache.end()){
        return (cash_ite->second);
    }

    const ConfigLocation* tmp_location = NULL;
    int max_point = 0;
    for (size_t i = 0; i < server->get_location_size(); i++) {
        for (size_t j = 0; j < server->location(i)->pathes().size(); j++) {
            std::string const &location_path = server->location(i)->pathes()[j];
            DEBUG("Config::get_location location_path:" + location_path);
            Split lp(location_path, "/");

            size_t min_num = std::min(lp.size(), path_sp.size());
            if (path_sp.size() > 0){
                int tmp_point = 0;
                for(size_t k=0;k<min_num;k++){
                    if (lp[k] == path_sp[k]){
                        tmp_point++;
                    }else{
                        break;
                    }
                }
                if (tmp_point > max_point){
                    tmp_location = server->location(i);
                }
            }else{
                if (location_path == "/"){
                    tmp_location = server->location(i);
                    break;
                }

            }
        }
    }

    if (tmp_location == NULL){
        ERROR("Config::get_location() cant't find location");
        throw HttpException("404");
    }
    if(Config::locations_cache.size() > 100){
        Config::locations_cache.clear();
    }
    Config::locations_cache.insert(std::make_pair(std::make_pair(server, path), tmp_location));
    return tmp_location;
}

vector<string> Config::get_location_paths(Port const& port, string const& host) const
{
    /*
    map<pair<Port, string>, vector<string> >::iterator cash_ite = locations_cache.find(make_pair(port, host));
    if (cash_ite != locations_cache.end()) {
        return (cash_ite->second);
    }
    */
    ConfigServer const* servers = get_server(port, host);
    vector<string> locations;

    for (size_t j = 0; j < servers->get_location_size(); j++) {
        for (size_t k = 0; k < servers->location(j)->pathes().size(); k++) {
            locations.push_back(servers->location(j)->pathes()[k]);
        }
    }
    Utility::sort_orderby_len(locations);
    //locations_cache.insert(make_pair(make_pair(port, host), locations));
    return (locations);
}

//map<string, vector<string> > Config::get_locations_contents(Port const& port, string const& host,
//                                                            string const& location) const
//{
//    /*
//    map<pair<pair<Port, string>, string>, map<string, vector<string> > >::iterator cash_ite =
//        _locations_content_cache.find(make_pair(make_pair(port, host), location));
//    if (cash_ite != _locations_content_cache.end()) {
//        return (cash_ite->second);
//    }
//    */
//
//    vector<map<string, vector<string> > > properties;
//    ConfigServer const* servers = get_server(port, host);
//    for (size_t j = 0; j < servers->get_location_size(); j++) {
//        for (size_t k = 0; k < servers->location(j)->pathes().size(); k++) {
//            if (servers->location(j)->pathes()[k] == location) {
//                //_locations_content_cache.insert(
//                    //make_pair(make_pair(make_pair(port, host), location), servers->location(j)->properties));
//
//                return (servers->location(j)->properties);
//            }
//        }
//    }
//    map<string, vector<string> > rval;
//    return (rval);
//}

//static string get_partial_equaled_path(Split& req_path_sp, Split& cgi_path_sp)
//{
//    string path = "";
//    for (size_t i = 0; i < cgi_path_sp.size(); i++) {
//        if (req_path_sp[i] == cgi_path_sp[i]) {
//            path += "/";
//            path += req_path_sp[i];
//        } else {
//            break;
//        }
//    }
//    return (path);
//}


//std::map<std::string, std::vector<std::string> > Config::get_locations_properties(Port const &port,
//                                                                                  const string& host,
//                                                                                  const string& filepath) const
//{
//    /*
//    map<pair<pair<Port, string>, string>, map<string, vector<string> > >::iterator cash_ite =
//        _locations_properties_cache.find(make_pair(make_pair(port, host), filepath));
//    if (cash_ite != _locations_properties_cache.end()) {
//        return (cash_ite->second);
//    }
//    */
//
//    const std::vector<std::string> lo = Config::get_location_paths(port, host);
//    Split req_path_sp(filepath, "/");
//
//    string path;
//    string tmp_path_cfg;
//    for (size_t i = 0; i < lo.size(); i++) {
//        tmp_path_cfg = lo[i];
//        Split cgi_path_sp(tmp_path_cfg, "/");
//        if (req_path_sp.size() > 0)
//            path = get_partial_equaled_path(req_path_sp, cgi_path_sp);
//        if (path != "") {
//            break;
//        } else if (path == "" && lo[i] == "/") {
//            path = "/";
//            break;
//        }
//    }
//
//    //std::map<std::string, std::vector<std::string> > properties =
//        //Config::get_locations_contents(port, host, tmp_path_cfg);
//    //_locations_properties_cache.insert(make_pair(make_pair(make_pair(port, host), filepath), properties));
//    return (properties);
//}

void Config::assign_properties(std::vector<std::vector<std::string> > &properties)
{
    DEBUG("Config::assign_properties()");
    if(properties.size() > 0){
        ERROR("Invalid Config Error: There is invalid properties");
        throw std::runtime_error("config parser error:");
    }
}

void Config::assign_out_properties(std::vector<std::string> &properties)
{
    if(properties.size() > 0){
        ERROR("Invalid Config Error: There is invalid properties");
        throw std::runtime_error("config parser error:");
    }
}

void Config::push_all(std::vector<ConfigHttp*> const &vec)
{
    DEBUG("Config::push_all");
    if(vec.size() == 1){
        this->http = vec[0];
        DEBUG("Config::push_all No.2");
        printf("http=%p\n", http);
    }else{
        ERROR("Invalid Config Error: http is only one");
        throw std::runtime_error("config parser error: http is only one");
    }
}

void Config::print_cfg()
{

    Config *cfg = this;
    cout << "Config contents:" << endl;
    cout << "servers.size:" << cfg->http->get_server_size() << endl;
    for(size_t i=0;i<cfg->http->get_server_size();i++){
        cout << endl <<"server No. << " << i << endl;
        cout << "listen:" << cfg->http->server(i)->listen().to_string() << endl;
        cout << "server_name:" << cfg->http->server(i)->server_name() << endl;
        cout << "is_default_server:" << cfg->http->server(i)->is_default_server() << endl;
        cout << "location size:" << cfg->http->server(i)->get_location_size() << endl;
        cout << "max_body_size:" << cfg->http->server(i)->get_max_body_size() << endl;



        for(size_t j=0;j< cfg->http->server(i)->get_location_size();j++){
            cout << "location No." << j << endl;
            ConfigLocation const *tmp = cfg->http->server(i)->location(j);
            cout << "location root:" << tmp->root() << endl;
            cout << "location cgi_pass:" << tmp->cgi_pass() << endl;
            cout << "location autoindex:" << tmp->autoindex() << endl;

            std::map<StatusCode, std::string> const & error_page = tmp->error_pages();
            std::map<StatusCode, std::string>::const_iterator ite = error_page.begin();
            std::map<StatusCode, std::string>::const_iterator end = error_page.end();
            while(ite != end){
                cout << "server error_pages[" << ite->first.to_int() << "]:" << ite->second << endl;
                ite++;
            }



            std::vector<std::string> const & pathes = tmp->pathes();
            for(size_t i=0;i<pathes.size();i++){
                cout << "location path[" << i << "]:" << pathes[i] << endl;
            }
            for(size_t i=0;i<tmp->indexes().size();i++){
                cout << "location index[" << i << "]:" << tmp->indexes()[i] << endl;
            }
            for(size_t i=0;i<tmp->get_limit_size();i++){
                const ConfigLimit *limit = tmp->limit();

                for(size_t m=0;m< limit->allowed_method().size();m++){
                    cout << "limit allowed method:" << limit->allowed_method()[i].to_string();
                }

                std::vector<std::pair<CIDR, bool> > const &allowed_cidr = tmp->limit()->allowed_cidr();
                std::vector<std::pair<CIDR, bool> >::const_iterator allowed_cidr_ite = allowed_cidr.begin();
                std::vector<std::pair<CIDR, bool> >::const_iterator allowed_cidr_end = allowed_cidr.end();
                while(allowed_cidr_ite != allowed_cidr_end){

                    cout << "allowed?:" << allowed_cidr_ite->second << endl;
                    allowed_cidr_ite++;

                }

            }

            cout << "" << j << endl;
        }

    }

}


void Config::check()
{
    DEBUG("Config::check()");
    printf("http=%p\n", http);
    if(http == NULL){
        ERROR("Config::check(),  http is null");
        throw std::runtime_error("Config::check(),  http is null");
    }
}

void Config::check() const
{
    DEBUG("Config::check() const");
    if(http == NULL){
        ERROR("Config::check(),  http is null");
        throw std::runtime_error("Config::check(),  http is null");
    }
}

void Config::check(SocketRepository *socket_repository)
{
    struct addrinfo *res;
    struct addrinfo hints;
    Utility::memset( &hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    (void)socket_repository;
    for (size_t i = 0; i < http->get_server_size(); i++)
    {
        char const *hostname = http->server(i)->server_name().c_str();
        cout << "hostname:" << hostname << endl;
        int rval = getaddrinfo(hostname, NULL, &hints, &res);
        cout << "rval:" << rval << endl;
        if (rval < 0) {
            ERROR("Host name is invalid:" + http->server(i)->server_name());
            //throw std::runtime_error("hostname is invalid");
        }
        freeaddrinfo(res);
    }

}

File *Config::get_error_file(Request const *req, StatusCode &code) const
{
    DEBUG("Config::get_error_file");
    const ConfigServer *server = NULL;
    const ConfigLocation *location= NULL;
    File *file = NULL;

    try{
        server = this->get_server(req);
        location = this->get_location(server, req);
    }catch (...){
        file = NULL;
    }

    if(server && location){
        try{
            std::string page_path = location->get_error_file_path(code);
            //cout << page_path << endl;
            file = NormalFile::from_filepath(page_path, std::ios::in | std::ios::binary);
        }catch(std::runtime_error &e){
            delete file;
            return (NULL);
        }
    }
    return (file);
}

