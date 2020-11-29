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
#include <assert.h>

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;
using namespace gsc::utility;

void decode_file( const string& in_path, string& out_path )
{
    cout << "decode_file( )..." << endl;

    ifstream referenceCiphertext( in_path, ios::in );
    ofstream decodedCleartext( out_path, ios::out | ios::binary );
    Base64::decode( referenceCiphertext, decodedCleartext );
    referenceCiphertext.close( );
    decodedCleartext.close( );
}


int main(int argc, char const *argv[])
{
    int returnValue =  -1; // Assume failure

    try
    {
        const string input( "./test-files/CaseCounts_CrudeRates_SelectedRegions_090620_2359.xlsx.b64.txt" );
        string output( input );
        output.erase( input.find( ".b64.txt" ), string::npos );

        decode_file( input, output);
        
        returnValue = 0; // Decode succeeded
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return returnValue;
}
