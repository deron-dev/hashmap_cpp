#pragma once

template <typename K, typename V, typename M>
class entry
{
    public:

    K key;
    V value;
    M meta;

    entry() {}

    entry( K newkey, V newval )
    {
        key = newkey;
        value = newval;
    }

    entry( K newkey, V newval, M newmeta )
    {
        key = newkey;
        value = newval;
        meta = newmeta;
    }

    void operator=( const entry& wentry )
    {
        key = wentry.key;
        value = wentry.value;
        meta = wentry.meta;
        // return *wentry;
        // TODO: ask about the above line
    }
};
