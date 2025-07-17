#include "getlibpath.h"

#ifdef _WIN32
#define VC_EXTRALEAN			//!< less Visual C headers
#define WIN32_LEAN_AND_MEAN		//!< less Windows headers
#include <windows.h>
#endif

#ifdef _AIX

#include <sys/ldr.h>

//code copied from https://root.cern.ch/blog/aix-and-dladdr
//slightly adapted to use stackbased memory

struct Dl_info
{
    const char* dli_fname;
};

int dladdr( void* s, Dl_info* i )
{
    static const size_t bufSize = 8192;
    ld_info buf[bufSize] = {};
    char* pldi = ( char* )&buf[0];
    int r = loadquery( L_GETINFO,  pldi,  bufSize );

    if ( r == -1 )
    {
        i->dli_fname = 0;
        return 0;
    }

    // First is main(), skip.
    ld_info* ldi = ( ld_info* )pldi;

    while ( ldi->ldinfo_next )
    {
        pldi += ldi->ldinfo_next;
        ldi = ( ld_info* )pldi;
        char* textBegin = ( char* )ldi->ldinfo_textorg;

        if ( textBegin < s )
        {
            char* textEnd = textBegin + ldi->ldinfo_textsize;

            if ( textEnd > s )
            {
                i->dli_fname = ldi->ldinfo_filename;
                return 1;
            }
        }
    }

    i->dli_fname = 0;
    return 0;
}

#endif

std::string getLibPath()
{
    static std::string libPath;

    if ( true == libPath.empty() )
    {
#ifdef _AIX
        Dl_info dl_info;

        //strange way of casting to function ptr found at http://stackoverflow.com/questions/10020070/aix-loadquery-return-value-interpretation-was-also-segv-in-64-bit-mode
        //explanation at http://stackoverflow.com/questions/1335715/process-is-failing-in-ptrgl-64-ptrgl-functionon-aix/1343437#1343437
        //also __extension__ is used to suppress gccs pedantic warning
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wstrict-aliasing"
        int retVal = dladdr( __extension__ ( void* ) * ( ( ulong* )&getLibPath ), &dl_info );
        #pragma GCC diagnostic pop
        if ( retVal != 0 )
        {
            std::string path( dl_info.dli_fname );
            libPath = path.substr( 0, path.find_last_of( '/' ) + 1 );
        }
#else
        char path[MAX_PATH] = {0};

        HMODULE hm = NULL;

        if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | 
                GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                (LPCSTR) &getLibPath, 
                &hm))
        {
            std::string tmp;
            GetModuleFileNameA(hm, path, sizeof(path));
            tmp = path;

            libPath = tmp.substr( 0, tmp.find_last_of( '\\' ) + 1 );
        }
        

#endif
    }
    return libPath;
}

