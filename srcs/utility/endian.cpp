#include "endian.hpp"
#include <stdio.h>

Endian::Endian(){
    detect_endian();
}

Endian::~Endian(){
}

Endian* Endian::singleton = NULL;
Endian *Endian::get_instance()
{
    if (Endian::singleton == NULL){
        singleton = new Endian();
    }
    return (singleton);
}

ENDIAN Endian::get_endian()
{
    if (Endian::endian == INVALID){
        detect_endian();
    }
    return (Endian::endian);
}

ENDIAN Endian::endian = INVALID;
void Endian::detect_endian()
{
    int endian_test = 0xFFFEFC00;
    int *int_tmp = &endian_test;
    char *char_tmp= (char *)int_tmp;
    if (char_tmp[0] == 0 && char_tmp[3] == -1){
        Endian::endian = LITTLE;
    }else if (char_tmp[0] == -1 && char_tmp[3] == 0){
        Endian::endian = BIG;
    }else{
        Endian::endian = INVALID;
    }
}
