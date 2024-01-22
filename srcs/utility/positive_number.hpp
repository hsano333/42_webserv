#ifndef POSITIVE_NUMBER_HPP
#define POSITIVE_NUMBER_HPP

class PositiveNumber
{
    public:
        ~PositiveNumber(){};
        virtual int to_int() const = 0;
};
#endif
