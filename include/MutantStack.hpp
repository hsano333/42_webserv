#ifndef MUTANTSTACK_H
#define MUTANTSTACK_H

#include <stack>
#include <list>
#include <iterator>
#include <iostream>
#include <climits>

template <typename T, class Iter = std::deque<T>, class Inherit = std::stack<T> >
//template <typename T, class Iter = std::list<T>, class Inherit = std::list<T> >
class MutantStack : public Inherit 
{
    private:
        void copys(MutantStack<T, std::deque<T>, std::stack<T> > &dst, MutantStack<T, std::deque<T>, std::stack<T> > const &src);
        void copys(MutantStack<T, Iter, Iter> &dst, MutantStack<T, Iter, Iter> const &src);

        void clear(MutantStack<T, std::deque<T>, std::stack<T> > &dst);
        void clear(MutantStack<T, Iter, Iter> &dst);

    public:
        MutantStack();
        MutantStack(const MutantStack &stack);
        MutantStack& operator=(const MutantStack &stack);
        virtual ~MutantStack();

        typedef typename Iter::iterator iterator;
        typedef typename Iter::const_iterator const_iterator;
        typedef typename Iter::reverse_iterator reverse_iterator;
        typedef typename Iter::const_reverse_iterator const_reverse_iterator;


        template < template <typename, typename, typename> class Container>
        typename Iter::iterator getBeginIte(Container<T, std::deque<T>, std::stack<T> > &obj);

        template < template <typename, typename, typename> class Container>
        typename Iter::iterator getEndIte(Container<T, std::deque<T>, std::stack<T> > &obj);

        template < template <typename, typename, typename> class Container>
        typename Iter::iterator getBeginIte(Container<T, Iter, Iter> &obj);

        template < template <typename, typename, typename> class Container>
        typename Iter::iterator getEndIte(Container<T, Iter, Iter> &obj);


        template < template <typename, typename, typename> class Container>
        typename Iter::const_iterator getBeginIte(Container<T, std::deque<T>, std::stack<T> > &obj) const;

        template < template <typename, typename, typename> class Container>
        typename Iter::const_iterator getEndIte(Container<T, std::deque<T>, std::stack<T> > &obj) const;

        template < template <typename, typename, typename> class Container>
        typename Iter::const_iterator getBeginIte(Container<T, Iter, Iter> &obj) const;

        template < template <typename, typename, typename> class Container>
        typename Iter::const_iterator getEndIte(Container<T, Iter, Iter> &obj) const;


        template < template <typename, typename, typename> class Container>
        typename Iter::reverse_iterator getRBeginIte(Container<T, std::deque<T>, std::stack<T> > &obj);

        template < template <typename, typename, typename> class Container>
        typename Iter::reverse_iterator getREndIte(Container<T, std::deque<T>, std::stack<T> > &obj);

        template < template <typename, typename, typename> class Container>
        typename Iter::reverse_iterator getRBeginIte(Container<T, Iter, Iter> &obj);

        template < template <typename, typename, typename> class Container>
        typename Iter::reverse_iterator getREndIte(Container<T, Iter, Iter> &obj);


        template < template <typename, typename, typename> class Container>
        typename Iter::const_reverse_iterator getRBeginIte(Container<T, std::deque<T>, std::stack<T> > &obj) const;

        template < template <typename, typename, typename> class Container>
        typename Iter::const_reverse_iterator getREndIte(Container<T, std::deque<T>, std::stack<T> > &obj) const;

        template < template <typename, typename, typename> class Container>
        typename Iter::const_reverse_iterator getRBeginIte(Container<T, Iter, Iter> &obj) const;

        template < template <typename, typename, typename> class Container>
        typename Iter::const_reverse_iterator getREndIte(Container<T, Iter, Iter> &obj) const;

        template <template <typename, typename> class Container>
        void Test(Container<T, Iter > &obj);
        template <template <typename> class Container>
        void Test(Container<T> &obj);

        iterator begin(void);
        iterator end(void);
        const_iterator begin(void) const;
        const_iterator end(void) const;
        reverse_iterator rbegin(void);
        reverse_iterator rend(void);
        const_reverse_iterator rbegin(void) const;
        const_reverse_iterator rend(void) const;
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const MutantStack<T>& stack);
template <typename T>
std::ostream& operator<<(std::ostream& os, MutantStack<T>& stack);

#include "MutantStack.tpp"
#endif
