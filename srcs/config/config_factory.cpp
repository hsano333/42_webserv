
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
        ConfigParser<Config, ConfigHttp> &parser_config,
        ConfigParser<ConfigHttp, ConfigServer> &parser_http,
        ConfigParser<ConfigServer, ConfigLocation> &parser_server,
        ConfigParser<ConfigLocation, ConfigLimit> &parser_location,
        ConfigParser<ConfigLimit, ConfigCgi> &parser_limit,
        ConfigParser<ConfigCgi, ConfigLastObject> &parser_cgi
        )
                            :
                            raw_getter(getter),
                            parser_config(parser_config),
                            parser_http(parser_http),
                            parser_server(parser_server),
                            parser_location(parser_location),
                            parser_limit(parser_limit),
                            parser_cgi(parser_cgi)
{
    ;
}

ConfigFactory::~ConfigFactory()
{
    ;
}


std::vector<ConfigCgi*> ConfigFactory::create_cgi(ConfigParseredData &parsered_cgi_data)
{
    std::vector<ConfigCgi*> cgis;
    for(size_t i=0; i<parsered_cgi_data.size();i++){
        ConfigCgi *cgi = new ConfigCgi();
        parser_cgi.parser(parsered_cgi_data.raw_data(i), cgi);
        cgi->check();
        cgis.push_back(cgi);
    }
    return (cgis);
}


std::vector<ConfigLimit*> ConfigFactory::create_limit(ConfigParseredData &parsered_limit_data)
{
    std::vector<ConfigLimit*> limits;
    for(size_t i=0; i<parsered_limit_data.size();i++){
        ConfigLimit *limit = new ConfigLimit();
        ConfigParseredData parsered_cgi_data = parser_limit.parser(parsered_limit_data.raw_data(i), limit);

        limit->assign_out_properties(parsered_limit_data.properties(i));
        limit->push_all(this->create_cgi(parsered_cgi_data));
        limit->check();
        limits.push_back(limit);
    }
    return (limits);
}

std::vector<ConfigLocation*> ConfigFactory::create_location(ConfigParseredData &parsered_location_data)
{
    std::vector<ConfigLocation*> locations;
    DEBUG("parsered_location_data.size():" + Utility::to_string(parsered_location_data.size()));
    for(size_t i=0; i<parsered_location_data.size();i++){
        ConfigLocation *location = new ConfigLocation();
        ConfigParseredData parsered_limit_data = parser_location.parser(parsered_location_data.raw_data(i), location);

        location->assign_out_properties(parsered_location_data.properties(i));
        location->push_all(this->create_limit(parsered_limit_data));
        location->check();
        locations.push_back(location);
    }
    return (locations);
}

std::vector<ConfigServer*> ConfigFactory::create_server(ConfigParseredData &parsered_server_data)
{
    std::vector<ConfigServer* > servers;
    for(size_t i=0; i<parsered_server_data.size();i++){
        ConfigServer *server = new ConfigServer();
        ConfigParseredData parsered_location_data = parser_server.parser(parsered_server_data.raw_data(i), server);

        server->assign_out_properties(parsered_server_data.properties(i));
        server->push_all(this->create_location(parsered_location_data));
        server->check();
        servers.push_back(server);
    }
    return (servers);
}


std::vector<ConfigHttp*> ConfigFactory::create_http(ConfigParseredData &parsered_http_data)
{

    std::vector<ConfigHttp*> https;
    if(parsered_http_data.size() == 1){
        ConfigHttp *http = new ConfigHttp();
        ConfigParseredData parsered_server_http = parser_http.parser(parsered_http_data.raw_data(0), http);
        http->assign_out_properties(parsered_http_data.properties(0));
        http->push_all(this->create_server(parsered_server_http));
        http->check();
        https.push_back(http);
    }else{
        ERROR("Invalid Config Error: There is no http directive or more than 2 ");
        throw std::runtime_error("config parser error:http");
    }
    return (https);
}


Config* ConfigFactory::create(FDManager *fd_manager)
{
    (void)fd_manager;
    Config *config = new Config(fd_manager);
    std::string raw_data = raw_getter.get_raw_data();
    ConfigParseredData servers_http = parser_config.parser(raw_data, config);

    config->push_all(this->create_http(servers_http));

    return (config);
}
