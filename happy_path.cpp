/*
    This file is meant to be a full test of all functionalities of this project.
    It is currently unfinished.
*/

// ========== INCLUDE ==========
#include <iostream>
#include <iomanip>
#include <string>
#include "djk33.hpp"
#include "dynamic_array.hpp"
#include "entry.hpp"
#include "hashmap.hpp"

// ========== NAMESPACE ==========
using namespace std;

// ========== CONSTANTS ==========
const unsigned int  LINE_LEN    = 72;
const char          LINE_SYMBOL = '=';

// ========== PROTOTYPES ==========
int main();
void line( unsigned int len = LINE_LEN, char symbol = LINE_SYMBOL);
void nbline( unsigned int len = LINE_LEN, char symbol = LINE_SYMBOL);
bool passfail( bool test );
void pass();
void fail();
void textline( const char* text, unsigned int len = LINE_LEN, char symbol = LINE_SYMBOL );

// ========== FUNCTIONS ==========

int main()
{
    textline("TESTING: djk33.hpp");

    cout << "Trial 1: hash \"asdf\"...";
    passfail( djk33("asdf") == 3880211263 );
    cout << "Trial 2: hash \"the hash is\"...";
    passfail( djk33("the hash is") == 1372617762 );

    return 0;
}

void line( unsigned int len, char symbol)
{
    for ( int lcv = 0; lcv < len; lcv++ )
    {
        cout << symbol;
    }
    cout << endl;
}

void nbline( unsigned int len, char symbol)
{
    for ( int lcv = 0; lcv < len; lcv++ )
    {
        cout << symbol;
    }
}

void textline( const char* rtext, unsigned int len, char symbol )
{
    string text = rtext;
    const unsigned int SYMSPACE_MIN = 30;
    const unsigned int PADDING = 2;
    const char WHITESPACE = ' ';
    unsigned int lcv;
    unsigned int textlen = text.length();
    unsigned int frontlen, backlen;
    unsigned int symspace = len - (textlen + PADDING*2);
    bool symspace_odd = symspace % 2;

    if ( symspace < SYMSPACE_MIN )
    {
        // TODO: verify
        throw runtime_error(
            "textline(): text too long for line len; " \
            "use <= line length - 30;"
        );
    }

    symspace /= 2;
    frontlen = symspace;

    if ( symspace_odd )
    {
        backlen = symspace + 1;
    }
    else
    {
        backlen = symspace;
    }

    nbline( frontlen, symbol );
    nbline( PADDING, WHITESPACE );
    cout << text;
    nbline( PADDING, WHITESPACE );
    line( backlen, symbol );
}

bool passfail( bool test )
{
    if ( test )
    {
        pass();
        return true;
    }
    else
    {
        fail();
        return false;
    }
}

void pass()
{
    cout << "success" << endl;
}

void fail()
{
    cout << "failure" << endl;
}