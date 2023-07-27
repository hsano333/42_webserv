#ifndef CONFIG_LOCATION_HPP
#define CONFIG_LOCATION_HPP

#include <vector>
#include <map>
#include <string>

class ConfigLocation
{
    public:
        std::vector<std::string> urls;
        std::map<std::string, std::vector<std::string> > properties;
    private:


};

#endif
