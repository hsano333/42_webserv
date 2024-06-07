#include "config_parsered_data.hpp"
#include "global.hpp"

ConfigParseredData::ConfigParseredData()
{
    ;
}
ConfigParseredData::~ConfigParseredData()
{
    ;
}


void ConfigParseredData::push_back_next_string(std::string const &str)
{
    next_raw_data.push_back(str);
}

void ConfigParseredData::push_back_next_properties(std::vector<std::string> &vec)
{
    std::vector<std::string> tmp;
    for(size_t i=0;i<vec.size();i++){
        tmp.push_back(vec[i]);
    }
    next_target_properties.push_back(tmp);
}

std::string ConfigParseredData::raw_data(size_t i)
{
    if(next_raw_data.size() > i){
        return (next_raw_data[i]);
    }else{
        ERROR("Invalid Argument: ConfigParseredData::raw_data()");
        throw std::invalid_argument("Invalid Argument: ConfigParseredData::raw_data()");
    }
}

std::vector<std::string> &ConfigParseredData::properties(size_t i)
{
    if(next_target_properties.size() > i){
        return (next_target_properties[i]);
    }else{
        ERROR("Invalid Argument: ConfigParseredData::properties() i=");
        throw std::invalid_argument("Invalid Argument: ConfigParseredData::raw_data()");
    }
}

size_t ConfigParseredData::size()
{
    return (next_raw_data.size());
}
