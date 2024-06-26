#ifndef MYFILE_HPP
#define MYFILE_HPP
#include <unistd.h>
#include <string>
#include <iostream>
#include "global.hpp"
#include "utility.hpp"


typedef enum E_FileState
{
    FILE_NOT_OPEN,
    FILE_OPEN,
    FILE_READING,
    FILE_WRITING,
    FILE_COMPLETED_READ,
    FILE_CLOSE,
    FILE_ERROR,
}FileState;


class FileConcept
{
    public:
        virtual ~FileConcept(){};
        virtual int open() = 0;
        virtual int read(char **data, size_t size) = 0;
        virtual int write(char **data, size_t size) = 0;
        virtual int close() = 0;
        virtual int remove() = 0;
        virtual bool can_read() = 0;
        virtual bool can_write() = 0;
        virtual size_t size() = 0;
        virtual bool is_chunk() = 0;
        virtual void set_chunk(bool flag) = 0;
        virtual bool completed() = 0;
        virtual void *file_address() = 0;
        virtual void delete_file() = 0;
        virtual std::string const &path() = 0;
};

template<typename FilePointer, typename OpenStrategyPointer, typename ReadStrategyPointer, typename WriteStrategyPointer, typename CloseStrategyPointer, typename RemoveStrategyPointer, typename CanReadStrategyPointer, typename CanWriteStrategyPointer, typename PathStrategyPointer, typename SizeStrategyPointer, typename IsChunkStrategyPointer, typename SetChunkStrategyPointer, typename CompletedStrategyPointer>
class OwningFileModel : public FileConcept
{
    public:
        OwningFileModel(FilePointer *file, OpenStrategyPointer open, ReadStrategyPointer read, WriteStrategyPointer write, CloseStrategyPointer close, RemoveStrategyPointer remove, CanReadStrategyPointer can_read, CanWriteStrategyPointer can_write, PathStrategyPointer path, SizeStrategyPointer size, IsChunkStrategyPointer is_chunk, SetChunkStrategyPointer set_chunk, CompletedStrategyPointer completed) : file_(file), open_(open), read_(read), write_(write), close_(close), remove_(remove), can_read_(can_read), can_write_(can_write), path_(path), size_(size), is_chunk_(is_chunk), set_chunk_(set_chunk), completed_(completed){};
        ~OwningFileModel(){
        };
        int open()  {return open_(file_);}
        int read(char **data, size_t size)  {return read_(file_, data, size);}
        int write(char **data, size_t size)  {return write_(file_, data, size);}
        int close()  {return close_(file_);}
        int remove()  {return remove_(file_);}
        bool can_read()  {return can_read_(file_);}
        bool can_write()  {return can_write_(file_);}
        std::string const &path() {return path_(file_);}
        size_t size()  {return size_(file_);}
        bool is_chunk()  {return is_chunk_(file_);}
        void set_chunk(bool flag)  {set_chunk_(file_, flag);}
        bool completed() {return completed_(file_);}
        void *file_address() {return (file_);}
        void delete_file() {
            DEBUG("OwningFileModel delete file:" + Utility::to_string(file_));
            delete (this->file_);
        }

    private:
        FilePointer *file_;
        OpenStrategyPointer open_;
        ReadStrategyPointer read_;
        WriteStrategyPointer write_;
        CloseStrategyPointer close_;
        RemoveStrategyPointer remove_;
        CanReadStrategyPointer can_read_;
        CanWriteStrategyPointer can_write_;
        PathStrategyPointer path_;
        SizeStrategyPointer size_;
        IsChunkStrategyPointer is_chunk_;
        SetChunkStrategyPointer set_chunk_;
        CompletedStrategyPointer completed_;
};

class WebservFile
{
    public:
        template<typename FilePointer, typename OpenStrategyPointer, typename ReadStrategyPointer, typename WriteStrategyPointer, typename CloseStrategyPointer, typename RemoveStrategyPointer, typename CanReadStrategyPointer, typename CanWriteStrategyPointer, typename PathStrategyPointer, typename SizeStrategyPointer, typename IsChunkStrategyPointer, typename SetChunkStrategyPointer, typename CompletedStrategyPointer>
        WebservFile(FilePointer *file, OpenStrategyPointer open, ReadStrategyPointer read, WriteStrategyPointer write, CloseStrategyPointer close, RemoveStrategyPointer remove, CanReadStrategyPointer can_read, CanWriteStrategyPointer can_write, PathStrategyPointer path, SizeStrategyPointer size, IsChunkStrategyPointer is_chunk, SetChunkStrategyPointer set_chunk, CompletedStrategyPointer completed)
        {
            typedef OwningFileModel<FilePointer, OpenStrategyPointer, ReadStrategyPointer, WriteStrategyPointer, CloseStrategyPointer, RemoveStrategyPointer, CanReadStrategyPointer, CanWriteStrategyPointer, PathStrategyPointer, SizeStrategyPointer, IsChunkStrategyPointer, SetChunkStrategyPointer, CompletedStrategyPointer > Model;
            pimpl_ = new Model(file, open, read, write, close, remove, can_read, can_write, path, size, is_chunk, set_chunk, completed);
            DEBUG("WebserbFile register file:" + Utility::to_string(file));
        }


        template<typename FilePointer, typename OpenStrategyPointer, typename ReadStrategyPointer, typename WriteStrategyPointer, typename CloseStrategyPointer, typename RemoveStrategyPointer, typename CanReadStrategyPointer, typename CanWriteStrategyPointer, typename PathStrategyPointer, typename SizeStrategyPointer, typename IsChunkStrategyPointer, typename SetChunkStrategyPointer , typename CompletedStrategyPointer>
        WebservFile(FilePointer *file)
        {
            typedef OwningFileModel<FilePointer, OpenStrategyPointer, ReadStrategyPointer, WriteStrategyPointer, CloseStrategyPointer, RemoveStrategyPointer, CanReadStrategyPointer, CanWriteStrategyPointer, PathStrategyPointer, SizeStrategyPointer, IsChunkStrategyPointer, SetChunkStrategyPointer, CompletedStrategyPointer > Model;
            pimpl_ = new Model(file);
            DEBUG("WebserbFile register file:" + Utility::to_string(file));
        }

        ~WebservFile(){
            DEBUG("WebservFile delete Model:" + Utility::to_string(pimpl_));
            delete pimpl_;
        };

        int test(){return 1;};
        int open(){return pimpl_->open();};
        int read(char **data, size_t size){return pimpl_->read(data, size);};
        int write(char **data, size_t size){return pimpl_->write(data, size);};
        int close(){return pimpl_->close();};
        int remove(){return pimpl_->remove();};
        bool can_read(){return pimpl_->can_read();};
        bool can_write(){return pimpl_->can_write();};
        std::string const &path(){return pimpl_->path();};
        size_t size(){return pimpl_->size();};
        void set_chunk(bool flag){
            return pimpl_->set_chunk(flag);
        };
        void *file_address(){
            return (void *)(pimpl_->file_address());
        };
        void delete_file(){
            pimpl_->delete_file();
        };
        bool is_chunk(){
            return pimpl_->is_chunk();
        };

        bool completed(){
            return (pimpl_->completed());
        }

    private:
        FileConcept *pimpl_;
};

#endif
