#pragma once

template <typename T>
class array
{
    public:

    T* data;
    unsigned int capacity;
    unsigned int count;

    array()
    {
        // initialize empty array of capacity 10
        capacity = 10;
        count = 0;
        data = (T*) malloc( sizeof(T) * capacity );
    }

    array( unsigned int initial_capacity )
    {
        // intialize empty array of capacity initial_capacity
        capacity = initial_capacity;
        count = 0;
        data = (T*) malloc( sizeof(T) * capacity );
    }

    // TODO: verify
    void operator=( const array& warray )
    {
        capacity = warray.capacity;
        count    = warray.count;
        data     = warray.copy_data();
    }

    T& operator[](unsigned int index)
    {
        // allow usage of dynarr[index] format
        if ( index < capacity && index >= 0 )
        {
            return data[index];
        }
        throw std::out_of_range("array::[] | Index out of range.");
    }

    void reserve( unsigned int newcapacity )
    {
        /* while capacity < newcapacity, double capacity;
           reallocate array with updated capacity */
        bool resize = false;
        while ( capacity < newcapacity )
        {
            capacity *= 2;
            resize = true;
        }
        if ( resize )
        {
            data = (T*) realloc( data, sizeof(T) * capacity );
        }
    }

    void push( T value )
    {
        // append value to array (at count)
        reserve( count + 1 );
        data[count] = value; // TODO: need entry = operator overload?
        count++;
    }

    T pop()
    {
        // remove and return ("pop") last element of array
        if ( count != 0 )
        {
            return data[count--];
        }
        else
        {
            throw std::runtime_error("cannot use array::pop() ",
                                      "on empty array");
        }
    }

    void fill( T dataval, unsigned int start = 0 )
    {
        // fill array from data[start] up to data[capacity] w/ dataval
        // WARNING: use of this function is inconsistent with the use of
        //          the "count" data member
        for ( int lcv = start; lcv < capacity; lcv++ )
        {
            data[lcv] = dataval;
        }
    }

    void fill( T dataval, unsigned int start, unsigned int end )
    {
        // fill array from data[start] up to data[end] with dataval
        for ( int lcv = start; lcv < end; lcv++ )
        {
            data[lcv] = dataval;
        }
    }

    // TODO: verify
    T* dump()
    {
        T* bucket = (T*) malloc( sizeof(T) * count );
        for ( int lcv = 0; lcv < count; lcv++ )
        {
            bucket[lcv] = pop();
        }
        return bucket;
    }

    ~array()
    {
        free(data);
    }

    private:

    // TODO: verify
    // not sure why I need "const" here for this to work properly
    T* copy_data() const
    {
        T* newdata = (T*) malloc( sizeof(T) * capacity );
        for ( int lcv = 0; lcv < count; lcv++ )
        {
            newdata[lcv] = data[lcv];
        }
        return newdata;
    }
};
