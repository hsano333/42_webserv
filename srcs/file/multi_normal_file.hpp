#ifndef MULTI_NORMAL_FILE_HPP
#define MULTI_NORMAL_FILE_HPP

#include "file_discriptor.hpp"
#include "ireader.hpp"
#include "webserv_file.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <iostream>
#include "global.hpp"
#include "http_exception.hpp"
#include "webserv_file_factory.hpp"
#include "global.hpp"

class MultiNormalFile
{
    public:
        ~MultiNormalFile();
        static MultiNormalFile* from_directory_path(std::string const &directory_path, std::string const &boundary, FileDiscriptor const &fd);
        //static MultiNormalFile* from_fd(std::string const &directory_path, FileDiscriptor const &fd);
        int open();
        int close();
        int read(char **buf, size_t size);
        int write(char **buf, size_t size);
        int save(char *data, size_t size);
        //bool can_read();
        //bool can_write();

        FileDiscriptor const &fd();
        bool is_chunk();
        void set_chunk(bool flag);
        size_t size();
        int remove();
        std::string const &path();
        FileState   const &state();
        std::string const &content_type_boundary();
        std::string const &directory_path();
        void set_content_type_boundary(std::string &boundary);
        bool exist_file();
        void set_file(WebservFile *file);
        void set_completed(bool flag);
        bool completed();
        void set_buf(const char *buf, size_t size);
        void add_buf(const char *buf, size_t size);
        void clear_buf();
    private:
        //MultiNormalFile(IReader* ireader, std::string const &directory_path, std::ios_base::openmode mode);
        MultiNormalFile(std::string const &directory_path,std::string const &boundary, FileDiscriptor const &fd);
        FileDiscriptor const &fd_;
        IReader *ireader;
        //IWriter *writer;
        std::string const &directory_path_;
        std::string const &boundary_;
        //std::ios_base::openmode option;
        FileState   state_;
        std::fstream iofs;
        bool is_chunked_;
        WebservFile *file;
        std::vector<char> buf;
        bool    completed_;
        //std::ios_base::openmode mode;

};
//
//


//
//
//

namespace MultiFileFunc{


    template <class FileT>
    int write_file(FileT *file, char **data, size_t &size, std::string const &boundary)
    {
        MYINFO("MultiFileFunc write_file() size=" + Utility::to_string(size));
        size_t boundary_size = boundary.size();
        //size_t rest_size = size - total;
        size_t write_size;
        char *pos = Utility::strnstr(*data, boundary.c_str(), size);
        //char *buf_begin = &((*data)[total]);
        if(pos){
            write_size = (pos - *data);
            //MYINFO("MultiFileFunc write_file() No.2 write_size=" + Utility::to_string(write_size));
            if(write_size <= 2){
                return -1;
            }
            // -2 is CRLF
            write_size -= 2;
            int tmp_size = (file->write(data, write_size));
            //MYINFO("MultiFileFunc write_file() No.2 tmp_size=" + Utility::to_string(tmp_size));
            if(tmp_size <= 0){
                ERROR("Failure to write file:" + file->path());
                throw HttpException("500");
            }
            //MYINFO("MultiFileFunc write_file() No.3 close file");
            file->close();
            file->set_file(NULL);
            file->clear_buf();
            *data = pos;
            return tmp_size;
            //total += tmp_size + boundary_size + 2;
            /*
            //+2 is CRLF
            if(size == total + 2 && pos[boundary_size] == '-' && pos[boundary_size+1] == '-'){
                // ファイルすべて書き込み完了
                file->close();
                file->set_file(NULL);
                file->clear_buf();
                file->set_completed(true);
                return (total+2);
            }else if(size > total + 2 && pos[boundary_size] == '\r' && pos[boundary_size+1] == '\n'){
                // 次のファイルに書き込み準備完了
                *data = &(pos[boundary_size+2]);
                size -= total;
                //file->set_buf(&(pos[boundary_size+2]), size - total);
                file->close();
                file->set_file(NULL);
                continue;
            }else{
                // boundaryがちょうど区切りとなり、続く改行を消去できないため、
                //　再度bufにboundaryを入れ,読み込み処理から実行する。
                file->set_buf(pos, size - total + boundary_size + 2);
                return (total);
            }
            */
        }else{
            //MYINFO("MultiFileFunc write_file() No.4");
            //bounaryがないので全部(boundary_size分だけ残して)書き込み、
            //再度read処理を実行するため、returnする

            write_size = size - (boundary_size);
            if(write_size <= 2){
                return -1;
            }
            // -2 is CRLF
            write_size -= 2;
            //MYINFO("MultiFileFunc write_file() No.5 write_size=" + Utility::to_string(write_size));
            int tmp_size = (file->write(data, write_size));
            if(tmp_size <= 0){
                ERROR("Failure to write file:" + file->path());
                throw HttpException("500");
            }
            *data += write_size;
            //MYINFO("MultiFileFunc write_file() No.6 write_size=" + Utility::to_string(write_size));
            return (tmp_size);
        }
    }

    template <class FileT>
    int search_file(FileT *file, char **data, size_t &size, std::string const &directory, std::string const &boundary)
    {
        DEBUG("search_file() boundary:" + boundary);
        size_t boundary_size = boundary.size();
        size_t size_bk = size;
        //MYINFO("MultiFileFunc search_file() size=" + Utility::to_string(size));
        char *pos = Utility::strnstr(*data, boundary.c_str(), size);
        if(pos == NULL){
            MYINFO("not found \\r\\n");
            // writeできない分は上位層でbufに保存して次のタイミングでloadする。
            return (-1);
        }

        size_t tmp = (pos + boundary_size) - *data;
        //MYINFO("MultiFileFunc search_file() No.1 tmp=" + Utility::to_string(tmp));
        if(tmp+2 >= size){
            // 改行部分が途切れてしまうので、何もしないで終了
            MYINFO("not found \\r\\n");
            return (-1);
        }
        if(pos[boundary_size] == '-' && pos[boundary_size+1] == '-'){
            //MYINFO("MultiFileFunc search_file() completed");
            file->set_completed(true);
            return (size);
        }

        pos += boundary_size+2;
        size_t rest_size = size - (pos - *data);
        //MYINFO("MultiFileFunc search_file() No.2 rest_size=" + Utility::to_string(rest_size));
        char *pos2 = Utility::strnstr(pos, CRLF2, rest_size);
        if(pos2 == NULL){
            MYINFO("not found \\r\\n\\r\\n");
            //*data = pos;
            return (size_bk);
        }
        pos2[0] = '\0';
        Split sp(pos, CRLF, false, true);
        Header header = Header::from_splited_data(sp, 0);
        std::string const &disposition = header.find(CONTENT_DISPOSITION);
        if(disposition == header.not_find()){
            ERROR(" Invalid format: not find Content-Disposition in body");
            throw HttpException("400");
        }
        pos2 += 4;
        Split sp2(disposition, "; ", true, true);
        Split::iterator ite = sp2.begin();
        Split::iterator end = sp2.end();
        std::string name = "";
        while(ite != end){
            size_t pos_name = (*ite).find("name=");
            if(pos_name == 0){
                name = Utility::trim_char((*ite).substr(5), '\"');
                break;
            }
            ite++;
        }

        std::string filepath = directory + "/" + name;
        MYINFO("filepath=" + filepath);

        std::ios_base::openmode mode;
        std::string const &content_type = header.get_content_type();
        // Content-Typeの指定がなければtextとみなす
        if(content_type == header.not_find() || content_type.find("text/") != std::string::npos){
            mode = std::ios::out;
        }else{
            mode = std::ios::out | std::ios::binary;
        }

        WebservFileFactory *file_factory = WebservFileFactory::get_instance();
        WebservFile *normal_file = file_factory->make_normal_file(file->fd(), filepath, mode);
        normal_file->open();
        file->set_file(normal_file);
        size_t read_size = pos2-*data;
        *data = pos2;
        DEBUG("search_file() read_size:" + Utility::to_string(read_size));
        return ((int)read_size);
    }

    template <class FileT>
    int write(FileT *file, char **data, size_t size)
    {
        DEBUG("MultiFileFunc write() size=" + Utility::to_string(size));
        if(file->completed()){
            return -1;
        }

        std::string const &directory = file->directory_path();
        std::string const &boundary = file->content_type_boundary();
        size_t boundary_size = boundary.size();
        //MYINFO("MultiFileFunc write() directory:" + directory);
        //MYINFO("MultiFileFunc write() boundary:" + boundary);

        //MYINFO("MultiFileFunc write() No.1");
        //size_t total = 0;
        //int tmp_size = 0;

        size_t size_bk = size;
        int write_size;
        // +2 is CRLF or "--"
        while(size >= boundary_size + 2){
            //MYINFO("MultiFileFunc write() No.1 size=" + Utility::to_string(size));
            if(file->exist_file()){
                write_size = write_file(file, data, size, boundary);
            }else{
                write_size = search_file(file, data, size, directory, boundary);
            }
            if(write_size <= 0){
                break;
            }
            size -= (size_t)write_size;
        }
        return (size_bk - size);
    }
}

#endif
