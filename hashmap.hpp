
// TODO: delete mapping
// TODO: rehash
// TODO: allow more/any data type keys

#pragma once

#ifndef HASHMAP_H
#define HASHMAP_H

#define INITIAL_HASH_DIST_WIDTH 10
#define TRANSLATION_NULL_VAL -1
#define ENTRY_META_TERMINATOR -1
#define LOAD_FACTOR_THRESHOLD 0.1 // TODO: change to higher
#define REHASH_DIST_WIDTH_INC_FACTOR 4

#include "dynamic_array.hpp"
#include "entry.hpp"
#include "djk33.hpp"

// initial hash distribution width
// the default initial capacity of the translation array


// this implementation of a hashmap currently only works with string-type keys
template <typename K, typename V>
class hashmap
{
    public:

    array<entry<K,V,int>> entries;
    array<int> translation;
    long double load_factor;

    hashmap()
    {
        entries = array<entry<K,V,int>>();
        translation = array<int>(INITIAL_HASH_DIST_WIDTH);
        translation.fill(TRANSLATION_NULL_VAL);
        load_factor = 0;
    }

    hashmap( unsigned int initial_capacity,
             unsigned int distribution_width = INITIAL_HASH_DIST_WIDTH )
    {
        entries = array<entry<K,V,int>>(initial_capacity);
        translation = array<int>(distribution_width);
        translation.fill(TRANSLATION_NULL_VAL);
        load_factor = 0;
    }

    unsigned int hash( std::string wkey )
    {
        // hash key, apply distribution width, return
        return djk33( wkey ) % translation.capacity;
    }

    bool key_exists( std::string lookup_key )
    {
        try
        {
            locate_entry( lookup_key );
            return true;
        }
        catch ( std::runtime_error )
        {
            return false;
        }
    }

    void load_factor_eval()
    {
        update_load_factor();
        if ( load_factor > LOAD_FACTOR_THRESHOLD )
        {
            rehash();
        }
    }

    void rehash()
    {
        // save entry count
        unsigned int e_ct = entries.count;

        // make a copy of entries
        entry<std::string,V,int>* bucket = entries.dump();

        // multiply distribution width
        translation.reserve( translation.capacity * REHASH_DIST_WIDTH_INC_FACTOR );

        // clear translation, entries, and entry count
        entries.count = 0;
        translation.fill(TRANSLATION_NULL_VAL);

        // push entries onto expanded distribution
        for ( unsigned int lcv = 0; lcv < e_ct; lcv++ )
        {
            push( bucket[lcv] );
        }
    }

    entry<K,V,int>* locate_entry( std::string lookup_key )
    {
        // ptr to traverse entries
        entry<std::string,V,int>* eptr = NULL;

        // hash lookup_key to determine the translation index
        unsigned int t_index = hash( lookup_key ); // translation index
        int translation_value = translation[t_index];

        // translation does not point to an entry -- no entry for lookup_key
        if ( translation_value == TRANSLATION_NULL_VAL )
        {
            throw std::runtime_error
            (
                "hashmap::locate_entry" \
                "||| no translation for key '" + lookup_key + "'"
            );
        }
        // translation key points to an entry
        else
        {
            // parse through entries
            eptr = &entries[translation_value];

            // while current entry key is nonmatching and the entry meta does
            //      not indicate being the last entry w/ the t_index translation
            while ( eptr->key != lookup_key && eptr->meta != TRANSLATION_NULL_VAL )
            {
                eptr = &entries[eptr->meta];
            }
            if ( eptr->key == lookup_key )
            {
                return eptr;
            }
            else
            {
                throw std::runtime_error
                (
                    "key '" + lookup_key + "' does not exist in map"
                );
            }
        }
    }

    V& operator[]( std::string lookup_key )
    {
        return locate_entry( lookup_key )->value;
    }

    void push( entry<std::string,V,int> newentry )
    {

        int* translation_value;
        entry<std::string,V,int>* eptr;

        if ( key_exists(newentry.key) )
        {
            throw std::runtime_error
            (
                "key \"" + newentry.key + "\" already exists;"
            );
        }
        else
        {
            // get translation
            unsigned int t_index = hash( newentry.key );
            translation_value = &translation[t_index];

            // update new entry's meta value to terminator
            // newentry.meta = ENTRY_META_TERMINATOR; // TODO: delete?
            newentry.meta = ENTRY_META_TERMINATOR;

            // push newentry
            entries.push( newentry );

            if ( *translation_value != TRANSLATION_NULL_VAL )
            {
                // point to last entry added to current translation
                eptr = &entries[*translation_value];

                while ( eptr->meta != ENTRY_META_TERMINATOR )
                {
                    eptr = &entries[eptr->meta];
                }

                // update the formerly last entry of the translation
                //      to point at the new entry's index
                eptr->meta = entries.count - 1;
            }
            else
            {
                // update translation
                *translation_value = entries.count - 1;
            }

            int trash = 0; // TODO: remove testing
        }
        // load_factor_eval();
    }

    void unmap( std::string lookup_key )
    {
        entry<std::string,V,int>* e_unmap = locate_entry( lookup_key );
        entry<std::string,V,int>* eptr = NULL;
        unsigned int t_index;
        int* t_val_ptr;

        if ( e_unmap != NULL )
        {
            // determine translation record index
            t_index = hash(lookup_key);
            t_val_ptr = &translation[t_index];

            eptr = &entries[*t_val_ptr];

            // e_unmap is only entry
            if ( e_unmap->key == eptr->key && e_unmap->meta == ENTRY_META_TERMINATOR )
            {
                *t_val_ptr = TRANSLATION_NULL_VAL;
            }
            // e_unmap is first entry from translation
            //      i.e., the translation points at it
            //      and there are further entries
            else if ( e_unmap->key == eptr->key && e_unmap->meta != ENTRY_META_TERMINATOR )
            {
                *t_val_ptr = e_unmap->meta;
            }
            // e_unmap has entries on either side of it
            //      in the translation string of entries.
            //      or it is the last entry in the string
            else
            {
                while ( entries[eptr->meta].key != e_unmap->key )
                {
                    eptr = &entries[eptr->meta];
                }

                eptr->meta = e_unmap->meta;
            }

        }
        // key does not exist
        else
        {
            throw std::runtime_error
            (
                "key '" + lookup_key + "' does not exist in map"
            );
        }
    }

    void update_load_factor()
    {
        load_factor = (long double) entries.count /
                      (long double) translation.capacity;
    }

    void visualize()
    {
        entry<K,V,int>* eptr;
        int t_val;
        unsigned int t_index;
        int e_index;
        bool cont = true;

        for ( t_index = 0; t_index < translation.capacity; t_index++ )
        {

            t_val = translation[t_index];

            std::cout << t_index << ": ";

            if ( t_val != TRANSLATION_NULL_VAL )
            {
                std::cout << t_val << std::endl;

                eptr = &entries[t_val];

                while ( eptr->meta != ENTRY_META_TERMINATOR )
                {
                    std::cout << "- " << eptr->key << " --> " << eptr->value << std::endl;
                    eptr = &entries[eptr->meta];
                }

                std::cout << "- " << eptr->key << " --> " << eptr->value << std::endl;

            }
            else
            {
                std::cout << TRANSLATION_NULL_VAL << std::endl;
            }
        }
    }

    ~hashmap() {}
};


#endif