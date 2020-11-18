#include "base64.hpp"

#include <string>
#include <sstream>
#include <iostream>
#include <assert.h>

using namespace std;
using namespace gsc::utility;

const string canonicalCleartext = 
    "Man is distinguished, not only by his reason, but by this singular passion from other animals, "
    "which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable "
    "generation of knowledge, exceeds the short vehemence of any carnal pleasure.";

const string canonicalCiphertext = 
    "TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0aGlz"
    "IHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1c3Qgb2Yg"
    "dGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0aGUgY29udGlu"
    "dWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdlLCBleGNlZWRzIHRo"
    "ZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4=";

int main(int argc, char const *argv[])
{
    try
    {
        ostringstream decodedCleartext;

        string encodedCiphertext;
        istringstream cleartext( canonicalCleartext );
        Base64::encode( cleartext, encodedCiphertext );
        assert( encodedCiphertext == canonicalCiphertext );

        Base64::decode( canonicalCiphertext, decodedCleartext );
        assert( decodedCleartext.str() == canonicalCleartext );
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    

    return 0;
}
