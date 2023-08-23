#include "config.hpp"
#include "config_http.hpp"
#include "config_server.hpp"
#include "utility.hpp"
#include "split.hpp"
#include <iostream>
#include <string>
#include "log.hpp"
#include "global.hpp"

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::map;
using std::pair;
using std::make_pair;

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




Config::Config()
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
    delete Config::_singleton;
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

Config* Config::get_instance()
{
    //if (Config::_singleton == NULL){
        //Config::_singleton =  new Config();
    //}
    //return (Config::_singleton);
    return (NULL);
}

map<pair<Port, string>, ConfigServer const*> Config::_servers_cache;
map<pair<Port, string>, vector<string> > Config::_locations_cache;
map<pair<pair<Port, string>, string>, map<string, vector<string> > > Config::_locations_content_cache;
map<pair<pair<Port, string>, string>, map<string, vector<string> > > Config::_locations_properties_cache;

ConfigServer const* Config::get_server(Port const& port, string const& host) const
{
    /*
    map<pair<Port, string>, ConfigServer const*>::iterator cash_ite = _servers_cache.find(make_pair(port, host));
    if (cash_ite != _servers_cache.end())
        return (cash_ite->second);
    */

    vector<ConfigServer const*> servers;
    for (size_t i = 0; i < http->get_server_size(); i++) {
        if (http->server(i)->listen() == port && http->server(i)->server_name() == host) {
            //_servers_cache.insert(make_pair(make_pair(port, host), http->server(i)));
            return (http->server(i));
        }
    }
    for (size_t i = 0; i < http->get_server_size(); i++) {
        if (http->server(i)->listen() == port && http->server(i)->is_default_server()) {
            //_servers_cache.insert(make_pair(make_pair(port, host), http->server(i)));
            return (http->server(i));
        }
    }
    for (size_t i = 0; i < http->get_server_size(); i++) {
        if (http->server(i)->listen() == port) {
            //_servers_cache.insert(make_pair(make_pair(port, host), http->server(i)));
            return (http->server(i));
        }
    }
    return (NULL);
}

ConfigLocation const* Config::get_location(Port const& port, string const& host, string const& path) const
{
    ConfigServer const* server = get_server(port, host);
    if (server == NULL){
        throw std::runtime_error("Config: ConfigServer(port: " + port.to_string() + " , host: " + host + " ) not found");
    }
    //vector<ConfigLocation*> locations = server->locations;
    vector<pair<const ConfigLocation*, string> > candidate;
    for (size_t i = 0; i < server->get_location_size(); i++) {
        for (size_t j = 0; j < server->location(i)->pathes().size(); j++) {
            if (server->location(i)->pathes()[j].size() <= path.size() &&
                path.substr(0, server->location(i)->pathes()[j].size()) == server->location(i)->pathes()[j]) {
                candidate.push_back(make_pair(server->location(i), server->location(i)->pathes()[j]));
                continue;
            }
        }
    }
    size_t max_match_character_count = 0;
    ConfigLocation const* max_match_location = NULL;
    for (size_t i = 0; i < candidate.size(); i++) {
        if (candidate[i].second.size() > max_match_character_count) {
            max_match_character_count = candidate[i].second.size();
            max_match_location = candidate[i].first;
        }
    }
    return max_match_location;
}

vector<string> Config::get_location_paths(Port const& port, string const& host) const
{
    /*
    map<pair<Port, string>, vector<string> >::iterator cash_ite = _locations_cache.find(make_pair(port, host));
    if (cash_ite != _locations_cache.end()) {
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
    //_locations_cache.insert(make_pair(make_pair(port, host), locations));
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
    if(vec.size() == 1){
        this->http = vec[0];
    }else{
        ERROR("Invalid Config Error: http is only one");
        throw std::runtime_error("config parser error: http is only one");
    }
}

void Config::print_cfg()
{

    Config *cfg = this;
    cout << "Config contents:" << endl;
    cout << "max_body_size:" << cfg->http->get_max_body_size() << endl;
    cout << "servers.size:" << cfg->http->get_server_size() << endl;
    for(size_t i=0;i<cfg->http->get_server_size();i++){
        cout << endl <<"server No. << " << i << endl;
        cout << "listen:" << cfg->http->server(i)->listen().to_string() << endl;
        cout << "server_name:" << cfg->http->server(i)->server_name() << endl;
        cout << "is_default_server:" << cfg->http->server(i)->is_default_server() << endl;
        cout << "location size:" << cfg->http->server(i)->get_location_size() << endl;

        for(size_t j=0;j< cfg->http->server(i)->get_location_size();j++){
            cout << "location No." << j << endl;
            ConfigLocation const *tmp = cfg->http->server(i)->location(j);
            cout << "location root:" << tmp->root() << endl;
            cout << "location cgi_pass:" << tmp->cgi_pass() << endl;
            cout << "location autoindex:" << tmp->autoindex() << endl;
            std::vector<std::string> const & pathes = tmp->pathes();
            for(size_t i=0;i<pathes.size();i++){
                cout << "location path[" << i << "]:" << pathes[i] << endl;
            }
            for(size_t i=0;i<tmp->indexes().size();i++){
                cout << "location index[" << i << "]:" << tmp->indexes()[i] << endl;
            }
            std::map<StatusCode, std::string> const & error_page = tmp->error_pages();
            std::map<StatusCode, std::string>::const_iterator ite = error_page.begin();
            std::map<StatusCode, std::string>::const_iterator end = error_page.end();
            while(ite != end){
                cout << "location error_pages[" << ite->first.to_int() << "]:" << ite->second << endl;
                ite++;
            }
            for(size_t i=0;i<tmp->get_limit_size();i++){
                ConfigLimit const *limit = (tmp->limit(i));

                for(size_t m=0;m< limit->allowed_method().size();m++){
                    cout << "limit allowed method:" << limit->allowed_method()[i].to_string();
                }

                std::vector<std::pair<CIDR, bool> > const &allowed_cidr = tmp->limit(i)->allowed_cidr();
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
    if(http == NULL){
        ERROR("Config::check(),  http is null");
        throw std::runtime_error("Config::check(),  http is null");
    }
}
