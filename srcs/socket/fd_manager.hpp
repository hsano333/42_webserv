#ifndef FD_MANAGER_H
#define FD_MANAGER_H

#include "request.hpp"
#include "response.hpp"
#include "socket_data.hpp"

class FDManager : public SocketData
{
  public:
    FDManager(FileDiscriptor fd);
    ~FDManager();
    void insert(Request* req);
    void insert(Response* res);
    Request* get_req(void);
    Response* get_res(void);
    void delete_req();
    void delete_res();
    void delete_all();
    void close_fd();

  private:
    int _fd;
    Request* _req;
    Response* _res;
};
#endif /* FD_MANAGER_H */
