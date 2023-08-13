#ifndef CONFIG_FACTORY_HPP
#define CONFIG_FACTORY_HPP

#include "config.hpp"
#include "ireader.hpp"
#include "config_http.hpp"
#include "config_server.hpp"
#include "config_location.hpp"
#include "config_limit.hpp"
#include "config_parser.hpp"
#include "iconfig_raw_getter.hpp"


class ConfigFactory
{
    public:
        ConfigFactory(
                IConfigRawGetter<std::string> &getter,
                ConfigParser<ConfigHttp, ConfigServer> &parser_http,
                ConfigParser<ConfigServer, ConfigLocation> &parser_server,
                ConfigParser<ConfigLocation, ConfigLimit> &parser_location,
                ConfigParser<ConfigLimit, ConfigLastObject> &parser_limit
                );
        ~ConfigFactory();
        Config* create();
    private:
        //Reader &reader;
        IConfigRawGetter<std::string> &raw_getter;
        ConfigParser<ConfigHttp, ConfigServer> &parser_http;
        ConfigParser<ConfigServer, ConfigLocation> &parser_server;
        ConfigParser<ConfigLocation, ConfigLimit> &parser_location;
        ConfigParser<ConfigLimit, ConfigLastObject> &parser_limit;
        ConfigHttp* create_http(std::string &raw_data);
        ConfigServer* create_server(std::string &server_data);
        ConfigLocation* create_location(std::string &location_data);
        ConfigLimit* create_limit(std::string &limit_data);
        //ConfigHttp& http;

};
#endif
