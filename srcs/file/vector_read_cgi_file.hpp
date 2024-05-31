#ifndef VECTOR_READ_CGI_FILE_HPP
#define VECTOR_READ_CGI_FILE_HPP
#include <dirent.h>
#include <vector>
#include <string>
#include "webserv_file.hpp"
#include "ireader.hpp"
#include "iwriter.hpp"
#include "status_code.hpp"
#include "global.hpp"
#include "buffer_controller.hpp"
#include "file_discriptor.hpp"
#include "process_id.hpp"

class VectorReadCGIFile
{
    public:
        VectorReadCGIFile();
        ~VectorReadCGIFile();
        //static VectorReadCGIFile* from_ref(std::string const& buf_ref);
        static VectorReadCGIFile* from_buf_size(size_t buf_size, ProcessID const &pid);
        //static VectorReadCGIFile* from_buf(char *buf, size_t size);
        int read(char **buf, size_t size);
        int write(char **buf, size_t size);
        int save(char *data, size_t size);
        size_t size();
        //bool is_chunk();
        FileState   state;
        void        clear_read();
        bool        completed();

    private:
        VectorReadCGIFile(size_t max_buf_size, ProcessID const &pid);
        //change_reader(IReader *ireader);
        //std::vector<char> buf;
        BufferController buffer;
        size_t max_buf_size;
        ProcessID pid;
        //IReader *reader;
        //IWriter *writer;
        //size_t buf_size;
        //std::vector<char> tmp_buf;
        //char buf_c[MAX_BUF];
};

namespace CheckSocketReadEndForCGIFunc{
    template <class FileT>
    bool completed(FileT *file){
        DEBUG("CheckSocketReadEndForCGIFunc::completed()");
        bool flag = false;
        if(file->size() >= MAX_REAUEST_EXCEPT_BODY ){
            flag = true;
        }else{
            char *buf;
            size_t read_size = file->read(&buf, file->size());
            char *pos = Utility::strnstr(buf, NL2_CGI, read_size);
            if(pos){
                flag = true;
            }
        }
        file->clear_read();


        if(flag == false){
            return (file->completed());
        }
        return (flag);
    }
}


#endif
