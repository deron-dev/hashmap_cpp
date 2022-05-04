
// TODO: delete mapping
// TODO: rehash
// TODO: allow more/any data type keys

#pragma once

#ifndef HASHMAP_H
#define HASHMAP_H

#define INITIAL_HASH_DIST_WIDTH 10
#define TRANSLATION_NULL_VAL -1
#define ENTRY_META_TERMINATOR -1

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
        // TODO: rehash if necessary

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

        // ===

        // bool dupe = false; // key duplication indicator
        // unsigned int t_index; // translation record index
        // int* translation_record;

        // // generate hash and apply hash to distribution
        // t_index = djk33( newentry.key ) % translation.capacity;
        // // select corresponding translation record
        // translation_record = &translation[t_index];

        // // if the hash has not been translated
        // if ( *translation_record == TRANSLATION_NULL_VAL )
        // {
        //     // update translation record with new entry's index
        //     *translation_record = entries.count;
        //     // set new entry's index to terminator
        //     newentry.meta = ENTRY_META_TERMINATOR;
        //     // push new entry
        //     entries.push( newentry );
        // }
        // else
        // {
        //     // check for duplicate keys
        //     try
        //     {
        //         operator[](newentry.key);
        //         dupe = true;
        //     }
        //     // catch runtime errors
        //     catch (std::runtime_error)
        //     {
        //         // record old translation to new entry's meta
        //         newentry.meta = *translation_record;
        //         // update translation record with new entry's index
        //         *translation_record = entries.count;
        //         // push new entry;
        //         entries.push( newentry );
        //     }
        //     if (dupe)
        //     {
        //         throw std::runtime_error
        //         (
        //             "key \"" + newentry.key + "\" already exists;"
        //         );
        //     }
        // }
    }

    // void unmap( std::string lookup_key )
    // {
    //     entry<std::string,V,int>* eptr = NULL;
    //     entry<std::string,V,int>* eptr2 = NULL;
    //     // generate hash and apply distribution width
    //     unsigned int t_index = djk33(lookup_key) % translation.capacity;
    //     int translation_record = translation[t_index];
    //     entry<std::string,V,int>* prev_entry = NULL;
    //     entry<std::string,V,int>* last_entry = &entries[entries.count - 1];
    //     int unmap_index;

    //     // translation record indicates nonexistent key
    //     if ( translation_record == -1 )
    //     {
    //         throw std::runtime_error
    //         (
    //             "hashmap::[](std::string lookup_key) " \
    //             "no translation for key \"" + lookup_key + "\"; i.e., "\
    //             "key does not exist in map"
    //         );
    //     }
    //     // follow translation to entry
    //     eptr = &entries[translation_record];


    //     // while current entry does not match lookup_key, go to the next
    //     // ...entry with the same hash using the meta value
    //     while ( eptr->key != lookup_key && eptr->meta != -1 )
    //     {
    //         prev_entry = eptr;
    //         eptr = &entries[eptr->meta];
    //     }
    //     if ( eptr->key == lookup_key ) // found matching entry
    //     {
    //         // unmap_index = eptr->meta; // save index to be filled later

    //         // if found entry's hash is unique, update translation record to -1
    //         if ( prev_entry == NULL )
    //         {
    //             unmap_index = translation_record; // save initial translation
    //             translation[t_index] = -1;
    //         }
    //         else
    //         {
    //             // "remove" the matching entry by transferring its meta value
    //             //      to the previous entry.
    //             unmap_index = prev_entry->meta;
    //             prev_entry->meta = eptr->meta;
    //         }

    //         // determine the entry preceding the last entry in "entries"
    //         //      or determine last entry is only entry with its hash

    //         t_index = djk33(last_entry->key) % translation.capacity;
    //         translation_record = translation[t_index];

    //         if ( eptr->key != last_entry->key )
    //         {
    //             eptr2 = &entries[translation_record]; // follow translation to entry

    //             // if more than one entry with last_entry's hash, find preceding entry
    //             if ( last_entry->meta != -1 )
    //             {
    //                 // while current entry does not match lookup_key, go to the next
    //                 // ...entry with the same hash using the meta value
    //                 while ( eptr2->key != last_entry->key && eptr2->meta != -1 )
    //                 {
    //                     prev_entry = eptr2;
    //                     eptr2 = &entries[eptr->meta];
    //                 }
    //                 // update meta of entry preceding last_entry
    //                 prev_entry->meta = unmap_index;
    //             }
    //             // else, update translation table to point at slot being filled
    //             else
    //             {
    //                 translation[t_index] = unmap_index;
    //             }
    //         }

    //         // take last entry in entries and move it to now empty space

    //         *eptr = entry<std::string,std::string,int>( last_entry->key, last_entry->value, last_entry->meta );
    //         entries.pop();

    //     }
    //     else
    //     {
    //         throw std::runtime_error
    //         (
    //             "key \"" + lookup_key + "\" does not exist in map"
    //         );
    //     }
    // }

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

        // entry<K,V,int> wentry;
        // int wtranslation;
        // int t_index;
        // int e_index;

        // // parse translation array
        // for ( t_index = 0; t_index < translation.capacity; t_index++ )
        // {
        //     // current translation => wtranslation
        //     wtranslation = translation[t_index];

        //     // check for empty translation
        //     if ( wtranslation != TRANSLATION_NULL_VAL )
        //     {
        //         // output translation index and value
        //         std::cout << t_index << ": " << wtranslation << std::endl;

        //         // parse entries from current translation until last entry
        //         e_index = wtranslation;
        //         while ( entries[e_index].meta != ENTRY_META_TERMINATOR )
        //         {
        //             // current entry => wentry
        //             wentry = entries[e_index];

        //             // output entry's key/value pair
        //             std::cout << "- " << wentry.key << " --> " << wentry.value << std::endl;

        //             // update e_index to next entry to seek to
        //             e_index = wentry.meta;
        //         }
        //         // output last entry

        //         // current entry => wentry
        //         wentry = entries[e_index];

        //         // output entry's key/value pair
        //         std::cout << "- " << wentry.key << " --> " << wentry.value << std::endl;
        //     }
        //     else
        //     {
        //         std::cout << t_index << ": " << wtranslation << std::endl;
        //     }

        // }
    }

    ~hashmap() {}
};


#endif