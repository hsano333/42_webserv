
#ifndef SOCKET_CHUNK_FILE_HPP
#define SOCKET_CHUNK_FILE_HPP
#include <dirent.h>
#include <string>
#include <vector>
#include "webserv_file.hpp"
#include "ireader.hpp"
#include "iwriter.hpp"
#include "status_code.hpp"
#include "buffer_controller.hpp"
#include "webserv_file.hpp"
#include "global.hpp"
#include "http_exception.hpp"
#include "webserv_file_factory.hpp"

class SocketChunkFile
{
    public:
        ~SocketChunkFile();
        static SocketChunkFile* from_file(FileDiscriptor const &fd, WebservFile *file);
        //static SocketChunkFile* from_fd(IReader* ireader, FileDiscriptor fd);
        //static SocketChunkFile* from_fd(IWriter* iwriter, FileDiscriptor fd);
        //int open();
        //int close();
        WebservFile *file();
        int read(char **buf, size_t size);
        int write(char **buf, size_t size);
        //int save(char *data, size_t size);
        bool can_read();
        //bool is_chunk();
        FileState   state;

        size_t chunked_size();
        void set_chunked_size(size_t size);
        bool is_chunked();
        void set_is_chunked(bool chunked);
        char              *buf();
        size_t            buf_size();
        size_t            buf_p();
        void              clear_buf();
        void              set_buf(char *data, size_t size);
        void              add_buf(char *data, size_t size);
        void              set_buf_p(size_t pos);
        void              set_completed(bool flag);
        bool              completed();
        //void              append_buf(char *data, size_t size);
        size_t            size();
        void              clear_read();

    private:
        SocketChunkFile(FileDiscriptor const &fd, WebservFile *file);
        FileDiscriptor const &fd;
        IReader *reader;
        IWriter *writer;
        BufferController buffer;
        size_t chunked_size_;
        size_t buf_p_;
        bool   is_chunked_;
        WebservFile *file_;
        std::vector<char> buf_;
        bool completed_;
        size_t total_write_size;
        //size_t buf_size_;
        //std::vector<char> buf;
        //size_t            chunked_size;
        //std::vector<char> tmp_buf;
};

namespace ChunkedFunc{
    template <class FileT>
    ssize_t get_chunked_size(char **buf, ssize_t size, ssize_t *chunked_str_size)
    {
        MYINFO("MYINFO::get_chunked_size size=" + Utility::to_string(size));

        char *pos = Utility::strnstr(*buf, CRLF, size);
        if(pos == NULL){
            WARNING("not found CRLF");
            return (-1);
            //throw HttpException("400");
        }
        *chunked_str_size = (pos - &((*buf)[0]) );
        pos[0] = '\0';
        size_t chunked_size = Utility::hex_string_to_number(*buf);
        //pos += 2;
        //*buf = pos;
        return (chunked_size);
    }

    template <class FileT>
    int read(FileT *file, char **data, size_t size)
    {
        DEBUG("Chunked read()");
        if (file->state != FILE_OPEN){
            return (0);
        }
        if(file->completed()){
            return -1;
        }

        ssize_t buf_size = file->buf_size();
        MYINFO("Chunked read() buf_size=" + Utility::to_string(buf_size));
        // 64bitの16進数は16桁まで + CRLFの2を足して18
        // さらに終了を表すCRLFの2を追加して20
        if(buf_size > 20){
            buf_size = file->buf_size();
            *data = file->buf();
        }else if(buf_size > 0){
            // buf_size <= 20;
            char tmp[21];
            char *tmp_p = tmp;
            int tmp_size = (DefaultFunc::read(file, &(tmp_p), 20 - buf_size));
            //int tmp_size = file->file()->read(&(tmp_p), 20 - buf_size);

            if(tmp_size > 0){
                file->add_buf(tmp, tmp_size);
            }
            buf_size = file->buf_size();
            *data = file->buf();
        }else{
            buf_size = (DefaultFunc::read(file, data, size));
            //buf_size = file->file()->read(data, size);
        }
        if(buf_size <= 0){
            return (-1);
        }
        MYINFO("Chunked read() buf_siz No.2 buf_size=" + Utility::to_string(buf_size));
        printf("test No.1 chuned read=[");
        for(int i=0;i<buf_size;i++){
            printf("%c", (*data)[i]);
        }
        printf("]end read chunked\n");

        ssize_t chunked_size = file->chunked_size();
        ssize_t chunked_str_size;
        if(chunked_size == 0){
            chunked_size = get_chunked_size<FileT>(data, buf_size, &chunked_str_size);
            if(chunked_size < 0){
                return (-1);
                //buf_size = (DefaultFunc::read(file, data, 20));
                //buf_size = (DefaultFunc::read(file, data, size));
                //chunked_size = get_chunked_size<FileT>(data, buf_size, &chunked_str_size);
                //return -1;

            }
            // +2 is CRLF
            chunked_str_size += 2;
            cout << "chunked_str_size=" << chunked_str_size << endl;
            //*data = &((*data)[chunked_str_size+0]);
            *data += chunked_str_size;
        }
        MYINFO("chunked_size = " + Utility::to_string(chunked_size));
        MYINFO("chunked_size_str = " + Utility::to_string(chunked_str_size));
        if(chunked_size == 0 && (*data)[1] == '\r' && (*data)[2] == '\n'){
            if(buf_size < chunked_size + chunked_str_size + 2){
                // 最後まで読み込めていない可能性があるので、追加で読み込み処理を行う
                char tmp[5];
                char *tmp_p = tmp;
                buf_size = (DefaultFunc::read(file, &tmp_p, 5));
                //buf_size = file->file()->read(&tmp_p, 5);
                if(buf_size > 0){
                    file->add_buf(tmp, buf_size);
                    *data = file->buf();
                }
            }
            DEBUG("Chunked completed test No.1");
            if(buf_size >= chunked_size + chunked_str_size + 2){
            DEBUG("Chunked completed test No.2");
                if((*data)[3] == '\r' && (*data)[4] == '\n'){
            DEBUG("Chunked completed test No.3");
                    file->set_completed(true);
                    file->clear_buf();
                    DEBUG("Chunked completed");
                    //*data = file->buf();
                }
            }
            DEBUG("Chunked completed test No.5");
            return (-1);
        }

        MYINFO("chunked_size = " + Utility::to_string(chunked_size));
        MYINFO("buf_size = " + Utility::to_string(chunked_size));
        MYINFO("chunked_str_size = " + Utility::to_string(chunked_str_size));
        // 2 is CRLF
        if(chunked_size - (buf_size - chunked_str_size) > 2){
            // Data remain
            file->set_buf_p(chunked_size + chunked_str_size+2);
            file->set_chunked_size(chunked_size - (buf_size - chunked_str_size));
            //file->set_buf_p(0);
        }else{
            // read completed
            //file->set_buf_p(chunked_size + chunked_str_size);
            file->set_chunked_size(0);
            file->set_buf_p(0);
            file->clear_buf();

        }
        MYINFO("return buf_size = " + Utility::to_string(buf_size));
        MYINFO("return chunked_size = " + Utility::to_string(chunked_size));
        return (chunked_size);
    }


    template <class FileT>
    int write(FileT *file, char **data, size_t size)
    {
        (void)size;
        DEBUG("Chunked write_buf()");
        MYINFO("size=" + Utility::to_string(size));
        if (!(file->state == FILE_OPEN)){
            ERROR(" not open: ");
            throw std::runtime_error("not open");
            return (0);
        }

        size_t chunked_size = file->chunked_size();
        // ひとつのチャンクがすべて消化されるまで、書き込み禁止
        if(chunked_size > 0){
            ERROR("chunked size is not zero: ");
            throw std::runtime_error("chunked size is not zero");
        }
        //size_t chunked_str_size;
        MYINFO("No.2 size=" + Utility::to_string(size));
        //chunked_size = get_chunked_size<FileT>(data, size, &chunked_str_size);
        //*data = &((*data)[chunked_str_size+2]);

        //file->set_buf(*data, chunked_size);
        file->set_buf(*data, size);
        return (size);
    }
}

namespace ExistCRLF2{
    template <class FileT>
    bool completed(FileT *file)
    {
        char *buf;
        bool flag = false;
        size_t read_size = file->read(&buf, file->size());
        char *pos = Utility::strnstr(buf, CRLF2, read_size);
        if(pos){
            flag = true;
        }
        file->clear_read();
        return (flag);
    }
}

#endif
