#include <UnitTest++/UnitTest++.h>
#include "libbasarcmnutil.h"


//------------------------------------------------------------------------------
SUITE(TestEndianess)
{

//------------------------------------------------------------------------------
using basar::UInt8;
using basar::UInt32;

//------------------------------------------------------------------------------
TEST(UInt32)
{
	const UInt32   ui0  = 0x12345678;
	const UInt8  * pBuf = reinterpret_cast<const UInt8 *>(&ui0);

#ifdef _WIN32
	// little endian
	CHECK(0x78 == pBuf[0]);
	CHECK(0x56 == pBuf[1]);
	CHECK(0x34 == pBuf[2]);
	CHECK(0x12 == pBuf[3]);
#else
	// big endian
	CHECK(0x12 == pBuf[0]);
	CHECK(0x34 == pBuf[1]);
	CHECK(0x56 == pBuf[2]);
	CHECK(0x78 == pBuf[3]);
#endif
}

//------------------------------------------------------------------------------
}	// end TESTSUITE
