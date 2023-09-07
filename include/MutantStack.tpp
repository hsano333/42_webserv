#ifndef MUTANTSTACK_T
#define MUTANTSTACK_T

#include "MutantStack.hpp"
#include <algorithm>
#include <typeinfo>
#include <iomanip>

using std::cout;
using std::endl;

template <typename T, class Iter, class Inherit>
std::ostream& operator<<(std::ostream& os, const MutantStack<T, Iter, Inherit>& f);
template <typename T, class Iter, class Inherit>
std::ostream& operator<<(std::ostream& os, MutantStack<T, Iter, Inherit>& f);

template <typename T, class Iter, class Inherit>
MutantStack<T, Iter, Inherit>::MutantStack()
{
}

template <typename T, class Iter, class Inherit>
MutantStack<T, Iter, Inherit>::~MutantStack()
{
}

template <typename T, class Iter, class Inherit>
void MutantStack<T, Iter, Inherit>::copys(MutantStack<T, std::deque<T>, std::stack<T> > &dst, MutantStack<T, std::deque<T>, std::stack<T> > const &src)
{
    for (size_t i=0; i<src.size(); i++){
        dst.push(src.c.at(i));
    }
}

template <typename T, class Iter, class Inherit>
void MutantStack<T, Iter, Inherit>::copys(MutantStack<T, Iter, Iter> &dst, MutantStack<T, Iter, Iter> const &src)
{
    if (dst == src)
        return ;
    for (size_t i=0; i<src.size(); i++){
        this->push_back(src[i]);
    }
}

template <typename T, class Iter, class Inherit>
void MutantStack<T, Iter, Inherit>::clear(MutantStack<T, std::deque<T>, std::stack<T> > &obj)
{
    while (!(obj.empty()))
    {
        obj.pop();
    }
}

template <typename T, class Iter, class Inherit>
void MutantStack<T, Iter, Inherit>::clear(MutantStack<T, Iter, Iter> &obj)
{
    obj.clear();
}

template <typename T, class Iter, class Inherit>
MutantStack<T, Iter, Inherit>::MutantStack(const MutantStack<T, Iter, Inherit> &stack) : Inherit()
{
    this->copys(*this, stack);
}

template <typename T, class Iter, class Inherit>
MutantStack<T, Iter, Inherit>& MutantStack<T, Iter, Inherit>::operator=(const MutantStack<T, Iter, Inherit> &stack)
{
    if (*this != stack)
    {
        this->clear(*this);
        this->copys(*this, stack);
    }
    return (*this);
}

template <typename T, class Iter, class Inherit>
template <template <typename, typename> class Container>
void MutantStack<T, Iter, Inherit>::Test(Container<T, Iter> &obj)
{
    cout << "Test():" << &obj << endl;
}


template <typename T, class Iter, class Inherit>
template <template <typename> class Container>
void MutantStack<T, Iter, Inherit>::Test(Container<T> &obj)
{
    cout << "Test():" << &obj << endl;
}


template <typename T, class Iter, class Inherit>
template < template <typename, typename, typename> class Container>
typename MutantStack<T, Iter, Inherit>::iterator MutantStack<T, Iter, Inherit>::getBeginIte(Container<T, std::deque<T>, std::stack<T> > &obj)
{
    if (&obj == this)
        return (this->c.begin());
    return (this->c.begin());
}

template <typename T, class Iter, class Inherit>
template < template <typename, typename, typename> class Container>
typename MutantStack<T, Iter, Inherit>::iterator MutantStack<T, Iter, Inherit>::getBeginIte(Container<T, Iter, Iter> &obj)
{
    if (&obj == this)
        return Iter::begin();
    return Iter::begin();
}

template <typename T, class Iter, class Inherit>
template < template <typename, typename, typename> class Container>
typename MutantStack<T, Iter, Inherit>::iterator MutantStack<T, Iter, Inherit>::getEndIte(Container<T, std::deque<T>, std::stack<T> > &obj)
{
    if (&obj == this)
        return (this->c.end());
    return (this->c.end());
}
template <typename T, class Iter, class Inherit>
template < template <typename, typename, typename> class Container>
typename MutantStack<T, Iter, Inherit>::iterator MutantStack<T, Iter, Inherit>::getEndIte(Container<T, Iter, Iter> &obj)
{
    if (&obj == this)
        return Iter::end();
    return Iter::end();
}

template <typename T, class Iter, class Inherit>
template < template <typename, typename, typename> class Container>
typename MutantStack<T, Iter, Inherit>::const_iterator MutantStack<T, Iter, Inherit>::getBeginIte(Container<T, std::deque<T>, std::stack<T> > &obj) const
{
    if (&obj == this)
        return (this->c.begin());
    return (this->c.begin());
}

template <typename T, class Iter, class Inherit>
template < template <typename, typename, typename> class Container>
typename MutantStack<T, Iter, Inherit>::const_iterator MutantStack<T, Iter, Inherit>::getBeginIte(Container<T, Iter, Iter> &obj) const
{
    if (&obj == this)
        return Iter::begin();
    return Iter::begin();
}

template <typename T, class Iter, class Inherit>
template < template <typename, typename, typename> class Container>
typename MutantStack<T, Iter, Inherit>::const_iterator MutantStack<T, Iter, Inherit>::getEndIte(Container<T, std::deque<T>, std::stack<T> > &obj) const
{
    if (&obj == this)
        return (this->c.end());
    return (this->c.end());
}
template <typename T, class Iter, class Inherit>
template < template <typename, typename, typename> class Container>
typename MutantStack<T, Iter, Inherit>::const_iterator MutantStack<T, Iter, Inherit>::getEndIte(Container<T, Iter, Iter> &obj) const
{
    if (&obj == this)
        return Iter::end();
    return Iter::end();
}

template <typename T, class Iter, class Inherit>
template < template <typename, typename, typename> class Container>
typename MutantStack<T, Iter, Inherit>::reverse_iterator MutantStack<T, Iter, Inherit>::getRBeginIte(Container<T, std::deque<T>, std::stack<T> > &obj)
{
    if (&obj == this)
        return (this->c.rbegin());
    return (this->c.rbegin());
}

template <typename T, class Iter, class Inherit>
template < template <typename, typename, typename> class Container>
typename MutantStack<T, Iter, Inherit>::reverse_iterator MutantStack<T, Iter, Inherit>::getRBeginIte(Container<T, Iter, Iter> &obj)
{
    if (&obj == this)
        return Iter::rbegin();
    return Iter::rbegin();
}

template <typename T, class Iter, class Inherit>
template < template <typename, typename, typename> class Container>
typename MutantStack<T, Iter, Inherit>::reverse_iterator MutantStack<T, Iter, Inherit>::getREndIte(Container<T, std::deque<T>, std::stack<T> > &obj)
{
    if (&obj == this)
        return (this->c.rend());
    return (this->c.rend());
}
template <typename T, class Iter, class Inherit>
template < template <typename, typename, typename> class Container>
typename MutantStack<T, Iter, Inherit>::reverse_iterator MutantStack<T, Iter, Inherit>::getREndIte(Container<T, Iter, Iter> &obj)
{
    if (&obj == this)
        return Iter::rend();
    return Iter::rend();
}

template <typename T, class Iter, class Inherit>
template < template <typename, typename, typename> class Container>
typename MutantStack<T, Iter, Inherit>::const_reverse_iterator MutantStack<T, Iter, Inherit>::getRBeginIte(Container<T, std::deque<T>, std::stack<T> > &obj) const
{
    if (&obj == this)
        return (this->c.rbegin());
    return (this->c.rbegin());
}

template <typename T, class Iter, class Inherit>
template < template <typename, typename, typename> class Container>
typename MutantStack<T, Iter, Inherit>::const_reverse_iterator MutantStack<T, Iter, Inherit>::getRBeginIte(Container<T, Iter, Iter> &obj) const
{
    if (&obj == this)
        return Iter::rbegin();
    return Iter::rbegin();
}

template <typename T, class Iter, class Inherit>
template < template <typename, typename, typename> class Container>
typename MutantStack<T, Iter, Inherit>::const_reverse_iterator MutantStack<T, Iter, Inherit>::getREndIte(Container<T, std::deque<T>, std::stack<T> > &obj) const
{
    if (&obj == this)
        return (this->c.rend());
    return (this->c.rend());
}
template <typename T, class Iter, class Inherit>
template < template <typename, typename, typename> class Container>
typename MutantStack<T, Iter, Inherit>::const_reverse_iterator MutantStack<T, Iter, Inherit>::getREndIte(Container<T, Iter, Iter> &obj) const
{
    if (&obj == this)
        return Iter::rend();
    return Iter::rend();
}




template <typename T, class Iter, class Inherit>
typename MutantStack<T, Iter, Inherit>::iterator MutantStack<T, Iter, Inherit>::begin()
{
    return (getBeginIte(*this));
}

template <typename T, class Iter, class Inherit>
typename MutantStack<T, Iter, Inherit>::iterator MutantStack<T, Iter, Inherit>::end()
{
    return (getEndIte(*this));
}

template <typename T, class Iter, class Inherit>
typename MutantStack<T, Iter, Inherit>::const_iterator MutantStack<T, Iter, Inherit>::begin() const
{
    return (getBeginIte(*this));
}

template <typename T, class Iter, class Inherit>
typename MutantStack<T, Iter, Inherit>::const_iterator MutantStack<T, Iter, Inherit>::end() const
{
    return (getEndIte(*this));
}

template <typename T, class Iter, class Inherit>
typename MutantStack<T, Iter, Inherit>::reverse_iterator MutantStack<T, Iter, Inherit>::rbegin()
{
    return (getRBeginIte(*this));
}

template <typename T, class Iter, class Inherit>
typename MutantStack<T, Iter, Inherit>::reverse_iterator MutantStack<T, Iter, Inherit>::rend()
{
    return (getREndIte(*this));
}

template <typename T, class Iter, class Inherit>
typename MutantStack<T, Iter, Inherit>::const_reverse_iterator MutantStack<T, Iter, Inherit>::rbegin() const
{
    return (getRBeginIte(*this));
}

template <typename T, class Iter, class Inherit>
typename MutantStack<T, Iter, Inherit>::const_reverse_iterator MutantStack<T, Iter, Inherit>::rend() const
{
    return (getREndIte(*this));
}

template <typename T, class Iter, class Inherit>
std::ostream& operator<<(std::ostream& os, const MutantStack<T, Iter, Inherit>& stack)
{
    os << stack;
    return (os);
}
template <typename T, class Iter, class Inherit>
std::ostream& operator<<(std::ostream& os, MutantStack<T, Iter, Inherit>& stack)
{
    os << stack;
    return (os);
}

template <typename T, class Iter, class Inherit>
std::ostream& operator<<(std::ostream& os, const typename MutantStack<T, Iter, Inherit>::iterator& ite)
{
    os << ite;
    return (os);
}
template <typename T, class Iter, class Inherit>
std::ostream& operator<<(std::ostream& os, typename MutantStack<T, Iter, Inherit>::iterator& ite)
{
    os << ite;
    return (os);
}

#endif
