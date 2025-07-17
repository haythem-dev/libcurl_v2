//----------------------------------------------------------------------------
/*! \file
 *  \brief  definitions for common utilities
 *  \author Thomas Hörath
 *  \date   15.09.2005
 */
//----------------------------------------------------------------------------

#ifndef GUARD_LIBBASARCMNUTIL_DEFINITIONS_H
#define GUARD_LIBBASARCMNUTIL_DEFINITIONS_H

//----------------------------------------------------------------------------
#include <queue>

//----------------------------------------------------------------------------
namespace basar
{
namespace cmnutil
{

//----------------------------------------------------------------------------
// typedef section
//----------------------------------------------------------------------------

//! collection for stdio channel data (used by PipedProcess)
typedef std::queue < char >		CollStdIo;	


//----------------------------------------------------------------------------
// define section
//----------------------------------------------------------------------------

/*! \ingroup DIFF_WIN_UNIX */ 
#ifdef _WIN32

//! get unsigned 16 bit value from address regarding alignment
#define GET_ALIGNED_U16(addr)           ( *(basar::UInt16  *)(addr) ) 

//! get unsigned 32 bit value from address regarding alignment
#define GET_ALIGNED_U32(addr)           ( *(basar::UInt32  *)(addr) )

//! get unsigned 64 bit value from address regarding alignment
#define GET_ALIGNED_U64(addr)           ( *(basar::ULong64 *)(addr) )

//! set unsigned 16 bit value to address regarding alignment
#define SET_ALIGNED_U16(addr, val)      { *(basar::UInt16  *)(addr) = val; } 

//! set unsigned 32 bit value to address regarding alignment
#define SET_ALIGNED_U32(addr, val)      { *(basar::UInt32  *)(addr) = val; } 

//! set unsigned 64 bit value to address regarding alignment
#define SET_ALIGNED_U64(addr, val)      { *(basar::ULong64 *)(addr) = val; } 


#else	// _WIN32


//! get unsigned 16 bit value from address regarding alignment
#define GET_ALIGNED_U16(addr)           ( ((basar::UInt8 *)(addr))[0] <<  8 | \
                                          ((basar::UInt8 *)(addr))[1]         ) 

//! get unsigned 32 bit value from address regarding alignment
#define GET_ALIGNED_U32(addr)           ( ((basar::UInt8 *)(addr))[0] << 24 | \
                                          ((basar::UInt8 *)(addr))[1] << 16 | \
                                          ((basar::UInt8 *)(addr))[2] <<  8 | \
                                          ((basar::UInt8 *)(addr))[3]         )

//! get unsigned 64 bit value from address regarding alignment
#define GET_ALIGNED_U64(addr)           ( ((basar::UInt8 *)(addr))[0] << 56 | \
                                          ((basar::UInt8 *)(addr))[1] << 48 | \
                                          ((basar::UInt8 *)(addr))[2] << 40 | \
                                          ((basar::UInt8 *)(addr))[3] << 32 | \
                                          ((basar::UInt8 *)(addr))[4] << 24 | \
                                          ((basar::UInt8 *)(addr))[5] << 16 | \
                                          ((basar::UInt8 *)(addr))[6] <<  8 | \
                                          ((basar::UInt8 *)(addr))[7]         )

//! set unsigned 16 bit value to address regarding alignment
#define SET_ALIGNED_U16(addr, val)      { ((basar::UInt8 *)(addr))[0] = ((val &             0xFF00) >>  8); \
                                          ((basar::UInt8 *)(addr))[1] = ((val &             0x00FF)      ); } 

//! set unsigned 32 bit value to address regarding alignment
#define SET_ALIGNED_U32(addr, val)      { ((basar::UInt8 *)(addr))[0] = ((val &         0xFF000000) >> 24); \
                                          ((basar::UInt8 *)(addr))[1] = ((val &         0x00FF0000) >> 16); \
                                          ((basar::UInt8 *)(addr))[2] = ((val &         0x0000FF00) >>  8); \
                                          ((basar::UInt8 *)(addr))[3] = ((val &         0x000000FF)      ); } 

//! set unsigned 64 bit value to address regarding alignment
#define SET_ALIGNED_U64(addr, val)      { ((basar::UInt8 *)(addr))[0] = ((val & 0xFF00000000000000) >> 56); \
                                          ((basar::UInt8 *)(addr))[1] = ((val & 0x00FF000000000000) >> 48); \
                                          ((basar::UInt8 *)(addr))[2] = ((val & 0x0000FF0000000000) >> 40); \
                                          ((basar::UInt8 *)(addr))[3] = ((val & 0x000000FF00000000) >> 32); \
                                          ((basar::UInt8 *)(addr))[4] = ((val & 0x00000000FF000000) >> 24); \
                                          ((basar::UInt8 *)(addr))[5] = ((val & 0x0000000000FF0000) >> 16); \
                                          ((basar::UInt8 *)(addr))[6] = ((val & 0x000000000000FF00) >>  8); \
                                          ((basar::UInt8 *)(addr))[7] = ((val & 0x00000000000000FF)      ); } 


#endif	// _WIN32


//----------------------------------------------------------------------------
}	// namespace cmnutil
}	// namespace basar

//----------------------------------------------------------------------------
#endif	// GUARD
