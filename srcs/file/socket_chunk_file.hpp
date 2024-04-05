
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
        size_t            copy_buf(char *data, size_t size);
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
    ssize_t read_chunked_size(FileT *file, char **data)
    {

        ssize_t buf_size = file->buf_size();
        ssize_t chunked_size = file->chunked_size();
        char tmp_data[21];
        char *p_data = tmp_data;
        int tmp_size = buf_size;
        Utility::memcpy(tmp_data, file->buf(), buf_size);
        p_data += buf_size;
        if(buf_size < 20){
            DEBUG("read_chunked_size buf_size=" + Utility::to_string(buf_size));

            //bufferサイズは16未満
            //Utility::memcpy(tmp_data, file->buf(), buf_size);
            //p_data += buf_size;
            // 64bitの16進数は16桁まで + CRLFの2を足して18
            tmp_size = (DefaultFunc::read(file, &p_data, 20-buf_size));
            DEBUG("read_chunked_size tmp_size=" + Utility::to_string(tmp_size));
            if(tmp_size <= 0){
                return (tmp_size);
            }
            tmp_size += buf_size;
        }
        DEBUG("read_chunked_size tmp_size=" + Utility::to_string(tmp_size));
        tmp_data[tmp_size] = '\0';
        ssize_t chunked_str_size;
        chunked_size = get_chunked_size<FileT>(&p_data, tmp_size, &chunked_str_size);
        MYINFO("chunked_size = " + Utility::to_string(chunked_size));
        MYINFO("chunked_size_str = " + Utility::to_string(chunked_str_size));
        if(chunked_size < 0 || chunked_str_size > 16){
            //chunkedの読み取りに失敗しているのでbufに保存
            file->set_buf(tmp_data, tmp_size);
            return (-1);
        }

        DEBUG(" test No.1");
        file->set_chunked_size(chunked_size);
        DEBUG(" test No.2");
        if(chunked_size == 0 && tmp_data[chunked_str_size] == '\r' && tmp_data[chunked_str_size+1] == '\n')
        {
        DEBUG(" test No.3");
            if(tmp_size < chunked_str_size + 4){
        DEBUG(" test No.4");
                // 最後まで読み込めていない可能性があるので(0/r/n/r/n)、追加で読み込み処理を行う
                // ゴミデータを削除するのが目的なので、このデータに対しては特に何もしない
                char tmp[3];
                char *tmp_p = tmp;
                buf_size = (DefaultFunc::read(file, &tmp_p, 2));
            }
        DEBUG(" test No.5");
            // 読み込みデータはないので-1を返す
            return (-1);
        }
        //MYINFO("chunked_size = " + Utility::to_string(chunked_size));
        //MYINFO("chunked_size_str = " + Utility::to_string(chunked_str_size));
        DEBUG(" test No.6 tmp_size=" + Utility::to_string(tmp_size));
        *data += tmp_size;
        file->clear_buf();
        DEBUG(" test No.7");
        return (tmp_size);
    }

    template <class FileT>
    int read_data(FileT *file, char **data, size_t size)
    {
        DEBUG("read_data test No.1");
        ssize_t chunked_size = file->chunked_size();
        ssize_t read_size;
        if((ssize_t)size > chunked_size ){
            size -= chunked_size;
        }
        DEBUG("read_data test No.2");
        read_size = (DefaultFunc::read(file, data, size));
        if(read_size < 0){
            return (-1);
        }
        DEBUG("read_data test No.3");
        file->set_chunked_size(chunked_size - read_size);
        DEBUG("read_data test No.4");
        return (read_size);
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

        ssize_t chunked_size = file->chunked_size();
        DEBUG("chunked_size=" + Utility::to_string(chunked_size));
        ssize_t buf_size = file->buf_size();
        ssize_t read_size = 0;
        if(chunked_size == 0){
            try{
                read_size = read_chunked_size(file, data);
                DEBUG("read_size=" + Utility::to_string(read_size));
                if(read_size <= 0){
                    return (-1);
                }
            }catch(std::invalid_argument &e){
                ERROR("finding chunked size error");
                throw HttpException("400");
            }
        }

        size -= read_size;
        DEBUG("No.2 size=" + Utility::to_string(size));
        DEBUG("read end");
        return (read_data(file, data, size));


        MYINFO("Chunked read() buf_siz No.2 buf_size=" + Utility::to_string(buf_size));
        printf("test No.1 chuned read=[");
        for(int i=0;i<buf_size;i++){
            printf("%c", (*data)[i]);
        }
        printf("]end read chunked\n");

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
