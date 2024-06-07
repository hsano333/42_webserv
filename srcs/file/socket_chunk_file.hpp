
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
        WebservFile *file();
        int open();
        int read(char **buf, size_t size);
        int write(char **buf, size_t size);
        void save(std::vector<char> &buffer);
        bool can_read();
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
        size_t            size();
        void              clear_read();
        BufferController const &get_buffer();

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
        }
        *chunked_str_size = (pos - &((*buf)[0]) );
        pos[0] = '\0';
        size_t chunked_size = Utility::hex_string_to_number(*buf);
        return (chunked_size);
    }

    template <class FileT>
    ssize_t read_chunked_size(FileT *file, char **data)
    {

        ssize_t buf_size = file->buf_size();
        ssize_t chunked_size = file->chunked_size();
        const int TMP_READ_SIZE = 20;
        char tmp_data[TMP_READ_SIZE+1];
        char *p_data = tmp_data;
        int tmp_size = buf_size;
        Utility::memcpy(tmp_data, file->buf(), buf_size);
        p_data += buf_size;
        size_t exceed_size = 0;
        if(buf_size < TMP_READ_SIZE){
            DEBUG("read_chunked_size buf_size=" + Utility::to_string(buf_size));

            //bufferサイズは16未満
            // 64bitの16進数は16桁まで + CRLFの2を足して18
            tmp_size = (DefaultFunc::read(file, &p_data, TMP_READ_SIZE-buf_size));
            if(tmp_size <= 0){
                return (tmp_size);
            }
            tmp_size += buf_size;

        }
        tmp_data[tmp_size] = '\0';
        ssize_t chunked_str_size;
        chunked_size = get_chunked_size<FileT>(&p_data, tmp_size, &chunked_str_size);
        exceed_size = TMP_READ_SIZE - (chunked_str_size + 2);
        if(chunked_size < 0 || chunked_str_size > 16){
            //chunkedの読み取りに失敗しているのでbufに保存
            file->set_buf(tmp_data, tmp_size);
            return (-1);
        }

        if(chunked_size == 0 && p_data[3] == '\r' && p_data[4] == '\n')
        {
            file->set_chunked_size(0);
            if(tmp_size < chunked_str_size + 4){
                // 最後まで読み込めていない可能性があるので(0/r/n/r/n)、追加で読み込み処理を行う
                // ゴミデータを削除するのが目的なので、このデータに対しては特に何もしない
                char tmp[3];
                char *tmp_p = tmp;
                buf_size = (DefaultFunc::read(file, &tmp_p, 2));
            }
            // 読み込みデータはないので-1を返す
            return (-1);
        }

        for(size_t i=0;i<exceed_size;i++){
            (*data)[i] = p_data[chunked_str_size+i+2];
        }
        *data += exceed_size;
        //*data += tmp_size;
        file->set_chunked_size(chunked_size - exceed_size);
        file->clear_buf();
        return (exceed_size);
    }

    template <class FileT>
    int read_data(FileT *file, char **data, size_t size)
    {
        ssize_t chunked_size = file->chunked_size();
        ssize_t read_size;
        if((ssize_t)size > chunked_size ){
            size = chunked_size;
        }
        read_size = (DefaultFunc::read(file, data, size));
        if(read_size < 0){
            return (-1);
        }
        file->set_chunked_size(chunked_size - read_size);
        return (read_size);
    }


    template <class FileT>
    int read(FileT *file, char **data, size_t size)
    {
        if (file->state != FILE_OPEN){
            return (0);
        }
        if(file->completed()){
            return -1;
        }

        ssize_t chunked_size = file->chunked_size();
        ssize_t read_size = 0;
        if(chunked_size == 0){
            try{
                read_size = read_chunked_size(file, data);
                if(read_size <= 0){
                    return (-1);
                }
            }catch(std::invalid_argument &e){
                ERROR("finding chunked size error");
                throw HttpException("400");
            }
        }

        size -= read_size;
        int tmp_read_size = (read_data(file, data, size));
        if(tmp_read_size <= 0){
            return (tmp_read_size);
        }
        return (tmp_read_size + read_size);
    }

    template <class FileT>
    int read_body_and_copy_chunk(FileT *file, char** dst, size_t size)
    {
        (void)file;
        char *tmp = *dst;
        int chunk_size = 20;
        char *tmp2 = &(tmp[chunk_size]);
        int read_size = file->read(&(tmp2), size - chunk_size);

        // chunkサイズは16進数
        std::string size_str = Utility::to_hexstr(read_size);
        size_str += CRLF;
        size_t len = size_str.size();
        Utility::memcpy(tmp, size_str.c_str(), len);
        Utility::memcpy(&(tmp[len]), tmp2, read_size);
        tmp[len+read_size+1] = '\r';
        tmp[len+read_size+2] = '\n';
        tmp[read_size+len] = '\r';
        tmp[read_size+len+1] = '\n';
        read_size += 2;

        return (read_size+len);
    }

    template <class FileT>
    int read_ref_body_chunk(FileT *file, char** dst, size_t size)
    {
        (void)file;
        char *tmp = *dst;
        int chunk_size = 20;
        char *tmp2 = &(tmp[chunk_size]);
        int read_size = file->read(&(tmp2), size - chunk_size-5);

        // chunkサイズは16進数
        std::string size_str = Utility::to_hexstr(read_size);
        size_str += CRLF;
        size_t len = size_str.size();
        Utility::memcpy(&(tmp[chunk_size-len]), size_str.c_str(), len);
        *dst = &(tmp[chunk_size-len]);
        tmp2[read_size] = '\r';
        tmp2[read_size+1] = '\n';
        read_size += 2;

        return (read_size+len);
    }

    template <class FileT>
    int read_for_write(FileT *file, char **data, size_t size)
    {
        (void)size;
        if (!(file->state == FILE_OPEN)){
            ERROR(" not open: ");
            throw std::runtime_error("not open");
            return (0);
        }
        //return (file->write(data, size));

        int tmp2 = (read_body_and_copy_chunk(file, data, size));
        return (tmp2);
    }

    template <class FileT>
    int read_for_autoindex(FileT *file, char **data, size_t size)
    {
        (void)size;
        if (!(file->state == FILE_OPEN)){
            ERROR(" not open: ");
            throw std::runtime_error("not open");
            return (0);
        }

        int tmp2 = (read_ref_body_chunk(file, data, size));
        return (tmp2);
    }

    template <class FileT>
    bool is_chunk(FileT *file)
    {
        (void)file;
        return (true);
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
