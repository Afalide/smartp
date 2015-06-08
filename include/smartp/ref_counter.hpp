
#ifndef SMARTP_REF_COUNTER_H
#define SMARTP_REF_COUNTER_H

#include <iostream>
#include "smartp/commons.hpp"

class SMARTP_API ref_counter
{
    template <typename T> friend class smartp;
    template <typename T> friend class weakp;
    friend class tests;

    int _count;
    int _weak_count;

public:

    ref_counter();
    ~ref_counter();

    void add_ref();
    void remove_ref();
    void add_weak_ref();
    void remove_weak_ref();
};

#endif // SMARTP_REF_COUNTER_H

