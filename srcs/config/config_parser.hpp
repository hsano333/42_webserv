#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include <string>
#include <vector>

template <class T, class U>
class ConfigParser
{
    public:
        ConfigParser(const std::string &target);
        ~ConfigParser();
        std::vector<std::string> parser(std::string const &str, T * object);
    private:
        const std::string &target;
        //const std::string &target;
};

template <class T, class U>
ConfigParser<T,U>::ConfigParser(const std::string &target) : target(target)
{
    ;
}

template <class T, class U>
ConfigParser<T,U>::~ConfigParser()
{
    ;
}

template <class T, class U>
std::vector<std::string> ConfigParser<T, U>::parser(std::string const &str, T * object)
{
    (void)str;
    (void)object;
    std::vector<U> next_vec;
    //U next_obj;


    std::vector<std::string> next_obj_strings;
    std::string key = "key";
    std::string value = "value";

    object->add_properties(key, value);

    return next_obj_strings;
}

#endif
