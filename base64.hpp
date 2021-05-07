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

#ifndef __BASE64_HPP_INCLUDED__
#define __BASE64_HPP_INCLUDED__

#include <string>
#include <istream>
#include <ostream>
#include <iostream>

namespace gsc
{
    namespace utility
    {
        class Octets
        {
            // Four octets of bits. Only three least significant are used. So, 
            // sign is irrelevant.
            unsigned int _Scope; 
            static const unsigned int _QuantumMask = 0b11111111; // Looks like 0b1111'1111 is a c++17 thing.
            static const unsigned int _OffsetQuantum = 8;
            static const unsigned int _QuantumNumber = 3;
            
        public:
            Octets( ) : _Scope( 0 ) { };

            void setQuantumValue( const unsigned int index, const unsigned char value );
            const unsigned char getQuantumValue( const unsigned int index );
        };

        struct Sextets
        {
            unsigned int _Scope;
            static const unsigned int _QuantumMask = 0b00111111;
            static const unsigned int _OffsetQuantum = 6;
            static const unsigned int _QuantumNumber = 4;

        public:
            Sextets( ) : _Scope( 0 ) { };

            void setQuantumValue( const unsigned int index, const unsigned char value );
            const unsigned char getQuantumValue( const unsigned int index );
        };
        
        class Base64
        {
            typedef struct cidx
            { 
                unsigned char _Sextet : 6; 

                cidx( unsigned char bits ) : _Sextet( bits ) { };
                cidx& operator=( const unsigned char bits ) { _Sextet = bits; return *this; };
                // Overloaded int cast
                operator int( ) const { return _Sextet; }
            } cipherIndex;

            struct codeRow 
            {
                cipherIndex _ClearBitPattern;
                char _CipherSubstitute;
            } static const _CodePage [64];

            static const char _Pad =  '=';

            static const cipherIndex lookupCipherIndex( const char cipherChar );

            struct codeJig
            {
                union quantaOverlay
                {
                    Octets _Cleartext;
                    Sextets _CipherIndexes;

                    quantaOverlay( ) : _Cleartext( ) {};
                } _QuantaOverlay;
                bool _OctetIsUsed[ 3 ] = { false, false, false };
                static const std::streamsize _CipherBlockLength = 4;
                char _CipherText[ _CipherBlockLength + 1 ] = { 0 };

                static const bool getNextCipherBlock( std::istream& source, char& byte1, char& byte2, char& byte3, char& byte4 );
                const bool isOctetUsed( const int index ){ return _OctetIsUsed[ index ]; };

                codeJig( const unsigned char byte0, const unsigned char byte1, const unsigned char byte2 ) :
                    _OctetIsUsed{ true, true, true }
                { 
                    _QuantaOverlay._Cleartext.setQuantumValue( 0, byte0 );
                    _QuantaOverlay._Cleartext.setQuantumValue( 1, byte1 );
                    _QuantaOverlay._Cleartext.setQuantumValue( 2, byte2 );

                    _CipherText[ 0 ] = Base64::_CodePage[ _QuantaOverlay._CipherIndexes.getQuantumValue( 0 ) ]._CipherSubstitute;
                    _CipherText[ 1 ] = Base64::_CodePage[ _QuantaOverlay._CipherIndexes.getQuantumValue( 1 ) ]._CipherSubstitute;
                    _CipherText[ 2 ] = Base64::_CodePage[ _QuantaOverlay._CipherIndexes.getQuantumValue( 2 ) ]._CipherSubstitute;
                    _CipherText[ 3 ] = Base64::_CodePage[ _QuantaOverlay._CipherIndexes.getQuantumValue( 3 ) ]._CipherSubstitute;
                    _CipherText[ 4 ] = '\0';
                };

                codeJig( const unsigned char byte0, const unsigned char byte1 ) : 
                    _OctetIsUsed{ true, true, false }
                { 
                    _QuantaOverlay._Cleartext.setQuantumValue( 0, byte0 );
                    _QuantaOverlay._Cleartext.setQuantumValue( 1, byte1 );

                    _CipherText[ 0 ] = Base64::_CodePage[ _QuantaOverlay._CipherIndexes.getQuantumValue( 0 ) ]._CipherSubstitute;
                    _CipherText[ 1 ] = Base64::_CodePage[ _QuantaOverlay._CipherIndexes.getQuantumValue( 1 ) ]._CipherSubstitute;
                    _CipherText[ 2 ] = Base64::_CodePage[ _QuantaOverlay._CipherIndexes.getQuantumValue( 2 ) ]._CipherSubstitute;
                    _CipherText[ 3 ] = Base64::_Pad;
                    _CipherText[ 4 ] = '\0';
                };

                codeJig( const unsigned char byte0 ) : 
                    _OctetIsUsed{ true, false, false }
                { 
                    _QuantaOverlay._Cleartext.setQuantumValue( 0, byte0 );

                    _CipherText[ 0 ] = Base64::_CodePage[ _QuantaOverlay._CipherIndexes.getQuantumValue( 0 ) ]._CipherSubstitute;
                    _CipherText[ 1 ] = Base64::_CodePage[ _QuantaOverlay._CipherIndexes.getQuantumValue( 1 ) ]._CipherSubstitute;
                    _CipherText[ 2 ] = Base64::_Pad;
                    _CipherText[ 3 ] = Base64::_Pad;
                    _CipherText[ 4 ] = '\0';
                };

                codeJig( const char cipherChar0, const char cipherChar1, const char cipherChar2, const char cipherChar3 ) : 
                    _CipherText{  cipherChar0, cipherChar1, cipherChar2, cipherChar3, '\0' }
                { 
                    _QuantaOverlay._CipherIndexes.setQuantumValue( 0, Base64::lookupCipherIndex( cipherChar0 ) );
                    _QuantaOverlay._CipherIndexes.setQuantumValue( 1, Base64::lookupCipherIndex( cipherChar1 ) );
                    _OctetIsUsed[ 0 ] = true;

                    if ( cipherChar2 != Base64::_Pad )
                    {
                        _QuantaOverlay._CipherIndexes.setQuantumValue( 2, Base64::lookupCipherIndex( cipherChar2 ) );
                        _OctetIsUsed[ 1 ] = true;
                    }

                    if ( cipherChar3 != Base64::_Pad )
                    {
                        _QuantaOverlay._CipherIndexes.setQuantumValue( 3, Base64::lookupCipherIndex( cipherChar3 ) );;
                        _OctetIsUsed[ 2 ] = true;
                    }
                };
            };

        public:
            static std::string& encode( std::istream&, unsigned int, std::string& );
            static std::ostream& encode( std::istream&, unsigned int, std::ostream& );
            static std::ostream& decode( const std::string&, std::ostream& );
            static std::ostream& decode( std::istream&, std::ostream& );

        private:
            //Base64 lacks a default constructor;
            Base64( void ) = delete;
            //Base64 is not copyable
            Base64( const Base64& ) = delete;
            Base64& operator=( const Base64& ) = delete;
        };

    };
};

#endif //__BASE64_HPP_INCLUDED__ 