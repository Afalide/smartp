
#ifndef SMARTP_WEAKP_HPP
#define SMARTP_WEAKP_HPP

#include <sstream>
#include "smartp/ref_counter.hpp"
#include "smartp/smartp.hpp"

template <typename T>
class smartp;

template <typename T>
class weakp
{

    friend class tests;

    T* _ptr;
    ref_counter* _counter;

public:

    weakp()
        :_ptr(NULL), _counter(NULL)
    {
        SMARTP_LOG(std::cout << "weakp ctor ()" << std::endl);
        _counter = new ref_counter();
        _counter->add_weak_ref();
        print();
    }

    weakp(const weakp& other)
        :_ptr(other._ptr), _counter(other._counter)
    {
        SMARTP_LOG(std::cout << "weakp ctor (weakp&)" << std::endl);
        _counter->add_weak_ref();
        print();
    }

    weakp(const smartp<T>& sptr)
        :_ptr(sptr._ptr), _counter(sptr._counter)
    {
        SMARTP_LOG(std::cout << "weakp ctor (smartp&)" << std::endl);
        _counter->add_weak_ref();
        print();
    }

    ~weakp()
    {
        SMARTP_LOG(std::cout << "weakp destructor" << std::endl);
        _counter->remove_weak_ref();
        print();
        delete_counter_if_last();
    }

    smartp<T>
    expose()
    {
        if(0 == _counter->_count)
            return smartp<T>(NULL);

        smartp<T> ret(_ptr);
        delete ret._counter;

        ret._counter = this->_counter;
        ret._counter->add_ref();
        return ret;
    }

    void
    print()
    {
        std::cout << "  "  << describe() << std::endl;
    }

    //the weak pointer never deletes the referee ptr.
    //however it may delete the counter, if it's no longer used:
    //do nothing if weak ref != 0
    //do nothing if strong ref != 0
    //delete the counter if weak ref is 0

    void delete_counter_if_last()
    {
        SMARTP_LOG(std::cout << "weakp dcil" << std::endl);
        print();

        if(NULL == _counter)
        {
            SMARTP_LOG(std::cout << "  weak check refused (null counter)" << std::endl);
            return;
        }

        if(0 == _counter->_count)
        {
            if(0 == _counter->_weak_count)
            {
                SMARTP_LOG(std::cout << "  weak check accepted (deleting counter)" << std::endl);
                delete _counter;
                _counter = NULL;
                return;
            }
            else
            {
                SMARTP_LOG(std::cout
                           << "  weak check refused (" << _counter->_weak_count
                           << " weak refs remaining)" << std::endl);
            }
        }
        else
        {
            SMARTP_LOG(std::cout
                       << "  weak check refused (" << _counter->_count
                       << " strong refs remaining)" << std::endl);
        }
    }

    std::string
    describe()
    {
        std::stringstream str;
        str << "weakp at " << this << " refs " << _ptr
            << " [" << _counter->_count << " sr] ["
            << _counter->_weak_count << " wr]";
        return str.str();
    }

};

#endif // SMARTP_WEAKP_HPP

