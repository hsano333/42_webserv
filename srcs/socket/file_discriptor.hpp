#ifndef FILE_DISCRIPTOR_HPP
#define FILE_DISCRIPTOR_HPP
#include <iostream>
#include <fstream>
#include "utility.hpp"

class FileDiscriptor
{
    public:
        FileDiscriptor();
        ~FileDiscriptor();
        static FileDiscriptor from_int(int fd);
        FileDiscriptor(FileDiscriptor const &fd);
        FileDiscriptor& operator=(FileDiscriptor const &fd);

        bool operator==(const FileDiscriptor &fd) const;
        //bool operator==(FileDiscriptor &fd);
        //bool operator==(FileDiscriptor const &fd) const;
        //bool operator==(FileDiscriptor const &fd);
        bool operator>=(FileDiscriptor const &fd) const;
        bool operator>(FileDiscriptor const &fd) const;
        bool operator<=(FileDiscriptor const &fd) const;
        bool operator<(FileDiscriptor const &fd) const;

        bool operator==(int fd) const;
        bool operator==(int fd);
        bool operator>=(int fd) const;
        bool operator>(int fd) const;
        bool operator<=(int fd) const;
        bool operator<(int fd) const;


        void close() const;
        int to_int() const;
        int to_int();
        std::string const to_string();
        //int operator=(FileDiscriptor const &fd);
        //FileDiscriptor& operator=(int);
        
        int ttt;
    private:
        FileDiscriptor(int fd);
        int fd;


};

//std::ostream& operator<<(std::ostream& os, const FileDiscriptor &fd);
std::ostream& operator<<(std::ostream& os, const FileDiscriptor &fd);

#endif
