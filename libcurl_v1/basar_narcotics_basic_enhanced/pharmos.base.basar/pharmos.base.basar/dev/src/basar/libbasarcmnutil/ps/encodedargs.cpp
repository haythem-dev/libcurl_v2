//----------------------------------------------------------------------------
/*! \file
 *  \brief  encoded arguments for ghostscript
 *  \date   13.07.2015
 */
//----------------------------------------------------------------------------

#include "encodedargs.h"

#include "../libbasarcmnutil_locale.h"

#include "../loggerpool.h"
#include "../libbasarcmnutil_logging.h"

#ifdef _WIN32
#define VC_EXTRALEAN            //!< less Visual C headers
#define WIN32_LEAN_AND_MEAN     //!< less Windows headers
#include <windows.h>
#endif

#ifdef _AIX
#include <boost/scoped_array.hpp>
#include <iconv.h>
#include <cstring>
#include <errno.h>
#endif



//----------------------------------------------------------------------------
namespace basar
{
    namespace cmnutil
    {

//------------------------------------------------------------------------------
        EncodedArgs::EncodedArgs( const CollArgs& rawargs ) :
            m_PtrArray ( new CollArgs::value_type::value_type * [rawargs.size() ] )
        {
            setEncodedArgs( rawargs );
            setPtrArray   ();
        }

//------------------------------------------------------------------------------
        char** EncodedArgs::get() const
        {
            return m_PtrArray.get();
        }

//------------------------------------------------------------------------------
        void EncodedArgs::setEncodedArgs( const CollArgs& rawargs )
        {
            for ( CollArgs::const_iterator it = rawargs.begin(); it != rawargs.end(); ++it )
            {
                I18nString utf = conv2Utf( *it );
                m_encodedArgs.push_back( utf );
            }
        }

//------------------------------------------------------------------------------
        void EncodedArgs::setPtrArray()
        {
            // m_encodedArgs's stable c_str() addresses must be stored
            for ( size_t i = 0; i < m_encodedArgs.size(); ++i )
            {
                m_PtrArray.get()[i] = const_cast<I18nString::value_type*>( m_encodedArgs[i].c_str() );
            }
        }

//------------------------------------------------------------------------------
        UInt32 EncodedArgs::getCodepageId()
        {
            // codepages Windows CP / ISO
            static const UInt32 CODEPAGE[] =
            {
#ifdef _WIN32
                1252,       // ANSI - Latin I
                1251,       // ANSI - Cyrillic
                1250        // ANSI - Central European
#else
                28605,      // ISO 8859-15 Latin 9
                28595,      // ISO 8859-5  Cyrillic
                28592       // ISO 8859-2  Central Europe
#endif
            };

            // get suitable charset
            switch ( Locale::getCodepage() )
            {
                case Locale::CP_EAST:
                    return CODEPAGE[2];

                case Locale::CP_CYRILLIC:
                    return CODEPAGE[1];

                case Locale::CP_WEST:
                default:
                    return CODEPAGE[0];
            }
        }

        basar::ConstBuffer EncodedArgs::getCodepageName()
        {
            // codepages names per iconv --list
            static ConstString CODEPAGE_NAME[] =
            {
#ifdef _WIN32
                "1252",     // ANSI - Latin I
                "1251",     // ANSI - Cyrillic
                "1250"      // ANSI - Central European
#else
                "ISO-8859-15",      // ISO-8859-15 Latin 9
                "ISO-8859-5",       // ISO-8859-5  Cyrillic
                "ISO-8859-2"        // ISO-8859-2  Central Europe
#endif
            };

            switch ( Locale::getCodepage() )
            {
                case Locale::CP_EAST:
                    return CODEPAGE_NAME[2];

                case Locale::CP_CYRILLIC:
                    return CODEPAGE_NAME[1];

                case Locale::CP_WEST:
                default:
                    return CODEPAGE_NAME[0];
            }
        }

//------------------------------------------------------------------------------
        I18nString EncodedArgs::conv2Utf( const I18nString& native )
        {
            I18nString ret;

#ifdef _WIN32
            // + terminating null
            const int LENGTH  = int(native.size()) + 1;
            const int WIDELEN = LENGTH * sizeof( wchar_t );

            std::unique_ptr<wchar_t[]> pWideBuf( new wchar_t[LENGTH] );
            memset( pWideBuf.get(), 0, WIDELEN );

            // --- convert to UTF-16
            if ( 0 == ::MultiByteToWideChar(
                        getCodepageId(), 0,
                        native.c_str() , LENGTH,
                        pWideBuf.get() , WIDELEN ) )
            {
                return ret;
            }

            const int UTF8LEN = LENGTH * 4;  // to be safe
            std::unique_ptr<I18nString::value_type[]> pUtf8Buf( new I18nString::value_type[UTF8LEN] );
            memset( pUtf8Buf.get(), 0, UTF8LEN );

            // --- convert from UTF-16 to UTF-8
            if ( 0 == ::WideCharToMultiByte(
                        CP_UTF8       , 0,
                        pWideBuf.get(), -1,
                        pUtf8Buf.get(), UTF8LEN,
                        0, 0 ) )
            {
                return ret;
            }

            ret = pUtf8Buf.get();
#endif
#ifdef _AIX

            ret = native;

//conversion to utf-8 seems not required on aix
/*            static basar::ConstString toCode = "UTF-8";

            static basar::ConstString fromCode = "ISO-8859-15";

            const size_t LENGTH = native.size() + 1;

            const size_t UTF8LEN = LENGTH * 4; //to be safe

            boost::scoped_array<I18nString::value_type> pNativeBuf ( new I18nString::value_type[LENGTH] );
            memset( pNativeBuf.get(), 0, LENGTH );
            memcpy( pNativeBuf.get(), native.c_str(), native.size() );

            boost::scoped_array<I18nString::value_type> pUtf8Buf( new I18nString::value_type[UTF8LEN] );
            memset( pUtf8Buf.get(), 0, UTF8LEN );

            iconv_t it = iconv_open( toCode, fromCode );

            if ( it != reinterpret_cast<iconv_t>( -1 ) )
            {
                char* pTarBuf = pUtf8Buf.get();
                const char* pSrcBuf = pNativeBuf.get();

                if ( iconv( it, &pSrcBuf, const_cast<size_t*>( &LENGTH ), &pTarBuf, const_cast<size_t*>( &UTF8LEN ) ) == static_cast<size_t>( -1 ) )
                {
                    iconv_close( it );
                    return ret;
                }

                iconv_close( it );

                ret = pUtf8Buf.get();

                BLOG_DEBUG_STREAM( LoggerPool::psInterpreter(), "converted <" << native << "> to <" << ret << ">" );
            }
            else
            {
                BLOG_ERROR_STREAM( LoggerPool::psInterpreter(), "iconv_open failed, errno is <" << errno << ">" );
            }
*/
#endif
            return ret;
        }

//----------------------------------------------------------------------------
    }   // namespace cmnutil
}   // namespace basar

