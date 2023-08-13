
#include "config_factory.hpp"
#include "utility.hpp"
#include "config_http.hpp"
#include "config_server.hpp"
#include <vector>
#include <string>

using std::vector;
using std::string;

ConfigFactory::ConfigFactory(
        IConfigRawGetter<std::string> &getter,
        ConfigParser<ConfigHttp, ConfigServer> &parser_http,
        ConfigParser<ConfigServer, ConfigLocation> &parser_server,
        ConfigParser<ConfigLocation, ConfigLimit> &parser_location,
        ConfigParser<ConfigLimit, ConfigLastObject> &parser_limit
        )
                            :
                            raw_getter(getter),
                            parser_http(parser_http),
                            parser_server(parser_server),
                            parser_location(parser_location),
                            parser_limit(parser_limit)
{
    ;
}

ConfigFactory::~ConfigFactory()
{
    ;
}



ConfigLimit* ConfigFactory::create_limit(std::string &limit_data)
{
    ConfigLimit *limit = new ConfigLimit();
    vector<string> limit_strings = parser_limit.parser(limit_data, limit);
    return (limit);
}

ConfigLocation* ConfigFactory::create_location(std::string &location_data)
{
    ConfigLocation *location = new ConfigLocation();
    vector<string> limit_strings = parser_location.parser(location_data, location);
    for(size_t i=0; i<limit_strings.size();i++){
        ConfigLimit *tmp = this->create_limit(limit_strings[i]);
        location->limits.push_back(tmp);
    }
    return (location);
}

ConfigServer* ConfigFactory::create_server(std::string &server_data)
{
    ConfigServer *server = new ConfigServer();
    vector<string> location_strings = parser_server.parser(server_data, server);
    for(size_t i=0; i<location_strings.size();i++){
        ConfigLocation *tmp = this->create_location(location_strings[i]);
        server->locations.push_back(tmp);
    }
    return (server);
}


ConfigHttp* ConfigFactory::create_http(std::string &raw_data)
{
    ConfigHttp *http = new ConfigHttp();
    vector<string> servers_strings = parser_http.parser(raw_data, http);
    for(size_t i=0; i<servers_strings.size();i++){
        ConfigServer *tmp = this->create_server(servers_strings[i]);
        http->servers.push_back(tmp);
    }
    return (http);
}


Config* ConfigFactory::create()
{
    Config *config = new Config();
    std::string raw_data = raw_getter.get_raw_data();
    config->http = this->create_http(raw_data);

    return (config);
}
