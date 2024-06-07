
#ifndef CONFIG_OBJECT_HPP
#define CONFIG_OBJECT_HPP

#include <string>
#include <vector>

template <class T>
class ConfigObject
{
    public:
        virtual ~ConfigObject(){};
        virtual void assign_properties(std::vector<std::vector<std::string> > &properties) = 0;
        virtual void assign_out_properties(std::vector<std::string> &properties) = 0;
        virtual void push_all(std::vector<T> const &object) = 0;
        virtual void check() = 0;
    private:
};

#endif
