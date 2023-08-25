#include "fd_manager.hpp"
#include "config.hpp"
#include <unistd.h>

FDManager::FDManager(FileDiscriptor fd) : SocketData(), _fd(fd), _req(NULL), _res(NULL) {}

FDManager::~FDManager()
{
    cout << "fd manager delete all" << endl;
    delete_all();
}

void FDManager::insert(Request* req)
{
    if (this->_req) {
        delete this->_req;
    }
    this->_req = req;
}

void FDManager::insert(Response* res)
{
    if (this->_res) {
        delete this->_res;
    }
    this->_res = res;
}

Request* FDManager::get_req()
{
    return (this->_req);
}

Response* FDManager::get_res()
{
    return (this->_res);
}

void FDManager::delete_req()
{
    if (this->_req){
        delete (this->_req);
        this->_req = NULL;
    }
}

void FDManager::delete_res()
{
    if (this->_res){
        delete (this->_res);
        this->_res = NULL;
    }
}

void FDManager::delete_all()
{
    this->delete_res();
    this->delete_req();
}

void FDManager::close_fd()
{
    cout << "fd manager close:" << _fd << endl;
    close(_fd);
}
