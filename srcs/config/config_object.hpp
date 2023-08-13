
#ifndef CONFIG_OBJECT_HPP
#define CONFIG_OBJECT_HPP

#include <string>

template <class T>
class ConfigObject
{
    public:
        virtual ~ConfigObject(){};
        virtual void add_properties(std::string &key, std::string &value) = 0;
        virtual T get_next_object() = 0;
    private:
};

#endif
