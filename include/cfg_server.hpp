#ifndef CFG_SERVER_HPP
#define CFG_SERVER_HPP

#include <string>
#include <vector>

class CFGServer
{
  public:
    CFGServer();
    ~CFGServer();

  private:
    std::vector<std::string> _root_path;
    std::vector<std::string> _extension;
    std::vector<std::string> _cfg_path;
};
#endif
