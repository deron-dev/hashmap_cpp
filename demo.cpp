#include <iostream>
#include <string>
#include "hashmap.hpp"

using namespace std;

int main()
{

    hashmap<string,string> themap;

    string tempstr0;
    string tempstr1;
    string tempstr2;

    while ( tempstr1 != "next" )
    {
        tempstr1 = "";
        tempstr2 = "";

        cin >> tempstr1;
        cin >> tempstr2;

        if ( tempstr1 != "" && tempstr2 != "" )
        {
            themap.push( entry<string,string,int>( tempstr1, tempstr2 ) );
        }
    }

    themap.visualize();

    // while ( tempstr0 != "next" )
    // {
    //     cout << "Unmap test: ";
    //     cin >> tempstr0;
    //     themap.unmap( tempstr0 );
    //     themap.visualize();
    // }

    // themap.visualize();


    return 0;
}