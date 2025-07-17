
#ifdef WIN32
#include <io.h>
#endif

#include <fcntl.h>

#include <libbasarcmnutil.h>

#include "printdata.h"

//------------------------------------------------------------------------------
PrintData::PrintData ( const basar::VarString & rPsFile, const basar::VarString & rFormName, 
					   const basar::VarString & rFile)
					 : m_PsFile  (rPsFile),
					   m_FormName(rFormName)
{
	if ( rFile.empty() )
		m_hFile = -1;
	else
	{
		m_hFile = open( rFile.c_str(), O_RDONLY);
		assert(-1 != m_hFile);
	}
}

//------------------------------------------------------------------------------
PrintData::~PrintData()
{
	if (-1 != m_hFile)
		close(m_hFile);
}

//------------------------------------------------------------------------------
void PrintData::onSendData(basar::cmnutil::CollPrintRawInput & collPrintRawInput)
{
	static const basar::Int32 BUFLEN = 1024;
	char   buffer[BUFLEN];
	const char * pBuf;
	const char * pBufEnd;

	basar::Int32  i;
    
	basar::Int32 readbytes = read(m_hFile, buffer, BUFLEN);
	assert(readbytes <= BUFLEN);

#ifndef NDEBUG
	static basar::Int32 count = 0;
	static basar::Int32 bytes = 0;

	++count;
	bytes += readbytes;

	if (0 == readbytes)
	{
		count = bytes = 0;
	}
#endif

	for (i = 0, pBuf = buffer, pBufEnd = (buffer + readbytes); 
		pBuf < pBufEnd; 
		++pBuf)
	{
		collPrintRawInput.push(*pBuf);
	}

	if (0 == readbytes)
	{
		lseek(m_hFile, 0L, SEEK_SET );
	}
}

//------------------------------------------------------------------------------
basar::VarString PrintData::getFormName() const
{
	return m_FormName;
}

//------------------------------------------------------------------------------
basar::VarString PrintData::getFormFileName() const
{
	return m_PsFile;
}
