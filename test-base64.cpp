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

const string referenceCleartext = 
    "Man is distinguished, not only by his reason, but by this singular passion from other animals, "
    "which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable "
    "generation of knowledge, exceeds the short vehemence of any carnal pleasure.";

const string referenceCiphertext = 
    "TWFuIGlzIGRpc3Rpbmd1aXNoZWQsIG5vdCBvbmx5IGJ5IGhpcyByZWFzb24sIGJ1dCBieSB0aGlz"
    "IHNpbmd1bGFyIHBhc3Npb24gZnJvbSBvdGhlciBhbmltYWxzLCB3aGljaCBpcyBhIGx1c3Qgb2Yg"
    "dGhlIG1pbmQsIHRoYXQgYnkgYSBwZXJzZXZlcmFuY2Ugb2YgZGVsaWdodCBpbiB0aGUgY29udGlu"
    "dWVkIGFuZCBpbmRlZmF0aWdhYmxlIGdlbmVyYXRpb24gb2Yga25vd2xlZGdlLCBleGNlZWRzIHRo"
    "ZSBzaG9ydCB2ZWhlbWVuY2Ugb2YgYW55IGNhcm5hbCBwbGVhc3VyZS4=";

const string pathReferenceCipherText( "./test-files/reference-ciphertext.txt" );
const string pathReferenceClearText( "./test-files/reference-cleartext.txt" );
static const unsigned int EncodedFileLineLength = 76;
static const unsigned int EncodedStringLineLength = 0;

const bool isEqual( istream& stream1, istream& stream2 )
{
    bool result = false; // assume failure

    if ( stream1.good() && stream2.good( ) )
    {
        if ( stream1.tellg( ) == stream2.tellg( ) )
        {
            stream1.seekg( 0 );
            stream2.seekg( 0 );

            istreambuf_iterator< char > begin1( stream1 );
            istreambuf_iterator< char > begin2( stream2 );

            result =  equal( begin1, istreambuf_iterator< char >( ), begin2 ); //Second argument is end-of-range iterator
        }
    } else {
        throw invalid_argument( "Files cannot be opened for comparison.");
    }

    return result;
}

const bool test_string_encode( void )
{
    cout << "test_string_encode( )..." << endl;

    string encodedCiphertext;
    istringstream cleartext( referenceCleartext );
    
    Base64::encode( cleartext, EncodedStringLineLength, encodedCiphertext );
    stringstream output( encodedCiphertext, ios::in );
    stringstream reference( referenceCiphertext, ios::in );
    const bool returnValue = isEqual( output, reference );
    cout << "Test " << string( returnValue ? "succeeded" : "failed") << "." << endl;
    cout << "Cleartext:" << endl;
    cout << cleartext.str( ) << endl;
    cout << "Ciphertext:" << endl;
    cout << encodedCiphertext << endl;

    return returnValue;
}

const bool test_string_decode( void )
{
    cout << "test_string_decode( )..." << endl;
    ostringstream decodedCleartext;

    Base64::decode( referenceCiphertext, decodedCleartext );
    stringstream output( decodedCleartext.str(), ios::in | ios::binary );
    stringstream reference( referenceCleartext, ios::in | ios::binary );
    const bool returnValue = isEqual( output, reference );
    cout << "Test " << string( returnValue ? "succeeded" : "failed") << "." << endl;
    cout << "Ciphertext:" << endl;
    cout << referenceCiphertext << endl;
    cout << "Cleartext:" << endl;
    cout << decodedCleartext.str( ) << endl;

    return returnValue;
}

const bool test_file_encode( void )
{
    cout << "test_file_encode( )..." << endl;

    const string pathEncodedCipherText( "./test-files/encode-encoded-ciphertext.txt" );
    const string pathDecodedClearText( "./test-files/encode-decoded-cleartext.txt" );

    ifstream referenceCleartext( pathReferenceClearText, ios::in | ios::binary );
    ofstream encodedCiphertext( pathEncodedCipherText, ios::out );
    Base64::encode( referenceCleartext, EncodedFileLineLength, encodedCiphertext );
    referenceCleartext.close( );
    encodedCiphertext.close( );

    ifstream testCiphertext( pathEncodedCipherText, ios::in );
    ofstream testCleartext( pathDecodedClearText, ios::out | ios::binary );
    Base64::decode( testCiphertext, testCleartext );
    testCiphertext.close( );
    testCleartext.close( );

    ifstream input( pathReferenceClearText, ios::in | ios::binary );
    ifstream output( pathEncodedCipherText, ios::in );
    ifstream loop_back( pathDecodedClearText, ios::in | ios::binary );
    const bool returnValue = isEqual( input, loop_back );
    cout << "Test " << string( returnValue ? "succeeded" : "failed" ) << "." << endl;
    cout << "Cleartext:" << endl;
    cout << input.rdbuf( ) << endl;
    cout << "Encoded text:" << endl;
    cout << output.rdbuf( ) << endl;
    cout << "Decoded text:" << endl;
    cout << loop_back.rdbuf( ) << endl;

    return returnValue;
}

const bool test_file_decode( void )
{
    cout << "test_file_decode( )..." << endl;

    const string pathEncodedCipherText( "./test-files/decode-encoded-ciphertext.txt" );
    const string pathDecodedClearText( "./test-files/decode-decoded-cleartext.txt" );

    ifstream referenceCiphertext( pathReferenceCipherText, ios::in );
    ofstream decodedCleartext( pathDecodedClearText, ios::out | ios::binary );
    Base64::decode( referenceCiphertext, decodedCleartext );
    referenceCiphertext.close( );
    decodedCleartext.close( );

    ifstream testCleartext( pathDecodedClearText, ios::in | ios::binary );
    ofstream testCiphertext( pathEncodedCipherText, ios::out );
    Base64::encode( testCleartext, EncodedFileLineLength, testCiphertext );
    testCleartext.close( );
    testCiphertext.close( );

    ifstream input( pathReferenceCipherText, ios::in );
    ifstream output( pathDecodedClearText, ios::in | ios::binary );
    ifstream loop_back( pathEncodedCipherText, ios::in );
    const bool returnValue = isEqual( input, loop_back );
    cout << "Test " << string( returnValue ? "succeeded" : "failed" ) << "." << endl;
    cout << "Ciphertext:" << endl;
    cout << input.rdbuf( ) << endl;
    cout << "Decoded text:" << endl;
    cout << output.rdbuf( ) << endl;
    cout << "Encoded text:" << endl;
    cout << loop_back.rdbuf( ) << endl;

    return returnValue;
}


int main(int argc, char const *argv[])
{
    int returnValue =  -1; // Assume failure

    try
    {
        const bool stringEncodeSuccessful = test_string_encode( );
        assert( stringEncodeSuccessful );

        const bool stringDecodeSuccessful = test_string_decode( );
        assert( stringDecodeSuccessful );

        const bool fileEncodeSuccessful = test_file_encode( );
        assert( fileEncodeSuccessful );

        const bool fileDecodeSuccessful = test_file_decode( );
        assert( fileDecodeSuccessful );

        returnValue = stringEncodeSuccessful && stringDecodeSuccessful && fileEncodeSuccessful && fileDecodeSuccessful; // Tests succeeded
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return returnValue;
}
