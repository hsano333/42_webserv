#include "socket_factory.hpp"

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
    for (size_t i = 0; i < server_cnt; i++) {
        Port port = Config::get_instance()->http->server(i)->listen();
        Socket socket = Socket::create(port);
        socket_repository->insert(socket);
    }
    return (socket_repository);
}

SocketManager *SocketFactory::create(Config *cfg)
{
    SocketManager* manager = new SocketManager();
    SocketRepository* socket_repository = this->create_socket_repository(cfg);
    manager->set_base_repository(socket_repository);
    manager->set_rw_repository(new SocketRepository());
    return (manager);
}
