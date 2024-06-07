#ifndef CONFIG_FACTORY_HPP
#define CONFIG_FACTORY_HPP

#include "config.hpp"
#include "ireader.hpp"
#include "config_http.hpp"
#include "config_server.hpp"
#include "config_location.hpp"
#include "config_limit.hpp"
#include "config_cgi.hpp"
#include "config_parser.hpp"
#include "iconfig_raw_getter.hpp"
#include "fd_manager.hpp"


class ConfigFactory
{
    public:
        ConfigFactory(
                IConfigRawGetter<std::string> &getter,
                ConfigParser<Config, ConfigHttp> &parser_config,
                ConfigParser<ConfigHttp, ConfigServer> &parser_http,
                ConfigParser<ConfigServer, ConfigLocation> &parser_server,
                ConfigParser<ConfigLocation, ConfigLimit> &parser_location,
                ConfigParser<ConfigLimit, ConfigCgi> &parser_limit,
                ConfigParser<ConfigCgi, ConfigLastObject> &parser_cgi
                );
        ~ConfigFactory();
        Config* create(FDManager *fd_manager);
    private:
        IConfigRawGetter<std::string> &raw_getter;
        ConfigParser<Config, ConfigHttp> &parser_config;
        ConfigParser<ConfigHttp, ConfigServer> &parser_http;
        ConfigParser<ConfigServer, ConfigLocation> &parser_server;
        ConfigParser<ConfigLocation, ConfigLimit> &parser_location;
        ConfigParser<ConfigLimit, ConfigCgi> &parser_limit;
        ConfigParser<ConfigCgi, ConfigLastObject> &parser_cgi;

        std::vector<ConfigHttp*> create_http(ConfigParseredData &parsered_data);
        std::vector<ConfigServer*> create_server(ConfigParseredData &parsered_data);
        std::vector<ConfigLocation*> create_location(ConfigParseredData &parsered_data);
        std::vector<ConfigLimit*> create_limit(ConfigParseredData &parsered_data);
        std::vector<ConfigCgi*> create_cgi(ConfigParseredData &parsered_cgi_data);

};
#endif
