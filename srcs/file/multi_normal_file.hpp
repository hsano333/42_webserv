#ifndef MULTI_NORMAL_FILE_HPP
#define MULTI_NORMAL_FILE_HPP

#include "file_discriptor.hpp"
#include "ireader.hpp"
#include "webserv_file.hpp"
#include "status_code.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <iostream>
#include "global.hpp"
#include "http_exception.hpp"
#include "webserv_file_factory.hpp"
#include "global.hpp"
#include "http_version.hpp"

class MultiNormalFile
{
    public:
        ~MultiNormalFile();
        static MultiNormalFile* from_directory_path(std::string const &directory_path, std::string const &boundary, FileDiscriptor const &fd);
        int open();
        int close();
        int read(char **buf, size_t size);
        int write(char **buf, size_t size);
        int save(char *data, size_t size);

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
        void register_file(WebservFile *file, int status_code);
        bool register_file_flag();
        std::map<WebservFile *, StatusCode> &uploaded_files();
        void set_register_file_flag(bool flag);
        size_t uploaded_number_of_files();
        void set_uploaded_number_of_files(size_t size);
        bool can_write_file();
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
        bool    register_file_flag_;
        size_t  uploaded_number_of_files_;
        std::map<WebservFile *, StatusCode> uploaded_files_;

        //std::ios_base::openmode mode;

};
//
//


//
//
//

namespace MultiFileFunc{

    template <class FileT>
    bool is_ok(FileT *file)
    {
        std::map<WebservFile *, StatusCode>::iterator ite = file->uploaded_files().begin();
        std::map<WebservFile *, StatusCode>::iterator end = file->uploaded_files().end();
        while(ite != end)
        {
            if(ite->second.to_int() >= 400){
                return (false);
            }
            ite++;
        }
        return (true);
    }

    template <class FileT>
    int write_file(FileT *file, char **data, size_t &size, std::string const &boundary)
    {
        MYINFO("MultiFileFunc write_file() size=" + Utility::to_string(size));
        size_t boundary_size = boundary.size();
        size_t write_size;
        char *pos = Utility::strnstr(*data, boundary.c_str(), size);
        if(pos){
            MYINFO("MultiFileFunc write_file() No.2 size=" + Utility::to_string(size));
            write_size = (pos - *data);
            if(write_size <= 2){
                return -1;
            }
            // -2 is CRLF
            write_size -= 2;
            int tmp_size;
            if(file->can_write_file()){
            MYINFO("MultiFileFunc write_file() No.3 size=" + Utility::to_string(size));
                tmp_size = (file->write(data, write_size));
            }else{
            MYINFO("MultiFileFunc write_file() No.4 size=" + Utility::to_string(size));
                tmp_size = write_size;
            }
            if(tmp_size <= 0){
                ERROR("Failure to write file:" + file->path());
                throw HttpException("500");
            }
            //MYINFO("MultiFileFunc write_file() No.3 close file");
            //
            //bool is_ok = is_ok(file);

            MYINFO("MultiFileFunc write_file() No.5 size=" + Utility::to_string(size));
            file->close();
            file->set_file(NULL);
            file->clear_buf();
            *data = pos;
            MYINFO("MultiFileFunc write_file() No.6 tmp_size=" + Utility::to_string(tmp_size + 2));
            //size_t read_size = pos2-*data;
            // +2 is CRLF
            return (tmp_size+2);
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
        MYINFO("MultiFileFunc write_file() No.5 size=" + Utility::to_string(size));
            //MYINFO("MultiFileFunc write_file() No.4");
            //bounaryがないので全部(boundary_size分だけ残して)書き込み、
            //再度read処理を実行するため、returnする

            //うしろからboundary_size分だけ[--]がないかどうかを確認し、あればそれ以降がbounaryでないかを確認する
            //　boundaryの文字で途切れているならその部分だけをbufferに保存する。
            char *check_pos = NULL;
            write_size = size;
            if(size > (boundary_size+2)){
                check_pos = *data + (size -  (boundary_size+2));
                pos = Utility::strnstr(check_pos, CRLF, boundary_size+2);
                if(pos){
                    write_size = pos - *data;
                }else if((*data)[write_size] == '\r'){
                    write_size--;
                }
            }else{
                return (-1);
            }
        MYINFO("MultiFileFunc write_file() No.6 size=" + Utility::to_string(size));
            //int tmp_size = (file->write(data, write_size));

            //write_size = size - (boundary_size);
            //if(write_size <= 2){
                //return -1;
            //}
            // -2 is CRLF
            //write_size -= 2;
            MYINFO("MultiFileFunc write_file() No.5 write_size=" + Utility::to_string(write_size));
            int tmp_size = (file->write(data, write_size));
            if(tmp_size <= 0){
                ERROR("Failure to write file:" + file->path());
                throw HttpException("500");
            }
            size_t rval_size;
            if(pos){
        MYINFO("MultiFileFunc write_file() No.7 size=" + Utility::to_string(size));
                //int diff = size - tmp_size;
                //file->set_buf(pos, diff);
                rval_size = pos-(*data);
                *data = pos;
            }else{
                rval_size = tmp_size;
                *data = &((*data)[tmp_size]);

        MYINFO("MultiFileFunc write_file() No.8 size=" + Utility::to_string(size));
            }
        MYINFO("MultiFileFunc write_file() No.9 rval_size=" + Utility::to_string(rval_size));
            //*data += write_size;
            //MYINFO("MultiFileFunc write_file() No.6 write_size=" + Utility::to_string(write_size));
            return (rval_size);
        }
    }

    template <class FileT>
    int search_file(FileT *file, char **data, size_t &size, std::string const &directory, std::string const &boundary)
    {
        DEBUG("search_file() boundary:" + boundary);
        size_t boundary_size = boundary.size();
        //size_t size_bk = size;
        //MYINFO("MultiFileFunc search_file() size=" + Utility::to_string(size));
        char *pos = Utility::strnstr(*data, boundary.c_str(), size);
        if(pos == NULL){
            MYINFO("not found \\r\\n");
            // writeできない分は上位層でbufに保存して次のタイミングでloadする。
            return (-1);
        }

        size_t tmp = (pos + boundary_size) - *data;
        //MYINFO("MultiFileFunc search_file() No.1 tmp=" + Utility::to_string(tmp));
        if(tmp+2 > size){
            // 改行部分が途切れてしまうので、何もしないで終了
            MYINFO("not found \\r\\n");
            return (-1);
        }
        DEBUG("search_file() boundary No.2:" + boundary);
        if(pos[boundary_size] == '-' && pos[boundary_size+1] == '-'){
            file->set_completed(true);
            return (size);
        }
        DEBUG("search_file() boundary No.3:" + boundary);

        pos += boundary_size+2;
        size_t rest_size = size - (pos - *data);
        //MYINFO("MultiFileFunc search_file() No.2 rest_size=" + Utility::to_string(rest_size));
        char *pos2 = Utility::strnstr(pos, CRLF2, rest_size);
        if(pos2 == NULL){
            MYINFO("not found \\r\\n\\r\\n");
            //*data = pos;
            return (-1);
        }
        DEBUG("search_file() boundary No.4:" + boundary);
        pos2[0] = '\0';
        Split sp(pos, CRLF, false, true);
        Header header = Header::from_splited_data(sp, 0);
        std::string const &disposition = header.find(CONTENT_DISPOSITION);
        if(disposition == header.not_find()){
            ERROR(" Invalid format: not find Content-Disposition in body");
            throw HttpException("400");
        }
        DEBUG("search_file() boundary No.5:" + boundary);
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
        DEBUG("search_file() boundary No.6:" + boundary);

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
        DEBUG("search_file() boundary No.7 filepath:" + filepath);

        WebservFileFactory *file_factory = WebservFileFactory::get_instance();
        WebservFile *normal_file = file_factory->make_normal_file(file->fd(), filepath, mode);
        DEBUG("search_file() boundary No.8:" + boundary);
        //if(normal_file->can_read()){
        //DEBUG("search_file() boundary No.9:" + boundary);
            try{
                normal_file->open();
                file->register_file(normal_file, 200);
            }catch(std::runtime_error &e){
                file->register_file(normal_file, 500);
            }
        //}else{
        //DEBUG("search_file() boundary No.10:" + boundary);
            //file->register_file(normal_file, 500);
        //}
        DEBUG("search_file() boundary No.11:" + boundary);
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
        // +4 is CRLF and "--"
        while(size >= boundary_size + 2){
            MYINFO("MultiFileFunc write() No.1 size=" + Utility::to_string(size));
            if(file->exist_file()){
                write_size = write_file(file, data, size, boundary);
            }else{
                write_size = search_file(file, data, size, directory, boundary);
            }
            MYINFO("MultiFileFunc write() No.2 write_size=" + Utility::to_string(write_size));
            if(write_size <= 0){
                break;
            }
            if((int)size != write_size){
                MYINFO("MultiFileFunc write() No.30 read and write diff=" + Utility::to_string(size));
            }
            //if(size < size - write_size){
                //ERROR("overflow");
                //throw HttpException("500");
            //}
            size -= (size_t)write_size;
        }
        //*data = &((*data)[write_size]);
        return (size_bk - size);
    }

    template <class FileT>
    int read_file_data(FileT *file, char **data, size_t size, size_t written_size)
    {
        DEBUG("MultiFileFunc::read_file_data");
        //HttpVersion version(HTTP_VERSION);
        const string xml_str1 = "\t<D:response>\r\n\t\t<D:href>";
        const string xml_str2 = "<D:href>\r\n\t\t<D:status>" + string(HTTP_VERSION) + " ";
        const string xml_str3 = "</D:status>\r\n\t</D:response>\r\n";
        const string xml_str_last = "</D:multistatus>";
        //const size_t xlm_str_size = xml_str1.size() + xml_str2.size() + xml_str3.size() + xml_str_last.size();
        //size_t write_size;
       
        // referring to https://datatracker.ietf.org/doc/html/rfc4918#section-9.10.9
        std::map<WebservFile *, StatusCode>::iterator ite = file->uploaded_files().begin();
        std::map<WebservFile *, StatusCode>::iterator end = file->uploaded_files().end();
        bool end_flag = true;
        while(ite != end){

            WebservFile *file = ite->first;
            StatusCode &code = ite->second;
            const char* message = code.message();
            std::string code_str = code.to_string();
            std::string write_data = xml_str1 + file->path() + xml_str2 + code_str + " " + string(message) + xml_str3;
            DEBUG("MultiFileFunc::read_file_data No.1 write_data:" + write_data);
            DEBUG("MultiFileFunc::read_file_data No.1 write_data size1:" + Utility::to_string(write_data.size()));
            DEBUG("MultiFileFunc::read_file_data No.1 write_data size2:" + Utility::to_string(write_data.size() + xml_str_last.size()));
            DEBUG("MultiFileFunc::read_file_data No.1 size:" + Utility::to_string(size));
            if(write_data.size() + xml_str_last.size() >=size){
            DEBUG("MultiFileFunc::read_file_data No.1-0 break size:" + Utility::to_string(size));
                end_flag = false;
                break;
            }
            DEBUG("MultiFileFunc::read_file_data No.1-1 written_size:" + Utility::to_string(written_size));
            for(size_t i=0;i<write_data.size();i++){
                (*data)[written_size+i] = write_data[i];
            }
            written_size += write_data.size();
            (*data)[written_size] = '\0';
            DEBUG("MultiFileFunc::read_file_data No.1-2 written_size:" + Utility::to_string(written_size));
            //printf("data=[%s]\n", *data);
            //file->uploaded_files.erase(*ite);

            ite++;
        }
            DEBUG("MultiFileFunc::read_file_data No.2");
        if(end_flag){
            DEBUG("MultiFileFunc::read_file_data No.3");
            for(size_t i=0;i<xml_str_last.size();i++){
                (*data)[written_size+i] = xml_str_last[i];
            }
            file->uploaded_files().clear();
        }else{
            DEBUG("MultiFileFunc::read_file_data No.4");
            file->uploaded_files().erase(file->uploaded_files().begin(), ite);
        }
        written_size += xml_str_last.size();
            DEBUG("MultiFileFunc::read_file_data No.5");
        return (written_size);
    }

    template <class FileT>
    int read_result(FileT *file, char **data, size_t size)
    {
        DEBUG("MultiFileFunc::read_result");
        size_t written_size = 0;
        //初回の一度だけステータスコードを返す
        if(file->register_file_flag() == false){
            file->set_register_file_flag(true);
            if(is_ok(file)){
                return (200);
            }
            return (207);
        }
        size_t file_size = file->uploaded_files().size();
        DEBUG("MultiFileFunc::read_result No.1");
        if(file_size > 0){
        DEBUG("MultiFileFunc::read_result No.2");
            if(file_size == file->uploaded_number_of_files()){
        DEBUG("MultiFileFunc::read_result No.3");
                const string start_xml = "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\r\n<D:multistatus xmlns:D=\"DAV:\">\r\n";
                if(start_xml.size() > size){
        DEBUG("MultiFileFunc::read_result No.4");
                    return -1;
                }
        DEBUG("MultiFileFunc::read_result No.5");
                for(size_t i=0;i<start_xml.size();i++){
                    (*data)[i] = start_xml[i];
                }
                size -= start_xml.size();
                written_size = start_xml.size();
            }
        DEBUG("MultiFileFunc::read_result No.6");
            return (read_file_data(file, data, size, written_size));
        }
        return 0;
    }
}

#endif
