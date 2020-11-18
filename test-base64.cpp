/*
MIT License
Copyright (c) 2020 BlockScience
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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
        cout << "Base64::encode()..." << endl;
        cout << "Cleartext:" << endl;
        cout << cleartext.str( ) << endl;
        cout << "Ciphertext:" << endl;
        cout << encodedCiphertext << endl;

        Base64::decode( canonicalCiphertext, decodedCleartext );
        assert( decodedCleartext.str() == canonicalCleartext );
        cout << "Base64::decode()..." << endl;
        cout << "Ciphertext:" << endl;
        cout << canonicalCiphertext << endl;
        cout << "Cleartext:" << endl;
        cout << decodedCleartext.str( ) << endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    

    return 0;
}
