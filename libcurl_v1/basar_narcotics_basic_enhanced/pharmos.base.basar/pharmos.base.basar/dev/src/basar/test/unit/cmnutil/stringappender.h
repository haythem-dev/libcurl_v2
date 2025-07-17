
//------------------------------------------------------------------------------
#ifndef __STRING_APPENDER_H__
#define __STRING_APPENDER_H__

#include <log4cplus/appender.h>

//------------------------------------------------------------------------------
class StringAppender : public log4cplus::Appender 
{
public:
	StringAppender ();
	StringAppender (const log4cplus::helpers::Properties & properties);

    ~StringAppender();

    virtual void			 close	  ();

	const basar::VarString & getLogs  () const;
	void					 clearLogs();

private:
					 StringAppender		(const StringAppender &);
	StringAppender & operator =			(const StringAppender &);

    virtual void append(const log4cplus::spi::InternalLoggingEvent & event);

	basar::VarString m_LogBuffer;
};

//------------------------------------------------------------------------------
#endif
