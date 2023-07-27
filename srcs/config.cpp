#include "config.hpp"
#include "config_http.hpp"
#include "config_server.hpp"
#include "utility.hpp"
#include "split.hpp"
#include <iostream>

using std::string;
using std::vector;
using std::map;
using std::pair;
using std::make_pair;

Config *Config::_singleton = NULL;
std::string Config::_filepath = "webserv.conf";
//std::ifstream Config::_fin;

Config::Config()
{
    _fin.open(_filepath.c_str());
    if (!_fin){
        throw std::runtime_error("can not open file");
    }

    this->http.parse();
    _fin.close();
}

Config::~Config()
{
    delete Config::_singleton;
}

void Config::getline(std::string &line)
{
    std::getline(_fin, line);
}


void Config::set_filepath(const char *filepath)
{
    Config::_filepath = string(filepath);
}

Config* Config::get_instance()
{
    if (Config::_singleton == NULL){
        Config::_singleton =  new Config();
    }
    return (Config::_singleton);
}

map<pair<string, string>, ConfigServer const*> Config::_servers_cache;
map<pair<string, string>, vector<string> > Config::_locations_cache;
map<pair<pair<string, string>, string>, map<string, vector<string> > > Config::_locations_content_cache;
map<pair<pair<string, string>, string>, map<string, vector<string> > > Config::_locations_properties_cache;

ConfigServer const* Config::get_server(string const& port, string const& host) const
{
    map<pair<string, string>, ConfigServer const*>::iterator cash_ite = _servers_cache.find(make_pair(port, host));
    if (cash_ite != _servers_cache.end())
        return (cash_ite->second);

    vector<ConfigServer const*> servers;
    for (size_t i = 0; i < http.servers.size(); i++) {
        if (http.servers[i]->listen == port && http.servers[i]->server_name == host) {
            _servers_cache.insert(make_pair(make_pair(port, host), http.servers[i]));
            return (http.servers[i]);
        }
    }
    for (size_t i = 0; i < http.servers.size(); i++) {
        if (http.servers[i]->listen == port && http.servers[i]->is_default_server) {
            _servers_cache.insert(make_pair(make_pair(port, host), http.servers[i]));
            return (http.servers[i]);
        }
    }
    for (size_t i = 0; i < http.servers.size(); i++) {
        if (http.servers[i]->listen == port) {
            _servers_cache.insert(make_pair(make_pair(port, host), http.servers[i]));
            return (http.servers[i]);
        }
    }
    return (NULL);
}

ConfigLocation const* Config::get_location(string const& port, string const& host, string const& path) const
{
    ConfigServer const* server = get_server(port, host);
    if (server == NULL)
        throw std::runtime_error("Config: ConfigServer(port: " + port + " , host: " + host + " ) not found");
    vector<ConfigLocation*> locations = server->locations;
    vector<pair<ConfigLocation*, string> > candidate;
    for (size_t i = 0; i < locations.size(); i++) {
        for (size_t j = 0; j < locations[i]->urls.size(); j++) {
            if (locations[i]->urls[j].size() <= path.size() &&
                path.substr(0, locations[i]->urls[j].size()) == locations[i]->urls[j]) {
                candidate.push_back(make_pair(locations[i], locations[i]->urls[j]));
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

vector<string> Config::get_location_paths(string const& port, string const& host) const
{
    map<pair<string, string>, vector<string> >::iterator cash_ite = _locations_cache.find(make_pair(port, host));
    if (cash_ite != _locations_cache.end()) {
        return (cash_ite->second);
    }
    ConfigServer const* servers = get_server(port, host);
    vector<string> locations;

    for (size_t j = 0; j < servers->locations.size(); j++) {
        for (size_t k = 0; k < servers->locations[j]->urls.size(); k++) {
            locations.push_back(servers->locations[j]->urls[k]);
        }
    }
    Utility::sort_orderby_len(locations);
    _locations_cache.insert(make_pair(make_pair(port, host), locations));
    return (locations);
}

map<string, vector<string> > Config::get_locations_contents(string const& port, string const& host,
                                                            string const& location) const
{
    map<pair<pair<string, string>, string>, map<string, vector<string> > >::iterator cash_ite =
        _locations_content_cache.find(make_pair(make_pair(port, host), location));
    if (cash_ite != _locations_content_cache.end()) {
        return (cash_ite->second);
    }

    vector<map<string, vector<string> > > properties;
    ConfigServer const* servers = get_server(port, host);
    for (size_t j = 0; j < servers->locations.size(); j++) {
        for (size_t k = 0; k < servers->locations[j]->urls.size(); k++) {
            if (servers->locations[j]->urls[k] == location) {
                _locations_content_cache.insert(
                    make_pair(make_pair(make_pair(port, host), location), servers->locations[j]->properties));

                return (servers->locations[j]->properties);
            }
        }
    }
    map<string, vector<string> > rval;
    return (rval);
}

static string get_partial_equaled_path(Split& req_path_sp, Split& cgi_path_sp)
{
    string path = "";
    for (size_t i = 0; i < cgi_path_sp.size(); i++) {
        if (req_path_sp[i] == cgi_path_sp[i]) {
            path += "/";
            path += req_path_sp[i];
        } else {
            break;
        }
    }
    return (path);
}


std::map<std::string, std::vector<std::string> > Config::get_locations_properties(const string& port,
                                                                                  const string& host,
                                                                                  const string& filepath) const
{
    map<pair<pair<string, string>, string>, map<string, vector<string> > >::iterator cash_ite =
        _locations_properties_cache.find(make_pair(make_pair(port, host), filepath));
    if (cash_ite != _locations_properties_cache.end()) {
        return (cash_ite->second);
    }

    const std::vector<std::string> lo = Config::get_location_paths(port, host);
    Split req_path_sp(filepath, "/");

    string path;
    string tmp_path_cfg;
    for (size_t i = 0; i < lo.size(); i++) {
        tmp_path_cfg = lo[i];
        Split cgi_path_sp(tmp_path_cfg, "/");
        if (req_path_sp.size() > 0)
            path = get_partial_equaled_path(req_path_sp, cgi_path_sp);
        if (path != "") {
            break;
        } else if (path == "" && lo[i] == "/") {
            path = "/";
            break;
        }
    }

    std::map<std::string, std::vector<std::string> > properties =
        Config::get_locations_contents(port, host, tmp_path_cfg);
    _locations_properties_cache.insert(make_pair(make_pair(make_pair(port, host), filepath), properties));
    return (properties);
}


