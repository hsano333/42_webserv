#include "socket_factory.hpp"
#include <set>

SocketFactory::SocketFactory()
{
;
}

SocketFactory::~SocketFactory()
{
    ;
}



SocketRepository *SocketFactory::create_socket_repository(Config *cfg)
{
    SocketRepository *socket_repository = new SocketRepository();
    size_t server_cnt = cfg->http->get_server_size();
    std::set<Port> ports;
    for (size_t i = 0; i < server_cnt; i++) {
        cout << "listen:" << cfg->http->server(i)->listen().to_string() << endl;
        ports.insert(cfg->http->server(i)->listen());
    }

    std::set<Port>::const_iterator ite = ports.begin();
    std::set<Port>::const_iterator end = ports.end();
    while(ite != end){
        Socket socket = Socket::create_from_port(*ite);
        socket_repository->insert(socket);
        ite++;
    }
    return (socket_repository);
}

/*
SocketManager *SocketFactory::create(Config *cfg)
{
    SocketManager* manager = new SocketManager();
    SocketRepository* socket_repository = this->create_socket_repository(cfg);
    //cout << "repository size:" << socket_repository->size() << endl;

    manager->set_base_repository(socket_repository);
    manager->set_rw_repository(new SocketRepository());
    return (manager);
}
*/

SocketRepository *SocketFactory::create_from_config(Config *cfg)
{
    //SocketManager* manager = new SocketManager();
    SocketRepository* socket_repository = this->create_socket_repository(cfg);
    //cout << "repository size:" << socket_repository->size() << endl;

    //manager->set_base_repository(socket_repository);
    //manager->set_rw_repository(new SocketRepository());
    return (socket_repository);
}
