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
    int write(FileT *file, char **data, size_t size)
    {
        DEBUG("MultiFileFunc write() size=" + Utility::to_string(size));
        if(file->completed()){
            return -1;
        }

        std::string const &directory = file->directory_path();
        std::string const &boundary = file->content_type_boundary();
        size_t boundary_size = boundary.size();
        MYINFO("MultiFileFunc write() directory:" + directory);
        MYINFO("MultiFileFunc write() boundary:" + boundary);

        MYINFO("MultiFileFunc write() No.1");
        size_t total = 0;
        int tmp_size = 0;
        while(size > total){
        MYINFO("MultiFileFunc write() No.1");
            if(file->exist_file()){
        MYINFO("MultiFileFunc write() No.2");
                size_t rest_size = size - total;
                size_t write_size;
                char *pos = Utility::strnstr(&((*data)[total]), boundary.c_str(), rest_size);
                char *buf_begin = &((*data)[total]);
                if(pos){
        MYINFO("MultiFileFunc write() No.3");
                    write_size = (pos-buf_begin) - 2;
                    // -2 is CRLF
                    if((pos-buf_begin) - 2 > 0){
        MYINFO("MultiFileFunc write() No.4");
                        tmp_size = (file->write(&buf_begin, write_size));
                    }
                    if(tmp_size <= 0){
        MYINFO("MultiFileFunc write() No.5");
                        file->set_buf(buf_begin, rest_size);
                        return (-1);
                    }
                    total += tmp_size + boundary_size + 2;
                    //+2 is CRLF
                    if(size == total + 2 && pos[boundary_size] == '-' && pos[boundary_size+1] == '-'){
        MYINFO("MultiFileFunc write() No.6");
                        // ファイルすべて書き込み完了
                        file->close();
                        file->set_file(NULL);
                        file->clear_buf();
                        file->set_completed(true);
                        return (total+2);
                    }else if(size > total + 2 && pos[boundary_size] == '\r' && pos[boundary_size+1] == '\n'){
        MYINFO("MultiFileFunc write() No.7");
                        // 次のファイルに書き込み準備完了
                        file->set_buf(&(pos[boundary_size+2]), size - total);
                        file->close();
                        file->set_file(NULL);
                        continue;
                    }else{
        MYINFO("MultiFileFunc write() No.8");
                        // boundaryがちょうど区切りとなり、続く改行を消去できないため、
                        //　再度bufにboundaryを入れ,読み込み処理から実行する。
                        file->set_buf(pos, size - total + boundary_size + 2);
                        return (total);
                    }
                }else{
        MYINFO("MultiFileFunc write() No.9");
                    //bounaryがないので全部書き込み、再度read処理を実行するため、returnする
                    write_size = size - total;
                    tmp_size = (file->write(data, write_size));
                    if(tmp_size <= 0){
        MYINFO("MultiFileFunc write() No.10");
                        file->set_buf(buf_begin, write_size);
                        return (-1);
                    }
                    total += tmp_size;
                    return (total);
                }
                continue;
            }
            //if(tmp_size <= 0){
                //return (-1);
            //}
            //total += tmp_size;
            //if(total >= size){
                //return (total);
            //}
        MYINFO("MultiFileFunc write() No.11");

            MYINFO("size=" + Utility::to_string(size));
            char *pos = Utility::strnstr(*data, boundary.c_str(), size);

            if(pos == NULL){
                ERROR(" not chunked format");
        MYINFO("MultiFileFunc write() No.12");
                throw HttpException("400");
            }

            pos += boundary_size+2;
            char *pos2 = Utility::strnstr(pos, CRLF2, size - (boundary_size+2));
            if(pos2 == NULL){
        MYINFO("MultiFileFunc write() No.13");
                ERROR(" not chunked format");
            }
        MYINFO("MultiFileFunc write() No.14");
            pos2[0] = '\0';
            Split sp(pos, CRLF, false, true);
            Header header = Header::from_splited_data(sp, 0);
            std::string const &disposition = header.find(CONTENT_DISPOSITION);
            if(disposition == header.not_find()){
                ERROR(" Invalid format: not find Content-Disposition");
                throw HttpException("400");
            }
        MYINFO("MultiFileFunc write() No.15");
            pos2 += 4;
            Split sp2(disposition, "; ", true, true);
            Split::iterator ite = sp2.begin();
            Split::iterator end = sp2.end();
            std::string name = "";
            //MYINFO("test No.2 sp.size()=" + Utility::to_string(sp.size()));
            while(ite != end){
                size_t pos_name = (*ite).find("name=");
        MYINFO("MultiFileFunc write() No.16");
                if(pos_name == 0){
        MYINFO("MultiFileFunc write() No.17");
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
        MYINFO("MultiFileFunc write() No.18");
            if(content_type == header.not_find() || content_type.find("text/") != std::string::npos){
        MYINFO("MultiFileFunc write() No.19");
                mode = std::ios::out;
            }else{
        MYINFO("MultiFileFunc write() No.20");
                mode = std::ios::out | std::ios::binary;
            }

            MYINFO("Make file MultiFileFunc write() No.20");
            WebservFileFactory *file_factory = WebservFileFactory::get_instance();
            WebservFile *normal_file = file_factory->make_normal_file(file->fd(), filepath, mode);
            normal_file->open();
            file->set_file(normal_file);
            (void)pos;

            size_t body_size = size - (pos2 - (*data));
            char *pos3 = Utility::strnstr(pos2, "--", size);
            DEBUG("normal_file->write() body_size=" + Utility::to_string(body_size));
            if(pos3 == NULL){
        MYINFO("MultiFileFunc write() No.21");
                DEBUG("pos3 is NULL");
                normal_file->write(&pos2, body_size);
            }else{
        MYINFO("MultiFileFunc write() No.22");
                DEBUG("pos3 is not NULL");
                MYINFO("size=" + Utility::to_string(size));
                MYINFO("pos3=" + Utility::to_string(pos3));
                MYINFO("pos2=" + Utility::to_string(pos2));
                MYINFO("body_size=" + Utility::to_string(body_size));
                // 2 is CRLF
                body_size = (pos3 - pos2-2);
                MYINFO("No.2 body_size=" + Utility::to_string(body_size));
                normal_file->write(&pos2, body_size);
                normal_file->close();
                MYINFO("write _size=" + Utility::to_string(size-(pos3 - (*data))));
                file->add_buf(pos3, size-(pos3 - (*data)));
            }
            return (size);
        }
        return (size);
    }
}

#endif
