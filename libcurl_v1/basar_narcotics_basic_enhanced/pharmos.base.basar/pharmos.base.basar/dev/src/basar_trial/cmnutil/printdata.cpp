
#include <libbasarcmnutil.h>

#include "testcmnutil.h"


#ifdef WIN32
#include <io.h>
#endif

#include <fcntl.h>

#include "printdata.h"


//------------------------------------------------------------------------------
namespace gui_for_test	{

//------------------------------------------------------------------------------
PrintData::PrintData() : m_hDataFile(-1)
{
}

//------------------------------------------------------------------------------
PrintData::PrintData ( const VarString & dataFile,
					   const VarString & formFile,
					   const VarString & formName)
					 : m_hDataFile(-1)      ,
					   m_FormFile (formFile),
					   m_FormName (formName)
{
	m_hDataFile = open( dataFile.c_str(), O_RDONLY);
	assert(-1 != m_hDataFile);
}

//------------------------------------------------------------------------------
PrintData::PrintData ( const VarString & formFile, const VarString & formName)
					 : m_hDataFile(-1)      ,
					   m_FormFile (formFile),
					   m_FormName (formName)
{
}

//------------------------------------------------------------------------------
PrintData::~PrintData()
{
	if (-1 != m_hDataFile)
		close(m_hDataFile);
}

//------------------------------------------------------------------------------
void PrintData::onSendData(basar::cmnutil::CollPrintRawInput & collPrintRawInput)
{
	if (-1 == m_hDataFile)
		return;

	static const Int32 BUFLEN = 1024;
	char   buffer[BUFLEN];
	const char * pBuf;
	const char * pBufEnd;

	Int32  i;
    
	Int32 readbytes = read(m_hDataFile, buffer, BUFLEN);
	assert(readbytes <= BUFLEN);

#ifndef NDEBUG
	static int count = 0;
	static int bytes = 0;

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
		lseek(m_hDataFile, 0L, SEEK_SET );
	}
}

//------------------------------------------------------------------------------
VarString PrintData::getFormName() const
{
	return m_FormName;
}

//------------------------------------------------------------------------------
VarString PrintData::getFormFileName() const
{
	return m_FormFile;
}

//------------------------------------------------------------------------------
}	// namespace gui_for_test
