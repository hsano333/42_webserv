#ifndef VECTOR_FILE_HPP
#define VECTOR_FILE_HPP
#include <dirent.h>
#include <vector>
#include <string>
#include "webserv_file.hpp"
#include "ireader.hpp"
#include "iwriter.hpp"
#include "status_code.hpp"
#include "global.hpp"
#include "buffer_controller.hpp"

class VectorFile
{
    public:
        VectorFile();
        ~VectorFile();
        static VectorFile* from_ref(std::string const& buf_ref);
        static VectorFile* from_buf_size(size_t buf_size);
        static VectorFile* from_buf(char *buf, size_t size);
        int read(char **buf, size_t size);
        int write(char **buf, size_t size);
        int save(char *data, size_t size);
        size_t size();
        FileState   state;
        void        clear_read();

    private:
        VectorFile(size_t max_buf_size);
        BufferController buffer;
        size_t max_buf_size;
};


namespace CheckSocketReadEndFunc{
    template <class FileT>
    bool completed(FileT *file){
        DEBUG("CheckSocketReadEndFunc::completed()");
        bool flag = false;
        if(file->size() >= MAX_REAUEST_EXCEPT_BODY ){
            flag = true;
        }else{
            char *buf = NULL;
            int read_size = file->read(&buf, file->size());
            if(read_size <= 0){
                return (false);
            }
            char *pos = Utility::strnstr(buf, CRLF2, read_size);
            if(pos){
                flag = true;
            }
        }
        file->clear_read();
        return (flag);
    }
}

#endif
