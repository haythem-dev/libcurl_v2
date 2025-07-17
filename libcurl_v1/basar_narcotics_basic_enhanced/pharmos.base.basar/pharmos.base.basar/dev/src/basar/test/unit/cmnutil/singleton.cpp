#include <UnitTest++/UnitTest++.h>
#include "libbasarcmnutil.h"

#include "singleton.h"

//------------------------------------------------------------------------------
namespace basar   {
namespace cmnutil {

//------------------------------------------------------------------------------
template <>
typename Singleton<SingleBase>::InstancePtr & Singleton<SingleBase>::inst()
{
    static InstancePtr s_Inst;	//!< the one and only Instance
	    
    return s_Inst;
}

//------------------------------------------------------------------------------
}
}

//------------------------------------------------------------------------------
// counts c'tor calls
static int cnt = 0;

SingleBase::SingleBase() : m_Int(10)
{
	++cnt;
}

void SingleBase::init()
{
	m_Int = 20;
}

//------------------------------------------------------------------------------
SUITE(TestSingleton)
{

TEST(useSingleton)
{
	SingleDerived::clear();

	CHECK_EQUAL(10, SingleDerived::getInstance().m_Int);
	
	SingleDerived::getInstance().init();

	CHECK_EQUAL(20, SingleDerived::getInstance().m_Int);

	SingleDerived::clear();
}

//------------------------------------------------------------------------------
TEST(repeatedInstantiation)
{
	SingleDerived::clear();
	cnt = 0;

	int prev = 0;
	
	for (int i = 0; i < 10; ++i)
	{
		SingleDerived::getInstance();

		CHECK(cnt != prev);
        prev = cnt;

		SingleDerived::clear();
	}
}

//------------------------------------------------------------------------------
}	// end TESTSUITE
