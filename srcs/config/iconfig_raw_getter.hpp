#ifndef CONFIG_RAW_GETTER
#define CONFIG_RAW_GETTER

#include <string>

template <typename T>
class IConfigRawGetter
{
    public:
        IConfigRawGetter(){};
        virtual ~IConfigRawGetter(){};
        //virtual std::basic_string<T, std::char_traits<T>, std::allocator<T> >  get_raw_data() = 0;
        virtual T get_raw_data() = 0;
};
#endif
