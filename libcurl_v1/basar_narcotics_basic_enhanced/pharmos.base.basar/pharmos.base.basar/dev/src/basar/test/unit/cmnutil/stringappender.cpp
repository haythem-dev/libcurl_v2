#include <UnitTest++/UnitTest++.h>
#include "libbasarcmnutil.h"

#include "stringappender.h"

//------------------------------------------------------------------------------
StringAppender::StringAppender()
{
}

//------------------------------------------------------------------------------
StringAppender::StringAppender(const log4cplus::helpers::Properties & properties)
: Appender(properties)
{
}

//------------------------------------------------------------------------------
StringAppender::~StringAppender()
{
    destructorImpl();
}

//------------------------------------------------------------------------------
void StringAppender::close()
{
	closed = true;
}

//------------------------------------------------------------------------------
void StringAppender::append(const log4cplus::spi::InternalLoggingEvent & event)
{
	std::stringbuf buf;
	std::ostream   out(&buf);

	layout->formatAndAppend(out, event);

	m_LogBuffer += buf.str();
}

//------------------------------------------------------------------------------
const basar::VarString & StringAppender::getLogs() const
{
	return m_LogBuffer;
}

//------------------------------------------------------------------------------
void StringAppender::clearLogs()
{
	m_LogBuffer.clear();
}
