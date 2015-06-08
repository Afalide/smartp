
#include "smartp/ref_counter.hpp"

ref_counter::ref_counter()
    :_count(0), _weak_count(0)
{
}

ref_counter::~ref_counter()
{
}

void
ref_counter::add_ref()
{
    ++ _count;
}

void
ref_counter::remove_ref()
{
    -- _count;
}

void
ref_counter::add_weak_ref()
{
    ++ _weak_count;
}

void
ref_counter::remove_weak_ref()
{
    -- _weak_count;
}
