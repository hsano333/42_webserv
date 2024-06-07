#include "fd_manager.hpp"
#include "config.hpp"
#include <unistd.h>

FDManager::FDManager() 
{
    ;
}

FDManager::~FDManager()
{
    close_all();
}

void FDManager::close_all()
{

    {
        std::map<FileDiscriptor, FileDiscriptor>::iterator ite = epoll_fd_and_socket.begin();
        std::map<FileDiscriptor, FileDiscriptor>::iterator end = epoll_fd_and_socket.end();
        while(ite != end){
            close((ite->second).to_int());
            ite++;
        }
    }
    {
        std::vector<FileDiscriptor>::iterator ite = fd_of_sockets.begin();
        std::vector<FileDiscriptor>::iterator end = fd_of_sockets.end();
        while(ite != end){
            close((*ite).to_int());
            ite++;
        }
    }
}

void FDManager::add_socket(FileDiscriptor fd)
{
    fd_of_sockets.push_back(fd);
    DEBUG("FDManager::add_socket fd" + fd.to_string());
}

void FDManager::close_socket(FileDiscriptor fd)
{
    DEBUG("FDManager::close_socket fd:" + fd.to_string());
    std::vector< FileDiscriptor>::iterator ite = find(fd_of_sockets.begin(), fd_of_sockets.end(), fd);
    if(ite != fd_of_sockets.end()){
        DEBUG("FDManager::close_socket close fd:" + (*ite).to_string());
        fd_of_sockets.erase(ite);
        close((*ite).to_int());
    }
}

void FDManager::add_socket_and_epoll_fd(FileDiscriptor io_fd, FileDiscriptor socket_fd)
{
    DEBUG("FDManager::add_socket_and_epoll_fd fd:" + socket_fd.to_string());
    epoll_fd_and_socket.insert(std::make_pair(io_fd, socket_fd));
}

void FDManager::close_fd(FileDiscriptor fd)
{
    DEBUG("FDManager::close_fd():" + fd.to_string());
    std::map<FileDiscriptor, FileDiscriptor>::iterator ite = epoll_fd_and_socket.find(fd);
    if(ite != epoll_fd_and_socket.end()){
        MYINFO("FDManager::close_fd() : Success");
        epoll_fd_and_socket.erase(fd);
        close(fd.to_int());
    }
}

bool FDManager::is_registered(FileDiscriptor epoll_fd)
{
    std::map<FileDiscriptor, FileDiscriptor>::iterator ite = epoll_fd_and_socket.find(epoll_fd);
    if(ite != epoll_fd_and_socket.end()){
        DEBUG("FDManager::is_registered() epoll_fd:" + epoll_fd.to_string() + " is registered");
        return (true);
    }
    DEBUG("FDManager::is_registered() epoll_fd:" + epoll_fd.to_string() + " is not registered");
    return (false);
}
FileDiscriptor &FDManager::socket_fd_from_epoll_fd(FileDiscriptor epoll_fd)
{
    DEBUG("FDManager::socket_fd_from_epoll_fd");
    if(this->is_registered(epoll_fd)){
        return (epoll_fd_and_socket[epoll_fd]);
    }
    WARNING("can't find epoll_fd:" + epoll_fd.to_string());
    throw std::runtime_error("can't find epoll_fd");
}

FileDiscriptor FDManager::get_sockfd(FileDiscriptor fd)
{
    if(epoll_fd_and_socket.find(fd) == epoll_fd_and_socket.end()){
        WARNING("fd don't find:" + Utility::to_string(fd.to_int()));
        throw std::invalid_argument("fd don't find:");
    }
    return (epoll_fd_and_socket[fd]);
}

std::map<FileDiscriptor, FileDiscriptor> &FDManager::fd_sockets()
{
    return (this->epoll_fd_and_socket);

}

