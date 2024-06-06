#include "normal_writer.hpp"
#include "global.hpp"
#include <sys/types.h>
#include <sys/socket.h>


NormalWriter::NormalWriter()
{
    ;
}
NormalWriter::~NormalWriter()
{
    //delete (singleton);
}

NormalWriter* NormalWriter::singleton = NULL;
NormalWriter *NormalWriter::get_instance()
{
    if (NormalWriter::singleton == NULL){
        singleton = new NormalWriter();
    }
    return (singleton);
}

int NormalWriter::write(FileDiscriptor const &fd, char const *buf, size_t size, std::fstream *ifs)
{
    DEBUG("NormalWriter::write fd=" + fd.to_string());
    if(buf == NULL){
        DEBUG("NormalWriter::write buf is NULL");
    }
        DEBUG("NormalWriter::write No.2");
        printf("\n size=%zu buf=[[[[[[[[[[[[[[[[[[[[[[", size);
        for(size_t i=0;i<size;i++){
            printf("%c", buf[i]);
        }
        printf("]]]]]]]]]]]]]]]]]]] end\n\n");
        DEBUG("printf test");

        if(!ifs){
            int val = ::write(fd.to_int(), buf, size);
            DEBUG("NormalWriter::write No.2 val:" + Utility::to_string(val));
            return val;
        }

        if(ifs->fail()){
            DEBUG("printf test No.1");
            return -1;
        }
        DEBUG("printf test No.2");


        ifs->write(buf, size);
        if(ifs->fail()){
            return -1;
        }
        return (ifs->gcount());
        //int val = ::write(fd.to_int(), &(buf[i]), 1);
        //DEBUG("val=" + Utility::to_string(val));
        //int cnt = 0;
        /*
        for(size_t i=0;i<size;i++){
            int val = ::write(fd.to_int(), &(buf[i]), 1);
            DEBUG("val=" + Utility::to_string(val));
            DEBUG("cnt=" + Utility::to_string(cnt));
            if(val == 1){
                cnt += val;
            }else{
                break;
            }
        }
        */
            /*
        try{
            val = ::write(fd.to_int(), &(buf[1]), size-1);
            DEBUG("NormalWriter::write No.2 val:" + Utility::to_string(val));
            return val;
        }catch(...){
            DEBUG("NormalWriter::write No.3 val try catch");

        }
        */
        //return val;
    (void)ifs;
    //return ::write(fd.to_int(), buf, size);
}
