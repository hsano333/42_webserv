
#include <stdlib.h>

#include <iostream>
#include <string>

#include "config.hpp"
#include "config_factory.hpp"
#include "config_raw_loader.hpp"
#include "iconfig_raw_getter.hpp"
#include "base64.hpp"
#include "split.hpp"
#include "utility.hpp"
#include "epoll_manager.hpp"
#include "webserv.hpp"
#include "global.hpp"
#include "endian.hpp"
#include "log.hpp"
#include "file.hpp"
#include "read_raw.hpp"
#include <fstream>


using std::cout;
using std::endl;
using std::string;
using std::vector;

// std::string base_path = "/home/sano/work/42/webserv/github/webserv/srcs/dir";
std::string base_path = "srcs/dir";

void server(Webserv& webserv)
{
    try {
        webserv.communication();
    } catch (std::exception& e) {
        webserv.reset();
    }
}

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "uri.hpp"


int main(int argc, char const* argv[])
{
    std::string cfg_file = "./webserv.conf";
    if(argc > 2){
        return 0;
    }else if(argc == 2){
        cfg_file = argv[1];
    }

    ReadRaw read_raw;
    File file = File(read_raw, cfg_file, READ_ONLY);
    ConfigRawLoader raw_getter(file);

    ConfigParser<Config, ConfigHttp> parser_config("http");
    ConfigParser<ConfigHttp, ConfigServer> parser_http("server");
    ConfigParser<ConfigServer, ConfigLocation> parser_server("location");
    ConfigParser<ConfigLocation, ConfigLimit> parser_location("limit_except");
    ConfigParser<ConfigLimit, ConfigLastObject> parser_limit("");

    ConfigFactory cfg_factory = ConfigFactory(raw_getter,
                                              parser_config,
                                              parser_http,
                                              parser_server,
                                              parser_location,
                                              parser_limit
                                                );
    cout << "test:parser target:" << parser_http.target << endl;
    Config *cfg = cfg_factory.create();
    (void)cfg;
    cout << "servers.size:" << cfg->http->servers.size() << endl;
    for(size_t i=0;i<cfg->http->servers.size();i++){
        cout << "listen:" << cfg->http->servers[i]->listen.to_string() << endl;
        cout << "server_name:" << cfg->http->servers[i]->server_name << endl;
        cout << "is_default_server:" << cfg->http->servers[i]->is_default_server << endl;
    }

    exit(1);

    /*
    ConfigRepository *repository = factory.create();
    create(){
        repository.set(raw_data);
        repository.parse();
        repository.make_http(raw_data);
        repository.make_server(raw_data_server);
        repository.make_location(raw_data_location);
        return repository;
    }
    Config cfg(repository);


    URI uri;

    std::string tmp_path = "test3";
    try{
        //ReadRaw* read_raw = new ReadRaw();
        //IReader* iread_raw = new ReadRaw();
        //j.int fd = read_raw->iread(0, buf);
        //j.cout << fd << endl;
        //(void)read_raw;
        File file = File(read_raw);
        file.open_file(tmp_path, WRITE_ONLY);
        char buf[4000];
        int size = file.read(buf);
        buf[size] = '\0';
        cout << buf << endl;
        //cout << "Success:" << filepath.to_str() << endl;
        std::ofstream ofs;
        ofs.open(tmp_path.c_str());
        if (ofs){

            cout << "write test " << endl;
            ofs << "write test1" << endl;
            ofs << "write test2" << endl;
            //std::string line;
            //if(std::getline(ofs, line)){
                //cout << line << endl;
//
            //}

        }
        ofs.close();


    }catch(std::exception &e){

        cout << e.what() << endl;
        cout << "error" << endl;
        exit(1);
    }
    exit(1);


    //std::string test="test=%21%22%23%24%25%26%27%28%29%3D%7E%7C%7B%60%7D%2A%2B_%3F%3E%3C%2C.%2F%5C%5D%3A%3B%5B%40%5C%5E-098765432%E3%81%82%E3%81%84%E3%81%86%E3%81%88%E3%81%8A%EF%BC%81%E2%80%9D%EF%BC%83%EF%BC%84%EF%BC%85%EF%BC%86%E2%80%99%EF%BC%88%EF%BC%89%EF%BC%9D%EF%BD%9E%EF%BD%9C%0D%0A";
    //uri.uri_encode(test);
    std::string test="http://test/cat.html?/?abc=test/def=te?/?st2?fgh=123/%21%22%23%24%25%26%27%28%29%3D%7E%7C%7B%60%7D%2A%2B_%3F%3E%3C%2C.%2F%5C%5D%3A%3B%5B%40%5C%5E-098765432%E3%81%82%E3%81%84%E3%81%86%E3%81%88%E3%81%8A%EF%BC%81%E2%80%9D%EF%BC%83%EF%BC%84%EF%BC%85%EF%BC%86%E2%80%99%EF%BC%88%EF%BC%89%EF%BC%9D%EF%BD%9E%EF%BD%9C%0D%0A/$fragmentTest#abc";
    cout << "test before" << test << endl;
    uri.ignore_fragment(test);
    uri.retrieve_query(test);
    uri.print_uri();
    cout << "test after" << test << endl;
    //exit(1);

    char value = Utility::hex_string_to_int("11");
    int output;
    output = int(value);
    cout << "value: 11="  << output;
    value = Utility::hex_string_to_int("22");
    output = int(value);
    cout << "value: 22="  << output;
    value = Utility::hex_string_to_int("33");
    output = int(value);
    cout << "value: 33="  << output;
    value = Utility::hex_string_to_int("99");
    output = int(value);
    cout << "value: 99="  << output;
    value = Utility::hex_string_to_int("aa");
    output = int(value);
    cout << "value: aa="  << output;
    value = Utility::hex_string_to_int("bb");
    output = int(value);
    cout << "value: bb="  << output;
    value = Utility::hex_string_to_int("ff");
    output = int(value);
    cout << "value: ff="  << output;
    value = Utility::hex_string_to_int("gg");
    output = int(value);
    cout << "value: gg="  << output;
    value = Utility::hex_string_to_int("AF");
    output = int(value);
    cout << "value: AF="  << output;
    value = Utility::hex_string_to_int("F0");
    output = int(value);
    cout << "value: F0="  << output;

    //int fd = 1;
    //DEBUG("Webserv start" + Utility::to_string(fd) + ",te ");
    Log::check_writable();
    std::string test_u = "𠀋bウィキペディア";
    cout << "ウィキペディア:" << test_u << endl;

    cout << "endian::"  << Endian::get_endian() << endl;
    char *ttmp = &(test_u[0]);
    unsigned char *tmp = (unsigned char *)ttmp;
    size_t cnt = 0;
    while(test_u.size() > cnt){
        printf("[0x:%x]", tmp[cnt]);
        cnt++;
        printf("[0x::%x]", tmp[cnt]);
        cnt++;
    }
    cout << "loop end" << endl;
    */

    (void)argc;
    (void)argv;
    if (argc != 2) {
        std::cout << "Error: Specify config file" << std::endl;
        return (EXIT_FAILURE);
    }
    //detect_endian();
    //Config::set_filepath((argv[1]));
    /*
    try {
        Config::set_filepath((argv[1]));
        Config::get_instance();
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
        return (EXIT_FAILURE);
    }

    Webserv webserv;
    while (1) {
        server(webserv);
    }
    */
    return 0;
}
