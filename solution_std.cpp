#include <iostream>
#include <map>
#include <string>

using namespace std;

int main(void) {

    unsigned int n; // number of lines after n containing entries
    unsigned int q; // number of lines after q containing queries
    unsigned int lcv1, i;
    string wstr, hostname, address;
    string* responses;

    map < string, string > dns;

    // get n (number of dns entries)
    cin >> n;

    // get dns entries
    for ( lcv1 = 0; lcv1 < n; lcv1++ )
    {
        cin >> hostname;
        cin >> address;
        dns[hostname] = address; 
    }

    // get q (number of queries)
    cin >> q;
    responses = (string*) malloc( sizeof(string) * q );

    // get queries
    i = 0;
    for ( lcv1 = 0; lcv1 < q; lcv1++ )
    {
        cin >> wstr;
        // try writing corresponding entry to responses array
        try
        {
            responses[lcv1] = dns.at(wstr);
        }
        // catch queries for nonexistent entries
        catch(out_of_range)
        {
            responses[lcv1] = "no match found";
        }
    }

    // write out responses
    for ( lcv1 = 0; lcv1 < q; lcv1++ )
    {
        cout << responses[lcv1] << endl;
    }

    // output 

    return 0;
}
