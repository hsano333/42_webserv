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
                ConfigParser<Config, ConfigHttp> &parser_config,
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
        ConfigParser<Config, ConfigHttp> &parser_config;
        ConfigParser<ConfigHttp, ConfigServer> &parser_http;
        ConfigParser<ConfigServer, ConfigLocation> &parser_server;
        ConfigParser<ConfigLocation, ConfigLimit> &parser_location;
        ConfigParser<ConfigLimit, ConfigLastObject> &parser_limit;

        std::vector<ConfigHttp*> create_http(ConfigParseredData &parsered_data);
        std::vector<ConfigServer*> create_server(ConfigParseredData &parsered_data);
        std::vector<ConfigLocation*> create_location(ConfigParseredData &parsered_data);
        std::vector<ConfigLimit*> create_limit(ConfigParseredData &parsered_data);
        //ConfigHttp& http;

};
#endif
