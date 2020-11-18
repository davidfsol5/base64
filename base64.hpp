#ifndef __BASE64_HPP_INCLUDED__
#define __BASE64_HPP_INCLUDED__

#include <string>
#include <istream>
#include <ostream>
#include <iostream>

namespace gsc::utility
{
    class Base64
    {
        // struct Sextet
        // {
        //     static const unsigned  _Mask = 

        // };

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

        struct Octets
        {
            // Four octets of bits. Only three least significant are used. So, 
            // sign is irrelevant.
            unsigned int _Scope; 
            static const unsigned int _QuantumMask = 0b1111'1111;
            static const unsigned int _OffsetQuantum = 8;
            static const unsigned int _QuantumNumber = 3;
            
        public:
            Octets( ) : _Scope( 0 ) { };

            void setQuantumValue( const int index, const unsigned char value )
            {
                std::cout << "Octets::setQuantumValue( " << index << ", " << value << " (0x" << std::hex << value << ") )..." << std::endl;
                std::cout << "_Scope = " << std::hex << _Scope << std::endl;
                if ( (index >= 0) && (index < _QuantumNumber) )
                {
                    if ( (value & Octets::_QuantumMask) == value )
                    {
                        unsigned int offset = (_QuantumNumber - 1 - index) * _OffsetQuantum;
                        std::cout << std::dec << offset << " = (" << _QuantumNumber << " - 1 - " << index << ") x " << _OffsetQuantum << std::endl;
                        unsigned int mask = 0;
                        mask = _QuantumMask << offset;
                        std::cout << std::hex << mask << " = " << _QuantumMask << " << " << std::dec << offset << std::endl;
                        unsigned int scopedValue = 0;
                        scopedValue = value << offset;
                        std::cout << std::hex << scopedValue << " = " << value << " << " << std::dec << offset << std::endl;
                        std::cout << std::hex << ((_Scope & (~mask)) | scopedValue) << " = " << (_Scope & ~(mask)) << " | " << scopedValue << std::endl;
                        _Scope = (_Scope & (~mask)) | scopedValue;
                    } else {
                        throw std::out_of_range( "Octets::setQuantumValue parameter is out of range: value must be a value between 0 and 255." );
                    }
                } else {
                    throw std::out_of_range( "Octets::setQuantumValue parameter is out of range: index must be a value between 0 and 2." );
                }
            };

            const unsigned char getQuantumValue( const int index )
            {
                std::cout << "Octets::getQuantumValue( " << index << " )" << std::endl;
                unsigned int unscopedValue = 0;

                if ( (index >= 0) && (index < _QuantumNumber) )
                {
                        unsigned int offset = (_QuantumNumber - 1 - index) * _OffsetQuantum;
                        std::cout << std::dec << offset << " = (" << _QuantumNumber << " - 1 - " << index << ") x " << _OffsetQuantum << std::endl;
                        unsigned int mask = 0;
                        mask = (_QuantumMask << offset);
                        std::cout << std::hex << mask << " = (" << _QuantumMask << " << " << std::dec << offset << ")" << std::endl;
                        unscopedValue = (_Scope & mask) >> offset;
                        std::cout << std::hex << unscopedValue << " = (" << _Scope << " & " << mask << ") >> " << std::dec << offset << std::endl;
                } else {
                    throw std::out_of_range( "Octets::getQuantumValue parameter is out of range: index must be a value between 0 and 2." );
                }

                return unscopedValue;
            };
        };

        struct Sextets
        {
            unsigned int _Scope;
            static const unsigned int _QuantumMask = 0b0011'1111;
            static const unsigned int _OffsetQuantum = 6;
            static const unsigned int _QuantumNumber = 4;

        public:
            Sextets( ) : _Scope( 0 ) { };

            void setQuantumValue( const int index, const unsigned char value )
            {
                std::cout << "Sextets::setQuantumValue( " << index << ", " << value << " (0" << std::oct << value << ") )" << std::endl;
                std::cout << "_Scope = " << std::hex << _Scope << std::endl;
                if ( (index >= 0) && (index < _QuantumNumber) )
                {
                    if ( (value & _QuantumMask) == value )
                    {
                        unsigned int offset = (_QuantumNumber - 1 - index) * _OffsetQuantum;
                        std::cout << std::dec << offset << " = (" << _QuantumNumber << " - 1 - " << index << ") x " << _OffsetQuantum << std::endl;
                        unsigned int mask = 0;
                        mask = _QuantumMask << offset;
                        std::cout << std::oct << mask << " = " << _QuantumMask << " << " << std::dec << offset << std::endl;
                        unsigned int scopedValue = 0;
                        scopedValue = value << offset;
                        std::cout << std::oct << scopedValue << " = " << value << " << " << std::dec << offset << std::endl;
                        _Scope = (_Scope & mask) & scopedValue;
                        std::cout << std::oct << _Scope << " = " << (_Scope & mask) << " & " << scopedValue << std::endl;
                    } else {
                        throw std::out_of_range( "Sextets::setQuantumValue parameter is out of range: value must be a value between 0 and 63." );
                    }
                } else {
                    throw std::out_of_range( "Sextets::setQuantumValue parameter is out of range: index must be a value between 0 and 2." );
                }
            };

            const unsigned char getQuantumValue( const int index )
            {
                std::cout << "Sextets::getQuantumValue( " << index << " )" << std::endl ;
                unsigned int unscopedValue = 0;

                if ( (index >= 0) && (index < _QuantumNumber) )
                {
                        unsigned int offset = (_QuantumNumber - 1 - index) * _OffsetQuantum;
                        std::cout << std::dec << offset << " = (" << _QuantumNumber << " - 1 - " << index << ") x " << _OffsetQuantum << std::endl;
                        unsigned int mask = 0;
                        mask = (_QuantumMask << offset);
                        std::cout << std::oct << mask << " = (" << _QuantumMask << " << " << offset << ")" << std::endl;
                        unscopedValue = (_Scope & mask) >> offset;
                        std::cout << std::oct << unscopedValue << " = (" << _Scope << " & " << mask << ") >> " << offset << std::endl;
                } else {
                    throw std::out_of_range( "Sextets::getQuantumValue parameter is out of range: index must be a value between 0 and 3." );
                }

                return unscopedValue;
            };
        };
        
        struct codeJig
        {
            union quantaOverlay
            {
                Octets _Cleartext;
                Sextets _CipherIndexes;

                quantaOverlay( ) : _Cleartext( ) {};
            } _QuantaOverlay;
            bool _OctetIsUsed[ 3 ] = { false, false, false };
            char _CipherText[ 5 ] = { 0 };

        public:
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
            };

            codeJig( const unsigned char byte0 ) : 
                _OctetIsUsed{ true, false, false }
            { 
                _QuantaOverlay._Cleartext.setQuantumValue( 0, byte0 );

                _CipherText[ 0 ] = Base64::_CodePage[ _QuantaOverlay._CipherIndexes.getQuantumValue( 0 ) ]._CipherSubstitute;
                _CipherText[ 1 ] = Base64::_CodePage[ _QuantaOverlay._CipherIndexes.getQuantumValue( 1 ) ]._CipherSubstitute;
                _CipherText[ 2 ] = Base64::_Pad;
                _CipherText[ 3 ] = Base64::_Pad;
            };

            codeJig( const char cipherChar0, const char cipherChar1, const char cipherChar2, const char cipherChar3 ) : 
                _CipherText{  cipherChar0, cipherChar1, cipherChar2, cipherChar3 }
            { 
                _QuantaOverlay._CipherIndexes.setQuantumValue( 0, Base64::lookupCipherIndex( cipherChar0 ) );
                _QuantaOverlay._CipherIndexes.setQuantumValue( 0, Base64::lookupCipherIndex( cipherChar1 ) );
                _OctetIsUsed[ 0 ] = true;

                if ( cipherChar2 != Base64::_Pad )
                {
                    _QuantaOverlay._CipherIndexes.setQuantumValue( 0, Base64::lookupCipherIndex( cipherChar2 ) );
                    _OctetIsUsed[ 1 ] = true;
                }

                if ( cipherChar3 != Base64::_Pad )
                {
                    _QuantaOverlay._CipherIndexes.setQuantumValue( 0, Base64::lookupCipherIndex( cipherChar3 ) );;
                    _OctetIsUsed[ 2 ] = true;
                }
            };
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