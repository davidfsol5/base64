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
#include "exceptions.hpp"
#include <memory>
#include <sstream>
#include <iostream>

using namespace gsc::utility;
using namespace std;

const Base64::codeRow Base64::_CodePage[ 64 ] = 
{
    { 0b000000, 'A' }, { 0b000001, 'B' }, { 0b000010, 'C' }, { 0b000011, 'D'},
    { 0b000100, 'E' }, { 0b000101, 'F' }, { 0b000110, 'G' }, { 0b000111, 'H'},
    { 0b001000, 'I' }, { 0b001001, 'J' }, { 0b001010, 'K' }, { 0b001011, 'L'},
    { 0b001100, 'M' }, { 0b001101, 'N' }, { 0b001110, 'O' }, { 0b001111, 'P'},
    { 0b010000, 'Q' }, { 0b010001, 'R' }, { 0b010010, 'S' }, { 0b010011, 'T'},
    { 0b010100, 'U' }, { 0b010101, 'V' }, { 0b010110, 'W' }, { 0b010111, 'X'},
    { 0b011000, 'Y' }, { 0b011001, 'Z' }, { 0b011010, 'a' }, { 0b011011, 'b'},
    { 0b011100, 'c' }, { 0b011101, 'd' }, { 0b011110, 'e' }, { 0b011111, 'f'},
    { 0b100000, 'g' }, { 0b100001, 'h' }, { 0b100010, 'i' }, { 0b100011, 'j'},
    { 0b100100, 'k' }, { 0b100101, 'l' }, { 0b100110, 'm' }, { 0b100111, 'n'},
    { 0b101000, 'o' }, { 0b101001, 'p' }, { 0b101010, 'q' }, { 0b101011, 'r'},
    { 0b101100, 's' }, { 0b101101, 't' }, { 0b101110, 'u' }, { 0b101111, 'v'},
    { 0b110000, 'w' }, { 0b110001, 'x' }, { 0b110010, 'y' }, { 0b110011, 'z'},
    { 0b110100, '0' }, { 0b110101, '1' }, { 0b110110, '2' }, { 0b110111, '3'},
    { 0b111000, '4' }, { 0b111001, '5' }, { 0b111010, '6' }, { 0b111011, '7'},
    { 0b111100, '8' }, { 0b111101, '9' }, { 0b111110, '+' }, { 0b111111, '/'} 
};

string& Base64::encode( istream& cleartext, unsigned int maxLineLength, string& ciphertext )
{
    if ( cleartext.bad() )
    {
        throw invalid_argument( "Base64::encode first parameter, clearText, refers to a bad stream." );
    }

    ostringstream ciphertextBuilder;
    
    Base64::encode( cleartext, maxLineLength, ciphertextBuilder );

    ciphertext = ciphertextBuilder.str( );
    return ciphertext;
}

ostream& Base64::encode( istream& cleartext, unsigned int maxLineLength, ostream& ciphertext )
{
    if ( cleartext.bad() )
    {
        throw invalid_argument( "Base64::encode first parameter, clearText, refers to a bad stream." );
    }

    char byte1;
    char byte2;
    char byte3;
    unique_ptr<codeJig> jig;
    unsigned int lineLength = 0;
    
    while ( cleartext.get( byte1 ) )
    {
        if ( cleartext.get( byte2 ) )
        {
            if ( cleartext.get( byte3 ) )
            {
                // jig = std::make_unique< codeJig >( byte1, byte2, byte3 ); // make_unique requires c++14 or greater
                jig = unique_ptr< codeJig >( new codeJig( byte1, byte2, byte3 ) ); // POC only; exception unsafe
            } else {
                // jig = std::make_unique< codeJig >( byte1, byte2 ); // make_unique requires c++14 or greater
                jig = unique_ptr< codeJig >( new codeJig( byte1, byte2 ) ); // POC only; exception unsafe
           }
        } else {
            // jig = std::make_unique< codeJig >( byte1 ); // make_unique requires c++14 or greater
            jig = unique_ptr< codeJig >( new codeJig( byte1 ) ); // POC only; exception unsafe
        }
        for ( int i = 0; i < Base64::codeJig::_CipherBlockLength; i++ )
        {
            if ( maxLineLength && (lineLength == maxLineLength) )
            {
                ciphertext << endl;
                lineLength = 0;
            }
            ciphertext << jig->_CipherText[ i ];
            lineLength++;
        }
    }

    return ciphertext;
}

ostream& Base64::decode( const string& ciphertext, ostream& cleartext )
{
    istringstream cipherStream( ciphertext );

    Base64::decode( cipherStream, cleartext );
    
    return cleartext;
}

ostream& Base64::decode( istream& ciphertext, ostream& cleartext )
{
    unique_ptr<codeJig> jig;

    char byte1;
    char byte2;
    char byte3;
    char byte4;

    do
    {
        byte1 = byte2 = byte3 = byte4 = '\0';
        if ( Base64::codeJig::getNextCipherBlock( ciphertext, byte1, byte2, byte3, byte4 ) )
        {
            // jig = make_unique< codeJig >( byte1, byte2, byte3, byte4 ); // make_unique requires c++14 or greater
            jig = unique_ptr< codeJig >( new codeJig( byte1, byte2, byte3, byte4 ) ); // POC only; exception unsafe

            for ( int i = 0; i < 3; i++ )
            {
                if ( jig->isOctetUsed( i ) )
                {
                    cleartext.put( jig->_QuantaOverlay._Cleartext.getQuantumValue( i ) );
                }
            }

            ciphertext.peek( );
        }
    } while ( !(ciphertext.eof( )) );
    
    return cleartext;
}

const Base64::cipherIndex Base64::lookupCipherIndex( const char cipherChar )
{
    int index = -1;

    for ( int i = 0; i < 64; ++i)
    {
        if ( cipherChar == Base64::_CodePage[ i ]._CipherSubstitute )
        {
            index = Base64::_CodePage[ i ]._ClearBitPattern;
            break;
        }
    }

    if ( (index & 0b111111) != index )
    {
        throw out_of_range( "Base64::lookupIndex failed. Invalid ciphertext.");
    }
    
    return index;
}

void Octets::setQuantumValue( const unsigned int index, const unsigned char value )
{
    if ( (index >= 0) && (index < _QuantumNumber) )
    {
        if ( (value & Octets::_QuantumMask) == value )
        {
            unsigned int offset = (_QuantumNumber - 1 - index) * _OffsetQuantum;
            unsigned int mask = 0;
            mask = _QuantumMask << offset;
            unsigned int scopedValue = 0;
            scopedValue = value << offset;
            _Scope = (_Scope & (~mask)) | scopedValue;
        } else {
            throw std::out_of_range( "Octets::setQuantumValue parameter is out of range: value must be a value between 0 and 255." );
        }
    } else {
        throw std::out_of_range( "Octets::setQuantumValue parameter is out of range: index must be a value between 0 and 2." );
    }
}

const unsigned char Octets::getQuantumValue( const unsigned int index )
{
    unsigned int unscopedValue = 0;

    if ( (index >= 0) && (index < _QuantumNumber) )
    {
            unsigned int offset = (_QuantumNumber - 1 - index) * _OffsetQuantum;
            unsigned int mask = 0;
            mask = (_QuantumMask << offset);
            unscopedValue = (_Scope & mask) >> offset;
    } else {
        throw std::out_of_range( "Octets::getQuantumValue parameter is out of range: index must be a value between 0 and 2." );
    }

    return unscopedValue;
}

void Sextets::setQuantumValue( const unsigned int index, const unsigned char value )
{
    if ( (index >= 0) && (index < _QuantumNumber) )
    {
        if ( (value & _QuantumMask) == value )
        {
            unsigned int offset = (_QuantumNumber - 1 - index) * _OffsetQuantum;
            unsigned int mask = 0;
            mask = _QuantumMask << offset;
            unsigned int scopedValue = 0;
            scopedValue = value << offset;
            _Scope = (_Scope & (~mask)) | scopedValue;
        } else {
            throw std::out_of_range( "Sextets::setQuantumValue parameter is out of range: value must be a value between 0 and 63." );
        }
    } else {
        throw std::out_of_range( "Sextets::setQuantumValue parameter is out of range: index must be a value between 0 and 2." );
    }
}

const unsigned char Sextets::getQuantumValue( const unsigned int index )
{
    unsigned int unscopedValue = 0;

    if ( (index >= 0) && (index < _QuantumNumber) )
    {
            unsigned int offset = (_QuantumNumber - 1 - index) * _OffsetQuantum;
            unsigned int mask = 0;
            mask = (_QuantumMask << offset);
            unscopedValue = (_Scope & mask) >> offset;
    } else {
        throw std::out_of_range( "Sextets::getQuantumValue parameter is out of range: index must be a value between 0 and 3." );
    }

    return unscopedValue;
}

const bool Base64::codeJig::getNextCipherBlock( istream& source, char& byte1, char& byte2, char& byte3, char& byte4 )
{
    static string filter( " \t\r\n" );
    char *cipherBlock[ Base64::codeJig::_CipherBlockLength ]  = { &byte1, &byte2, &byte3, &byte4 };
    int i = 0;
    char c = '\0';

    do
    {
        if ( source.get( c ) )
        {
            // Skip byte if it is in the filter string
            if ( filter.find( c ) == string::npos )
            {
                *(cipherBlock[ i ]) = c;
                i++;
            }
        }
    } while ( (i < Base64::codeJig::_CipherBlockLength) && source.good( ) );
    
    return i == Base64::codeJig::_CipherBlockLength;
}