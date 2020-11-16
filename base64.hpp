#ifndef __BASE64_HPP_INCLUDED__
#define __BASE64_HPP_INCLUDED__

#include <string>
#include <istream>
#include <ostream>

namespace gsc::utility
{
    class Base64
    {
        typedef struct cidx
        { 
            unsigned char _ClearBits : 6; 

            cidx( unsigned char bits ) : _ClearBits( bits ) { };
            cidx& operator=( const unsigned char bits ) { _ClearBits = bits; };
        } cipherIndex;

        struct codeRow 
        {
            cipherIndex _ClearBitPattern;
            char _CipherSubstitute;
        } static const _CodePage [64];

        struct codeJig
        {
            union 
            {
                unsigned char _ClearBytes[ 3 ];
                cipherIndex _CipherIndexes[ 4 ];
            };

            bool _ClearByteIsUsed[ 3 ];
            char _CipherText[ 4 ];

            codeJig( unsigned char byte0, unsigned char byte1, unsigned char byte2 ) : 
                _ClearBytes( { byte0, byte1, byte2 } ),
                _ClearByteIsUsed( { true, true, true } )
            { 

            };

            codeJig( unsigned char byte0, unsigned char byte1 ) : 
                _ClearBytes( { byte0, byte1, 0 } ),
                _ClearByteIsUsed( { true, true, false } )
            { 
                // Still need to initialize _CipherText using _CipherIndexes and which constructor was called to determine padding
                // See https://en.wikipedia.org/wiki/Base64 for algorithm details.
            };

            codeJig( unsigned char byte0 ) : 
                _ClearBytes( { byte0, 0, 0 } ),
                _ClearByteIsUsed( { true, false, false } )
            { 

            };

            codeJig( cipherIndex indexes[ 4 ] ) : _CipherIndexes( {indexes[0], indexes[1], indexes[2], indexes[4]} ) { };
        };

    public:
        static std::string& encode( std::istream&, std::string& );
        static std::ostream& decode( const std::string&, std::ostream& );

        private:
            //Base64 lacks a default constructor;
            Base64( void ) = delete;
            //Base64 is not copyable
            Base64( const Base64& ) = delete;
            Base64& operator=( const Base64& ) = delete;
    };
};

#endif //__BASE64_HPP_INCLUDED__ 