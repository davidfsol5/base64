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

string& Base64::encode( istream& cleartext, string& ciphertext )
{
    if ( cleartext.bad() )
    {
        throw invalid_argument( "Base64::encode first parameter, clearText, refers to a bad stream." );
    }

    char byte1;
    char byte2;
    char byte3;
    unique_ptr<codeJig> jig;
    ostringstream ciphertextBuilder;
    
    while ( cleartext.get( byte1 ) )
    {
        if ( cleartext.get( byte2 ) )
        {
            if ( cleartext.get( byte3 ) )
            {
                jig = std::make_unique< codeJig >( byte1, byte2, byte3 );
            } else {
                jig = std::make_unique< codeJig >( byte1, byte2 );
           }
        } else {
            jig = std::make_unique< codeJig >( byte1 );
        }
        cout << jig->_CipherText << endl;
        ciphertextBuilder << jig->_CipherText;
    }
    cout <<endl;

    ciphertext = ciphertextBuilder.str( );
    return ciphertext;
}

ostream& Base64::decode( const string& ciphertext, ostream& cleartext )
{
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
};

