#ifndef MYFILE_HPP
#define MYFILE_HPP
#include <unistd.h>
#include <string>
#include <iostream>


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
        //virtual File *make() const = 0;
        virtual int open() = 0;
        virtual int read(char **data, size_t size) = 0;
        virtual int write(char **data, size_t size) = 0;
        virtual int close() = 0;
        virtual int remove() = 0;
        virtual bool can_read() = 0;
        virtual size_t size() = 0;
        virtual bool is_chunk() = 0;
        virtual std::string const &path() = 0;
};

template<typename FilePointer, typename OpenStrategyPointer, typename ReadStrategyPointer, typename WriteStrategyPointer, typename CloseStrategyPointer, typename RemoveStrategyPointer, typename CanReadStrategyPointer, typename PathStrategyPointer, typename SizeStrategyPointer, typename IsChunkStrategyPointer>
class OwningFileModel : public FileConcept
{
    public:
        OwningFileModel(FilePointer *file, OpenStrategyPointer open, ReadStrategyPointer read, WriteStrategyPointer write, CloseStrategyPointer close, RemoveStrategyPointer remove, CanReadStrategyPointer can_read, PathStrategyPointer path, SizeStrategyPointer size, IsChunkStrategyPointer is_chunk) : file_(file), open_(open), read_(read), write_(write), close_(close), remove_(remove), can_read_(can_read), path_(path), size_(size), is_chunk_(is_chunk){};
        //OwningFileModel(FilePointer file, OpenStrategyPointer open, ReadStrategyPointer read, WriteStrategyPointer write, CloseStrategyPointer close) :file_(file), open_(open), read_(read), write_(write), read_(read){};
        //kkOwningFileModel(FilePointer file) :file_(file), open_(NULL), read_(NULL), write_(NULL), read_(NULL){};
        int open()  {return open_(file_);}
        int read(char **data, size_t size)  {return read_(file_, data, size);}
        int write(char **data, size_t size)  {return write_(file_, data, size);}
        int close()  {return close_(file_);}
        int remove()  {return remove_(file_);}
        bool can_read()  {return can_read_(file_);}
        std::string const &path() {return path_(file_);}
        size_t size()  {return size_(file_);}
        bool is_chunk()  {return is_chunk_(file_);}

    private:
        FilePointer *file_;
        OpenStrategyPointer open_;
        ReadStrategyPointer read_;
        WriteStrategyPointer write_;
        CloseStrategyPointer close_;
        RemoveStrategyPointer remove_;
        CanReadStrategyPointer can_read_;
        PathStrategyPointer path_;
        SizeStrategyPointer size_;
        IsChunkStrategyPointer is_chunk_;
};

class WebservFile
{
    public:
        template<typename FilePointer, typename OpenStrategyPointer, typename ReadStrategyPointer, typename WriteStrategyPointer, typename CloseStrategyPointer, typename RemoveStrategyPointer, typename CanReadStrategyPointer, typename PathStrategyPointer, typename SizeStrategyPointer, typename IsChunkStrategyPointer>
        WebservFile(FilePointer *file, OpenStrategyPointer open, ReadStrategyPointer read, WriteStrategyPointer write, CloseStrategyPointer close, RemoveStrategyPointer remove, CanReadStrategyPointer can_read, PathStrategyPointer path, SizeStrategyPointer size, IsChunkStrategyPointer is_chunk)
        {
            typedef OwningFileModel<FilePointer, OpenStrategyPointer, ReadStrategyPointer, WriteStrategyPointer, CloseStrategyPointer, RemoveStrategyPointer, CanReadStrategyPointer, PathStrategyPointer, SizeStrategyPointer, IsChunkStrategyPointer > Model;
            pimpl_ = new Model(file, open, read, write, close, remove, can_read, path, size, is_chunk);
        }


        template<typename FilePointer, typename OpenStrategyPointer, typename ReadStrategyPointer, typename WriteStrategyPointer, typename CloseStrategyPointer, typename RemoveStrategyPointer, typename CanReadStrategyPointer, typename PathStrategyPointer, typename SizeStrategyPointer, typename IsChunkStrategyPointer >
        WebservFile(FilePointer *file)
        {
            typedef OwningFileModel<FilePointer, OpenStrategyPointer, ReadStrategyPointer, WriteStrategyPointer, CloseStrategyPointer, RemoveStrategyPointer, CanReadStrategyPointer, PathStrategyPointer, SizeStrategyPointer, IsChunkStrategyPointer > Model;
            pimpl_ = new Model(file);
        }



        /*
        template<typename FilePointer, typename OpenStrategyPointer, typename ReadStrategyPointer, typename WriteStrategyPointer, typename CloseStrategyPointer, typename RemoveStrategyPointer, typename CanReadStrategyPointer, typename PathStrategyPointer>
        WebservFile(FilePointer *file, OpenStrategyPointer open, ReadStrategyPointer read, WriteStrategyPointer write, CloseStrategyPointer close, )
        {
            typedef OwningFileModel<FilePointer, OpenStrategyPointer, ReadStrategyPointer, WriteStrategyPointer, CloseStrategyPointer, RemoveStrategyPointer, CanReadStrategyPointer> Model;
            pimpl_ = new Model(file, open, read, write, close, default_remove, default_can_read);
        }
        */
        ~WebservFile(){delete pimpl_;};

        int test(){return 1;};
        int open(){return pimpl_->open();};
        int read(char **data, size_t size){return pimpl_->read(data, size);};
        int write(char **data, size_t size){return pimpl_->write(data, size);};
        int close(){return pimpl_->close();};
        int remove(){return pimpl_->remove();};
        bool can_read(){return pimpl_->can_read();};
        std::string const &path(){return pimpl_->path();};
        size_t size(){return pimpl_->size();};
        size_t is_chunk(){
            std::cout << "is_chunk test" << std::endl;

            return pimpl_->is_chunk();
        };
        //int save(char *data, size_t size){return pimpl_->save();}

        /*
        virtual ~File(){};
        virtual int open() = 0;
        virtual int read(char **data, size_t size) = 0;
        virtual int write(char **data, size_t size) = 0;
        virtual int save(char *data, size_t size) = 0;
        virtual int close() = 0;

        virtual bool can_read() = 0;
        virtual int remove() = 0;
        //virtual bool is_chunk() = 0;
        //virtual size_t size() = 0;
        virtual std::string const &path() = 0;
        */
    private:
        FileConcept *pimpl_;
        /*
        friend int open(WebservFile *file)
        {
            return file->pimpl_->open();
        }
        friend int read(WebservFile *file, char **data, size_t size)
        {
            return file->pimpl_->read(data, size);
        }
        friend int write(WebservFile *file, char **data, size_t size)
        {
            return file->pimpl_->write(data, size);
        }
        friend int close(WebservFile *file)
        {
            return file->pimpl_->close();
        }
        friend int remove(WebservFile *file)
        {
            return file->pimpl_->remove();
        }
        friend bool can_read(WebservFile *file)
        {
            return file->pimpl_->can_read();
        }
        friend std::string const &path(WebservFile *file)
        {
            return file->pimpl_->path();
        }
        */
};

#endif
