
#ifndef SMARTP_SMARTP_HPP
#define SMARTP_SMARTP_HPP

#include <sstream>
#include "smartp/commons.hpp"
#include "smartp/ref_counter.hpp"
#include "smartp/weakp.hpp"

template <typename T>
class smartp
{

    friend class tests;
    friend class weakp<T>;

    T* _ptr;
    ref_counter* _counter;

public:

    //empty ctor

    smartp() 
        :_ptr(NULL) , _counter(NULL)
    {
        SMARTP_LOG(std::cout << "smartp() " << std::endl);

        //std::cout << "smartp() " << std::endl;

        _counter = new ref_counter();
        _counter->add_ref();


        SMARTP_LOG(std::cout << "  " << describe() << std::endl);
    }

    //init ctor

    smartp(T* ptr)
        :_ptr(ptr), _counter(NULL)
    {
        SMARTP_LOG(std::cout << "smartp(T* ptr) " << std::endl);

        _counter = new ref_counter();
        _counter->add_ref();

        SMARTP_LOG(std::cout << "  " << describe() << std::endl);
    }

    //copy ctor

    smartp(const smartp<T>& other)
        :_ptr(other._ptr), _counter(other._counter)
    {
        SMARTP_LOG(std::cout << "smartp(smartp&) " << std::endl);

        _counter->add_ref();

        SMARTP_LOG(std::cout << "  " << describe() << std::endl);
    }

    //destructor

    ~smartp()
    {
        SMARTP_LOG(std::cout << "smartp destructor " << std::endl);
        SMARTP_LOG(std::cout << "  " << describe() << std::endl);

        //remove a ref

        _counter->remove_ref();

        //delete the ptr if necessary

        delete_if_last();
    }

    //operator =

    smartp<T>& operator=(const smartp<T>& other)
    {
        SMARTP_LOG(std::cout << "smartp operator= old:" << _ptr << " new:" << other._ptr << std::endl);
        SMARTP_LOG(std::cout << "  " << describe() << std::endl);

        //ignore self affect

        if(this == &other)
            return *this;

        //remove a count, since after this call we loose
        //that current ref

        _counter->remove_ref();
        delete_if_last();

        //assign from other

        _counter = other._counter;
        _counter->add_ref();
        _ptr = other._ptr;

        return *this;
    }

    //operator ->

    T* operator->()
    {
        return _ptr;
    }

    void delete_if_last()
    {
        SMARTP_LOG(std::cout << "smartp dil start (sp at " << this << ")" << std::endl);
        SMARTP_LOG(std::cout << "  " << describe() << std::endl);
        delete_if_last_impl();
        SMARTP_LOG(std::cout << "smartp dil end   (sp at " << this << ")" << std::endl);
    }

    //cleanup:
    //delete the pointer if it has 0 strong refs
    //delete the counter if it has 0 strong refs and 0 weak refs

    void delete_if_last_impl()
    {
        if(0 != _counter->_count)
        {
            SMARTP_LOG(std::cout
                       << "    strong check refused (" << _counter->_count
                       << " strong refs remaining)" << std::endl
                       << "    weak check skipped (strong refs remaining)" << std::endl);
            return;
        }

        if(NULL == _ptr)
        {
            SMARTP_LOG(std::cout << "    strong check refused (ptr referee is null)" << std::endl);
        }
        else
        {
            SMARTP_LOG(std::cout << "    strong check accepted (deleting ptr referee)" << std::endl);
            delete _ptr;
            _ptr = NULL;
        }

        if(0 != _counter->_weak_count)
        {
            SMARTP_LOG(std::cout
                       << "    weak check refused (" << _counter->_weak_count
                       << " weak refs remaining) " << std::endl);
            return;
        }

        SMARTP_LOG(std::cout << "    weak check accepted (deleting counter) " << std::endl);
        delete _counter;
        _counter = NULL;
    }

    //stop watching the referee ptr:
    //- it won't be deleted when this smartpointer exits the scope.
    //- it becomes a whole new empty smart pointer.
    //- others smartp and weakp refs will see the counter decrease
    //  accordingly.

    T*
        release()
    {
        //*this = smartp<T>();
        T* ret = _ptr;


        _ptr = NULL;
        _counter->remove_ref();

        //if this was the last smartp, also delete the old counter
        //the user is responsible of the referee ptr deletion
        if(0 == _counter->_count && 0 == _counter->_weak_count)
            delete _counter;

        //this smartp will persist, so use a new and separate
        //ref counter
        _counter = new ref_counter();
        _counter->add_ref();
        return ret;
    }

    std::string
        describe()
    {
        std::stringstream str;
        str << "smartp at " << this << " refs " << _ptr
            << " [" << _counter->_count << " sr] ["
            << _counter->_weak_count << " wr]";
        return str.str();
    }

    //global comparison operators

    //compare directly a pointer and a smartpointer

    friend
        bool operator== (void* left, const smartp<T>& right)
    {
        return left == right._ptr;
    }


    friend
        bool operator== (const smartp<T>& left, void* right)
    {
        return left._ptr == right;
    }

    //compare 2 smartpointers

    friend
        bool operator== (smartp<T>& left, const smartp<T>& right)
    {
        return left._ptr == right._ptr;
    }
};



#endif // SMARTP_SMARTP_HPP








